#include "common.h"

gtDummyObjectImpl::gtDummyObjectImpl(): m_type( gtObjectType::Dummy ) {}
gtDummyObjectImpl::~gtDummyObjectImpl()   {}
gtObjectType gtDummyObjectImpl::getType() { return m_type; }
gtAabb* gtDummyObjectImpl::getAabb()      { return nullptr; }
gtObb* gtDummyObjectImpl::getObb()        { return nullptr; }
void gtDummyObjectImpl::render()          {}

void				gtDummyObjectImpl::update(){
	gtMatrix4 translationMatrix;
	math::makeTranslationMatrix( m_position, translationMatrix );

	gtMatrix4 rotationMatrix;
	math::makeRotationMatrix( rotationMatrix, m_orientation );

	gtMatrix4	scaleMatrix;
	scaleMatrix[ gtConst0U ].fill(1.f);
	scaleMatrix[ gtConst1U ].fill(1.f);
	scaleMatrix[ gtConst2U ].fill(1.f);

	m_worldMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	
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