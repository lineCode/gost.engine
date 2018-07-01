#include "common.h"


gtRenderModelD3D11::gtRenderModelD3D11( gtDriverD3D11* d ):
	//m_sModel( nullptr ),
	m_gs( d ),
	m_stride( 0u )
{}

gtRenderModelD3D11::~gtRenderModelD3D11(){
	auto sz = m_vBuffers.size();
	for( auto i = 0u; i < sz; ++i )
		m_vBuffers[ i ]->Release();

	sz = m_iBuffers.size();
	for( auto i = 0u; i < sz; ++i )
		m_iBuffers[ i ]->Release();
}

bool	gtRenderModelD3D11::init( gtModel* m ){

	//m_sModel = m;

	D3D11_BUFFER_DESC	vbd, ibd;
	ZeroMemory( &vbd, sizeof( D3D11_BUFFER_DESC ) );
	ZeroMemory( &ibd, sizeof( D3D11_BUFFER_DESC ) );

	vbd.Usage		=	D3D11_USAGE_DYNAMIC;
	vbd.BindFlags	=	D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	D3D11_SUBRESOURCE_DATA	vData, iData;
	ZeroMemory( &vData, sizeof( D3D11_SUBRESOURCE_DATA ) );
	ZeroMemory( &iData, sizeof( D3D11_SUBRESOURCE_DATA ) );

	ibd.Usage		=	D3D11_USAGE_DEFAULT;
	ibd.BindFlags	=	D3D11_BIND_INDEX_BUFFER;


	HRESULT	hr;

	u32 smc = m->getSubmodelsCount();

	m_stride = m->getStride();

	for( u32 i( 0u ); i < smc; ++i ){

		auto * sub = m->getSubModel( i );
		
		gtRenderModelD3D11_sub s;
		s.iCount = sub->m_iCount;
		s.vCount = sub->m_vCount;

		vbd.ByteWidth	=	m_stride * sub->m_vCount;
		
		vData.pSysMem = &sub->m_vertices[0];
		
		ID3D11Buffer* vBuffer = nullptr;

		hr = m_gs->getD3DDevice()->CreateBuffer( &vbd, &vData, &vBuffer );
		if( FAILED( hr ) ){
			gtLogWriter::printWarning( u"Can't create Direct3D 11 vertex buffer [%u]", hr );
			return false;
		}
		this->m_vBuffers.push_back( vBuffer );

		ibd.ByteWidth	=	sizeof( u16 ) * sub->m_iCount;
		iData.pSysMem	=	&sub->m_indices[ 0u ];

		ID3D11Buffer* iBuffer = nullptr;
		hr = m_gs->getD3DDevice()->CreateBuffer( &ibd, &iData, &iBuffer );
		if( FAILED( hr ) ){
			gtLogWriter::printWarning( u"Can't create Direct3D 11 index buffer [%u]", hr );
			return false;
		}
		this->m_iBuffers.push_back( iBuffer );

		s.material = sub->m_material;

		m_subs.push_back( s );
	}

	m_aabb = m->getAabb();
	m_obb  = m->getObb();

	return true;
}


u32 gtRenderModelD3D11::getSubModelCount(){
	return m_subs.size();
}

gtMaterial*	gtRenderModelD3D11::getMaterial( u32 id ){
	return &m_subs[ id ].material;
}

void		gtRenderModelD3D11::render(){
	this->m_gs->drawModel( this );
}

gtAabb* gtRenderModelD3D11::getAabb(){
	return &m_aabb;
}

gtObb* gtRenderModelD3D11::getObb(){
	return &m_obb;
}

/*
Copyright (c) 2018 532235

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