#pragma once
#ifndef __GT_FILE_SYSTEM_H__
#define __GT_FILE_SYSTEM_H__

namespace gost{

	constexpr u32 GT_MAX_PATH = 256u;

	struct gtTextFileInfo{

		enum format{

			//	utf-8
			utf_8,

			utf_16,

			utf_32

		}m_format;

		enum endian{

			//	0xFFFE0000
			little,

			//	0x0000FEFF
			big // not implemented !

		}m_endian;

		bool m_hasBOM;
	};

	class gtFile : public gtRefObject{
	public:

		

		virtual gtTextFileInfo	getTextFileInfo( void ) = 0;

		virtual void	setTextFileInfo( gtTextFileInfo info ) = 0;
			
		virtual u32		write( u8 * data, u32 size ) = 0;

		virtual void	write( const gtStringA& string ) = 0;

		virtual void	write( const gtString& string ) = 0;

		virtual void	write( const gtString32& string ) = 0;

		virtual void	flush( void ) = 0;

		virtual u64		read( u8 * data, u64 size ) = 0;

		virtual u64		size( void ) = 0;

		virtual u64		tell( void ) = 0;

		enum SeekPos{

			ESP_BEGIN,

			ESP_CURRENT,

			ESP_END
		};

		virtual void	seek( u64 distance, SeekPos pos ) = 0;
	};

#define gtFile_t gtPtr<gtFile>
	
	class gtFileSystem : public gtRefObject{
	public:

		enum FileAccessMode{

			EFAM_READ,

			EFAM_WRITE,

			EFAM_BOTH,
			
			EFAM_APPEND
		};

		enum FileMode{

			EFM_TEXT,

			EFM_BINARY
		};

		enum FileShareMode{

			EFSM_NONE,

			EFSM_DELETE,

			EFSM_READ,

			EFSM_WRITE
		};

		enum FileAction{

			EFA_OPEN,

			EFA_OPEN_NEW,

		};

		enum FileAttribute{

			EFA_NORMAL,

			EFA_HIDDEN,

			EFA_READONLY
		};

		GT_API static gtPtrNew<gtFile> createFile( 
			const gtString& fileName,
			FileMode mode,
			FileAccessMode access,
			FileAction action,
			FileShareMode shareMode = FileShareMode::EFSM_NONE,
			u32 attributeFlags = FileAttribute::EFA_NORMAL
		);

		GT_API static bool deleteFile( const gtString& file );

		GT_API static bool deleteDir( const gtString& dir );

		GT_API static bool existFile( const gtString& file );

		GT_API static bool existDir( const gtString& dir );

		GT_API static bool createDir( const gtString& dir );

		enum class DirObjectType{
			
			info,

			folder, 

			file 
		};

		struct DirObject{
			
			DirObject( void ){
				memset( path, 0u, GT_MAX_PATH * sizeof(wchar_t) );
			}

			wchar_t path[GT_MAX_PATH];

			DirObjectType type;

			u32 size;
		};

		GT_API static void scanDirBegin( gtString dir );

		GT_API static void scanDirEnd( void );

		GT_API static bool getDirObject( DirObject* inout );

		GT_API static bool copyFile( const gtString& existingFileName, const gtString& newFileName, bool overwrite );

		GT_API static gtString getProgramPath( void );

		GT_API static gtString getSystemPath( void );

		GT_API static gtString getRealPath( const gtString& in );

	};

	namespace util{
	

		GT_FORCE_INLINE gtPtrNew<gtFile> openFileForReadText( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_TEXT, gtFileSystem::FileAccessMode::EFAM_READ, 
				gtFileSystem::FileAction::EFA_OPEN );
		}
		
		GT_FORCE_INLINE gtPtrNew<gtFile> createFileForReadText( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_TEXT, gtFileSystem::FileAccessMode::EFAM_READ, 
				gtFileSystem::FileAction::EFA_OPEN_NEW );
		}

		GT_FORCE_INLINE gtPtrNew<gtFile> openFileForWriteText( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_TEXT, gtFileSystem::FileAccessMode::EFAM_APPEND, 
				gtFileSystem::FileAction::EFA_OPEN );
		}
		
		GT_FORCE_INLINE	gtPtrNew<gtFile> createFileForWriteText( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_TEXT, gtFileSystem::FileAccessMode::EFAM_WRITE,
				gtFileSystem::FileAction::EFA_OPEN_NEW );
		}

		GT_FORCE_INLINE gtPtrNew<gtFile> openFileForReadBin( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_BINARY, gtFileSystem::FileAccessMode::EFAM_READ, 
				gtFileSystem::FileAction::EFA_OPEN );
		}

		GT_FORCE_INLINE gtPtrNew<gtFile> openFileForReadBinShared( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_BINARY, gtFileSystem::FileAccessMode::EFAM_READ, 
				gtFileSystem::FileAction::EFA_OPEN, gtFileSystem::EFSM_READ );
		}

		GT_FORCE_INLINE gtPtrNew<gtFile> openFileForWriteBin( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_BINARY, gtFileSystem::FileAccessMode::EFAM_WRITE, 
				gtFileSystem::FileAction::EFA_OPEN );
		}

		GT_FORCE_INLINE gtPtrNew<gtFile> createFileForWriteBin( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileSystem::FileMode::EFM_BINARY, gtFileSystem::FileAccessMode::EFAM_WRITE, 
				gtFileSystem::FileAction::EFA_OPEN_NEW );
		}
	}


}

#endif

/*
Copyright (c) 2017-2018 532235

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