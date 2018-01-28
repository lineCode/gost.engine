#pragma once
#ifndef __GT_STATIC_OBJECT_IMPL_H__
#define __GT_STATIC_OBJECT_IMPL_H__

namespace gost{

	class gtStaticObjectImpl : public gtStaticObject{

		gtQuaternion	m_quaternion;
		v3f				m_rotation, m_old_rotation;

			//	масштаб
		v3f				m_scale;

		gtObjectType	m_type;

		gtRenderModel*	m_model;

	public:

		gtStaticObjectImpl( gtRenderModel* model );
		~gtStaticObjectImpl( void );


		//	=================================					gtGameObject
			//	Возвратит тип объекта
		gtObjectType		getType( void );
		
			//	Возвратит масштаб
		const v3f&			getScale( void );

			//	Возвратит вращение
		const v3f&	getRotation( void );

			//	Установит масштаб
		void				setScale( const v3f& );

			//	Установит вращение
		void				setRotation( const v3f& );

		void			setQuaternion( const gtQuaternion& quaternion );

		const gtQuaternion&			getQuaternion( void );

			//	Обновит информацию о позиции/вращении/масштабе
		void				update( void );

			//	Нарисует объект (если он рисуемый (например не 3D аудио))
		void				render( void );
		

					//	Вернёт модель которую рисует
		gtRenderModel*	getModel( void );
	};

}

#endif

