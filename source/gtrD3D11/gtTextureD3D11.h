//	GOST

#pragma once
#ifndef __GT_TEXTURE_D3D11_H__
#define __GT_TEXTURE_D3D11_H__


namespace gost{

	class gtTextureD3D11 GT_FINAL : public gtTexture{

		gtTextureType				m_type;
		ID3D11Resource*				m_texture;
		ID3D11ShaderResourceView*	m_textureResView;
		ID3D11SamplerState*			m_samplerState;
		ID3D11RenderTargetView*     m_RTV;

		gtDriverD3D11*				m_gs;

		HRESULT	createSamplerState( 
			D3D11_FILTER filter, 
			D3D11_TEXTURE_ADDRESS_MODE addressMode,
			u32 anisotropic_level = 1 );

		v2u		m_size;
	public:

		gtTextureD3D11( gtDriverD3D11* );
		virtual ~gtTextureD3D11();

		bool init( gtImage* image );
		bool initRTT( const v2u& size, gtImageFormat pixelFormat );

		ID3D11ShaderResourceView* const * getResourceView();
		ID3D11SamplerState*	const * getSamplerState();

		//	===========================================
		gtTextureType	getType();
		u32				getWidth();
		u32				getHeight();

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