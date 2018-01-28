#pragma once
#ifndef __GT_STATIC_OBJECT_IMPL_H__
#define __GT_STATIC_OBJECT_IMPL_H__

namespace gost{

	class gtStaticObjectImpl : public gtStaticObject{

		gtQuaternion	m_quaternion;
		v3f				m_rotation, m_old_rotation;

			//	�������
		v3f				m_scale;

		gtObjectType	m_type;

		gtRenderModel*	m_model;

	public:

		gtStaticObjectImpl( gtRenderModel* model );
		~gtStaticObjectImpl( void );


		//	=================================					gtGameObject
			//	��������� ��� �������
		gtObjectType		getType( void );
		
			//	��������� �������
		const v3f&			getScale( void );

			//	��������� ��������
		const v3f&	getRotation( void );

			//	��������� �������
		void				setScale( const v3f& );

			//	��������� ��������
		void				setRotation( const v3f& );

		void			setQuaternion( const gtQuaternion& quaternion );

		const gtQuaternion&			getQuaternion( void );

			//	������� ���������� � �������/��������/��������
		void				update( void );

			//	�������� ������ (���� �� �������� (�������� �� 3D �����))
		void				render( void );
		

					//	����� ������ ������� ������
		gtRenderModel*	getModel( void );
	};

}

#endif

