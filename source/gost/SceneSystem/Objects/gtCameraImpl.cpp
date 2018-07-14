#include "common.h"

gtCameraImpl::gtCameraImpl():
	m_updateCallback( nullptr ),
	m_up( 0.f, 1.f, 0.f ),
	m_fov( 0.785398185f ),
	m_near( 1.f ),
	m_far( 100.f ),
	m_aspect( 1.333333333f ),
	m_type( gtObjectType::Camera ),
	m_cameraType( gtCameraType::LookAt )
{}

gtCameraImpl::~gtCameraImpl()                   {}
gtAabb*	gtCameraImpl::getAabb()                 { return nullptr;            }
f32	gtCameraImpl::getAspect()                   { return m_aspect;           }
gtCameraType gtCameraImpl::getCameraType()      { return m_cameraType;       }
f32	gtCameraImpl::getFar()                      { return m_far;              }
f32	gtCameraImpl::getFOV()                      { return m_fov;              }
gtCameraFrustum* gtCameraImpl::getFrustum()     { return &m_frustum;         }
f32	gtCameraImpl::getNear()                     { return m_near;             }
gtObb*	gtCameraImpl::getObb()                  { return nullptr;            }
const gtMatrix4& 
             gtCameraImpl::getProjectionMatrix(){ return m_projectionMatrix; }
const v4f& gtCameraImpl::getTarget()            { return m_target;           }
gtObjectType gtCameraImpl::getType()            { return m_type;             }
const v4f& gtCameraImpl::getUpVector()          { return m_up;               }
const gtMatrix4& gtCameraImpl::getViewMatrix()  { return m_viewMatrix;       }
void gtCameraImpl::setAspect( f32 v )           { m_aspect = v;              }
void gtCameraImpl::setCameraType( gtCameraType type ){ m_cameraType = type;  }
void gtCameraImpl::setFar( f32 v )              { m_far = v;                 }
void gtCameraImpl::setFOV( f32 v )              { m_fov = v;                 }
void gtCameraImpl::setNear( f32 v )             { m_near = v;                }
void gtCameraImpl::setTarget( const v4f& t )    { m_target = t;              }
void gtCameraImpl::setViewPort( const v4f& v )  { m_viewPort = v;            }
void gtCameraImpl::setUpVector( const v4f& v )  { m_up = v;                  }
void gtCameraImpl::setViewMatrix( const gtMatrix4& m ){ m_viewMatrix = m;    }
void gtCameraImpl::setProjectionMatrix( const gtMatrix4& m ){ m_projectionMatrix = m; }

void gtCameraImpl::update(){

	gtMatrix4 translationMatrix;
	math::makeTranslationMatrix( -m_position, translationMatrix );

	m_worldMatrix = translationMatrix;

	m_positionInSpace = m_position;

	if( m_parent ){
		m_worldMatrix = m_parent->getAbsoluteWorldMatrix() * m_worldMatrix;
		m_positionInSpace += m_parent->getPositionInSpace();
	}

	m_worldMatrixAbsolute = m_worldMatrix;

	auto * childs = &getChildList();
	{
		auto it = childs->begin();
		for(; it != childs->end(); ++it){
			(*it)->update();
		}
	}
}

void gtCameraImpl::render(){
	switch( m_cameraType ){
	case gost::gtCameraType::Custom:{
		if( m_updateCallback ){
			m_updateCallback( this );
		}else{
			gtLogWriter::printWarning( u"Not set callback function for `Custom` camera" );
		}
	}break;
	case gost::gtCameraType::LookAt:{
		math::makePerspectiveRHMatrix(
			m_projectionMatrix,
			m_fov,
			m_aspect,
			m_near,
			m_far );
		math::makeLookAtRHMatrix(
			m_position,
			m_target,
			m_up,
			m_viewMatrix
		);
	}break;

	case gost::gtCameraType::Camera_2D:{
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
	case gost::gtCameraType::Free:{
		math::makePerspectiveRHMatrix(
			m_projectionMatrix,
			m_fov,
			m_aspect,
			m_near,
			m_far );

		/* Microsoft XNA Math

		v4f Angles( m_rotation.x, m_rotation.y, m_rotation.z, 0.f );
		v4f HalfAngles = Angles * 0.5f;
		v4f SinAngles = HalfAngles.sin();
		v4f CosAngles = HalfAngles.cos();
		
		v4f P0, P1, Y0, Y1, R0, R1;

		P0 = v4f( SinAngles.x, CosAngles.x, CosAngles.x, CosAngles.x );
		Y0 = v4f( CosAngles.y, SinAngles.y, CosAngles.y, CosAngles.y );
		R0 = v4f( CosAngles.z, CosAngles.z, SinAngles.z, CosAngles.z );

		P1 = v4f( CosAngles.x, SinAngles.x, SinAngles.x, SinAngles.x );
		Y1 = v4f( SinAngles.y, CosAngles.y, SinAngles.y, SinAngles.y );
		R1 = v4f( SinAngles.z, SinAngles.z, CosAngles.z, SinAngles.z );

		v4f Q1, Q0, Q;

		Q1 = P1 * v4f( 1.f, -1.f, -1.f, 1.f );
		Q0 = P0 * Y0;
		Q1 = Q1 * Y1;
		Q0 = Q0 * R0;
		Q  = (Q1 * R1) + Q0;
		
		m_orientation.set( Q.x, Q.y, Q.z, Q.w );
		*/


		gtQuaternion qPitch( v3f( m_rotation.x, 0.f, 0.f ) );
		gtQuaternion qYaw  ( v3f( 0.f, m_rotation.y, 0.f ) );
		gtQuaternion qRoll ( v3f( 0.f, 0.f, m_rotation.z ) );

		// I forgot how I did before. :(
	//	m_orientation = qYaw * qPitch * qRoll; //fps...
	//	math::makeRotationMatrix( m_rotationMatrix, m_orientation );

		gtMatrix4 R;
		gtMatrix4 P;
		gtMatrix4 Y;
		math::makeRotationMatrix( R, qRoll );
		math::makeRotationMatrix( P, qPitch );
		math::makeRotationMatrix( Y, qYaw );

		m_rotationMatrix = R * P * Y;

		m_viewMatrix = m_rotationMatrix * m_worldMatrixAbsolute;


	}break;
	case gost::gtCameraType::FPS:{
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
		m_orientation.normalize();

		math::makeRotationMatrix( m_rotationMatrix, m_orientation );

		m_viewMatrix = m_rotationMatrix * m_worldMatrixAbsolute;

	}break;
	}

	calculateFrustum();
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


	m_frustum.m_planes[ 0u ].x = clip[ 3 ] - clip[ 0 ];
	m_frustum.m_planes[ 0u ].y = clip[ 7 ] - clip[ 4 ];
	m_frustum.m_planes[ 0u ].z = clip[ 11u ] - clip[ 8 ];
	m_frustum.m_planes[ 0u ].w = clip[ 15u ] - clip[ 12u ];
	m_frustum.m_planes[ 0u ].normalize();

	m_frustum.m_planes[ 1 ].x = clip[ 3 ] + clip[ 0 ];
	m_frustum.m_planes[ 1 ].y = clip[ 7 ] + clip[ 4 ];
	m_frustum.m_planes[ 1 ].z = clip[ 11u ] + clip[ 8 ];
	m_frustum.m_planes[ 1 ].w = clip[ 15u ] + clip[ 12u ];
	m_frustum.m_planes[ 1 ].normalize();

	m_frustum.m_planes[ 2 ].x = clip[ 3 ] + clip[ 1 ];
	m_frustum.m_planes[ 2 ].y = clip[ 7 ] + clip[ 5 ];
	m_frustum.m_planes[ 2 ].z = clip[ 11u ] + clip[ 9 ];
	m_frustum.m_planes[ 2 ].w = clip[ 15u ] + clip[ 13u ];
	m_frustum.m_planes[ 2 ].normalize();

	m_frustum.m_planes[ 3 ].x = clip[ 3 ] - clip[ 1 ];
	m_frustum.m_planes[ 3 ].y = clip[ 7 ] - clip[ 5 ];
	m_frustum.m_planes[ 3 ].z = clip[ 11u ] - clip[ 9 ];
	m_frustum.m_planes[ 3 ].w = clip[ 15u ] - clip[ 13u ];
	m_frustum.m_planes[ 3 ].normalize();

	m_frustum.m_planes[ 4 ].x = clip[ 3 ] - clip[ 2 ];
	m_frustum.m_planes[ 4 ].y = clip[ 7 ] - clip[ 6 ];
	m_frustum.m_planes[ 4 ].z = clip[ 11u ] - clip[ 10u ];
	m_frustum.m_planes[ 4 ].w = clip[ 15u ] - clip[ 14u ];
	m_frustum.m_planes[ 4 ].normalize();

	m_frustum.m_planes[ 5 ].x = clip[ 3 ] + clip[ 2 ];
	m_frustum.m_planes[ 5 ].y = clip[ 7 ] + clip[ 6 ];
	m_frustum.m_planes[ 5 ].z = clip[ 11u ] + clip[ 10u ];
	m_frustum.m_planes[ 5 ].w = clip[ 15u ] + clip[ 14u ];
	m_frustum.m_planes[ 5 ].normalize();

}

void gtCameraImpl::setUpdateCallback( void(*callback)(gtCamera*) ){
	m_updateCallback = callback;
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