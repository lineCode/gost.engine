//	GOST

#pragma once
#ifndef __GT_FILE_SYSTEM_WIN_32_H__
#define __GT_FILE_SYSTEM_WIN_32_H__

namespace gost{

	class gtFileSystemWin32 GT_FINAL : public gtFileSystemCommon{

			//	чтобы вызывать scanDirBegin и scanDirEnd в правильном порядке
		bool m_dirScanBegin;

			//	чтобы вызвать FindFirstFile 1 раз
		bool m_firstCall;

			//	текущий хэндл найденного файла/папки при сканировании
		HANDLE hFind;

		gtString m_dir;

		gtString m_exePath, m_systemPath;

	public:

		gtFileSystemWin32( void );
		virtual ~gtFileSystemWin32( void );


		gtFile* createFile( const gtString& fileName, gtFileSystem::FileMode mode,
			gtFileSystem::FileAccessMode access,
			gtFileSystem::FileAction action,
			gtFileSystem::FileShareMode EFSM,
			u32 EFA
		);

		bool deleteFile( const gtString& fileName );
			//	если папка доступна, она будет удалена
		bool deleteDir( const gtString& dir );
		bool existFile( const gtString& fileName );
			//	true если папка доступна
		bool existDir( const gtString& dir );
			//	true если папка создана
		bool createDir( const gtString& dir );
		//	//	получить количество объектов в указанной папке
		//u32 getDirItemsCount( const gtString& dir );

			//	перед сканированием папки нужно вызвать это
		void scanDirBegin( gtString dir );
			//	после завершения сканирования нужно вызвать это
		void scanDirEnd( void );
			//	заполнит структуру DirObject если есть файл/папка
			//	если, файла/папки нет, или сканирование папки завершено - возврат false
		bool getDirObject( gtFileSystem::DirObject* );
			//	копирует существующий файл. true если успех
		bool copyFile( const gtString& existingFileName, const gtString& newFileName, bool overwrite );

			//	возвращает путь к папке в которой лежит исполняемый файл программы
			//	например "C:/Games/SuperGame/"
		gtString getProgramPath( void );

			//	возвращает путь к системной папке
		gtString getSystemPath( void );
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