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
	/*v3f pos = getPositionInSpace();

	f32 _far = m_far * (m_fov*0.5f);
	f32 _near = m_near * (m_fov*0.5f);

	v3f farX  = v3f(  _far,  _far,   -m_far );
	v3f farY  = v3f( -_far,  -_far,  -m_far );
	v3f nearX = v3f(  _near,  _near, -m_near );
	v3f nearY = v3f( -_near, -_near, -m_near );

	switch( m_cameraType ){
	case gost::gtCameraType::CT_LOOK_AT:
		break;
	case gost::gtCameraType::CT_FPS:
	case gost::gtCameraType::CT_2D:
		m_frustum.m_farX  = math::mul( farX, m_rotationMatrix );
		m_frustum.m_farY  = math::mul( farY, m_rotationMatrix );
		m_frustum.m_nearX = math::mul( nearX,m_rotationMatrix );
		m_frustum.m_nearY = math::mul( nearY,m_rotationMatrix );
		break;
	}

	m_frustum.m_farY  -= pos;*/

	f32 zMinimum, r;

	zMinimum = m_projectionMatrix[ 3u ].z / m_projectionMatrix[ 2u ].z;
	r = m_far / ( m_far - zMinimum );
	//m_projectionMatrix[ 2u ].z = r;
	//m_projectionMatrix[ 3u ].z = r * zMinimum;

	gtMatrix4 matrix = m_viewMatrix * m_projectionMatrix;

	m_frustum.m_planes[ 0u ].x = matrix[ 0u ].w + matrix[ 0u ].z;
	m_frustum.m_planes[ 0u ].y = matrix[ 1u ].w + matrix[ 1u ].z;
	m_frustum.m_planes[ 0u ].z = matrix[ 2u ].w + matrix[ 2u ].z;
	m_frustum.m_planes[ 0u ].w = matrix[ 3u ].w + matrix[ 3u ].z;
	m_frustum.m_planes[ 0u ].normalize();

	m_frustum.m_planes[ 1u ].x = matrix[ 0u ].w - matrix[ 0u ].z;
	m_frustum.m_planes[ 1u ].y = matrix[ 1u ].w - matrix[ 1u ].z;
	m_frustum.m_planes[ 1u ].z = matrix[ 2u ].w - matrix[ 2u ].z;
	m_frustum.m_planes[ 1u ].w = matrix[ 3u ].w - matrix[ 3u ].z;
	m_frustum.m_planes[ 1u ].normalize();

	m_frustum.m_planes[ 2u ].x = matrix[ 0u ].w + matrix[ 0u ].x;
	m_frustum.m_planes[ 2u ].y = matrix[ 1u ].w + matrix[ 1u ].x;
	m_frustum.m_planes[ 2u ].z = matrix[ 2u ].w + matrix[ 2u ].x;
	m_frustum.m_planes[ 2u ].w = matrix[ 3u ].w + matrix[ 3u ].x;
	m_frustum.m_planes[ 2u ].normalize();

	m_frustum.m_planes[ 3u ].x = matrix[ 0u ].w - matrix[ 0u ].x;
	m_frustum.m_planes[ 3u ].y = matrix[ 1u ].w - matrix[ 1u ].x;
	m_frustum.m_planes[ 3u ].z = matrix[ 2u ].w - matrix[ 2u ].x;
	m_frustum.m_planes[ 3u ].w = matrix[ 3u ].w - matrix[ 3u ].x;
	m_frustum.m_planes[ 3u ].normalize();

	m_frustum.m_planes[ 4u ].x = matrix[ 0u ].w - matrix[ 0u ].y;
	m_frustum.m_planes[ 4u ].y = matrix[ 1u ].w - matrix[ 1u ].y;
	m_frustum.m_planes[ 4u ].z = matrix[ 2u ].w - matrix[ 2u ].y;
	m_frustum.m_planes[ 4u ].w = matrix[ 3u ].w - matrix[ 3u ].y;
	m_frustum.m_planes[ 4u ].normalize();

	m_frustum.m_planes[ 5u ].x = matrix[ 0u ].w + matrix[ 0u ].y;
	m_frustum.m_planes[ 5u ].y = matrix[ 1u ].w + matrix[ 1u ].y;
	m_frustum.m_planes[ 5u ].z = matrix[ 2u ].w + matrix[ 2u ].y;
	m_frustum.m_planes[ 5u ].w = matrix[ 3u ].w + matrix[ 3u ].y;
	m_frustum.m_planes[ 5u ].normalize();

}

gtAabb*	gtCameraImpl::getAabb( void ){
	return nullptr;
}
gtObb*	gtCameraImpl::getObb( void ){
	return nullptr;
}