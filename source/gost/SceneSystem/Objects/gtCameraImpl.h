#pragma once
#ifndef __GT_CAMERA_IMPL_H__
#define __GT_CAMERA_IMPL_H__

namespace gost{

	class gtCameraImpl : public gtCamera{

		void(*m_updateCallback)(gtCamera*);

		gtMatrix4		m_viewMatrix;
		gtMatrix4		m_rotationMatrix;
		gtMatrix4		m_projectionMatrix;
		v4f				m_viewPort;
		v4f				m_target;
		v4f				m_up;
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

		gtAabb*				getAabb();
		f32					getAspect();
		gtCameraType		getCameraType();
		f32					getFar();
		f32					getFOV();
		gtCameraFrustum*    getFrustum();
		f32					getNear();
		gtObb*				getObb();
		const gtMatrix4&	getProjectionMatrix();
		const v4f&			getTarget();
		gtObjectType		getType();
		const v4f&			getUpVector();
		const gtMatrix4&	getViewMatrix();
		void				render();
		void				setAspect( f32 );
		void				setCameraType( gtCameraType type );
		void				setFar( f32 );
		void				setFOV( f32 );
		void				setNear( f32 );
		void				setTarget( const v4f& );
		void				setUpVector( const v4f& );
		void                setUpdateCallback( void(*callback)(gtCamera*) );
		void	            setViewMatrix( const gtMatrix4& );
		void	            setProjectionMatrix( const gtMatrix4& );
		void				setViewPort( const v4f& v );


		void				update();
	};

}

#endif

