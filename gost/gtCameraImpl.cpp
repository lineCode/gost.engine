#include "common.h"

gtCameraImpl::gtCameraImpl( void ):
	m_fov( 0.785398185f ),
	m_near( 1.f ),
	m_far( 100.f ),
	m_aspect( 1.333333333f ),
	m_type( gtObjectType::CAMERA ),
	m_up( 0.f, 1.f, 0.f ),
	m_cameraType( gtCameraType::CT_LOOK_AT )
{
#ifdef GT_DEBUG
	m_debugName.assign(u"gtCamera");
#endif
}

gtCameraImpl::~gtCameraImpl( void ){
}


//	=================================					gtGameObject
			//	Возвратит тип объекта
gtObjectType		gtCameraImpl::getType( void ){
	return m_type;
}


	//	Обновит информацию о позиции/вращении/масштабе
void				gtCameraImpl::update( void ){

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

	//	Нарисует объект (если он рисуемый (например не 3D аудио))
void				gtCameraImpl::render( void ){
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
	//	Вернёт тип камеры
gtCameraType	gtCameraImpl::getCameraType( void ){
	return m_cameraType;
}

	//	Установит тип камеры
void		gtCameraImpl::setCameraType( gtCameraType type ){
	m_cameraType = type;
}

const gtMatrix4&	gtCameraImpl::getViewMatrix( void ){
	return m_viewMatrix;
}

const gtMatrix4&	gtCameraImpl::getProjectionMatrix( void ){
	return m_projectionMatrix;
}

const v3f&			gtCameraImpl::getTarget( void ){
	return m_target;
}

void				gtCameraImpl::setTarget( const v3f& t ){
	m_target = t;
}

	//	Вернёт up вектор look at камеры
const v3f&			gtCameraImpl::getUpVector( void ){
	return m_up;
}

	//	Установит up вектор look at камеры
void gtCameraImpl::setUpVector( const v3f& v ){
	m_up = v;
}

	//	Установит ближнюю границу с которой начинается рисование
void gtCameraImpl::setNear( f32 v ){
	m_near = v;
}

	//	Установит дальнюю границу после которой рисование заканчивается
void gtCameraImpl::setFar( f32 v ){
	m_far = v;
}

	//	Установит соотношение сторон (напр. aspect = 800 : 600 )
void gtCameraImpl::setAspect( f32 v ){
	m_aspect = v;
}

	//	Установит поле зрения (field of view)
void gtCameraImpl::setFOV( f32 v ){
	m_fov = v;
}

	//	Вернёт ближнюю границу с которой начинается рисование
f32	gtCameraImpl::getNear( void ){
	return m_near;
}

	//	Вернёт дальнюю границу после которой рисование заканчивается
f32	gtCameraImpl::getFar( void ){
	return m_far;
}

	//	Вернёт соотношение сторон
f32	gtCameraImpl::getAspect( void ){
	return m_aspect;
}

	//	Вернёт поле зрения (field of view)
f32	gtCameraImpl::getFOV( void ){
	return m_fov;
}

void gtCameraImpl::setViewPort( const v4f& v ){
	m_viewPort = v;
}


gtCameraFrustum* gtCameraImpl::getFrustum( void ){
	return &m_frustum;
}

void gtCameraImpl::calculateFrustum( void ){

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

gtAabb*	gtCameraImpl::getAabb( void ){
	return nullptr;
}
gtObb*	gtCameraImpl::getObb( void ){
	return nullptr;
}