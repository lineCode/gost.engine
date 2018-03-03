#include "common.h"

gtDummyObjectImpl::gtDummyObjectImpl( void ):
	m_type( gtObjectType::DUMMY )
{
}

gtDummyObjectImpl::~gtDummyObjectImpl( void ){
}

gtObjectType		gtDummyObjectImpl::getType( void ){
	return m_type;
}

gtAabb*				gtDummyObjectImpl::getAabb( void ){
	return nullptr;
}

void				gtDummyObjectImpl::update( void ){
	gtMatrix4 translationMatrix;
	math::makeTranslationMatrix( translationMatrix, m_position );

	gtMatrix4 rotationMatrix;
	math::makeRotationMatrix( rotationMatrix, m_orientation );

	gtMatrix4	scaleMatrix;
	scaleMatrix[ 0u ].fill(1.f);
	scaleMatrix[ 1u ].fill(1.f);
	scaleMatrix[ 2u ].fill(1.f);

	m_worldMatrix = translationMatrix * rotationMatrix * scaleMatrix;

	if( m_parent )
		m_worldMatrix = m_parent->getAbsoluteWorldMatrix() * m_worldMatrix;

	m_worldMatrixAbsolute = m_worldMatrix;


	auto * childs = &getChildList();
	{
		auto it = childs->begin();
		for(; it != childs->end(); ++it){
			(*it)->update();
		}
	}

}

void				gtDummyObjectImpl::render( void ){}