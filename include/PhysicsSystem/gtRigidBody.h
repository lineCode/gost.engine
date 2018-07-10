#pragma once
#ifndef __GT_RIGID_BODY_H__
#define __GT_RIGID_BODY_H__


namespace gost{

	struct gtRigidBodyInfo{
		gtRigidBodyInfo():
			m_shape( nullptr ),
			m_mass( 1.f )
		{}

		gtCollisionShape * m_shape;
		f32                m_mass;
		v3f                m_position;
		gtQuaternion       m_rotation;
	};
	
	class gtRigidBody : public gtRefObject {
	public:
		
		virtual const gtRigidBodyInfo& getInfo()     = 0;
		virtual const v4f&             getPosition() = 0;
		virtual const gtQuaternion&    getRotation() = 0;
		virtual void                   setPosition( const v4f& ) = 0;
		virtual void                   setRotation( const gtQuaternion& ) = 0;
		virtual void                   update() = 0;
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