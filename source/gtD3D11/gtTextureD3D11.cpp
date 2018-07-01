//	GOST

#include "common.h"

gtTextureD3D11::gtTextureD3D11( gtDriverD3D11* d ):
	m_type( gtTextureType::Texture_2D ),
	m_texture( nullptr ),
	m_textureResView( nullptr ),
	m_samplerState( nullptr ),
	m_gs( d )
{}

gtTextureD3D11::~gtTextureD3D11(){

	if( m_samplerState ){
		m_samplerState->Release();
		m_samplerState = nullptr;
	}

	if( m_textureResView ){
		m_textureResView->Release();
		m_textureResView = nullptr;
	}

	if( m_texture ){
		m_texture->Release();
		m_texture = nullptr;
	}
}

bool gtTextureD3D11::init( gtImage* image, gtTextureFilterType filter ){

	if( !image ) return false;

	image->convert( gtImageFormat::R8G8B8A8 );

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory( &desc, sizeof( desc ) );
	desc.Width	=	image->width;
	desc.Height	=	image->height;
	desc.Format	=	DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count	=	1;
	desc.SampleDesc.Quality	=	0;
	desc.BindFlags	=	D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.MiscFlags	=	D3D11_RESOURCE_MISC_GENERATE_MIPS;
	desc.ArraySize	=	1;
	desc.MipLevels	=	1;
	desc.Usage	=	D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags	=	0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory( &initData, sizeof( initData ) );
	initData.pSysMem	 =	image->data;
	initData.SysMemPitch =	image->pitch;
	initData.SysMemSlicePitch = image->dataSize;

	ID3D11Texture2D * texture = nullptr;

	HRESULT hr = m_gs->getD3DDevice()->CreateTexture2D( &desc, &initData, &texture );
	if( FAILED( hr ) ){
		gtLogWriter::printWarning( u"Can't create 2D texture" );
		return false;
	}

	m_texture = texture;

	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory( &SRVDesc, sizeof( SRVDesc ) );
	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MostDetailedMip	=	0;
	SRVDesc.Texture2D.MipLevels = -1;

	hr = m_gs->getD3DDevice()->CreateShaderResourceView( m_texture,
			&SRVDesc, &this->m_textureResView );
	if( FAILED( hr ) ){
		gtLogWriter::printWarning( u"Can't create shader resource view" );
		return false;
	}

	hr = this->createSamplerState( (D3D11_FILTER)filter, D3D11_TEXTURE_ADDRESS_WRAP, 16u );
	if( FAILED( hr ) ){
		gtLogWriter::printWarning( u"Can't create sampler state" );
		return false;
	}

	m_size.x = image->width;
	m_size.y = image->height;

	return true;
}

ID3D11ShaderResourceView * const * gtTextureD3D11::getResourceView(){
	return &this->m_textureResView;
}

ID3D11SamplerState*	const * gtTextureD3D11::getSamplerState(){
	return &this->m_samplerState;
}

HRESULT	gtTextureD3D11::createSamplerState( D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, u32 anisotropic_level ){
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory( &samplerDesc, sizeof(samplerDesc) );
	samplerDesc.Filter = filter;

	samplerDesc.AddressU = addressMode;
	samplerDesc.AddressV = addressMode;
	samplerDesc.AddressW = addressMode;

	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MaxLOD = FLT_MAX;

	samplerDesc.MaxAnisotropy = anisotropic_level;

	return m_gs->getD3DDevice()->CreateSamplerState( &samplerDesc, &this->m_samplerState );
}

//============================================
gtTextureType	gtTextureD3D11::getType(){
	return m_type;
}
u32				gtTextureD3D11::getWidth(){
	return this->m_size.x;
}
u32				gtTextureD3D11::getHeight(){
	return this->m_size.y;
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