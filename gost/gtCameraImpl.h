#pragma once
#ifndef __GT_CAMERA_IMPL_H__
#define __GT_CAMERA_IMPL_H__

namespace gost{

	class gtCameraImpl : public gtCamera{

		gtMatrix4		m_viewMatrix;
		gtMatrix4		m_rotationMatrix;
		gtMatrix4		m_projectionMatrix;

		v4f				m_viewPort;

		v3f				m_rotation;
		v3f				m_target;
		v3f				m_up;

		f32				m_fov;
		f32				m_near, m_far;
		f32				m_aspect;

		gtObjectType	m_type;
		gtCameraType	m_cameraType;
		
		gtCameraFrustum m_frustum;

		gtQuaternion	m_orientation;

		void			calculateFrustum( void );

	public:

		gtCameraImpl( void );
		~gtCameraImpl( void );

		gtCameraFrustum* getFrustum( void );


		//	=================================					gtGameObject
			//	Возвратит тип объекта
		gtObjectType		getType( void );
		
			//	Возвратит масштаб
		const v3f&			getScale( void );

			//	Возвратит вращение
		const v3f&			getRotation( void );

			//	Установит масштаб
		void				setScale( const v3f& );

			//	Установит вращение
		void				setRotation( const v3f& );

			//	Обновит информацию о позиции/вращении/масштабе
		void				update( void );

			//	Нарисует объект (если он рисуемый (например не 3D аудио))
		void				render( void );
		
		//	====================================				gtCamera
			//	Вернёт тип камеры
		 gtCameraType		getCameraType( void );

		 	//	Установит тип камеры
		void				setCameraType( gtCameraType type );

			//	Вернёт видовую матрицу
		 const gtMatrix4&	getViewMatrix( void );

			//	Вернут проекционную мтрицу
		 const gtMatrix4&	getProjectionMatrix( void );

			//	Вернёт цель если это look at камера
		 const v3f&			getTarget( void );

			//	Установит цель если это look at камера
		 void				setTarget( const v3f& );

		 	//	Вернёт up вектор look at камеры
		const v3f&			getUpVector( void );

			//	Установит up вектор look at камеры
		void				setUpVector( const v3f& );

			//	Установит ближнюю границу с которой начинается рисование
		 void				setNear( f32 );

			//	Установит дальнюю границу после которой рисование заканчивается
		 void				setFar( f32 );

			//	Установит соотношение сторон (напр. aspect = 800 : 600 )
		 void				setAspect( f32 );

			//	Установит поле зрения (field of view)
		 void				setFOV( f32 );

			//	Вернёт ближнюю границу с которой начинается рисование
		 f32					getNear( void );

			//	Вернёт дальнюю границу после которой рисование заканчивается
		f32					getFar( void );

			//	Вернёт соотношение сторон
		f32					getAspect( void );

			//	Вернёт поле зрения (field of view)
		f32					getFOV( void );

		void					setViewPort( const v4f& v );
		 	//	Для некоторых типов камер(2D камера), установит зум
		//void					setZoom( f32 v );

		gtAabb*				getAabb( void );
	};

}

#endif

