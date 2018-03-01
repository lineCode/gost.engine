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
			//	��������� ��� �������
gtObjectType		gtCameraImpl::getType( void ){
	return m_type;
}


			//	��������� �������
const v3f&			gtCameraImpl::getScale( void ){
	return m_position;
}

			//	��������� ��������
const v3f&	gtCameraImpl::getRotation( void ){
	return m_rotation;
}

			//	��������� �������
void				gtCameraImpl::setScale( const v3f& ){}

			//	��������� ��������
void				gtCameraImpl::setRotation( const v3f& q ){
	m_rotation = q;
}

	//	������� ���������� � �������/��������/��������
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

	//	�������� ������ (���� �� �������� (�������� �� 3D �����))
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
	//	����� ��� ������
gtCameraType	gtCameraImpl::getCameraType( void ){
	return m_cameraType;
}

	//	��������� ��� ������
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

	//	����� up ������ look at ������
const v3f&			gtCameraImpl::getUpVector( void ){
	return m_up;
}

	//	��������� up ������ look at ������
void gtCameraImpl::setUpVector( const v3f& v ){
	m_up = v;
}

	//	��������� ������� ������� � ������� ���������� ���������
void gtCameraImpl::setNear( f32 v ){
	m_near = v;
}

	//	��������� ������� ������� ����� ������� ��������� �������������
void gtCameraImpl::setFar( f32 v ){
	m_far = v;
}

	//	��������� ����������� ������ (����. aspect = 800 : 600 )
void gtCameraImpl::setAspect( f32 v ){
	m_aspect = v;
}

	//	��������� ���� ������ (field of view)
void gtCameraImpl::setFOV( f32 v ){
	m_fov = v;
}

	//	����� ������� ������� � ������� ���������� ���������
f32	gtCameraImpl::getNear( void ){
	return m_near;
}

	//	����� ������� ������� ����� ������� ��������� �������������
f32	gtCameraImpl::getFar( void ){
	return m_far;
}

	//	����� ����������� ������
f32	gtCameraImpl::getAspect( void ){
	return m_aspect;
}

	//	����� ���� ������ (field of view)
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
	v3f pos = getPositionInSpace();

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

	m_frustum.m_farY  -= pos;

}

gtAabb*	gtCameraImpl::getAabb( void ){
	return nullptr;
}