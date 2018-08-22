#pragma once
#ifndef __GT_FILE_SYSTEM_H__
#define __GT_FILE_SYSTEM_H__

namespace gost{

	constexpr u32 GT_MAX_PATH = 0xffff;

	enum class gtTextFileFormat : u32{
		UTF_8,
		UTF_16,
		UTF_32
	};

	enum class gtTextFileEndian : u32{
		Little, //	0xFFFE0000
		Big // 0x0000FEFF, not implemented !
	};

	struct gtTextFileInfo{
		gtTextFileFormat m_format;
		gtTextFileEndian m_endian;
		bool m_hasBOM;
	};

	enum class gtFileSeekPos : u32{
		Begin,
		Current,
		End
	};

	class gtFile : public gtRefObject{
	public:
		virtual void	flush() = 0;
		virtual gtTextFileInfo	getTextFileInfo() = 0;
		virtual u64		read( u8 * data, u64 size ) = 0;
		virtual void	setTextFileInfo( gtTextFileInfo info ) = 0;
		virtual void	seek( u64 distance, gtFileSeekPos pos ) = 0;
		virtual u64		size() = 0;
		virtual u64		tell() = 0;
		virtual u32		write( u8 * data, u32 size ) = 0;
		virtual void	write( const gtStringA& string ) = 0;
		virtual void	write( const gtString& string ) = 0;
		virtual void	write( const gtString32& string ) = 0;
	};

//#define gtFile_t gtPtr<gtFile>
	GT_TYPE(gtFile_t,gtPtr<gtFile>);
	
	enum class gtFileAccessMode : u32{
		Read,
		Write,
		Both,
		Append
	};
	
	enum class gtFileMode : u32{
		Text,
		Binary
	};

	enum class gtFileShareMode : u32{
		None,
		Delete,
		Read,
		Write
	};
	
	enum class gtFileAction : u32{
		Open,
		Open_new,
	};

	enum class gtFileAttribute : u32{
		Normal,
		Hidden,
		Readonly
	};

	class gtFileSystem : public gtRefObject{
	public:

		GT_API static gtPtrNew<gtFile> createFile( 
			const gtString& fileName,
			gtFileMode mode,
			gtFileAccessMode access,
			gtFileAction action,
			gtFileShareMode shareMode = gtFileShareMode::None,
			u32 attributeFlags = 0u
		);

		GT_API static bool copyFile( const gtString& existingFileName, const gtString& newFileName, bool overwrite );
		GT_API static bool createDir( const gtString& dir );
		GT_API static bool deleteDir( const gtString& dir );
		GT_API static bool deleteFile( const gtString& file );
		GT_API static bool existDir( const gtString& dir );
		GT_API static bool existFile( const gtString& file );
		GT_API static gtString getProgramPath();
		GT_API static gtString getRealPath( const gtString& in );
		GT_API static gtString getSystemPath();

		enum class DirObjectType : u32{
			info,
			folder, 
			file 
		};

		struct DirObject{
			DirObject(){ memset( path, 0u, GT_MAX_PATH * sizeof(wchar_t) ); }
			wchar_t path[GT_MAX_PATH];
			DirObjectType type;
			u32 size;
		};
		GT_API static bool getDirObject( DirObject* inout );
		GT_API static void scanDirBegin( gtString dir );
		GT_API static void scanDirEnd();

		GT_API static gtPtr<gtXMLDocument> XMLRead( const gtString& file );
		GT_API static void XMLWrite( const gtString& file, gtXMLNode* rootNode, bool utf8 = false );

	};

	namespace util{
	

		GT_FORCE_INLINE gtPtrNew<gtFile> openFileForReadText( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileMode::Text, gtFileAccessMode::Read, 
				gtFileAction::Open );
		}
		
		GT_FORCE_INLINE gtPtrNew<gtFile> createFileForReadText( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileMode::Text, gtFileAccessMode::Read, 
				gtFileAction::Open_new );
		}

		GT_FORCE_INLINE gtPtrNew<gtFile> openFileForWriteText( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileMode::Text, gtFileAccessMode::Append, 
				gtFileAction::Open );
		}
		
		GT_FORCE_INLINE	gtPtrNew<gtFile> createFileForWriteText( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileMode::Text, gtFileAccessMode::Write,
				gtFileAction::Open_new );
		}

		GT_FORCE_INLINE gtPtrNew<gtFile> openFileForReadBin( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileMode::Binary, gtFileAccessMode::Read, 
				gtFileAction::Open );
		}

		GT_FORCE_INLINE gtPtrNew<gtFile> openFileForReadBinShared( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileMode::Binary, gtFileAccessMode::Read, 
				gtFileAction::Open, gtFileShareMode::Read );
		}

		GT_FORCE_INLINE gtPtrNew<gtFile> openFileForWriteBin( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileMode::Binary, gtFileAccessMode::Write, 
				gtFileAction::Open );
		}

		GT_FORCE_INLINE gtPtrNew<gtFile> createFileForWriteBin( const gtString& fileName ){
			return gtFileSystem::createFile( fileName, gtFileMode::Binary, gtFileAccessMode::Write, 
				gtFileAction::Open_new );
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