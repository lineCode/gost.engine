#include "common.h"

gtSceneSystemImpl::gtSceneSystemImpl( void ):
	m_mainSystem( nullptr ),
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

gtSprite*		gtSceneSystemImpl::addSprite( gtTexture * texture, gtDriver * driver, const v2f& size, const v3f& position, bool /*asBillboard*/ ){
	gtSprite * sprite = new gtSprite( texture, size, driver );

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
		camera->setPosition( v3f_t( 0.f, 0.f, 0.f ) );
		camera->setRotation( v3f_t( 0.f, PI, 0.f ) );
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
	gtLogWriter::printInfo(u"Remove object %s", name.to_utf16String().c_str() );
	if( object )
		object->release();
}

void gtSceneSystemImpl::sortTransparent(  gtArray<gtGameObject*>& opaque, gtArray<gtGameObject*>& transparent, gtGameObject* parent ){
	auto * childs = &parent->getChildList();
	auto it = childs->begin();
	for(; it != childs->end(); ++it){

		auto * var = *it;

		switch( var->getType() ){
		case gtObjectType::STATIC:{

			auto * model = ((gtStaticObject*)var)->getModel();

			u32 smc = model->getSubModelCount();

			bool isTransparent = false;

			for( u32 i = 0u; i < smc; ++i ){
				if( model->getMaterial( i )->flags & gtMaterialFlag::MF_BLEND ){
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
		sortTransparent(opaque,transparent,var);
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

	gtArray<gtGameObject*> opaqueObjects;
	gtArray<gtGameObject*> transparentObjects;
	
	sortTransparent(opaqueObjects,transparentObjects,m_rootNode);

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
}