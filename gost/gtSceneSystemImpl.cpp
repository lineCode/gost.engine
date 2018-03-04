#include "common.h"

gtSceneSystemImpl::gtSceneSystemImpl( void ):
	m_mainSystem( nullptr ),
	m_driver( nullptr ),
	m_rootNode( nullptr ),
	m_activeCamera( nullptr )
{
#ifdef GT_DEBUG
	m_debugName.assign(u"gtSceneSystemImpl");
#endif

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
		gtLogWriter::printWarning( u"Can not create static object. Name [%s], id[%i]", name.data(), id );
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


bool aabbInFrustum( gtCameraFrustum * frustum, gtAabb* aabb, const v3f& position ){
	v3f _min = aabb->m_min + position;
	v3f _max = aabb->m_max + position;

	/*if( max <= frustum->m_farX && max > frustum->m_nearX )
		return true;
	if( min >= frustum->m_nearY && min <= frustum->m_farY )
		return true;*/



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

		gtAabb * aabb = var->getAabb();
		if( aabb ){
			if( aabbInFrustum( frustum, aabb, var->getPositionInSpace() ) ){
				outArray.push_back( var );
				addChildsInArray( var, outArray );
			}else{
				frustumCull( var, outArray, frustum );
			}
		}

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

void gtSceneSystemImpl::renderScene( void ){

	auto * childs = &m_rootNode->getChildList();
	{
		auto it = childs->begin();
		for(; it != childs->end(); ++it){
			(*it)->update();
		}
	}

	if( m_activeCamera ){
		m_activeCamera->render();
		m_mainSystem->setMatrixProjection( m_activeCamera->getProjectionMatrix() );
		m_mainSystem->setMatrixView( m_activeCamera->getViewMatrix() );
	}

	gtArray<gtGameObject*> objectsInFrustum;
	frustumCull( m_rootNode, objectsInFrustum, ((gtCameraImpl*)m_activeCamera)->getFrustum() );

//	gtLogWriter::printInfo( u"Num of objects: %u", objectsInFrustum.size() );


	gtArray<gtGameObject*> opaqueObjects;
	gtArray<gtGameObject*> transparentObjects;
	
	sortTransparent(opaqueObjects,transparentObjects,objectsInFrustum);

	auto sz = opaqueObjects.size();

	for( auto i = 0u; i < sz; ++i ){
		auto * var = opaqueObjects[ i ];
		if( var )
			drawObject( var );
	}

	sz = transparentObjects.size();
	for( auto i = 0u; i < sz; ++i ){
		auto * var = transparentObjects[ i ];
		if( var )
			drawObject( var );
	}

}

void gtSceneSystemImpl::drawObject( gtGameObject * object ){
	auto name = object->getName();
	m_mainSystem->setMatrixWorld( object->getAbsoluteWorldMatrix() );
	object->render();

	if( object->isShowBV() ){

		gtObb * obb = object->getObb();

		if( obb ){

			auto& pos = object->getPositionInSpace();

			gtColor red( 1.f, 0.f, 0.f );

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
			/*m_driver->drawLine( obb->v1 + pos, obb->v4 + pos, red );
			m_driver->drawLine( obb->v5 + pos, obb->v8 + pos, red );
			m_driver->drawLine( obb->v1 + pos, obb->v5 + pos, red );
			m_driver->drawLine( obb->v4 + pos, obb->v8 + pos, red );
			m_driver->drawLine( obb->v3 + pos, obb->v7 + pos, red );
			m_driver->drawLine( obb->v6 + pos, obb->v2 + pos, red );
			m_driver->drawLine( obb->v3 + pos, obb->v6 + pos, red );
			m_driver->drawLine( obb->v7 + pos, obb->v2 + pos, red );
			m_driver->drawLine( obb->v2 + pos, obb->v8 + pos, red );
			m_driver->drawLine( obb->v4 + pos, obb->v7 + pos, red );
			m_driver->drawLine( obb->v5 + pos, obb->v6 + pos, red );
			m_driver->drawLine( obb->v1 + pos, obb->v3 + pos, red );*/

			gtAabb * aabb = object->getAabb();
			if( aabb ){
				gtColor green( 0.f, 1.f, 0.f );


			}
		}
	}
}