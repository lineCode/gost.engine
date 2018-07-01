//	GOST

#pragma once
#ifndef __GT_FILE_WIN_32_H__
#define __GT_FILE_WIN_32_H__

#if defined(GT_PLATFORM_WIN32)

namespace gost{

	class gtFileWin32 GT_FINAL : public gtFile{

		gtTextFileInfo	m_textInfo;
		HANDLE			m_handle;
		bool			m_isTextFile;

			// GENERIC_READ or GENERIC_WRITE
		DWORD			m_desiredAccess;
		u64				m_pointerPosition;
	public:
		gtFileWin32( const gtString& fileName, gtFileMode mode,
			gtFileAccessMode access,
			gtFileAction action,
			gtFileShareMode EFSM,
			u32 EFA );
		virtual ~gtFileWin32();


		gtTextFileInfo	getTextFileInfo();
		void			setTextFileInfo( gtTextFileInfo info );
		
		u32				write( u8 * data, u32 size );
		void			write( const gtStringA& string );
		void			write( const gtString& string );
		void			write( const gtString32& string );
		void			flush();
		u64				read( u8 * data, u64 size );
		u64				size();
		u64				tell();
		void 			seek( u64 distance, gtFileSeekPos pos );
	};

}
#endif
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
