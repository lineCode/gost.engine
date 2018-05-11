#pragma once
#ifndef __GT_SCENE_SYSTEM_IMPL_H__
#define __GT_SCENE_SYSTEM_IMPL_H__

namespace gost{

	class gtSceneSystemImpl : public gtSceneSystem{

		gtMainSystem *	m_mainSystem;
		gtDriver *		m_driver;
		gtGameObject *	m_rootNode;
		gtCamera *		m_activeCamera;

		void drawObject( gtGameObject * object );
		void sortTransparent(  gtArray<gtGameObject*>&, gtArray<gtGameObject*>&, gtArray<gtGameObject*>& );
		void sortTransparentDistance( gtArray<gtGameObject*>&, gtArray<gtGameObject*>& );
		void frustumCull( gtGameObject* root, gtArray<gtGameObject*>& outArray, gtCameraFrustum * frustum );

	public:

		gtSceneSystemImpl();
		virtual ~gtSceneSystemImpl();

		gtDummyObject*	addDummyObject( const v3f& position = v3f(), const gtStringA& name = gtStringA(), s32 id = -1 );
		gtStaticObject*	addStaticObject( gtRenderModel* model, const v3f& position = v3f(), const gtStringA& name = gtStringA(), s32 id = -1 );
		gtCamera*		addCamera( const v3f& position = v3f(), const v3f& target = v3f(), s32 id = -1, bool setActive = true );
		gtCamera*		addCamera2D( const v4f& viewport, s32 id = -1, bool setActive = true );
		gtSprite*		addSprite( gtTexture * texture, const v2f& size, const v3f& position = v3f(0.f,0.f,5.f), bool asBillboard = false );

		gtCamera*		getActiveCamera();
		void			setActiveCamera( gtCamera* camera );

		gtGameObject*	getRootObject();
		void			removeObject( gtGameObject* object );
		void			renderScene();
		void			clearScene();

		void			setCurrentRenderDriver( gtDriver * driver );
	};

}

#endif

/*
Copyright (c) 2017-2018 532235

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