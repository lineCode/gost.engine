#include "common.h"

gtStaticObjectImpl::gtStaticObjectImpl( gtRenderModel* model, gtGraphicsSystem* GS ):
	m_type( gtObjectType::Static ),
	m_model( model ),
	m_gs( GS )
{
	m_aabb = *m_model->getAabb();
	m_obb = *m_model->getObb();

	auto smc = m_model->getSubModelCount();
	for( auto i = 0; i < smc; ++i ){
		m_materials.push_back( *(m_model->getMaterial( (u32)i )) );
	}
}

gtStaticObjectImpl::~gtStaticObjectImpl()     {}
gtObjectType gtStaticObjectImpl::getType()    { return m_type; }
gtRenderModel* gtStaticObjectImpl::getModel() { return m_model; }
gtAabb* gtStaticObjectImpl::getAabb()         { return &m_aabb; }
gtObb* gtStaticObjectImpl::getObb()           { return &m_obb; }

gtMaterial& gtStaticObjectImpl::getMaterial( u32 i ){ return m_materials[ i ]; }
u32 gtStaticObjectImpl::getMaterialCount(){ return m_materials.size(); }

void gtStaticObjectImpl::update(){

	gtMatrix4 translationMatrix;
	math::makeTranslationMatrix( m_position, translationMatrix );

	math::makeRotationMatrix( m_rotationMatrix, m_orientation );

	gtMatrix4	scaleMatrix;
	math::makeScaleMatrix( m_scale, scaleMatrix );

	m_worldMatrix = translationMatrix * m_rotationMatrix * scaleMatrix;

	m_positionInSpace = m_position;

	if( m_parent ){
		m_worldMatrix = m_parent->getAbsoluteWorldMatrix() * m_worldMatrix;
		m_positionInSpace += m_parent->getPositionInSpace();
	}

	m_worldMatrixAbsolute = m_worldMatrix;

	auto * childs = &getChildList();
	{
		auto it = childs->begin();
		for(; it != childs->end(); ++it){
			(*it)->update();
		}
	}

	
}

void gtStaticObjectImpl::render(){
	m_gs->drawModel( m_model, &m_materials );
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