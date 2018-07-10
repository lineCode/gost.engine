#include "common.h"

gtCollisionShapeImpl::gtCollisionShapeImpl(gtPhysicsBullet * ps):
	m_ps( ps ),
	m_shape( nullptr ),
	m_shapeBase( nullptr )
{}

gtCollisionShapeImpl::~gtCollisionShapeImpl(){
	if( m_shape ){
		m_ps->_removeShape( m_shape );
	}
}

bool gtCollisionShapeImpl::initBox( const v3f& size ){
	m_shape = new btBoxShape(btVector3(size.x, size.y, size.z));
	if( !m_shape )
		return false;

	m_shapeBase = (btPolyhedralConvexShape*)m_shape;
	m_ps->_addShape( m_shape );
	
	return true;
}

btCollisionShape * gtCollisionShapeImpl::getBulletShape(){
	return m_shape;
}

u32 gtCollisionShapeImpl::getNumVertices(){
	return (u32)m_shapeBase->getNumVertices();
}

void gtCollisionShapeImpl::getVertex( u32 index, v3f& vertex ){
	btVector3 v;
	m_shapeBase->getVertex( (s32)index, v );
	vertex.set(v[0],v[1],v[2]);
}

u32 gtCollisionShapeImpl::getNumEdges(){
	return (u32)m_shapeBase->getNumEdges();
}

void gtCollisionShapeImpl::getEdge( u32 index, v3f& v1, v3f& v2 ){
	btVector3 pa, pb;
	m_shapeBase->getEdge( (s32)index, pa, pb );
	v1.set(pa[0],pa[1],pa[2]);
	v2.set(pb[0],pb[1],pb[2]);
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