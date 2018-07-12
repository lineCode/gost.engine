#pragma once
#ifndef __GT_PHYSICS_BULLET_H__
#define __GT_PHYSICS_BULLET_H__

namespace gost{

	class gtPhysicsRayResultCallback_bt : public btCollisionWorld::RayResultCallback{
	public:

		gtPhysicsRayResultCallback_bt():
			m_userCallback( nullptr )
		{}

		gtPhysicsRayResultCallback * m_userCallback;

		virtual	btScalar addSingleResult( btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace ){
			btAssert( rayResult.m_hitFraction <= m_closestHitFraction );
			
			btVector3	m_rayFromWorld;
			btVector3	m_rayToWorld;
			btVector3	m_hitNormalWorld;
			btVector3	m_hitPointWorld;

			m_closestHitFraction = rayResult.m_hitFraction;
			m_collisionObject = rayResult.m_collisionObject;
			if( normalInWorldSpace ){
				m_hitNormalWorld = rayResult.m_hitNormalLocal;
			}else{
				m_hitNormalWorld = m_collisionObject->getWorldTransform().getBasis()*rayResult.m_hitNormalLocal;
			}
			m_hitPointWorld.setInterpolate3( m_rayFromWorld, m_rayToWorld, rayResult.m_hitFraction );
			return rayResult.m_hitFraction;
		}
	};


	enum gtFilterModes{
		FILTER_GROUPAMASKB_AND_GROUPBMASKA2=0,
		FILTER_GROUPAMASKB_OR_GROUPBMASKA2
	};

	struct gtOverlapFilterCallback2 : public btOverlapFilterCallback{
		gtPhysicsFilterCallback * m_userCallback;
		s32 m_filterMode;
	
		gtOverlapFilterCallback2():
			m_userCallback( nullptr ),
			m_filterMode(FILTER_GROUPAMASKB_AND_GROUPBMASKA2)
		{}
	
		virtual ~gtOverlapFilterCallback2(){}

		virtual bool needBroadphaseCollision(btBroadphaseProxy* proxy0,btBroadphaseProxy* proxy1) const {

			if( m_userCallback )
				return m_userCallback->collision( proxy0, proxy1 );

			if( m_filterMode == FILTER_GROUPAMASKB_AND_GROUPBMASKA2 ){
				bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
				collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
				return collides;
			}
		
			if( m_filterMode == FILTER_GROUPAMASKB_OR_GROUPBMASKA2 ){
				bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
				collides = collides || (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
				return collides;
			}

			return false;
		}
	};

	class gtPhysicsBullet : public gtPhysicsSystem{

		btDefaultCollisionConfiguration*        m_collisionConfiguration;
		gtOverlapFilterCallback2*               m_filterCallback;
		gtPhysicsRayResultCallback_bt*          m_rayTestCallback;
		btCollisionDispatcher*	                m_dispatcher;
		btOverlappingPairCache*                 m_pairCache;
		btBroadphaseInterface*	                m_broadphase;
		
		class btRigidBody*	                    m_pickedBody;
		class btTypedConstraint*                m_pickedConstraint;
		s32	                                    m_savedState;

		btSequentialImpulseConstraintSolver*    m_solver;

		btDiscreteDynamicsWorld*                m_dynamicsWorld;

		btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;
		gtPhysicsSystemInfo                     m_params;
		bool                                    m_initialized;

		gtArray<gtRigidBodyImpl*>			    m_rigidBodyArray;
	public:

		                        gtPhysicsBullet( gtPhysicsSystemInfo* );
		                        ~gtPhysicsBullet();

		gtPtr<gtCollisionShape> createCollisionShapeBox( const v3f& size );
		gtPtr<gtRigidBody>      createRigidBody( const gtRigidBodyInfo& info );
		bool                    initialize();
		void                    rayTest( const v3f& ray_start, const v3f& ray_end );
		void                    setGravity( const v3f& gravity );
		void                    shutdown();
		void                    update( f32 delta );

		void                    _addShape( btCollisionShape* );
		void                    _removeShape( btCollisionShape* );
		void                    _addRigidBody( btRigidBody* );
		void                    _removeRigidBody( btRigidBody* );

	};

}

#endif


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