#pragma once
#ifndef __GT_STATIC_OBJECT_IMPL_H__
#define __GT_STATIC_OBJECT_IMPL_H__

namespace gost{

	class gtStaticObjectImpl : public gtStaticObject{
		
		gtObjectType	m_type;

		gtRenderModel*	m_model;

	public:

		gtStaticObjectImpl( gtRenderModel* model );
		~gtStaticObjectImpl( void );


		//	=================================					gtGameObject
			//	��������� ��� �������
		gtObjectType		getType( void );
		
			//	������� ���������� � �������/��������/��������
		void				update( void );

			//	�������� ������ (���� �� �������� (�������� �� 3D �����))
		void				render( void );
		

					//	����� ������ ������� ������
		gtRenderModel*	getModel( void );

		gtAabb*				getAabb( void );
		gtObb*				getObb( void );
	};

}

#endif

