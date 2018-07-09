#include "common.h"

gtCollisionShapeImpl::gtCollisionShapeImpl(gtPhysicsBullet * ps):
	m_ps( ps ),
	m_shape( nullptr )
{}

gtCollisionShapeImpl::~gtCollisionShapeImpl(){
	if( m_shape ){
		m_ps->_removeShape( m_shape );
	}
}

bool gtCollisionShapeImpl::initBox( const v3f& size ){
	m_shape = new btBoxShape(btVector3(size.x*0.5f, size.y*0.5f, size.z*0.5f));
	if( !m_shape )
		return false;

	m_ps->_addShape( m_shape );

	return true;
}

btCollisionShape * gtCollisionShapeImpl::getBulletShape(){
	return m_shape;
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