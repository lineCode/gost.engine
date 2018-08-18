#include "common.h"


gtRenderModelD3D11::gtRenderModelD3D11( gtDriverD3D11* d ):
	//m_sModel( nullptr ),
	m_gs( d ),
	m_lockedResource( nullptr ),
	m_stride( gtConst0U )
{}

gtRenderModelD3D11::~gtRenderModelD3D11(){
	unlock();
	
	auto sz = m_vBuffers.size();
	for( auto i = gtConst0U; i < sz; ++i )
		m_vBuffers[ i ]->Release();

	sz = m_iBuffers.size();
	for( auto i = 0u; i < sz; ++i )
		m_iBuffers[ i ]->Release();
}

bool	gtRenderModelD3D11::init( gtModel* m, gtRenderModelInfo * info ){

	if( info )
		m_info = *info;
	
	//m_sModel = m;

	D3D11_BUFFER_DESC	vbd, ibd;
	ZeroMemory( &vbd, sizeof( D3D11_BUFFER_DESC ) );
	ZeroMemory( &ibd, sizeof( D3D11_BUFFER_DESC ) );

	vbd.Usage		=	D3D11_USAGE_DEFAULT;
	vbd.BindFlags	=	D3D11_BIND_VERTEX_BUFFER;
	
	ibd.Usage		=	D3D11_USAGE_DEFAULT;
	ibd.BindFlags	=	D3D11_BIND_INDEX_BUFFER;

	if( info ){
		if( m_info.m_dynamic ){
			vbd.Usage		=	D3D11_USAGE_DYNAMIC;
			vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			ibd.Usage		=	D3D11_USAGE_DYNAMIC;
			ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
	}
	
	D3D11_SUBRESOURCE_DATA	vData, iData;
	ZeroMemory( &vData, sizeof( D3D11_SUBRESOURCE_DATA ) );
	ZeroMemory( &iData, sizeof( D3D11_SUBRESOURCE_DATA ) );

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
		iData.pSysMem	=	&sub->m_indices[ gtConst0U ];

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
	this->m_gs->drawModel( this, nullptr );
}

gtAabb* gtRenderModelD3D11::getAabb(){
	return &m_aabb;
}

gtObb* gtRenderModelD3D11::getObb(){
	return &m_obb;
}

bool gtRenderModelD3D11::lock( u32 id, void * ptr, lock_type type ){
	if( m_lockedResource ){
		gtLogWriter::printWarning( u"Can not lock D3D11 render model buffer. Model is locked." );
		return false;
	}
	
	if( !m_info.m_dynamic ){
		gtLogWriter::printWarning( u"Can not lock D3D11 render model buffer. Model is not dynamic." );
		return false;
	}

	if( id >= m_subs.size() ){
		gtLogWriter::printWarning( u"Can not lock D3D11 render model buffer. Bad index (id >= m_subs.size())." );
		return false;
	}
	
	ID3D11Buffer* d3dbuffer = nullptr;
	
	if( type == gtRenderModel::lock_type::vertex )
		d3dbuffer = m_vBuffers[ id ];
	else
		d3dbuffer = m_iBuffers[ id ];
	
	D3D11_MAPPED_SUBRESOURCE mapData;
	auto hr = m_gs->getD3DDeviceContext()->Map(
		d3dbuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mapData
	);
	
	m_lockedResource = d3dbuffer;
	
	return true;
}

void gtRenderModelD3D11::unlock(){
	if( m_lockedResource ){
		m_gs->getD3DDeviceContext()->Unmap( m_lockedResource, 0 );
		m_lockedResource = nullptr;
	}
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