#include "common.h"

gtStaticObjectImpl::gtStaticObjectImpl( gtRenderModel* model ):
	m_scale(1.f),
	m_type( gtObjectType::STATIC ),
	m_model( model )
{
}

gtStaticObjectImpl::~gtStaticObjectImpl( void ){
}


//	=================================					gtGameObject
		//	Возвратит тип объекта
gtObjectType		gtStaticObjectImpl::getType( void ){
	return m_type;
}


		//	Возвратит масштаб
const v3f&			gtStaticObjectImpl::getScale( void ){
	return m_scale;
}

		//	Возвратит вращение
const gtQuaternion&	gtStaticObjectImpl::getQuaternion( void ){
	return m_quaternion;
}

		//	Установит масштаб
void				gtStaticObjectImpl::setScale( const v3f& scale ){
	m_scale = scale;
}

		//	Установит вращение
void				gtStaticObjectImpl::setQuaternion( const gtQuaternion& q ){
	m_quaternion = q;
	m_quaternion.normalize();
}

void				gtStaticObjectImpl::setRotation( const v3f& rotation ){
	if( m_old_rotation != rotation ){
		this->m_rotation = rotation; 

		v3f r =  rotation - m_old_rotation;

		gtQuaternion q(r);

		m_quaternion = q * m_quaternion;
		m_quaternion.normalize();

		m_old_rotation = rotation;
	}
}

const v3f&			gtStaticObjectImpl::getRotation( void ){
	return m_rotation;
}


	//	Обновит информацию о позиции/вращении/масштабе
void				gtStaticObjectImpl::update( void ){

	gtMatrix4 translationMatrix;
	math::makeTranslationMatrix( translationMatrix, m_position );

	gtMatrix4 rotationMatrix;
	math::makeRotationMatrix( rotationMatrix, m_quaternion );

	gtMatrix4	scaleMatrix;
	scaleMatrix[ 0u ] *= m_scale.x;
	scaleMatrix[ 1u ] *= m_scale.y;
	scaleMatrix[ 2u ] *= m_scale.z;

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

	//	Нарисует объект (если он рисуемый (например не 3D аудио))
void				gtStaticObjectImpl::render( void ){
	GT_ASSERT1( m_model, "m_model is not valid", "m_model != nullptr" );
	
	m_model->render();
}



	//	Вернёт модель которую рисует
gtRenderModel*	gtStaticObjectImpl::getModel( void ){
	return m_model;
}

gtAabb* gtStaticObjectImpl::getAabb( void ){
	return m_model->getAabb();
}