#pragma once
#ifndef __GT_SCENE_SYSTEM_IMPL_H__
#define __GT_SCENE_SYSTEM_IMPL_H__

namespace gost{

	class gtSceneSystemImpl : public gtSceneSystem{

		gtMainSystem * m_mainSystem;

		gtGameObject * m_rootNode;

		gtCamera * m_activeCamera;

		void drawObject( gtGameObject * object );
		
		void sortTransparent(  gtArray<gtGameObject*>&, gtArray<gtGameObject*>&, gtArray<gtGameObject*>& );
		void frustumCull( gtGameObject* root, gtArray<gtGameObject*>& outArray, gtCameraFrustum * frustum );

	public:

		gtSceneSystemImpl( void );
		virtual ~gtSceneSystemImpl( void );

		gtDummyObject*	addDummyObject( const v3f& position = v3f(), const gtStringA& name = gtStringA(), s32 id = -1 );
		gtStaticObject*	addStaticObject( gtRenderModel* model, const v3f& position = v3f(), const gtStringA& name = gtStringA(), s32 id = -1 );
		gtCamera*		addCamera( const v3f& position = v3f(), const v3f& target = v3f(), s32 id = -1, bool setActive = true );
		gtCamera*		addCamera2D( const v4f& viewport, s32 id = -1, bool setActive = true );
		gtSprite*		addSprite( gtTexture * texture, gtDriver * driver, const v2f& size, const v3f& position = v3f({0.f,0.f,5.f}), bool asBillboard = false );

		gtCamera*		getActiveCamera( void );
		void			setActiveCamera( gtCamera* camera );

		gtGameObject*	getRootObject( void );
		void removeObject( gtGameObject* object );
		void renderScene( void );
		void clearScene( void );
	};

}

#endif