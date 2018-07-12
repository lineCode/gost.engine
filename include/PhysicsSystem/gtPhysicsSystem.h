#pragma once
#ifndef __GT_PHYSICS_SYSTEM_H__
#define __GT_PHYSICS_SYSTEM_H__


namespace gost{
	
	enum class gtPhysicsConstraintSolverType : u32{
	};
	class gtPhysicsFilterCallback{
	public:
		virtual ~gtPhysicsFilterCallback(){}

		virtual bool collision( void * body1, void * body2 ) = 0;
	};

	struct gtPhysicsSystemInfo{
		gtPhysicsSystemInfo():
			filterCallback( nullptr ),
			gravity(v3f(0.f,-10.f,0.f)){}

		gtPhysicsFilterCallback    * filterCallback;

		v3f gravity;
	};

	struct gtPhysicsRayTestNode{
		gtRigidBody* m_body;
		v4f          m_hitPoint;
		v4f          m_normal;
	};

	class gtPhysicsSystem : public gtRefObject {
	public:
		
		virtual gtPtr<gtCollisionShape>       createCollisionShapeBox( const v3f& size ) = 0;
		virtual gtPtr<gtRigidBody>            createRigidBody( const gtRigidBodyInfo& info ) = 0;
		virtual bool                          initialize() = 0;
		virtual gtRigidBody*                  rayTest( const v4f& ray_start, const v4f& ray_end, v4f& hitPoint, v4f& normal ) = 0;
		virtual gtRigidBody*                  rayTest( const gtRayf32& ray, v4f& hitPoint, v4f& normal ) = 0;
		virtual gtArray<gtPhysicsRayTestNode> rayTestMultiple( const gtRayf32& ray ) = 0;
		virtual void                          setGravity( const v3f& gravity ) = 0;
		virtual void                          shutdown() = 0;
		virtual void                          update( f32 delta ) = 0;

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