#pragma once
#ifndef __GT_CAMERA_IMPL_H__
#define __GT_CAMERA_IMPL_H__

namespace gost{

	class gtCameraImpl : public gtCamera{

		gtMatrix4		m_viewMatrix;
		gtMatrix4		m_rotationMatrix;
		gtMatrix4		m_projectionMatrix;

		v4f				m_viewPort;
		v3f				m_target;
		v3f				m_up;

		f32				m_fov;
		f32				m_near, m_far;
		f32				m_aspect;

		gtObjectType	m_type;
		gtCameraType	m_cameraType;
		
		gtCameraFrustum m_frustum;


		void			calculateFrustum( void );

	public:

		gtCameraImpl( void );
		~gtCameraImpl( void );

		gtCameraFrustum* getFrustum( void );


		//	=================================					gtGameObject
		gtObjectType		getType( void );
		
		void				update( void );

		void				render( void );
		
		//	====================================				gtCamera
		 gtCameraType		getCameraType( void );
		void				setCameraType( gtCameraType type );
		const gtMatrix4&	getViewMatrix( void );
		const gtMatrix4&	getProjectionMatrix( void );
		const v3f&			getTarget( void );
		void				setTarget( const v3f& );
		const v3f&			getUpVector( void );
		void				setUpVector( const v3f& );
		void				setNear( f32 );
		void				setFar( f32 );
		void				setAspect( f32 );
		void				setFOV( f32 );
		f32					getNear( void );
		f32					getFar( void );
		f32					getAspect( void );
		f32					getFOV( void );
		void				setViewPort( const v4f& v );
		//void					setZoom( f32 v );

		gtAabb*				getAabb( void );
		gtObb*				getObb( void );
	};

}

#endif

