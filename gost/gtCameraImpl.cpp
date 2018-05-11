#include "common.h"

gtCameraImpl::gtCameraImpl():
	m_up( 0.f, 1.f, 0.f ),
	m_fov( 0.785398185f ),
	m_near( 1.f ),
	m_far( 100.f ),
	m_aspect( 1.333333333f ),
	m_type( gtObjectType::CAMERA ),
	m_cameraType( gtCameraType::CT_LOOK_AT )
{}

gtCameraImpl::~gtCameraImpl(){
}


//	=================================					gtGameObject
gtObjectType		gtCameraImpl::getType(){
	return m_type;
}

void				gtCameraImpl::update(){

	gtMatrix4 translationMatrix;
	math::makeTranslationMatrix( translationMatrix, -m_position );

	m_worldMatrix = translationMatrix;

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

void				gtCameraImpl::render(){
	switch( m_cameraType ){
	case gost::gtCameraType::CT_LOOK_AT:{
		math::makePerspectiveRHMatrix(
			m_projectionMatrix,
			m_fov,
			m_aspect,
			m_near,
			m_far );
		math::makeLookAtRHMatrix(
			m_viewMatrix,
			m_position,
			m_target,
			m_up
		);
	}break;

	case gost::gtCameraType::CT_2D:{
		math::makeOrthoRHMatrix(
			m_projectionMatrix,
			m_viewPort.z * 0.02f * m_fov,
			m_viewPort.w * 0.02f * m_fov,
			m_near,
			m_far
		);

		gtQuaternion qPitch( v3f( m_rotation.x, 0.f, 0.f ) );
		gtQuaternion qYaw( v3f( 0.f, m_rotation.y, 0.f ) );
		gtQuaternion qRoll( v3f( 0.f, 0.f, m_rotation.z ) );

		m_orientation = qYaw * qPitch * qRoll;

		math::makeRotationMatrix( m_rotationMatrix, m_orientation );

		m_viewMatrix = m_rotationMatrix * m_worldMatrixAbsolute;

	}break;
	//case gost::gtCameraType::CT_FREE:
	case gost::gtCameraType::CT_FPS:{
		math::makePerspectiveRHMatrix(
			m_projectionMatrix,
			m_fov,
			m_aspect,
			m_near,
			m_far );

		gtQuaternion qPitch( v3f( m_rotation.x, 0.f, 0.f ) );
		gtQuaternion qYaw( v3f( 0.f, m_rotation.y, 0.f ) );
		gtQuaternion qRoll( v3f( 0.f, 0.f, m_rotation.z ) );

		m_orientation = qYaw * qPitch * qRoll;

		math::makeRotationMatrix( m_rotationMatrix, m_orientation );

		m_viewMatrix = m_rotationMatrix * m_worldMatrixAbsolute;

	}break;
	}

	calculateFrustum();
}


		//	====================================				gtCamera
gtCameraType	gtCameraImpl::getCameraType(){
	return m_cameraType;
}

void		gtCameraImpl::setCameraType( gtCameraType type ){
	m_cameraType = type;
}

const gtMatrix4&	gtCameraImpl::getViewMatrix(){
	return m_viewMatrix;
}

const gtMatrix4&	gtCameraImpl::getProjectionMatrix(){
	return m_projectionMatrix;
}

const v3f&			gtCameraImpl::getTarget(){
	return m_target;
}

void				gtCameraImpl::setTarget( const v3f& t ){
	m_target = t;
}

const v3f&			gtCameraImpl::getUpVector(){
	return m_up;
}

void gtCameraImpl::setUpVector( const v3f& v ){
	m_up = v;
}

void gtCameraImpl::setNear( f32 v ){
	m_near = v;
}

void gtCameraImpl::setFar( f32 v ){
	m_far = v;
}

void gtCameraImpl::setAspect( f32 v ){
	m_aspect = v;
}

void gtCameraImpl::setFOV( f32 v ){
	m_fov = v;
}

f32	gtCameraImpl::getNear(){
	return m_near;
}

f32	gtCameraImpl::getFar(){
	return m_far;
}

f32	gtCameraImpl::getAspect(){
	return m_aspect;
}

f32	gtCameraImpl::getFOV(){
	return m_fov;
}

void gtCameraImpl::setViewPort( const v4f& v ){
	m_viewPort = v;
}


gtCameraFrustum* gtCameraImpl::getFrustum(){
	return &m_frustum;
}

void gtCameraImpl::calculateFrustum(){

	f32 *proj = m_projectionMatrix.getPtr();
	f32 *modl = m_viewMatrix.getPtr();
	float   clip[16]; //clipping planes

	clip[0] = modl[0] * proj[0] + modl[1] * proj[4] + modl[2] * proj[8] + modl[3] * proj[12];
	clip[1] = modl[0] * proj[1] + modl[1] * proj[5] + modl[2] * proj[9] + modl[3] * proj[13];
	clip[2] = modl[0] * proj[2] + modl[1] * proj[6] + modl[2] * proj[10] + modl[3] * proj[14];
	clip[3] = modl[0] * proj[3] + modl[1] * proj[7] + modl[2] * proj[11] + modl[3] * proj[15];

	clip[4] = modl[4] * proj[0] + modl[5] * proj[4] + modl[6] * proj[8] + modl[7] * proj[12];
	clip[5] = modl[4] * proj[1] + modl[5] * proj[5] + modl[6] * proj[9] + modl[7] * proj[13];
	clip[6] = modl[4] * proj[2] + modl[5] * proj[6] + modl[6] * proj[10] + modl[7] * proj[14];
	clip[7] = modl[4] * proj[3] + modl[5] * proj[7] + modl[6] * proj[11] + modl[7] * proj[15];

	clip[8] = modl[8] * proj[0] + modl[9] * proj[4] + modl[10] * proj[8] + modl[11] * proj[12];
	clip[9] = modl[8] * proj[1] + modl[9] * proj[5] + modl[10] * proj[9] + modl[11] * proj[13];
	clip[10] = modl[8] * proj[2] + modl[9] * proj[6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[8] * proj[3] + modl[9] * proj[7] + modl[10] * proj[11] + modl[11] * proj[15];

	clip[12] = modl[12] * proj[0] + modl[13] * proj[4] + modl[14] * proj[8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[1] + modl[13] * proj[5] + modl[14] * proj[9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[2] + modl[13] * proj[6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[3] + modl[13] * proj[7] + modl[14] * proj[11] + modl[15] * proj[15];


	m_frustum.m_planes[ 0u ].x = clip[ 3u ] - clip[ 0u ];
	m_frustum.m_planes[ 0u ].y = clip[ 7u ] - clip[ 4u ];
	m_frustum.m_planes[ 0u ].z = clip[ 11u ] - clip[ 8u ];
	m_frustum.m_planes[ 0u ].w = clip[ 15u ] - clip[ 12u ];
	m_frustum.m_planes[ 0u ].normalize();

	m_frustum.m_planes[ 1u ].x = clip[ 3u ] + clip[ 0u ];
	m_frustum.m_planes[ 1u ].y = clip[ 7u ] + clip[ 4u ];
	m_frustum.m_planes[ 1u ].z = clip[ 11u ] + clip[ 8u ];
	m_frustum.m_planes[ 1u ].w = clip[ 15u ] + clip[ 12u ];
	m_frustum.m_planes[ 1u ].normalize();

	m_frustum.m_planes[ 2u ].x = clip[ 3u ] + clip[ 1u ];
	m_frustum.m_planes[ 2u ].y = clip[ 7u ] + clip[ 5u ];
	m_frustum.m_planes[ 2u ].z = clip[ 11u ] + clip[ 9u ];
	m_frustum.m_planes[ 2u ].w = clip[ 15u ] + clip[ 13u ];
	m_frustum.m_planes[ 2u ].normalize();

	m_frustum.m_planes[ 3u ].x = clip[ 3u ] - clip[ 1u ];
	m_frustum.m_planes[ 3u ].y = clip[ 7u ] - clip[ 5u ];
	m_frustum.m_planes[ 3u ].z = clip[ 11u ] - clip[ 9u ];
	m_frustum.m_planes[ 3u ].w = clip[ 15u ] - clip[ 13u ];
	m_frustum.m_planes[ 3u ].normalize();

	m_frustum.m_planes[ 4u ].x = clip[ 3u ] - clip[ 2u ];
	m_frustum.m_planes[ 4u ].y = clip[ 7u ] - clip[ 6u ];
	m_frustum.m_planes[ 4u ].z = clip[ 11u ] - clip[ 10u ];
	m_frustum.m_planes[ 4u ].w = clip[ 15u ] - clip[ 14u ];
	m_frustum.m_planes[ 4u ].normalize();

	m_frustum.m_planes[ 5u ].x = clip[ 3u ] + clip[ 2u ];
	m_frustum.m_planes[ 5u ].y = clip[ 7u ] + clip[ 6u ];
	m_frustum.m_planes[ 5u ].z = clip[ 11u ] + clip[ 10u ];
	m_frustum.m_planes[ 5u ].w = clip[ 15u ] + clip[ 14u ];
	m_frustum.m_planes[ 5u ].normalize();

}

gtAabb*	gtCameraImpl::getAabb(){
	return nullptr;
}
gtObb*	gtCameraImpl::getObb(){
	return nullptr;
}

/*
Copyright (c) 2018 532235

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/