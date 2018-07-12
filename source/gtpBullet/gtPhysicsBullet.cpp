#include "common.h"

gtPhysicsBullet::gtPhysicsBullet( gtPhysicsSystemInfo* i ):
	m_collisionConfiguration( nullptr ),
	m_filterCallback( nullptr ),
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

	m_dispatcher = new	btCollisionDispatcher( m_collisionConfiguration );

	m_filterCallback = new gtOverlapFilterCallback2();
	m_filterCallback->m_userCallback = m_params.filterCallback;

	m_pairCache  = new btHashedOverlappingPairCache();
	m_pairCache->setOverlapFilterCallback( m_filterCallback );

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

	if( m_filterCallback )
		delete m_filterCallback;
	m_filterCallback = nullptr;
	
	if( m_dispatcher )
		delete m_dispatcher;
	m_dispatcher = nullptr;

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

		for( auto * i : m_rigidBodyArray ){
			i->update();
		}

	}
}

gtPtr<gtCollisionShape> gtPhysicsBullet::createCollisionShapeBox( const v3f& size ){
	auto ptr = new gtCollisionShapeImpl( this );
	gtPtr<gtCollisionShape> shape = gtPtrNew<gtCollisionShape>( ptr );

	if( !ptr->initBox( size ) ){
		gtLogWriter::printWarning( u"Can not init box collision shape" );
		return nullptr;
	}

	return shape;
}

void gtPhysicsBullet::_addShape( btCollisionShape* s ){
	m_collisionShapes.push_back( s );
}

void gtPhysicsBullet::_removeShape( btCollisionShape* s ){
	u32 sz = m_collisionShapes.size();
	for( u32 i = 0u; i < sz; ++i ){
		if( m_collisionShapes[ i ] == s ){
			m_collisionShapes.remove( s );
			return;
		}
	}
}

void gtPhysicsBullet::_addRigidBody( btRigidBody* b ){
	m_dynamicsWorld->addRigidBody( b );
}

void gtPhysicsBullet::_removeRigidBody( btRigidBody* b ){
	for( auto * i : m_rigidBodyArray ){
		if( i->getBulletRigidBody() == b ){
			m_rigidBodyArray.erase_first( i );
			break;
		}
	}

	for( auto i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--){
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if( b == body ){
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			m_dynamicsWorld->removeCollisionObject(obj);
			delete obj;
			return;
		}
	}
}

gtPtr<gtRigidBody> gtPhysicsBullet::createRigidBody( const gtRigidBodyInfo& info ){
	auto ptr = new gtRigidBodyImpl( info, this );
	gtPtr<gtRigidBody> body = gtPtrNew<gtRigidBody>( ptr );

	if( !ptr->init() ){
		gtLogWriter::printWarning( u"Can not init rigid body" );
		return nullptr;
	}

	m_rigidBodyArray.push_back( ptr );

	return body;
}

gtRigidBody* gtPhysicsBullet::rayTest( const v4f& ray_start, const v4f& ray_end, v4f& hitPoint, v4f& normal ){
	btVector3 from( ray_start.x, ray_start.y, ray_start.z );
	btVector3 to( ray_end.x, ray_end.y, ray_end.z );

	btCollisionWorld::ClosestRayResultCallback	closestResults(from,to);
	closestResults.m_flags |= btTriangleRaycastCallback::kF_FilterBackfaces;
	m_dynamicsWorld->rayTest(from,to,closestResults);
	if( closestResults.hasHit() ){
		btVector3 p = from.lerp(to,closestResults.m_closestHitFraction);
		btRigidBody * bd = (btRigidBody *)closestResults.m_collisionObject;
		hitPoint.set( p.x(), p.y(), p.z(), 0.f );
		normal.set( closestResults.m_hitNormalWorld.x(), closestResults.m_hitNormalWorld.y(), closestResults.m_hitNormalWorld.z(), 0.f );
		return (gtRigidBody*)bd->getUserPointer();
	}

	return nullptr;
}

gtRigidBody* gtPhysicsBullet::rayTest( const gtRayf32& ray, v4f& hitPoint, v4f& normal ){
	return rayTest( ray.m_begin, ray.m_end, hitPoint, normal );
}

gtArray<gtPhysicsRayTestNode> gtPhysicsBullet::rayTestMultiple( const gtRayf32& ray ){
	btVector3 from( ray.m_begin.x, ray.m_begin.y, ray.m_begin.z );
	btVector3 to( ray.m_end.x, ray.m_end.y, ray.m_end.z );

	btCollisionWorld::AllHitsRayResultCallback allResults(from,to);
	allResults.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal;
	allResults.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
			
	m_dynamicsWorld->rayTest(from,to,allResults);

	gtArray<gtPhysicsRayTestNode> arr;

	for( s32 i = 0; i < allResults.m_hitFractions.size(); ++i ){
		btVector3 p = from.lerp(to,allResults.m_hitFractions[i]);

		gtPhysicsRayTestNode node;
		node.m_body = (gtRigidBody*)((btRigidBody*)allResults.m_collisionObjects[ i ])->getUserPointer();
		node.m_hitPoint.set( p.x(), p.y(), p.z(), 0.f );
		node.m_normal.set( allResults.m_hitNormalWorld[i].x(), allResults.m_hitNormalWorld[i].y(), allResults.m_hitNormalWorld[i].z(), 0.f );

		arr.push_back( node );
	}

	return arr;
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