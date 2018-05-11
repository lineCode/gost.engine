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


		void			calculateFrustum();

	public:

		gtCameraImpl();
		~gtCameraImpl();

		gtCameraFrustum* getFrustum();


		//	=================================					gtGameObject
		gtObjectType		getType();
		
		void				update();

		void				render();
		
		//	====================================				gtCamera
		 gtCameraType		getCameraType();
		void				setCameraType( gtCameraType type );
		const gtMatrix4&	getViewMatrix();
		const gtMatrix4&	getProjectionMatrix();
		const v3f&			getTarget();
		void				setTarget( const v3f& );
		const v3f&			getUpVector();
		void				setUpVector( const v3f& );
		void				setNear( f32 );
		void				setFar( f32 );
		void				setAspect( f32 );
		void				setFOV( f32 );
		f32					getNear();
		f32					getFar();
		f32					getAspect();
		f32					getFOV();
		void				setViewPort( const v4f& v );
		//void					setZoom( f32 v );

		gtAabb*				getAabb();
		gtObb*				getObb();
	};

}

#endif

