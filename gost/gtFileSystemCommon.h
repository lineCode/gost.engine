//	GOST

#pragma once
#ifndef __GT_FILE_SYSTEM_COMMON_H__
#define __GT_FILE_SYSTEM_COMMON_H__

namespace gost{

	class gtFileSystemCommon{
	public:
		gtFileSystemCommon();
		virtual ~gtFileSystemCommon();


		virtual gtFile* createFile( const gtString& fileName, gtFileSystem::FileMode mode,
			gtFileSystem::FileAccessMode access,
			gtFileSystem::FileAction action,
			gtFileSystem::FileShareMode EFSM,
			u32 EFA
		) = 0;


		virtual bool deleteFile( const gtString& fileName ) = 0;

			//	если папка доступна, она будет удалена
		virtual bool deleteDir( const gtString& dir ) = 0;

			//	true если файл доступен
		virtual bool existFile( const gtString& fileName ) = 0;

			//	true если папка доступна
		virtual bool existDir( const gtString& dir ) = 0;

			//	true если папка создана
		virtual bool createDir( const gtString& dir ) = 0;

		//	//	получить количество объектов в указанной папке
		//virtual u32 getDirItemsCount( const gtString& dir ) = 0;

			//	перед сканированием папки нужно вызвать это
		virtual void scanDirBegin( gtString dir ) = 0;
			
			//	после завершения сканирования нужно вызвать это
		virtual void scanDirEnd( void ) = 0;
		
			//	заполнит структуру DirObject если есть файл/папка
			//	если, файла/папки нет, или сканирование папки завершено - возврат false
		virtual bool getDirObject( gtFileSystem::DirObject* ) = 0;

			//	копирует существующий файл. true если успех
		virtual bool copyFile( const gtString& existingFileName, const gtString& newFileName, bool overwrite ) = 0;

			//	возвращает путь к папке в которой лежит исполняемый файл программы
			//	например "C:/Games/SuperGame/"
		virtual gtString getProgramPath( void ) = 0;

			//	возвращает путь к системной папке
		virtual gtString getSystemPath( void ) = 0;

	};


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