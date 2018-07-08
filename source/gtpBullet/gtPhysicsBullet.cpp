#include "common.h"

gtPhysicsBullet::gtPhysicsBullet( gtPhysicsSystemInfo* i ):
	m_collisionConfiguration( nullptr ),
	m_filterCallback( nullptr ),
	m_userFilterCallback( nullptr ),
	m_dispatcher( nullptr ),
	m_pairCache( nullptr ),
	m_broadphase( nullptr ),
	m_pickedBody( nullptr ),
	m_pickedConstraint( nullptr ),
	m_solver( nullptr ),
	m_dynamicsWorld( nullptr ),
	m_params( *i ),
	m_initialized( false )
{
	setName( "Bullet Physics plugin" );
}

gtPhysicsBullet::~gtPhysicsBullet(){}

bool gtPhysicsBullet::initialize(){

	gtLogWriter::printInfo( u"Init Bullet Physics 2.87" );

	m_collisionConfiguration = new btDefaultCollisionConfiguration();

	if( !m_params.filterCallback )
		m_filterCallback = new gtOverlapFilterCallback2();
	else{
		m_userFilterCallback = new gtUserOverlapFilterCallback2();
		m_userFilterCallback->m_userCallback = m_params.filterCallback;
	}

	m_dispatcher = new	btCollisionDispatcher( m_collisionConfiguration );
	
	m_pairCache  = new btHashedOverlappingPairCache();
	m_pairCache->setOverlapFilterCallback( (m_params.filterCallback)?m_userFilterCallback:m_filterCallback );

	m_broadphase = new btDbvtBroadphase(m_pairCache);

	m_solver = new btSequentialImpulseConstraintSolver;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);

	auto g = m_params.gravity;
	setGravity( g );

	m_initialized = true;

	return true;
}

void gtPhysicsBullet::shutdown(){
	if( !m_initialized ) return;

	if( m_pickedConstraint ){
		m_pickedBody->forceActivationState(m_savedState);
		m_pickedBody->activate();
		m_dynamicsWorld->removeConstraint(m_pickedConstraint);
		delete m_pickedConstraint;
		m_pickedConstraint = 0;
		m_pickedBody = 0;
	}

	if( m_dynamicsWorld ){
        s32 i;
        for( i = m_dynamicsWorld->getNumConstraints() - 1; i >= 0; i-- ){
            m_dynamicsWorld->removeConstraint(m_dynamicsWorld->getConstraint(i));
        }

		for( i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i-- ){
			btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			m_dynamicsWorld->removeCollisionObject(obj);
			delete obj;
		}

		for( s32 j = 0; j<m_collisionShapes.size(); j++){
			btCollisionShape* shape = m_collisionShapes[j];
			delete shape;
		}
		m_collisionShapes.clear();
	}

	if( m_dynamicsWorld )
		delete m_dynamicsWorld;
	m_dynamicsWorld = nullptr;

	if( m_solver )
		delete m_solver;
	m_solver = nullptr;

	if( m_broadphase )
		delete m_broadphase;
	m_broadphase = nullptr;

	if( m_pairCache )
		delete m_pairCache;
	m_pairCache = nullptr;

	if( m_dispatcher )
		delete m_dispatcher;
	m_dispatcher = nullptr;

	if( m_userFilterCallback )
		delete m_userFilterCallback;
	m_userFilterCallback = nullptr;

	if( m_filterCallback )
		delete m_filterCallback;
	m_filterCallback = nullptr;

	if( m_collisionConfiguration )
		delete m_collisionConfiguration;
	m_collisionConfiguration = nullptr;

	m_initialized = false;
}

void gtPhysicsBullet::setGravity( const v3f& gravity ){
	m_params.gravity = gravity;
	if( m_initialized ){
		m_dynamicsWorld->setGravity( btVector3(m_params.gravity.x, m_params.gravity.y, m_params.gravity.z) );
	}
}

void gtPhysicsBullet::update( f32 delta ){
	if( m_initialized ){
		m_dynamicsWorld->stepSimulation( delta );
	}
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