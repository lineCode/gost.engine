//	GOST

#pragma once
#ifndef __GT_FILE_SYSTEM_WIN_32_H__
#define __GT_FILE_SYSTEM_WIN_32_H__

namespace gost{

	class gtFileSystemWin32 GT_FINAL : public gtFileSystemCommon{

			//	����� �������� scanDirBegin � scanDirEnd � ���������� �������
		bool m_dirScanBegin;

			//	����� ������� FindFirstFile 1 ���
		bool m_firstCall;

			//	������� ����� ���������� �����/����� ��� ������������
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
			//	���� ����� ��������, ��� ����� �������
		bool deleteDir( const gtString& dir );
		bool existFile( const gtString& fileName );
			//	true ���� ����� ��������
		bool existDir( const gtString& dir );
			//	true ���� ����� �������
		bool createDir( const gtString& dir );
		//	//	�������� ���������� �������� � ��������� �����
		//u32 getDirItemsCount( const gtString& dir );

			//	����� ������������� ����� ����� ������� ���
		void scanDirBegin( gtString dir );
			//	����� ���������� ������������ ����� ������� ���
		void scanDirEnd( void );
			//	�������� ��������� DirObject ���� ���� ����/�����
			//	����, �����/����� ���, ��� ������������ ����� ��������� - ������� false
		bool getDirObject( gtFileSystem::DirObject* );
			//	�������� ������������ ����. true ���� �����
		bool copyFile( const gtString& existingFileName, const gtString& newFileName, bool overwrite );

			//	���������� ���� � ����� � ������� ����� ����������� ���� ���������
			//	�������� "C:/Games/SuperGame/"
		gtString getProgramPath( void );

			//	���������� ���� � ��������� �����
		gtString getSystemPath( void );
		gtString getRealPath( const gtString& in );
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