#pragma once
#ifndef __GT_RIGID_BODY_IMPL_H__
#define __GT_RIGID_BODY_IMPL_H__

namespace gost{

	class gtRigidBodyImpl : public gtRigidBody{
		void *              m_userPtr;
		gtPhysicsBullet *   m_ps;
		gtRigidBodyInfo     m_info;
		btRigidBody*        m_body;
		v4f                 m_position;
		gtQuaternion        m_rotation;
		btTransform         m_transformation;
	public:
	                           gtRigidBodyImpl( const gtRigidBodyInfo&, gtPhysicsBullet * );
	                           ~gtRigidBodyImpl();
		
		btRigidBody*           getBulletRigidBody();

		bool                   init();

		void                   activate( bool );
		const gtRigidBodyInfo& getInfo();
		v4f                    getLinearVelocity();
		const v4f&             getPosition();
		const gtQuaternion&    getRotation();
		void *                 getUserData();
		void                   setLinearVelocity( const v4f& velocity );
		void                   setPosition( const v4f& );
		void                   setRotation( const gtQuaternion& );
		void                   setUserData( void * );
		void                   update();

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