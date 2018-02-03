#include "common.h"

gtCameraImpl::gtCameraImpl( void ):
	m_fov( 0.785398185f ),
	m_near( 1.f ),
	m_far( 100.f ),
	m_aspect( 1.333333333f ),
	m_type( gtObjectType::CAMERA ),
	m_cameraType( gtCameraType::CT_LOOK_AT ),
	m_up({0.f,1.f,0.f})
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


			//	Возвратит масштаб
const v3f&			gtCameraImpl::getScale( void ){
	return m_position;
}

			//	Возвратит вращение
const v3f&	gtCameraImpl::getRotation( void ){
	return m_rotation;
}

			//	Установит масштаб
void				gtCameraImpl::setScale( const v3f& ){}

			//	Установит вращение
void				gtCameraImpl::setRotation( const v3f& q ){
	m_rotation = q;
}

	//	Обновит информацию о позиции/вращении/масштабе
void				gtCameraImpl::update( void ){

	gtMatrix4 translationMatrix;
	math::makeTranslationMatrix( translationMatrix, m_position );

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
			m_viewPort.z_ * 0.02f * m_fov,
			m_viewPort.w_ * 0.02f * m_fov,
			m_near,
			m_far
		);
		
		gtQuaternion qPitch( v3f_t( m_rotation.x_, 0.f, 0.f ) );
		gtQuaternion qYaw( v3f_t( 0.f, m_rotation.y_, 0.f ) );
		gtQuaternion qRoll( v3f_t( 0.f, 0.f, m_rotation.z_ ) );
		
		gtQuaternion orientation = qYaw * qPitch * qRoll;


		math::makeRotationMatrix( m_viewMatrix, orientation );

		m_viewMatrix = m_viewMatrix * m_worldMatrixAbsolute;

	}break;

	case gost::gtCameraType::CT_FREE:{
		math::makePerspectiveRHMatrix( 
			m_projectionMatrix,
			m_fov,
			m_aspect,
			m_near,
			m_far );

		gtQuaternion qPitch( v3f_t( m_rotation.x_, 0.f, 0.f ) );
		gtQuaternion qYaw( v3f_t( 0.f, m_rotation.y_, 0.f ) );
		gtQuaternion qRoll( v3f_t( 0.f, 0.f, m_rotation.z_ ) );
		
		gtQuaternion orientation = qYaw * qPitch * qRoll;


		math::makeRotationMatrix( m_viewMatrix, orientation );

		m_viewMatrix = m_viewMatrix * m_worldMatrixAbsolute;

	}break;
	}
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
void				gtCameraImpl::setUpVector( const v3f& v ){
	m_up = v;
}

	//	Установит ближнюю границу с которой начинается рисование
void				gtCameraImpl::setNear( f32 v ){
	m_near = v;
}

	//	Установит дальнюю границу после которой рисование заканчивается
void				gtCameraImpl::setFar( f32 v ){
	m_far = v;
}

	//	Установит соотношение сторон (напр. aspect = 800 : 600 )
void				gtCameraImpl::setAspect( f32 v ){
	m_aspect = v;
}

	//	Установит поле зрения (field of view)
void				gtCameraImpl::setFOV( f32 v ){
	m_fov = v;
}

	//	Вернёт ближнюю границу с которой начинается рисование
f32					gtCameraImpl::getNear( void ){
	return m_near;
}

	//	Вернёт дальнюю границу после которой рисование заканчивается
f32					gtCameraImpl::getFar( void ){
	return m_far;
}

	//	Вернёт соотношение сторон
f32					gtCameraImpl::getAspect( void ){
	return m_aspect;
}

	//	Вернёт поле зрения (field of view)
f32					gtCameraImpl::getFOV( void ){
	return m_fov;
}

void				gtCameraImpl::setViewPort( const v4f& v ){
	m_viewPort = v;
}
