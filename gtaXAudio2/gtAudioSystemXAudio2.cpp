#include "common.h"

gtAudioSystemXAudio2_7* gtAudioSystemXAudio2_7::s_instance = nullptr;

gtAudioSystemXAudio2_7::gtAudioSystemXAudio2_7( void ):
	m_XAudio2( nullptr ),
	m_MasteringVoice( nullptr ),
	m_ReverbEffect( nullptr ),
	m_SubmixVoice( nullptr )
{
	s_instance = this;
	CoInitializeEx( NULL, COINIT_MULTITHREADED );
}

gtAudioSystemXAudio2_7::~gtAudioSystemXAudio2_7( void ){
	
	m_SubmixVoice->DestroyVoice();

	SAFE_RELEASE( m_ReverbEffect );

	m_MasteringVoice->DestroyVoice();

	SAFE_RELEASE( m_XAudio2 );
	CoUninitialize();
}

gtAudioSystemXAudio2_7*	gtAudioSystemXAudio2_7::getInstance( void ){
	return s_instance;
}

bool	gtAudioSystemXAudio2_7::checkFeature( gtAudioPluginFeatures feature ){
	switch( feature ){
		case gost::gtAudioPluginFeatures::streaming_audio:
		return true;
		break;
		case gost::gtAudioPluginFeatures::reverberation:
		return false;
		break;
		case gost::gtAudioPluginFeatures::pitch:
		return false;
		break;
		case gost::gtAudioPluginFeatures::_3D:
		return false;
		break;
		default:
		return false;
		break;
	}
}

u32		gtAudioSystemXAudio2_7::getSupportedExtensions( void ){
	return NUM_OF_SUPPORTED_EXTENSIONS;
}

const s8*		gtAudioSystemXAudio2_7::getSupportedExtension( u32 id ){
	if( id >= NUM_OF_SUPPORTED_EXTENSIONS ) return nullptr;
	return Extensions[ id ];
}

bool gtAudioSystemXAudio2_7::initialize( XAudioVersion version ){

	m_version = version;

	gtString path = gtFileSystem::getSystemPath();

	if( m_version == XAudioVersion::XAudio_2_7 )
		path += "XAudio2_7.dll";
	else if( m_version == XAudioVersion::XAudio_2_6 )
		path += "XAudio2_6.dll";

	m_libXAudio = LoadLibrary( (wchar_t*)path.c_str() );
	if( !m_libXAudio ){
		gtLogWriter::printError( u"Can not load XAudio library" );
		MessageBox( 0, L"See log.txt for details", L"Error", MB_OK );
		return false;
	}

	u32 flags = 0u;
	HRESULT hr = 0;

	if( FAILED( hr = XAudio2Create( &m_XAudio2, flags, XAUDIO2_DEFAULT_PROCESSOR ) ) ){
        gtLogWriter::printError( u"Failed to init XAudio2 engine: %i", hr );
		MessageBox( 0, L"See log.txt for details", L"Error", MB_OK );
        return false;
    }

	if( FAILED( hr = m_XAudio2->CreateMasteringVoice( 
		&m_MasteringVoice,
		XAUDIO2_DEFAULT_CHANNELS) ) ){
		gtLogWriter::printError( u"Failed creating mastering voice: %i", hr );
		MessageBox( 0, L"See log.txt for details", L"Error", MB_OK );
		return false;
    }

	XAUDIO2_DEVICE_DETAILS deviceDetails;
	if( FAILED( hr = m_XAudio2->GetDeviceDetails( 0, &deviceDetails ) ) ){
		gtLogWriter::printError( u"Failed get device details: %i", hr );
		MessageBox( 0, L"See log.txt for details", L"Error", MB_OK );
        return false;
	}

	if( deviceDetails.OutputFormat.Format.nChannels > OUTPUTCHANNELS ){
		gtLogWriter::printError( u"Failed create audio engine" );
		MessageBox( 0, L"See log.txt for details", L"Error", MB_OK );
		return false;
    }

	DWORD channelMask = deviceDetails.OutputFormat.dwChannelMask;
	u32 nuofchannels = deviceDetails.OutputFormat.Format.nChannels;
	if( FAILED( hr = XAudio2CreateReverb( &m_ReverbEffect, 0 ) ) ){
		gtLogWriter::printError( u"Can't create AudioFX: %i", hr );
		MessageBox( 0, L"See log.txt for details", L"Error", MB_OK );
		return false;
    }

	XAUDIO2_EFFECT_DESCRIPTOR effects[] = { { m_ReverbEffect, TRUE, 1 } };
    XAUDIO2_EFFECT_CHAIN effectChain = { 1, effects };
	if( FAILED( hr = m_XAudio2->CreateSubmixVoice( &m_SubmixVoice, 1,
			deviceDetails.OutputFormat.Format.nSamplesPerSec, 0, 0,
				NULL, &effectChain ) ) ){
		gtLogWriter::printError( u"Can't create Submix Voice: %i", hr );
		MessageBox( 0, L"See log.txt for details", L"Error", MB_OK );
        return false;
    }

	XAUDIO2FX_REVERB_PARAMETERS native;
    ReverbConvertI3DL2ToNative( &m_PRESET_PARAMS[1], &native );

	m_SubmixVoice->SetEffectParameters( 0, &native, sizeof( native ) );

	//X3DAudioInitialize( channelMask, X3DAUDIO_SPEED_OF_SOUND, X3DInstance );

	return true;
}





gtAudioStream*	gtAudioSystemXAudio2_7::createStream( const gtString& fileName ){

	gtAudioStreamImpl * stream = new gtAudioStreamImpl(m_XAudio2);

	if( !stream ){
		gtLogWriter::printError( u"Can not allocate memory for stream object" );
		return nullptr;
	}

	if( !stream->open( fileName ) ){
		gtLogWriter::printError( u"Can not open stream" );
		stream->release();
		return nullptr;
	}

	return stream;
}






gtAudioObject*	gtAudioSystemXAudio2_7::createAudioObject( const gtString& fileName, u32 sp ){

	gtAudioSource* source = loadAudioSource( fileName );

	if( !source ) return nullptr;

	return createAudioObject( source, sp );
}





gtAudioObject*	gtAudioSystemXAudio2_7::createAudioObject( gtAudioSource* source, u32 sp ){

	GT_ASSERT1( source, "Audio source not initialized", "source != nullptr" );
	if( !source ) return nullptr;

	gtAudioObjectImpl * audioObject = new gtAudioObjectImpl( source );

	if( !audioObject ){
		gtLogWriter::printWarning( u"Can not allocate memory for audio object" );
		return nullptr;
	}

	if( !audioObject->init( sp ) ){
		gtLogWriter::printWarning( u"Can not initialize audio object" );
		audioObject->release();
		return nullptr;
	}

	return audioObject;
}

void	gtAudioSystemXAudio2_7::gtAudioObjectImpl::setTime( f64 t ){
	m_time = t;
}


gtAudioSource*	gtAudioSystemXAudio2_7::loadAudioSource( const gtString& fileName ){

	gtString realPath = gtFileSystem::getRealPath( fileName );

	if( !gtFileSystem::existFile( realPath ) ){
		gtLogWriter::printWarning( u"File not exist: %s", realPath.data() );
		return nullptr;
	}

	gtAudioSourceImpl*	source = nullptr;

	gtString ext = util::stringGetExtension( realPath );
	util::stringToLower( ext );

	gtAudioSourceInfo info;

	if( ext == u"wav" ){
		Wave wave( realPath );
		
		if( wave.getInfo( info ) )
			source = wave.read( info );
		else{
			gtLogWriter::printInfo( u"Can not get audio information" );
			return nullptr;
		}

		if( !source )
			return nullptr;

	}else if( ext == u"ogg" ){
		Ogg ogg( realPath );

		if( ogg.getInfo( info ) )
			source = ogg.read( info );
		else{
			gtLogWriter::printInfo( u"Can not get audio information" );
			return nullptr;
		}

		if( !source )
			return nullptr;


	}else{
		gtLogWriter::printInfo( u"Unknown sound file" );
		return nullptr;
	}

	return source;
}



//	=========================================================
//	=========================================================
//	=========================================================
//	=========================================================

gtAudioSystemXAudio2_7::gtAudioObjectImpl::gtAudioObjectImpl( gtAudioSource * src ):
	m_source( src ),
	m_volume( 1.f ),
	m_sourceImpl( (gtAudioSourceImpl*)src ),
	m_currentSourceVoice( 0u ),
	m_numOfPlayedSounds( 0u )
{
	m_callback.p_num_of_played_sounds = &m_numOfPlayedSounds;
}

gtAudioSystemXAudio2_7::gtAudioObjectImpl::~gtAudioObjectImpl( void ){
	for( u32 i = 0u; i < m_sourceVoice.size(); ++i ){
		m_sourceVoice[ i ]->Stop();
		m_sourceVoice[ i ]->FlushSourceBuffers();
		m_sourceVoice[ i ]->DestroyVoice();
	}
	if( m_source ){
		m_source->release();
	}
}

bool gtAudioSystemXAudio2_7::gtAudioObjectImpl::init( u32 sp ){
//	HRESULT hr;

	auto * asys = gtAudioSystemXAudio2_7::getInstance();

	XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];
	sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER; // LPF direct-path
	sendDescriptors[1].Flags = XAUDIO2_SEND_USEFILTER; // LPF reverb-path -- omit for better performance at the cost of less realistic occlusion
	sendDescriptors[0].pOutputVoice = asys->m_MasteringVoice;
	sendDescriptors[1].pOutputVoice = asys->m_SubmixVoice;
	const XAUDIO2_VOICE_SENDS sendList = { 2, sendDescriptors };

	gtAudioSourceImpl* source = (gtAudioSourceImpl*)m_source;

	setTime( source->m_time );


	WAVEFORMATEX wfex;
	ZeroMemory( &wfex, sizeof( wfex ) );

	const gtAudioSourceInfo& info = source->getInfo();
	wfex.cbSize				= 0u;//source->getDataSize();
	wfex.nAvgBytesPerSec	= info.m_bytesPerSec;
	wfex.nBlockAlign		= (WORD)info.m_blockAlign;
	wfex.nChannels			= (WORD)info.m_channels;
	wfex.wBitsPerSample		= (WORD)info.m_bitsPerSample;
	wfex.wFormatTag			= (WORD)info.m_formatType;
	wfex.nSamplesPerSec		= info.m_sampleRate;

	m_sourceMax = sp;

	IXAudio2SourceVoice* sv = nullptr;

	for( u32 i = 0u; i < m_sourceMax; ++i ){
		if( FAILED( asys->m_XAudio2->CreateSourceVoice( &sv, &wfex, 0,
			XAUDIO2_DEFAULT_FREQ_RATIO, &m_callback, &sendList ) ) ){
			gtLogWriter::printWarning( u"Can not create source voice" );
			return false;
		}
		m_sourceVoice.push_back( sv );
	}


	return true;
}



void gtAudioSystemXAudio2_7::gtAudioObjectImpl::play( void ){

	m_state = gtAudioState::play;

	if( m_numOfPlayedSounds < this->m_sourceMax ){
		
		updateBuffer();

		if( FAILED( m_sourceVoice[ m_currentSourceVoice ]->Start( 0u, XAUDIO2_COMMIT_ALL ) ) ){
			gtLogWriter::printWarning( u"m_sourceVoice->Start failed" );
		}

		++m_currentSourceVoice;

		if( m_currentSourceVoice == this->m_sourceMax )
			m_currentSourceVoice = 0u;

		++m_numOfPlayedSounds;
	}
}



void gtAudioSystemXAudio2_7::gtAudioObjectImpl::pause( void ){

	if( m_state == gtAudioState::pause ){
		gtLogWriter::printWarning( u"Can not set pause audio playback" );
		return;
	}

	m_state = gtAudioState::pause;

	u32 sz = m_sourceVoice.size();

	m_numOfPlayedSounds = 0u;

	for( u32 i = 0u; i < sz; ++i )
		m_sourceVoice[ i ]->Stop();
}



void gtAudioSystemXAudio2_7::gtAudioObjectImpl::stop( void ){

	if( m_state == gtAudioState::stop ){
		gtLogWriter::printWarning( u"Can not stop audio playback" );
		return;
	}

	m_state = gtAudioState::stop;
	
	u32 sz = this->m_sourceVoice.size();

	m_currentSourceVoice = 0u;

	m_numOfPlayedSounds = 0u;

	for( u32 i = 0u; i < sz; ++i ){
		m_sourceVoice[ i ]->Stop();
		m_sourceVoice[ i ]->FlushSourceBuffers();
	}
}

void gtAudioSystemXAudio2_7::gtAudioObjectImpl::setVolume( f32 volume ){
	m_volume = volume;

	u32 sz = m_sourceVoice.size();
	for( u32 i = 0u; i < sz; ++i )
		m_sourceVoice[ i ]->SetVolume( volume );
}

f32  gtAudioSystemXAudio2_7::gtAudioObjectImpl::getVolume( void ){
	return m_volume;
}

void gtAudioSystemXAudio2_7::gtAudioObjectImpl::setLoop( bool loop ){
	m_sourceImpl->getXAUDIO2_BUFFER().LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;
}

//???
void gtAudioSystemXAudio2_7::gtAudioObjectImpl::setAudioSource( gtAudioSource* /*source*/ ){
}

gtAudioSource* gtAudioSystemXAudio2_7::gtAudioObjectImpl::getAudioSource( void ){
	return m_source;
}

void gtAudioSystemXAudio2_7::gtAudioObjectImpl::updateBuffer( void ){
	if( FAILED( m_sourceVoice[ m_currentSourceVoice ]->SubmitSourceBuffer( &m_sourceImpl->getXAUDIO2_BUFFER() ) ) ){
		gtLogWriter::printWarning( u"SubmitSourceBuffer failed" );
	}
}


/*
=============================================
=============================================
=============================================
*/


gtAudioSystemXAudio2_7::gtAudioStreamImpl::gtAudioStreamImpl( IXAudio2* d ):
	m_isOpen( false ),
	m_volume( 1.f ),
	m_sourceVoice( nullptr ),
	m_device( d ),
	m_format( AudioFileFormat::wav )
{}

gtAudioSystemXAudio2_7::gtAudioStreamImpl::~gtAudioStreamImpl( void ){
	close();
}

bool	gtAudioSystemXAudio2_7::gtAudioStreamImpl::close( void ){

	m_wave.closeStream();
	m_ogg.closeStream();

	if( m_sourceVoice )
		m_sourceVoice->DestroyVoice();
	m_sourceVoice = nullptr;


	if( m_isOpen ){


		m_isOpen = false;
		return true;
	}
	return false;
}

bool	gtAudioSystemXAudio2_7::gtAudioStreamImpl::open( const gtString& fileName ){

	if( !m_isOpen ){

		gtString fullPath = fileName;
		if( !gtFileSystem::existFile( fileName ) ){
			
			fullPath = gtFileSystem::getRealPath( fileName );
			if( !gtFileSystem::existFile( fullPath ) ){
				gtLogWriter::printWarning( u"Can not open file for streaming. %s", fullPath.data() );
				return false;
			}
		}

		gtString ext = util::stringGetExtension( fullPath );
		util::stringToLower( ext );

		if( ext != u"wav" && ext != u"ogg" ){
			gtLogWriter::printWarning( u"File format not supported. %s", fullPath.data() );
			return false;
		}

		if( ext == u"ogg" ){
			m_format = AudioFileFormat::ogg;
			m_ogg.m_fileName = fullPath;

			if( !m_ogg.getInfo( m_info )){
				gtLogWriter::printWarning( u"Bad file. %s", fullPath.data() );
				return false;
			}

			this->m_time = m_ogg.m_time;

		}else{
			m_wave.m_fileName = fullPath;

			if( !m_wave.getInfo( m_info )){
				gtLogWriter::printWarning( u"Bad file. %s", fullPath.data() );
				return false;
			}

			//this->m_time = 
		}

		WAVEFORMATEX wfex;
		ZeroMemory( &wfex, sizeof( wfex ) );
		wfex.nAvgBytesPerSec	= m_info.m_bytesPerSec;
		wfex.nBlockAlign		= (WORD)m_info.m_blockAlign;
		wfex.nChannels			= (WORD)m_info.m_channels;
		wfex.wBitsPerSample		= (WORD)m_info.m_bitsPerSample;
		wfex.wFormatTag			= (WORD)m_info.m_formatType;
		wfex.nSamplesPerSec		= m_info.m_sampleRate;
		
		if( FAILED( m_device->CreateSourceVoice( &m_sourceVoice, &wfex, 0, 1.0f, &m_voiceContext ) ) ){
			gtLogWriter::printWarning( u"Error creating source voice" );
			return false;
		}
		
	}

	return true;
}


void	gtAudioSystemXAudio2_7::gtAudioStreamImpl::play( void ){
	if( m_state != gtAudioState::play ){
		m_ogg.m_playBackCommand = PlayBackCommand::PBC_NONE;
		m_wave.m_playBackCommand = PlayBackCommand::PBC_NONE;

		if( isOgg() ){
			if( !m_ogg.prepareToStreaming(
										   m_sourceVoice,
										   &m_state)){
				gtLogWriter::printWarning( u"Can not start playback [!prepareToStreaming]" );
				return;
			}else{
				gtLogWriter::printInfo( u"Starting playback %s", m_ogg.m_fileName.data() );
			}
		}else{
			if( !m_wave.prepareToStreaming(
										   m_sourceVoice,
										   &m_state)){
				gtLogWriter::printWarning( u"Can not start playback [!prepareToStreaming]" );
				return;
			}else{
				gtLogWriter::printInfo( u"Starting playback %s", m_wave.m_fileName.data() );
			}
		}


	}else{
		gtLogWriter::printWarning( u"Can not start playback [m_state == play]" );
	}
}


void			gtAudioSystemXAudio2_7::gtAudioStreamImpl::pause( void ){
	if( m_state == gtAudioState::play ){
		m_ogg.m_playBackCommand = PlayBackCommand::PBC_PAUSE;
		m_wave.m_playBackCommand = PlayBackCommand::PBC_PAUSE;
	}else
		gtLogWriter::printWarning( u"Can not pause playback [m_state != play]" );
}


void			gtAudioSystemXAudio2_7::gtAudioStreamImpl::stop( void ){
	if( m_state == gtAudioState::play ){
		m_ogg.m_playBackCommand = PlayBackCommand::PBC_STOP;
		m_wave.m_playBackCommand = PlayBackCommand::PBC_STOP;
	}else
		gtLogWriter::printWarning( u"Can not stop playback [m_state != play]" );
}

void			gtAudioSystemXAudio2_7::gtAudioStreamImpl::setVolume( f32 volume ){
	m_volume = volume;
	m_sourceVoice->SetVolume( m_volume );
}

f32				gtAudioSystemXAudio2_7::gtAudioStreamImpl::getVolume( void ){
	return m_volume;
}

void			gtAudioSystemXAudio2_7::gtAudioStreamImpl::setLoop( bool loop ){
	m_isLoop = loop;
	m_wave.m_isLoop = loop ? 1u : 0u;
	m_ogg.m_isLoop = loop ? 1u : 0u;
}

void	gtAudioSystemXAudio2_7::gtAudioStreamImpl::setPlaybackPosition( f32 position ){
	f32 p = position;
	if( p < 0.00f ) p = 0.f;
	if( p > 1.00f ) p = 1.f;

	if( isOgg() ){
		m_ogg.setPos( p );
	}else{
		m_wave.setPos( p );
	}
	
}

f32		gtAudioSystemXAudio2_7::gtAudioStreamImpl::getPlaybackPosition( void ){
	if( isOgg() ){
		return m_ogg.getPos();
	}else{
		return m_wave.getPos();
	}
}

void			gtAudioSystemXAudio2_7::gtAudioStreamImpl::setAudioSource( gtAudioSource* /*source*/ ){}
gtAudioSource*	gtAudioSystemXAudio2_7::gtAudioStreamImpl::getAudioSource( void ){
	return nullptr;
}
