#pragma once
#ifndef __GT_TEXTURE_H__
#define __GT_TEXTURE_H__

namespace gost{

	enum gtTextureType : u32 {

		TEXTURE_TYPE_2D,

		TEXTURE_TYPE_CUBE
	};

	enum gtTextureFilterType : u32 {
		FILTER_PPP	= 0,				//< D3D11_FILTER_MIN_MAG_MIP_POINT
        FILTER_PPL	= 0x1,				//< D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR
        FILTER_PLP	= 0x4,				//< D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT
        FILTER_PLL	= 0x5,				//< D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR
        FILTER_LPP	= 0x10,				//< D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT
        FILTER_LPL	= 0x11,				//< D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR
        FILTER_LLP	= 0x14,				//< D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT
        FILTER_LLL	= 0x15,				//< D3D11_FILTER_MIN_MAG_MIP_LINEAR
        FILTER_ANISOTROPIC	= 0x55,		//< D3D11_FILTER_ANISOTROPIC
	};

	class gtTexture : public gtRefObject {
	public:

		virtual gtTextureType	getType( void ) = 0;

			// \return width
		virtual u32				getWidth( void ) = 0;

			// \return height
		virtual u32				getHeight( void ) = 0;

	};


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