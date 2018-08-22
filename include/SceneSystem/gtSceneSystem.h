#pragma once
#ifndef __GT_SCENE_SYSTEM_H__
#define __GT_SCENE_SYSTEM_H__

namespace gost{

	class gtGameObject;
	class gtGameObjectCommon;
	class gtStaticObject;
	class gtCamera;
	class gtDummyObject;
	class gtRenderModel;
	class gtSprite;
	class gtTexture;
	class gtGraphicsSystem;

	class gtSceneSystem : public gtRefObject{
	public:
		
		virtual gtCamera*		addCamera( const v4f& position = v4f(), const v4f& target = v4f(), s32 id = gtConst_1, bool setActive = true ) = 0;
		virtual gtCamera*		addCamera2D( const v4f& viewport, s32 id = -1, bool setActive = true ) = 0;
		virtual gtDummyObject*	addDummyObject( const v4f& position = v4f(), const gtStringA& name = gtStringA(), s32 id = gtConst_1 ) = 0;
		virtual gtSprite*		addSprite( gtTexture * texture, const v2f& size, const v4f& position = v4f(gtConst0F,gtConst0F,5.f, 0.f), bool asBillboard = false ) = 0;
		virtual gtStaticObject*	addStaticObject( gtRenderModel* model, const v4f& position = v4f(), const gtStringA& name = gtStringA(), s32 id = -gtConst_1 ) = 0;
		virtual void			clearScene() = 0;
		virtual gtCamera*		getActiveCamera() = 0;
		virtual gtGameObjectCommon*	getRootObject() = 0;
		virtual void			removeObject( gtGameObject* object ) = 0;
		virtual void			renderScene() = 0;
		virtual void			setActiveCamera( gtCamera* camera ) = 0;
		virtual void			setCurrentRenderDriver( gtGraphicsSystem * driver ) = 0;

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