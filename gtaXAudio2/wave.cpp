#include "common.h"

bool Wave::getInfo( gtAudioSourceInfo& info ){
	HRESULT hr;

	m_file = util::openFileForReadBinShared( m_fileName ).data();

	if( !m_file ){
		gtLogWriter::printWarning( u"Can not open file." );
		return false;
	}


	u32 bytesRead = m_file->read( (u8*)&m_header, sizeof(m_header) );
	m_file->release();
	m_file = nullptr;

	if( bytesRead != 44u ){
		gtLogWriter::printWarning( u"Can not read file. Bad header." );
		return false;
	}

	/*if( m_header.bitsPerSample > 16u ){
		gtLogWriter::printWarning( u"Unsupported bit depth." );
		return false;
	}*/

	if( m_header.audioFormat != 1u ){
		gtLogWriter::printWarning( u"Unsupported wav format." );
		return false;
	}

	info.m_bytesPerSec	= m_header.byteRate;
	info.m_bitsPerSample= m_header.bitsPerSample;
	info.m_blockAlign	= m_header.blockAlign;
	info.m_channels		= m_header.numChannels;
	info.m_formatType	= m_header.audioFormat;
	info.m_sampleRate	= m_header.sampleRate;


	return true;
}

gtAudioSourceImpl* Wave::read( gtAudioSourceInfo info ){

	gtAudioSourceImpl* source = new gtAudioSourceImpl;
	if( !source ) return nullptr;

	source->allocate( m_header.subchunk2Size );
	if( !source->getData() ){
		gtLogWriter::printWarning( u"Can not allocate memmory for audio source" );
		delete source;
		return nullptr;
	}

	m_file = util::openFileForReadBinShared( m_fileName ).data();
	if( !m_file ){
		gtLogWriter::printWarning( u"Can not open file." );
		delete source;
		return nullptr;
	}

	u32 offset = m_file->size() - m_header.subchunk2Size;



	m_file->seek( offset, gtFile::SeekPos::ESP_BEGIN );
	m_file->read( source->getData(), source->getDataSize() );
	m_file->release();
	m_file = nullptr;


	source->setInfo( info );

	source->updateData();

	return source;
}

/*
Copyright (c) 2018 532235

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/