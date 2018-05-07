﻿#include "common.h"

gtSceneSystemImpl::gtSceneSystemImpl( void ):
	m_mainSystem( nullptr ),
	m_driver( nullptr ),
	m_rootNode( nullptr ),
	m_activeCamera( nullptr )
{

	m_mainSystem = gtMainSystem::getInstance();

	m_rootNode = new gtDummyObjectImpl;
	m_rootNode->setName("Root object");

}

gtSceneSystemImpl::~gtSceneSystemImpl(){
	if( m_rootNode ){
		auto * childs = &m_rootNode->getChildList();
		{
			for( auto it = childs->begin(); it != childs->end();){
				removeObject( (*it) );
				it = childs->begin();
			}
		}
		m_rootNode->release();
	}
}

void gtSceneSystemImpl::clearScene( void ){
	if( m_rootNode ){
		auto * childs = &m_rootNode->getChildList();
		{
			for( auto it = childs->begin(); it != childs->end();){
				removeObject( (*it) );
				it = childs->begin();
			}
		}
	}

	m_activeCamera = nullptr;
}

void gtSceneSystemImpl::setCurrentRenderDriver( gtDriver * driver ){
	m_driver = driver;
}

gtSprite*		gtSceneSystemImpl::addSprite( gtTexture * texture, const v2f& size, const v3f& position, bool /*asBillboard*/ ){
	gtSprite * sprite = new gtSprite( texture, size, m_driver );

	if( !sprite ) return nullptr;

	sprite->setPosition( position );

	m_rootNode->addChild( sprite );

	sprite->setName("Sprite");

	return sprite;
}

gtCamera*		gtSceneSystemImpl::addCamera( const v3f& position, const v3f& target, s32 id, bool setActive ){
	gtPtr<gtCameraImpl> camera = gtPtrNew<gtCameraImpl>( new gtCameraImpl );

	if( setActive )
		m_activeCamera = camera.data();

	if( camera.data() ){
		camera->addRef();
		camera->setPosition( position );
		camera->setTarget( target );
		camera->setID( id );
		camera->setName("Camera");
		m_rootNode->addChild( camera.data() );

		auto rc = m_driver->getParams().m_outWindow->getRect();
		camera->setAspect( (f32)rc.getWidth() / (f32)rc.getHeight() );
	}

	return camera.data();
}

gtCamera*		gtSceneSystemImpl::addCamera2D( const v4f& viewport, s32 id, bool setActive ){
	gtCamera * camera = addCamera(v3f(),v3f(),id,setActive);

	if( camera ){
		camera->setCameraType( gtCameraType::CT_2D );
		camera->setPosition( v3f( 0.f, 0.f, 0.f ) );
		camera->setRotation( v3f( 0.f, PI, 0.f ) );
		camera->setNear( 0.1f );
		camera->setFar( 1000.f );
		camera->setViewPort( viewport );
		camera->setName("Camera2D");
		camera->setFOV( 1.f );
	}

	return camera;
}

gtCamera*		gtSceneSystemImpl::getActiveCamera( void ){
	return m_activeCamera;
}

void			gtSceneSystemImpl::setActiveCamera( gtCamera* camera ){
	m_activeCamera = camera;
}

gtDummyObject*	gtSceneSystemImpl::addDummyObject( const v3f& position, const gtStringA& name, s32 id ){
	gtDummyObjectImpl * node = new gtDummyObjectImpl;

	if( !node ) return nullptr;

	node->setName( name );
	node->setID( id );
	node->setPosition( position );

	return node;
}

gtStaticObject*	gtSceneSystemImpl::addStaticObject( gtRenderModel* model, const v3f& position, const gtStringA& name, s32 id ){
	if( !model ) return nullptr;

	gtPtr_t( gtStaticObjectImpl, object, new gtStaticObjectImpl( model ) );


	if( !object.data()){
        const char16_t * msg = u"Can not create static object. Name [%s], id[%i]";
		gtLogWriter::printWarning( msg, name.data(), id );
		return nullptr;
	}

	if( id != -1 )
		object->setID( id );

	if( name.size() )
		object->setName( name.data() );

	object->setPosition( position );

	object->setName("Static object");

	m_rootNode->addChild( object.data() );

	object->addRef();

	object->recalculateBV();

	return object.data();
}

gtGameObject*	gtSceneSystemImpl::getRootObject( void ){
	return m_rootNode;
}

void gtSceneSystemImpl::removeObject( gtGameObject* object ){

	if( object->getParent() )
		object->getParent()->removeChild( object );

	auto name = object->getName();
	auto * childs = &object->getChildList();

	auto end = childs->end();

	for( auto it = childs->begin(); it != end;){
		if(*it){
			removeObject(*it);
		}
		it = childs->begin();
	}
//	gtLogWriter::printInfo(u"Remove object %s", name.to_utf16String().c_str() );
	if( object ){
		object->release();
	}
}


bool aabbInFrustum( gtCameraFrustum * /*frustum*/, gtAabb* aabb, const v3f& position ){
	v3f _min = aabb->m_min + position;
	v3f _max = aabb->m_max + position;

	return true;
}

bool sphereInFrustum( gtCameraFrustum * frustum, f32 radius, const v3f& position ){
	for( u32 i = 0u; i < 6u; ++i ){
		if( ( frustum->m_planes[ i ].x * position.x + frustum->m_planes[ i ].y * position.y + frustum->m_planes[ i ].z * position.z
			+ frustum->m_planes[ i ].w ) <= -radius){
			return false;
		}
	}
	return true;
}

void addChildsInArray( gtGameObject* parent, gtArray<gtGameObject*>& outArray ){
	auto * childs = &parent->getChildList();
	auto it = childs->begin();
	for(; it != childs->end(); ++it){
		outArray.push_back( *it );
		addChildsInArray( *it, outArray );
	}
}

void gtSceneSystemImpl::frustumCull( gtGameObject* root, gtArray<gtGameObject*>& outArray, gtCameraFrustum * frustum ){
	auto * childs = &root->getChildList();
	auto it = childs->begin();
	for(; it != childs->end(); ++it){
		auto * var = *it;

		switch( var->getBVType() ){
		case gost::gtBoundingVolumeType::sphere:
			if( sphereInFrustum( frustum, var->getBVSphereRadius(), var->getPositionInSpace() ) ){
				outArray.push_back( var );
				addChildsInArray( var, outArray );
			}
			break;
		case gost::gtBoundingVolumeType::obb:
		case gost::gtBoundingVolumeType::aabb:
		case gost::gtBoundingVolumeType::convex:
		default:
				frustumCull( var, outArray, frustum );
			break;
		}

		/*gtAabb * aabb = var->getAabb();
		if( aabb ){
			if( aabbInFrustum( frustum, aabb, var->getPositionInSpace() ) ){
				outArray.push_back( var );
				addChildsInArray( var, outArray );
			}else{
			}
		}*/

	}
}

void gtSceneSystemImpl::sortTransparent(  gtArray<gtGameObject*>& opaque, gtArray<gtGameObject*>& transparent, gtArray<gtGameObject*>& objects ){
	auto sz = objects.size();
	for( u32 i = 0u; i < sz; ++i ){

		auto var = objects[ i ];

		switch( var->getType() ){
		case gtObjectType::STATIC:{

			auto * model = ((gtStaticObject*)var)->getModel();

			u32 smc = model->getSubModelCount();

			bool isTransparent = false;

			for( u32 i2 = 0u; i2 < smc; ++i2 ){
				if( model->getMaterial( i2 )->flags & gtMaterialFlag::MF_BLEND ){
					transparent.push_back( var );
					isTransparent = true;
					break;
				}
			}

			if( !isTransparent )
				opaque.push_back( var );

		}break;
		case gtObjectType::SPRITE:{
			if( ((gtSprite*)var)->getMaterial()->flags & gtMaterialFlag::MF_BLEND )
				transparent.push_back( var );
			else
				opaque.push_back( var );
		}break;
		case gtObjectType::CAMERA:
		case gtObjectType::DUMMY:
		default:
			break;
		}
	}
}

bool gtPairSortPredGreatOrEqual( const gtPair<f32,gtGameObject*>& o1, const gtPair<f32,gtGameObject*>& o2 ){
	return o1.m_first >= o2.m_first;
}

void gtSceneSystemImpl::sortTransparentDistance( gtArray<gtGameObject*>& in, gtArray<gtGameObject*>& out ){
	v3f position = m_activeCamera->getPositionInSpace();

	gtArray<gtPair<f32,gtGameObject*>> groups[ 4u ];


	auto sz = in.size();
	for( u32 i = 0u; i < sz; ++i ){

		f32 dist = 0.f;

		if( m_activeCamera->getCameraType() == gtCameraType::CT_2D ){
			dist = std::abs(in[ i ]->getPositionInSpace().z - position.z);
		}else{
			dist = position.distance( in[ i ]->getPositionInSpace() );
		}
	
		if( dist < 10.f ){
			groups[ 0u ].push_back( gtPair<f32,gtGameObject*>(dist,in[ i ],gtPairSortPredGreatOrEqual) );
		}else if( dist < 100.f ){
			groups[ 1u ].push_back( gtPair<f32,gtGameObject*>(dist,in[ i ],gtPairSortPredGreatOrEqual) );
		}else if( dist < 1000.f ){
			groups[ 2u ].push_back( gtPair<f32,gtGameObject*>(dist,in[ i ],gtPairSortPredGreatOrEqual) );
		}else{
			groups[ 3u ].push_back( gtPair<f32,gtGameObject*>(dist,in[ i ],gtPairSortPredGreatOrEqual) );
		}
	}


	for( s32 i = 3; i >= 0; --i ){
		sz = groups[ i ].size();

		util::mergesort( &groups[ i ], util::predicateGreatOrEqual );
		
		for( u32 o = 0u; o < sz; ++o ){
			out.push_back( groups[ i ][ o ].m_second );
		}
	}

}

void gtSceneSystemImpl::renderScene( void ){

	auto * childs = &m_rootNode->getChildList();
	{
		auto it = childs->begin();
		for(; it != childs->end(); ++it){
			(*it)->update();
		}
	}

	if( !m_activeCamera ){
		gtLogWriter::printWarning( u"Scene system: No active camera" );
		return;
	}

	m_activeCamera->render();
	m_mainSystem->setMatrixProjection( m_activeCamera->getProjectionMatrix() );
	m_mainSystem->setMatrixView( m_activeCamera->getViewMatrix() );

	bool m_useFrustumCulling = true;
	
	gtArray<gtGameObject*> opaqueObjects;
	gtArray<gtGameObject*> transparentUnsortObjects;
	gtArray<gtGameObject*> transparentObjects;

	if( m_useFrustumCulling ){
		gtArray<gtGameObject*> objectsInFrustum;
		frustumCull( m_rootNode, objectsInFrustum, ((gtCameraImpl*)m_activeCamera)->getFrustum() );

	//	gtLogWriter::printInfo( u"Num of objects: %u", objectsInFrustum.size() );

		sortTransparent( opaqueObjects, transparentUnsortObjects, objectsInFrustum );
		sortTransparentDistance( transparentUnsortObjects, transparentObjects );

		auto sz = opaqueObjects.size();
		for( auto i = 0u; i < sz; ++i ){
			auto * var = opaqueObjects[ i ];
			if( var ){
				drawObject( var );
			}
		}

		sz = transparentObjects.size();
		for( auto i = 0u; i < sz; ++i ){
			auto * var = transparentObjects[ i ];
			if( var ){
				drawObject( var );
			}
		}
	}else{
		auto * childs = &m_rootNode->getChildList();
		auto it = childs->begin();
		for(; it != childs->end(); ++it){
			drawObject( (*it) );
		}
	}
	//gtLogWriter::printInfo( u"end" );
}

void gtSceneSystemImpl::drawObject( gtGameObject * object ){
	auto name = object->getName();
	//gtLogWriter::printInfo( u"Draw: %s", (name.to_utf16String()).data() );

	m_mainSystem->setMatrixWorld( object->getAbsoluteWorldMatrix() );
	object->render();

	if( object->isShowBV() ){

		gtObb * obb = object->getObb();

		const auto& pos = object->getPositionInSpace();

		if( obb ){

			gtColor red( 1.f, 0.f, 0.f );
			gtColor green( 0.f, 1.f, 0.f );
			gtColor blue( 0.f, 0.f, 1.f );

			m_driver->drawLineBox(
				obb->v1,
				obb->v2,
				obb->v3,
				obb->v4,
				obb->v5,
				obb->v6,
				obb->v7,
				obb->v8,
				pos,
				red
			);

			gtAabb * aabb = object->getAabb();
			if( aabb ){

				v3f v1 = aabb->m_min;
				v3f v2 = aabb->m_max;
				v3f v3 = v3f( v1.x, v1.y, v2.z );
				v3f v4 = v3f( v2.x, v1.y, v1.z );
				v3f v5 = v3f( v1.x, v2.y, v1.z );
				v3f v6 = v3f( v1.x, v2.y, v2.z );
				v3f v7 = v3f( v2.x, v1.y, v2.z );
				v3f v8 = v3f( v2.x, v2.y, v1.z );

				m_driver->drawLineBox( v1, v2, v3, v4, v5, v6, v7, v8, pos, green  );
			}
			m_driver->drawLineSphere( pos, object->getBVSphereRadius(), 1u, red, green, blue );
		}

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
