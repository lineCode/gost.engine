//	GOST

#include "common.h"

gtFileWin32::gtFileWin32( const gtString& fileName, gtFileSystem::FileMode mode,
			gtFileSystem::FileAccessMode access,
			gtFileSystem::FileAction action,
			gtFileSystem::FileShareMode EFSM,
			u32 EFA ):
m_handle(nullptr), m_isTextFile(false),
m_pointerPosition( 0u )
{
#ifdef GT_DEBUG
	m_debugName.assign( u"gtFileWin32" );
#endif
	
	m_desiredAccess = 0;
	switch( access ){
	case gost::gtFileSystem::EFAM_READ:
		m_desiredAccess |= GENERIC_READ;
		break;
	case gost::gtFileSystem::EFAM_WRITE:
		m_desiredAccess |= GENERIC_WRITE;
		break;
	case gost::gtFileSystem::EFAM_BOTH:
		m_desiredAccess |= GENERIC_READ | GENERIC_WRITE;
		break;
	case gost::gtFileSystem::EFAM_APPEND:
		m_desiredAccess |= FILE_APPEND_DATA;
		break;
	}

	DWORD ShareMode = 0;
	switch( EFSM ){
	default:
	case gost::gtFileSystem::EFSM_NONE:
		break;
	case gost::gtFileSystem::EFSM_DELETE:
		ShareMode = FILE_SHARE_DELETE;
		break;
	case gost::gtFileSystem::EFSM_READ:
		ShareMode = FILE_SHARE_READ;
		break;
	case gost::gtFileSystem::EFSM_WRITE:
		ShareMode = FILE_SHARE_WRITE;
		break;
	}

	DWORD CreationDisposition = 0;
	switch( action ){
	case gost::gtFileSystem::EFA_OPEN:
		CreationDisposition = OPEN_ALWAYS;
		break;
	case gost::gtFileSystem::EFA_OPEN_NEW:
		CreationDisposition = CREATE_ALWAYS;
		break;
	}

	DWORD FlagsAndAttributes = 0;
	if( EFA & gost::gtFileSystem::EFA_NORMAL ){
		FlagsAndAttributes |= FILE_ATTRIBUTE_NORMAL;
	}else{
		if( EFA & gost::gtFileSystem::EFA_HIDDEN ){
			FlagsAndAttributes |= FILE_ATTRIBUTE_HIDDEN;
		}
		if( EFA & gost::gtFileSystem::EFA_READONLY ){
			FlagsAndAttributes |= FILE_ATTRIBUTE_READONLY;
		}
	}

	if( mode == gtFileSystem::FileMode::EFM_TEXT ){
		m_isTextFile = true;

	}

	m_handle = CreateFileW( (wchar_t*)fileName.data(), m_desiredAccess, ShareMode, NULL,
		CreationDisposition, FlagsAndAttributes, NULL );

	if( !m_handle ){
		gtLogWriter::printWarning( u"Can not create file [%s], error code[%u]",
			fileName.data(), GetLastError() );
	}


}


gtFileWin32::~gtFileWin32( void ){
	if( m_handle ){
	//	this->flush();

		CloseHandle( m_handle );
		m_handle = nullptr;
	}


}


gtTextFileInfo	gtFileWin32::getTextFileInfo( void ){
	return this->m_textInfo;
}

void	gtFileWin32::setTextFileInfo( gtTextFileInfo info ){
	m_textInfo = info;

	this->seek( 0u, gtFile::SeekPos::ESP_BEGIN );

	if( info.m_hasBOM ){

		gtStringA str;
		str.reserve( 256u );

		switch( info.m_format ){
			case gtTextFileInfo::format::utf_8:{
				str.data()[ 0u ] = 0xEF;
				str.data()[ 1u ] = 0xBB;
				str.data()[ 2u ] = 0xBF;
				str.setSize( 3u );
			}break;
			case gtTextFileInfo::format::utf_16:{
				if( info.m_endian == gtTextFileInfo::endian::big ){
					str.data()[ 0u ] = 0xFE;
					str.data()[ 1u ] = 0xFF;
				}else{
					str.data()[ 0u ] = 0xFF;
					str.data()[ 1u ] = 0xFE;
				}
				str.setSize( 2u );
			}break;
			case gtTextFileInfo::format::utf_32:{
				if( info.m_endian == gtTextFileInfo::endian::big ){
					str.data()[ 0u ] = 0x00;
					str.data()[ 1u ] = 0x00;
					str.data()[ 2u ] = 0xFE;
					str.data()[ 3u ] = 0xFF;
				}else{
					str.data()[ 0u ] = 0xFF;
					str.data()[ 1u ] = 0xFE;
					str.data()[ 2u ] = 0x00;
					str.data()[ 3u ] = 0x00;
				}
				str.setSize( 4u );
			}break;
		}
		this->write( str );
	}
}

//	для двоичной записи
u32	gtFileWin32::write( u8 * data, u32 size ){

	GT_ASSERT1( (m_desiredAccess & GENERIC_WRITE), "Can not write to file.", "File open in READ_ONLY mode" );

	if( !m_handle ){
		gtLogWriter::printWarning( u"Can not write text to file. m_handle == nullptr" );
		return 0;
	}

	DWORD bytesWritten = 0;
	if( WriteFile( m_handle, data, size, &bytesWritten, NULL ) == FALSE ){
		gtLogWriter::printWarning( u"Can not write text to file. Error code [%u]", GetLastError() );
	}else 
		m_pointerPosition += size;

	return bytesWritten;
}

//	для текста. Работает если файл открыт в текстовом режиме.
void	gtFileWin32::write( const gtStringA& string ){
	GT_ASSERT1( m_isTextFile, "This file opened in binary mode", "m_isTextFile == true" );
//	GT_ASSERT1( (m_desiredAccess & FILE_APPEND_DATA), "Can not write to file.", "File open in binary mode" );
	if( !m_handle ){
		gtLogWriter::printWarning( u"Can not write text to file. m_handle == nullptr" );
		return;
	}

	DWORD bytesWritten;
	if( WriteFile( m_handle, string.c_str(), string.size(), &bytesWritten, NULL ) == FALSE ){
		gtLogWriter::printWarning( u"Can not write text to file. Error code [%u]", GetLastError() );
	}else 
		m_pointerPosition += string.size();
}

void	gtFileWin32::write( const gtString& string ){
	GT_ASSERT1( m_isTextFile, "This file opened in binary mode", "m_isTextFile == true" );
//	GT_ASSERT1( (m_desiredAccess & FILE_APPEND_DATA), "Can not write to file.", "File open in binary mode" );
	if( !m_handle ){
		gtLogWriter::printWarning( u"Can not write text to file. m_handle == nullptr" );
		return;
	}

	DWORD bytesWritten;
	if( WriteFile( m_handle, string.c_str(), string.size() * sizeof(char16_t), &bytesWritten, NULL ) == FALSE ){
		gtLogWriter::printWarning( u"Can not write text to file. Error code [%u]", GetLastError() );
	}else 
		m_pointerPosition += string.size() * sizeof(char16_t);
}

void	gtFileWin32::write( const gtString32& string ){
	GT_ASSERT1( m_isTextFile, "This file opened in binary mode", "m_isTextFile == true" );
//	GT_ASSERT1( (m_desiredAccess & FILE_APPEND_DATA), "Can not write to file.", "File open in binary mode" );
	if( !m_handle ){
		gtLogWriter::printWarning( u"Can not write text to file. m_handle == nullptr" );
		return;
	}

	DWORD bytesWritten;
	if( WriteFile( m_handle, string.c_str(), string.size() * sizeof(char32_t), &bytesWritten, NULL ) == FALSE ){
		gtLogWriter::printWarning( u"Can not write text to file. Error code [%u]", GetLastError() );
	}else 
		m_pointerPosition += string.size() * sizeof(char32_t);
}

void	gtFileWin32::flush( void ){
	if( m_handle ){
		FlushFileBuffers( m_handle );
	}
}

	//	чтение
u64	gtFileWin32::read( u8 * data, u64 size ){
	GT_ASSERT1( (m_desiredAccess & GENERIC_READ), "Can not read from file.", "File open in WRITE_ONLY mode" );
	if( m_handle ){
		DWORD readBytesNum = 0u;
		if( ReadFile(	m_handle,
						data, size,
						&readBytesNum,
						NULL
			) == FALSE ){
			gtLogWriter::printWarning( u"Can not read file. Error code [%u]", GetLastError() );
		}
		return readBytesNum;
	}
	return 0u;
}

	//	размер в байтах
u64		gtFileWin32::size( void ){
	if( !m_handle ){
		gtLogWriter::printWarning( u"Can not get file size. m_handle == nullptr" );
		return 0u;
	}
	return static_cast<u64>( GetFileSize( m_handle, NULL ) );
}

	//	получить позицию указателя
u64		gtFileWin32::tell( void ){
	return m_pointerPosition;
}

	//	установить позицию указателя
void		gtFileWin32::seek( u64 distance, SeekPos pos ){
	if( (m_desiredAccess & GENERIC_READ) || (m_desiredAccess & GENERIC_WRITE) ){
		if( m_handle ){

			LARGE_INTEGER li;
			li.QuadPart = distance;

			li.LowPart = SetFilePointer( m_handle, li.LowPart, (PLONG)&li.HighPart, pos );
			m_pointerPosition = li.QuadPart;

		}
	}
}
/*
Copyright (c) 2017 532235

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