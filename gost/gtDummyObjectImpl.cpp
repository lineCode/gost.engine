#include "common.h"

gtDummyObjectImpl::gtDummyObjectImpl( void ):
	m_type( gtObjectType::DUMMY )
{
}

gtDummyObjectImpl::~gtDummyObjectImpl( void ){
}

void gtDummyObjectImpl::setQuaternion( const gtQuaternion& quaternion ){
	m_quaternion = quaternion;
	m_quaternion.normalize();
}

const gtQuaternion&			gtDummyObjectImpl::getQuaternion( void ){
	return m_quaternion;
}

gtObjectType		gtDummyObjectImpl::getType( void ){
	return m_type;
}


const v3f&			gtDummyObjectImpl::getRotation( void ){
	return m_rotation;
}

void				gtDummyObjectImpl::setRotation( const v3f& rotation ){
	if( m_old_rotation != rotation ){
		this->m_rotation = rotation; 

		v3f r =  rotation - m_old_rotation;

		gtQuaternion q(r);

		m_quaternion = q * m_quaternion;
		m_quaternion.normalize();

		m_old_rotation = rotation;
	}
}

gtAabb*				gtDummyObjectImpl::getAabb( void ){
	return nullptr;
}

void				gtDummyObjectImpl::update( void ){
	gtMatrix4 translationMatrix;
	math::makeTranslationMatrix( translationMatrix, m_position );

	gtMatrix4 rotationMatrix;
	math::makeRotationMatrix( rotationMatrix, m_quaternion );

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