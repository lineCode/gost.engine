//	GOST


#include "common.h"

template<>
gtFileSystemCommon * gtSingletone<gtFileSystemCommon>::s_instance = nullptr;

gtFileSystemCommon::gtFileSystemCommon(){}
gtFileSystemCommon::~gtFileSystemCommon(){}

void gtFileSystem_init(){
	if( !gtSingletone<gtFileSystemCommon>::s_instance )
			gtSingletone<gtFileSystemCommon>::s_instance = new gtFileSystemWin32;
}

gtPtrNew<gtFile> gtFileSystem::createFile( const gtString& fileName, gtFileMode mode,
			gtFileAccessMode access,
			gtFileAction action,
			gtFileShareMode EFSM,
			u32 EFA
		){
	gtFileSystem_init();
	return gtPtrNew<gtFile>( gtSingletone<gtFileSystemCommon>::s_instance->createFile( fileName, mode, access, action, EFSM, EFA ) );;
}

bool gtFileSystem::deleteFile( const gtString& fileName ){
	gtFileSystem_init();
	return gtSingletone<gtFileSystemCommon>::s_instance->deleteFile( fileName );
}

bool gtFileSystem::deleteDir( const gtString& dir ){
	gtFileSystem_init();
	return gtSingletone<gtFileSystemCommon>::s_instance->deleteDir( dir );
}

bool gtFileSystem::existFile( const gtString& fileName ){
	gtFileSystem_init();
	return gtSingletone<gtFileSystemCommon>::s_instance->existFile( fileName );
}

bool gtFileSystem::existDir( const gtString& dir ){
	gtFileSystem_init();
	return gtSingletone<gtFileSystemCommon>::s_instance->existDir( dir );
}

bool gtFileSystem::createDir( const gtString& dir ){
	gtFileSystem_init();
	return gtSingletone<gtFileSystemCommon>::s_instance->createDir( dir );
}

void gtFileSystem::scanDirBegin( gtString dir ){
	gtFileSystem_init();
	gtString dir2 = dir;
	gtSingletone<gtFileSystemCommon>::s_instance->scanDirBegin( dir2 );
}

void gtFileSystem::scanDirEnd(){
	gtFileSystem_init();
	gtSingletone<gtFileSystemCommon>::s_instance->scanDirEnd();
}

bool gtFileSystem::getDirObject( gtFileSystem::DirObject* o ){
	gtFileSystem_init();
	return gtSingletone<gtFileSystemCommon>::s_instance->getDirObject( o );
}

bool gtFileSystem::copyFile( const gtString& existingFileName, const gtString& newFileName, bool overwrite ){
	gtFileSystem_init();
	return gtSingletone<gtFileSystemCommon>::s_instance->copyFile( existingFileName, newFileName, overwrite );
}

gtString gtFileSystem::getProgramPath(){
	gtFileSystem_init();
	return gtSingletone<gtFileSystemCommon>::s_instance->getProgramPath();
}

gtString gtFileSystem::getSystemPath(){
	gtFileSystem_init();
	return gtSingletone<gtFileSystemCommon>::s_instance->getSystemPath();
}

gtString gtFileSystem::getRealPath( const gtString& in ){
	gtFileSystem_init();
	return gtSingletone<gtFileSystemCommon>::s_instance->getRealPath( in );
}

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
