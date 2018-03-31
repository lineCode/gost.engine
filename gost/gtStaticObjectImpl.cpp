#include "common.h"

gtStaticObjectImpl::gtStaticObjectImpl( gtRenderModel* model ):
	m_type( gtObjectType::STATIC ),
	m_model( model )
{
	m_aabb = *m_model->getAabb();
	m_obb = *m_model->getObb();
}

gtStaticObjectImpl::~gtStaticObjectImpl( void ){
}


//	=================================					gtGameObject
		//	��������� ��� �������
gtObjectType		gtStaticObjectImpl::getType( void ){
	return m_type;
}


	//	������� ���������� � �������/��������/��������
void				gtStaticObjectImpl::update( void ){

	gtMatrix4 translationMatrix;
	math::makeTranslationMatrix( translationMatrix, m_position );

	math::makeRotationMatrix( m_rotationMatrix, m_orientation );

	gtMatrix4	scaleMatrix;
	math::makeScaleMatrix( scaleMatrix, m_scale );

	m_worldMatrix = translationMatrix * m_rotationMatrix * scaleMatrix;

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

	//	�������� ������ (���� �� �������� (�������� �� 3D �����))
void				gtStaticObjectImpl::render( void ){
	GT_ASSERT1( m_model, "m_model is not valid", "m_model != nullptr" );
	
	m_model->render();
}



	//	����� ������ ������� ������
gtRenderModel*	gtStaticObjectImpl::getModel( void ){
	return m_model;
}

gtAabb* gtStaticObjectImpl::getAabb( void ){
	return &m_aabb;
}

gtObb* gtStaticObjectImpl::getObb( void ){
	return &m_obb;
}