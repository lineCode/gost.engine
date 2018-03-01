/*!	GOST
	\file gtSceneSystem.h
	\brief GoST scene system
*/

#pragma once
#ifndef __GT_SCENE_SYSTEM_H__
#define __GT_SCENE_SYSTEM_H__ ///< include guard

namespace gost{

	class gtGameObject;
	class gtStaticObject;
	class gtCamera;
	class gtDummyObject;
	class gtRenderModel;
	class gtSprite;
	class gtTexture;
	class gtDriver;

		/// scene system
	class gtSceneSystem : public gtRefObject{
	public:
		
			///	Add helper object.
			///	\param position: position
			///	\param name: name
			///	\param id: id
			///	\return created object
			///	\remark do not call release. For remove object, use gtSceneSystem::removeObject
		virtual gtDummyObject*	addDummyObject( const v3f& position = v3f(), const gtStringA& name = gtStringA(), s32 id = -1 ) = 0;
		
			///	Add static object
			///	\param model: render model
			///	\param position: position
			///	\param name: name
			///	\param id: id
			///	\return created static object
			///	\remark do not call release. For remove object, use gtSceneSystem::removeObject
		virtual gtStaticObject*	addStaticObject( gtRenderModel* model, const v3f& position = v3f(), const gtStringA& name = gtStringA(), s32 id = -1 ) = 0;

			///	Add target camera
			///	\param position: position
			/// \param target: target
			/// \param id: id
			/// \param setActive: \b true if this camera is active
			///	\return created camera
			///	\remark do not call release. For remove object, use gtSceneSystem::removeObject
		virtual gtCamera*		addCamera( const v3f& position = v3f(), const v3f& target = v3f(), s32 id = -1, bool setActive = true ) = 0;

			///	Add 2D camera
			///	\param viewport: viewport coords
			/// \param id: id
			/// \param setActive: \b true if this camera is active
			///	\return created camera
			///	\remark do not call release. For remove object, use gtSceneSystem::removeObject
		virtual gtCamera*		addCamera2D( const v4f& viewport, s32 id = -1, bool setActive = true ) = 0;

			///	Add sprite
			/// \param texture: texture
			/// \param driver: video driver
			/// \param size: width and height
			/// \param position: position
			/// \param asBillboard: \b true if you need billboard
			///	\return created sprite
		virtual gtSprite*		addSprite( gtTexture * texture, gtDriver * driver, const v2f& size, const v3f& position = v3f(0.f,0.f,5.f), bool asBillboard = false ) = 0;

			/// Get active camera
			/// \return active camera
		virtual gtCamera*		getActiveCamera( void ) = 0;

			/// Set active camera
			/// \param camera: new active camera
		virtual void			setActiveCamera( gtCamera* camera ) = 0;

			/// Get main object on scene. All objects added with "add" methods are childs of rootObject.
			///	You can change transformation, and all childs change transformations too
			/// \return Main object in virtual scene
			/// \remark do not release or use gtPtr
		virtual gtGameObject*	getRootObject( void ) = 0;

			/// Remove object from scene. Also remove all childs.
			/// \param object: object for remove
		virtual void removeObject( gtGameObject* object ) = 0;

			///	Render all objects, which added by method with "add" prefix
		virtual void renderScene( void ) = 0;

			///	Remove objects from scene
		virtual void clearScene( void ) = 0;

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