#include "common.h"

gtSceneSystemImpl::gtSceneSystemImpl():
	m_mainSystem( nullptr ),
	m_gs( nullptr ),
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

void gtSceneSystemImpl::clearScene(){
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

void gtSceneSystemImpl::setCurrentRenderDriver( gtGraphicsSystem * driver ){ m_gs = driver; }

gtSprite*		gtSceneSystemImpl::addSprite( gtTexture * texture, const v2f& size, const v4f& position, bool /*asBillboard*/ ){
	gtSprite * sprite = new gtSprite( texture, size, m_gs );
	if( !sprite ) return nullptr;
	sprite->setPosition( position );
	m_rootNode->addChild( sprite );
	sprite->setName("Sprite");
	return sprite;
}

gtCamera*		gtSceneSystemImpl::addCamera( const v4f& position, const v4f& target, s32 id, bool setActive ){
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

		auto rc = (m_gs) ? m_gs->getParams().m_outWindow->getRect() : v4i(0,0,800,600);
		camera->setAspect( (f32)rc.getWidth() / (f32)rc.getHeight() );
	}

	return camera.data();
}

gtCamera*		gtSceneSystemImpl::addCamera2D( const v4f& viewport, s32 id, bool setActive ){
	gtCamera * camera = addCamera(v4f(),v4f(),id,setActive);

	if( camera ){
		camera->setCameraType( gtCameraType::Camera_2D );
		camera->setPosition( v4f( 0.f ) );
		camera->setRotation( v4f( 0.f, math::PI, 0.f, 0.f ) );
		camera->setNear( 0.1f );
		camera->setFar( 1000.f );
		camera->setViewPort( viewport );
		camera->setName("Camera2D");
		camera->setFOV( 1.f );
	}

	return camera;
}

gtCamera* gtSceneSystemImpl::getActiveCamera(){ return m_activeCamera; }
void gtSceneSystemImpl::setActiveCamera( gtCamera* camera ){ m_activeCamera = camera; }

gtDummyObject*	gtSceneSystemImpl::addDummyObject( const v4f& position, const gtStringA& name, s32 id ){
	gtDummyObjectImpl * node = new gtDummyObjectImpl;

	if( !node ) return nullptr;

	node->setName( name );
	node->setID( id );
	node->setPosition( position );

	return node;
}

gtStaticObject*	gtSceneSystemImpl::addStaticObject( gtRenderModel* model, const v4f& position, const gtStringA& name, s32 id ){
	if( !model ) return nullptr;

	gtPtr_t( gtStaticObjectImpl, object, new gtStaticObjectImpl( model, m_gs ) );

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

gtGameObject*	gtSceneSystemImpl::getRootObject(){ return m_rootNode; }

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
	if( object ){
		object->release();
	}
}


bool aabbInFrustum( gtCameraFrustum * /*frustum*/, gtAabb* aabb, const v4f& position ){
	v4f _min = aabb->m_min + position;
	v4f _max = aabb->m_max + position;
	return true;
}

bool sphereInFrustum( gtCameraFrustum * frustum, f32 radius, const v4f& position ){
	for( u32 i = gtConst0U; i < gtConst6U; ++i ){
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
		case gost::gtBoundingVolumeType::Sphere:
			if( sphereInFrustum( frustum, var->getBVSphereRadius(), var->getPositionInSpace() ) ){
				outArray.push_back( var );
				addChildsInArray( var, outArray );
			}
			break;
		case gost::gtBoundingVolumeType::Obb:
		case gost::gtBoundingVolumeType::Aabb:
		case gost::gtBoundingVolumeType::Convex:
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
	for( u32 i = gtConst0U; i < sz; ++i ){

		auto var = objects[ i ];

		switch( var->getType() ){
		case gtObjectType::Static:{

			auto * model = ((gtStaticObject*)var)->getModel();

			u32 smc = model->getSubModelCount();

			bool isTransparent = false;

			for( u32 i2 = gtConst0U; i2 < smc; ++i2 ){
				if( model->getMaterial( i2 )->flags & (u32)gtMaterialFlag::AlphaBlend ){
					transparent.push_back( var );
					isTransparent = true;
					break;
				}
			}

			if( !isTransparent )
				opaque.push_back( var );

		}break;
		case gtObjectType::Sprite:{
			if( ((gtSprite*)var)->getMaterial()->flags & (u32)gtMaterialFlag::AlphaBlend )
				transparent.push_back( var );
			else
				opaque.push_back( var );
		}break;
		case gtObjectType::Camera:
		case gtObjectType::Dummy:
		default:
			break;
		}
	}
}

bool gtPairSortPredGreatOrEqual( const gtPair<f32,gtGameObject*>& o1, const gtPair<f32,gtGameObject*>& o2 ){
	return o1.m_first >= o2.m_first;
}

void gtSceneSystemImpl::sortTransparentDistance( gtArray<gtGameObject*>& in, gtArray<gtGameObject*>& out ){
	v4f position = m_activeCamera->getPositionInSpace();

	gtArray<gtPair<f32,gtGameObject*>> groups[ gtConst4U ];


	auto sz = in.size();
	for( u32 i = gtConst0U; i < sz; ++i ){

		f32 dist = 0.f;

		if( m_activeCamera->getCameraType() == gtCameraType::Camera_2D ){
			dist = std::abs(in[ i ]->getPositionInSpace().z - position.z);
		}else{
			dist = position.distance( in[ i ]->getPositionInSpace() );
		}
	
		if( dist < 10.f ){
			groups[ gtConst0U ].push_back( gtPair<f32,gtGameObject*>(dist,in[ i ],gtPairSortPredGreatOrEqual) );
		}else if( dist < 100.f ){
			groups[ gtConst1U ].push_back( gtPair<f32,gtGameObject*>(dist,in[ i ],gtPairSortPredGreatOrEqual) );
		}else if( dist < 1000.f ){
			groups[ gtConst2U ].push_back( gtPair<f32,gtGameObject*>(dist,in[ i ],gtPairSortPredGreatOrEqual) );
		}else{
			groups[ gtConst3U ].push_back( gtPair<f32,gtGameObject*>(dist,in[ i ],gtPairSortPredGreatOrEqual) );
		}
	}


	for( s32 i = 3; i >= 0; --i ){
		sz = groups[ i ].size();

		util::mergesort( &groups[ i ], util::predicateGreatOrEqual );
		
		for( u32 o = gtConst0U; o < sz; ++o ){
			out.push_back( groups[ i ][ o ].m_second );
		}
	}

}

void gtSceneSystemImpl::renderScene(){

	static bool render_effect = true;
	if( render_effect ){
		render_effect = false;
		m_gs->renderEffects();
		render_effect = true;
	}

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
		for( auto i = gtConst0U; i < sz; ++i ){
			auto * var = opaqueObjects[ i ];
			if( var ){
				drawObject( var );
			}
		}

		sz = transparentObjects.size();
		for( auto i = gtConst0U; i < sz; ++i ){
			auto * var = transparentObjects[ i ];
			if( var ){
				drawObject( var );
			}
		}
	}else{
		auto * childs2 = &m_rootNode->getChildList();
		auto it = childs2->begin();
		for(; it != childs2->end(); ++it){
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

			m_gs->drawLineBox(
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

				v4f v1 = aabb->m_min;
				v4f v2 = aabb->m_max;
				v4f v3 = v4f( v1.x, v1.y, v2.z );
				v4f v4 = v4f( v2.x, v1.y, v1.z );
				v4f v5 = v4f( v1.x, v2.y, v1.z );
				v4f v6 = v4f( v1.x, v2.y, v2.z );
				v4f v7 = v4f( v2.x, v1.y, v2.z );
				v4f v8 = v4f( v2.x, v2.y, v1.z );

				m_gs->drawLineBox( v1, v2, v3, v4, v5, v6, v7, v8, pos, green  );
			}
			m_gs->drawLineSphere( pos, object->getBVSphereRadius(), gtConst1U, red, green, blue );
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
