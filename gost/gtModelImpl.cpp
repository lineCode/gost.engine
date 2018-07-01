#include "common.h"

gtModelImpl::gtModelImpl( u32 s, gtVertexType* ta ):
	m_stride( s )
{
	u32 sz = 0u;
	gtVertexType * vt = &ta[ 0u ];
	while( *vt++ != gtVertexType::End ) sz++;

	m_typeArray = new gtVertexType[ sz + 1u ];

	memcpy( m_typeArray, ta, (sz + 1u) * sizeof(gtVertexType) );

}

gtModelImpl::~gtModelImpl(){

	auto sz = m_submodels.size();
	for( u32 i = 0u; i < sz; ++i ){
		delete m_submodels[ i ];
	}

	if( m_typeArray )
		delete []m_typeArray;
}


gtSubModel*	gtModelImpl::createSubModel( u32 v_count, u32 i_count, u32 stride ){
	gtSubModel* subModel = new gtSubModel;
	
	if( !subModel ) return nullptr;

	subModel->m_iCount = i_count;
	subModel->m_vCount = v_count;
	
	subModel->allocate( stride );
	
	if( !subModel->m_vertices ){
		delete subModel;
		return nullptr;
	}

	return subModel;
}


gtSubModel*	gtModelImpl::addSubModel( u32 v_count, u32 i_count, u32 s ){
	
	gtSubModel* subModel = createSubModel( v_count, i_count, s );
	
	if( subModel )
		m_submodels.push_back( subModel );

	return subModel;
}


gtSubModel*	gtModelImpl::addSubModel( gtSubModel* s ){

	gtSubModel* subModel = createSubModel( s->m_vCount, s->m_iCount, s->m_stride );
	
	if( subModel ){

		memcpy( subModel->m_vertices, s->m_vertices, subModel->m_vCount * subModel->m_stride );
		memcpy( subModel->m_indices, s->m_indices, subModel->m_iCount * sizeof( u16 ) );

		subModel->m_aabb = s->m_aabb;
		subModel->m_obb = s->m_obb;
		subModel->m_material = s->m_material;
		subModel->m_name = s->m_name;
		

		m_submodels.push_back( subModel );
	}

	return subModel;
}


gtSubModel*	gtModelImpl::getSubModel( u32 id ){
	return m_submodels[id];
}


u32		gtModelImpl::getSubmodelsCount(){
	return m_submodels.size();
}


u32		gtModelImpl::getStride(){
	return m_stride;
}


gtVertexType*	gtModelImpl::getTypeArray(){
	return &m_typeArray[0u];
}


const gtAabb&	gtModelImpl::getAabb(){
	return m_aabb;
}


void			gtModelImpl::setAabb( const gtAabb& box ){
	m_aabb = box;
}


void			gtModelImpl::updateBoundingVolume(){
	u32 sz = m_submodels.size();
	if( sz ){

		m_aabb.reset();
		m_obb.reset();

		auto msz = m_submodels.size();
		for( u32 i = 0u; i < msz; ++i ){
			m_submodels[ i ]->buildObb();
			m_obb.add(m_submodels[ i ]->m_obb);
			m_submodels[ i ]->m_obb.calculateBaseInfo();
		}
		m_obb.calculateBaseInfo();

		for( u32 i = 0u; i < msz; ++i ){
			m_submodels[ i ]->updateAabb();
			m_aabb.add( m_submodels[ i ]->m_aabb );
		}
	}
}

const gtObb&	gtModelImpl::getObb(){
	return m_obb;
}

void			gtModelImpl::setObb( const gtObb& obb ){
	m_obb = obb;
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