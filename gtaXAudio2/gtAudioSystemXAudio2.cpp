#include "common.h"

gtAudioSystemXAudio2_7* gtAudioSystemXAudio2_7::s_instance = nullptr;

gtAudioSystemXAudio2_7::gtAudioSystemXAudio2_7( void ):
	m_XAudio2( nullptr ),
	m_MasteringVoice( nullptr ),
	m_ReverbEffect( nullptr ),
	m_SubmixVoice( nullptr )
{
	s_instance = this;
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




gtAudioSource*	gtAudioSystemXAudio2_7::loadAudioSource( const gtString& fileName ){

	gtString realPath = gtFileSystem::getRealPath( fileName );

	if( !gtFileSystem::existFile( realPath ) ){
		gtLogWriter::printWarning( u"File not exist: %s", realPath.data() );
		return nullptr;
	}

	gtAudioSourceImpl*	source = nullptr;

	gtString ext = util::stringGetExtension( realPath );
	util::stringToLower( ext );

	if( ext == u"wav" ){
		Wave wave;
		source = wave.read( realPath );

		if( !source )
			return nullptr;

	}else if( ext == u"ogg" ){
		Ogg ogg;
		source = ogg.read( realPath );

		if( !source )
			return nullptr;

	}else{
		gtLogWriter::printInfo( u"Unknown sound file" );
		return nullptr;
	}

	return source;
}



///	=========================================================
///	=========================================================
///	=========================================================
///	=========================================================

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
	HRESULT hr;

	auto * asys = gtAudioSystemXAudio2_7::getInstance();

	XAUDIO2_SEND_DESCRIPTOR sendDescriptors[2];
	sendDescriptors[0].Flags = XAUDIO2_SEND_USEFILTER; // LPF direct-path
	sendDescriptors[1].Flags = XAUDIO2_SEND_USEFILTER; // LPF reverb-path -- omit for better performance at the cost of less realistic occlusion
	sendDescriptors[0].pOutputVoice = asys->m_MasteringVoice;
	sendDescriptors[1].pOutputVoice = asys->m_SubmixVoice;
	const XAUDIO2_VOICE_SENDS sendList = { 2, sendDescriptors };

	gtAudioSourceImpl* source = (gtAudioSourceImpl*)m_source;

	WAVEFORMATEX wfex;
	ZeroMemory( &wfex, sizeof( wfex ) );

	const gtAudioSourceInfo& info = source->getInfo();
	wfex.cbSize				= source->getDataSize();
	wfex.nAvgBytesPerSec	= info.m_AvgBytesPerSec;
	wfex.nBlockAlign		= info.m_BlockAlign;
	wfex.nChannels			= info.m_channels;
	wfex.wBitsPerSample		= info.m_BitsPerSample;
	wfex.wFormatTag			= info.m_formatType;
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
	u32 sz = m_sourceVoice.size();

	m_numOfPlayedSounds = 0u;

	for( u32 i = 0u; i < sz; ++i )
		m_sourceVoice[ i ]->Stop();
}



void gtAudioSystemXAudio2_7::gtAudioObjectImpl::stop( void ){
	u32 sz = this->m_sourceVoice.size();

	m_currentSourceVoice = 0u;

	m_numOfPlayedSounds = 0u;

	for( u32 i = 0u; i < sz; ++i ){
		m_sourceVoice[ i ]->Stop();
		m_sourceVoice[ i ]->FlushSourceBuffers();
	}
}

void gtAudioSystemXAudio2_7::gtAudioObjectImpl::setVolume( f32 volume ){
	if( volume < 0.0000f ) volume = 0.f;
	else if( volume > 1.f ) volume = 1.f;

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

///???
void gtAudioSystemXAudio2_7::gtAudioObjectImpl::setAudioSource( gtAudioSource* source ){
}

gtAudioSource* gtAudioSystemXAudio2_7::gtAudioObjectImpl::getAudioSource( void ){
	return m_source;
}

void gtAudioSystemXAudio2_7::gtAudioObjectImpl::updateBuffer( void ){
	if( FAILED( m_sourceVoice[ m_currentSourceVoice ]->SubmitSourceBuffer( &m_sourceImpl->getXAUDIO2_BUFFER() ) ) ){
		gtLogWriter::printWarning( u"SubmitSourceBuffer failed" );
	}
}