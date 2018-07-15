#include "common.h"

gtRigidBodyImpl::gtRigidBodyImpl( const gtRigidBodyInfo& i, gtPhysicsBullet * ps ):
	m_userPtr( nullptr ),
	m_ps( ps ),
	m_info( i ),
	m_body( nullptr )
{}

gtRigidBodyImpl::~gtRigidBodyImpl(){
	if( m_body ){
		m_ps->_removeRigidBody( m_body );
	}
}

const gtRigidBodyInfo& gtRigidBodyImpl::getInfo(){
	return m_info;
}

bool gtRigidBodyImpl::init(){
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(m_info.m_position.x,m_info.m_position.y,m_info.m_position.z));
	transform.setRotation( btQuaternion( m_info.m_rotation.x, m_info.m_rotation.y, m_info.m_rotation.z, m_info.m_rotation.w ) );

	btDefaultMotionState* myMotionState = new btDefaultMotionState( transform );

	if( !myMotionState )
		return false;

	bool isDynamic = (m_info.m_mass != 0.f);

	btVector3 localInertia( 0, 0, 0 );
	if( isDynamic )
		((gtCollisionShapeImpl*)m_info.m_shape)->getBulletShape()->calculateLocalInertia( m_info.m_mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo rbInfo( m_info.m_mass, myMotionState, ((gtCollisionShapeImpl*)m_info.m_shape)->getBulletShape(), localInertia );

	m_body = new btRigidBody( rbInfo );
	
	if( !m_body ){
		delete myMotionState;
		return false;
	}

	m_body->setUserPointer( this );

	m_ps->_addRigidBody( m_body );

	return true;
}

const v4f&          gtRigidBodyImpl::getPosition(){ return m_position; }
const gtQuaternion& gtRigidBodyImpl::getRotation(){ return m_rotation; }

void gtRigidBodyImpl::update(){
	if( m_body ){
		m_body->getMotionState()->getWorldTransform( m_transformation );
		auto & origin = m_transformation.getOrigin();
		auto rotation = m_transformation.getRotation();
		m_position.set( origin.x(), origin.y(), origin.z() );
		m_rotation.set( rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW() );
	}
}

btRigidBody* gtRigidBodyImpl::getBulletRigidBody(){
	return m_body;
}

void gtRigidBodyImpl::setPosition( const v4f& v ){
	if( m_body ){
		
		m_body->clearForces();
		
		btTransform t;
		m_body->getMotionState()->getWorldTransform(t);

		t.setOrigin( btVector3( v.x, v.y, v.z ) );

		m_body->setCenterOfMassTransform( t );
		m_body->setWorldTransform( t );
		m_body->getMotionState()->setWorldTransform( t );
		
	}
}

void gtRigidBodyImpl::setRotation( const gtQuaternion& q ){
	if( m_body ){
		btTransform t;
		m_body->getMotionState()->getWorldTransform(t);

		t.setRotation( btQuaternion( q.x, q.y, q.z, q.w ) );
	
		m_body->clearForces();

		m_body->setWorldTransform( t );
		m_body->getMotionState()->setWorldTransform( t );
	}
}

void * gtRigidBodyImpl::getUserData(){
	return m_userPtr;
}

void gtRigidBodyImpl::setUserData( void * ptr ){
	m_userPtr = ptr;
}

v4f gtRigidBodyImpl::getLinearVelocity(){
	auto btvec = m_body->getLinearVelocity();
	return v4f( btvec.x(), btvec.y(), btvec.z(), 0.f );
}

void       gtRigidBodyImpl::setLinearVelocity( const v4f& velocity ){
	btVector3 btvec(velocity.x,velocity.y,velocity.z);
	m_body->setLinearVelocity(btvec);
}

void gtRigidBodyImpl::activate( bool v ){
	m_body->activate( v );
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