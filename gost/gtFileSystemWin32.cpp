//	GOST

#include "common.h"

#if defined(GT_PLATFORM_WIN32)

gtFileSystemWin32::gtFileSystemWin32( void ):
	m_dirScanBegin( false ),
	m_firstCall( false ),
	hFind( nullptr )
{
	TCHAR szFileName[MAX_PATH];
	GetModuleFileName( NULL, szFileName, MAX_PATH );
	m_exePath.assign( (char16_t*)szFileName );

	util::stringFlipSlash<gtString>( m_exePath );
	util::stringPopBackBefore<gtString>( m_exePath, '/' );



	GetSystemDirectory( szFileName, MAX_PATH );
	m_systemPath.assign( (char16_t*)szFileName );
	m_systemPath += u"/";
	util::stringFlipSlash<gtString>( m_systemPath );

}


gtFileSystemWin32::~gtFileSystemWin32( void ){
	if( hFind )
		CloseHandle( hFind );
}

gtFile* gtFileSystemWin32::createFile( const gtString& fileName,
			gtFileSystem::FileMode mode,
			gtFileSystem::FileAccessMode access,
			gtFileSystem::FileAction action,
			gtFileSystem::FileShareMode EFSM,
			u32 EFA
		){
	return new gtFileWin32( fileName, mode, access, action, EFSM, EFA );
}

bool gtFileSystemWin32::deleteFile( const gtString& fileName ){

	if( DeleteFileW( (wchar_t*)fileName.data() ) == FALSE ){
		gtLogWriter::printWarning( u"Can not delete file [%s]. Error code [%u]",
			fileName.data(), GetLastError() );
		return false;
	}

	return true;
}

bool gtFileSystemWin32::existFile( const gtString& fileName ){
	WIN32_FIND_DATA FindFileData;
	HANDLE handle = FindFirstFile( (wchar_t*)fileName.data(), &FindFileData );
	int found = handle != INVALID_HANDLE_VALUE;
	if( found ){
		FindClose( handle );
		return true;
	}
	return false;
}

bool gtFileSystemWin32::existDir( const gtString& dir ){
	DWORD dwAttrib = GetFileAttributes( (wchar_t*)dir.data() );
	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
         (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void gtFileSystemWin32::deleteFolder( const gtString& dir ){
	gtArray<gtFileSystem::DirObject> objs;
	gtFileSystem::scanDirBegin( dir );
 	gtFileSystem::DirObject ob;
 	while( gtFileSystem::getDirObject( &ob ) ){
 		objs.push_back( ob );
 	}
 	gtFileSystem::scanDirEnd();
	u32 sz = objs.size();

 	for( u32 i = 0u; i < sz; ++i ){
 		auto * o = &objs[ i ];
  		if( o->type == gtFileSystem::DirObjectType::info ){
  			continue;

		}else if( o->type == gtFileSystem::DirObjectType::folder ){
			deleteFolder( (char16_t*)o->path );
		}else if( o->type == gtFileSystem::DirObjectType::file ){
			gtString path( (char16_t*)o->path );
			if( DeleteFile( o->path ) == FALSE ){
				gtLogWriter::printWarning( u"Can not delete file [%s]. Error code [%u]",
					o->path, GetLastError() );
			}
		}
	}

	if( RemoveDirectory( (wchar_t*)dir.data() ) == FALSE ){
		gtLogWriter::printWarning( u"Can not remove directory [%s]. Error code [%u]",
			dir.data(), GetLastError() );
	}
}

bool gtFileSystemWin32::deleteDir( const gtString& dir ){
	deleteFolder( dir );
	return true;
}

bool gtFileSystemWin32::createDir( const gtString& dir ){
	if( CreateDirectory( (wchar_t*)dir.data(), NULL ) == FALSE ){
		DWORD error = GetLastError();
		gtLogWriter::printWarning( u"Can not create directory [%s]. Error code [%u]",
			dir.data(), error );
		if( error == ERROR_ALREADY_EXISTS )
			gtLogWriter::printWarning( u"Directory already exists." );
		else if( error == ERROR_PATH_NOT_FOUND )
			gtLogWriter::printWarning( u"One or more intermediate directories do not exist." );
		return false;
	}
	return true;
}

	//	перед сканированием папки нужно вызвать это
void gtFileSystemWin32::scanDirBegin( gtString dir ){
	if( !m_dirScanBegin ){
		m_dirScanBegin = true;

		m_dir.clear();

		m_dir = dir;

		if( m_dir[ m_dir.size() - 1u ] != u'/'	&& m_dir[ m_dir.size() - 1u ] != u'\\' )
			m_dir.append( u"\\*" );
		else m_dir.append( u"*" );

	}
}
	//	после завершения сканирования нужно вызвать это
void gtFileSystemWin32::scanDirEnd( void ){
	if( m_dirScanBegin ){
		m_dirScanBegin = false;
		m_firstCall = false;

		if( hFind )
			FindClose( hFind );
		hFind = nullptr;
	}
}

	//	заполнит структуру DirObject если есть файл/папка
	//	если, файла/папки нет, или сканирование папки завершено - возврат false
bool gtFileSystemWin32::getDirObject( gtFileSystem::DirObject* o ){
	WIN32_FIND_DATA ffd;

	if( !m_firstCall ){
		m_firstCall = true;
		if( hFind )
			FindClose( hFind );

		hFind = FindFirstFile( (wchar_t*)m_dir.data(), &ffd );
		if( INVALID_HANDLE_VALUE == hFind ){
			DWORD error = GetLastError();
			gtLogWriter::printWarning( u"Can not scan dir. Error code [%u].", error );
			if( error == ERROR_FILE_NOT_FOUND ){
				gtLogWriter::printWarning( u"No matching files can be found." );
			}
			m_dirScanBegin = false;
			return false;
		}
	}else{
		if( FindNextFile( hFind, &ffd ) == FALSE ){
			return false;
		}
	}

	if( ffd.cFileName[ 0 ] == L'.' ){
		o->size = 0u;
		o->type = gtFileSystem::DirObjectType::info;
	}else{
		gtString fullPath( m_dir );
		fullPath.pop_back();// '*'
		fullPath += (char16_t*)ffd.cFileName;
#ifdef _MSC_VER
		wcscpy_s( o->path, GT_MAX_PATH, (wchar_t*)fullPath.data() );
#else
		wcscpy( o->path, (wchar_t*)fullPath.data() );
#endif

		if( ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ){
			o->type = gtFileSystem::DirObjectType::folder;
			o->size = 0u;
		}else{
			o->type = gtFileSystem::DirObjectType::file;

			LARGE_INTEGER i;
			i.LowPart = ffd.nFileSizeLow;
			i.HighPart = static_cast<LONG>( ffd.nFileSizeHigh );

			o->size = static_cast<u32>(i.QuadPart);
		}
	}

	return true;
}

	//	копирует существующий файл. true если успех
bool gtFileSystemWin32::copyFile( const gtString& existingFileName, const gtString& newFileName, bool overwrite ){
	if( !this->existFile( existingFileName ) ){
		gtLogWriter::printWarning( u"Can not copy file [%s]. File not exist.", existingFileName.data() );
		return false;
	}

	DWORD flag = COPY_FILE_FAIL_IF_EXISTS;
	if( overwrite ) flag = NULL;

	if( CopyFileEx( (LPWCH)existingFileName.data(),
					(LPWCH)newFileName.data(),
					NULL, NULL, NULL, flag )
		== FALSE ){
		gtLogWriter::printWarning( u"Can not copy file [%s]. Error code [%u].", existingFileName.data(), GetLastError() );
		return false;
	}

	return true;
}

	//	возвращает путь к папке в которой лежит исполняемый файл программы
	//	например "C:/Games/SuperGame/"
gtString gtFileSystemWin32::getProgramPath( void ){
	return m_exePath;
}

	//	возвращает путь к системной папке
gtString gtFileSystemWin32::getSystemPath( void ){
	return m_systemPath;
}

gtString gtFileSystemWin32::getRealPath( const gtString& in ){
	u32 dots_count = 0u;

	u32 in_sz = in.size();

	u32 last = in_sz - 1;

	for( u32 i = 0u; i < in_sz; ++i ){
		if( in[ i ] == u'.' ){

			if( i < last ){
				if( in[ i + 1u ] == u'.' ){
					++dots_count;
					++i;// skip second .
				}
			}
		}
	}

	gtString realPath = m_exePath;
	gtString relativePath = in;
	for( u32 i = 0u; i < dots_count; ++i ){
		util::stringPopBackBefore( realPath, '/' );
		relativePath.erase( 0u, 2u );
	}

	realPath += relativePath;


	return realPath;
}

#endif

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
