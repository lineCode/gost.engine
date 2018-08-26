#pragma once
#ifndef __GT_PLUGIN_SYSTEM_H__
#define __GT_PLUGIN_SYSTEM_H__

#include <PluginSystem/gtPlugin.h>

namespace gost{
	
	class gtMainSystem;


	GT_DEFINE_GUID(GT_UID_RENDER_D3D11, 0xd3a6e16b, 0x65d5, 0x4e44, 0x99, 0x90, 0x44, 0x6, 0x79, 0x13, 0x2e, 0x71);
	GT_DEFINE_GUID(GT_UID_AUDIO_XADUDIO2, 0x3f6d55c3, 0x4479, 0x4e50, 0x94, 0x26, 0x99, 0xc8, 0x75, 0x5, 0x3f, 0x9a);
	GT_DEFINE_GUID(GT_UID_INPUT_DINPUT, 0x27450a9f, 0x8272, 0x4b8f, 0xa2, 0xdf, 0x1d, 0xaa, 0x95, 0x80, 0x45, 0xe7);
	GT_DEFINE_GUID(GT_UID_IMPORT_IMAGE_BMP, 0xcb2c9c1, 0x233a, 0x4570, 0x8d, 0x1e, 0x31, 0xe, 0xb1, 0xa9, 0xf4, 0xf7);
	GT_DEFINE_GUID(GT_UID_IMPORT_IMAGE_PNG, 0x52b5e4a2, 0xcbda, 0x4492, 0x90, 0xb9, 0x97, 0xaf, 0xeb, 0xec, 0xf4, 0x90);
	GT_DEFINE_GUID(GT_UID_IMPORT_MODEL_OBJ, 0xddb35bd5, 0x41ca, 0x48d8, 0x9d, 0xf5, 0x5b, 0x67, 0xa3, 0xa5, 0xa1, 0x55);
	GT_DEFINE_GUID(GT_UID_PHYSICS_BULLET_3_2_87, 0x7cddd6fc, 0x6de0, 0x4024, 0xa1, 0xf5, 0xed, 0xb3, 0x79, 0x87, 0xb8, 0xfc);


	using gtGetPluginInfo			= void(GT_CDECL*)		        (gtPluginInfo*);
	using gtLoadGPUDriver_t			= gtGraphicsSystem*(GT_CDECL*)	(gtGraphicsSystemInfo*);
	using gtLoadPhysicsPlugin_t		= gtPhysicsSystem*(GT_CDECL*)	(gtPhysicsSystemInfo*);
	using gtLoadAudioDriver_t		= gtAudioSystem*(GT_CDECL*)     ();
	using gtLoadInputDriver_t		= gtInputController*(GT_CDECL*) ();
	using gtPluginGetExtCount_t		= u32(GT_CDECL*)				();				
	using gtPluginGetExtension_t	= const s8*(GT_CDECL*)		    ( u32 id );				
	using gtPluginLoadImage_t		= bool(GT_CDECL*)				(gtImage*,gtString*);	
	using gtPluginLoadModel_t		= gtModel*(GT_CDECL*)			(gtString*);			
	
	class gtPluginImpl{
	protected:
		gtArray<gtPair<gtStringA,void*>> m_functions;
	public:
		gtPluginImpl(){}
		virtual ~gtPluginImpl(){}
		
		u32 getFunctionsCount(){
			return m_functions.size();
		}
		
		const gtStringA& getFunctionName( u32 i ){
			GT_ASSERT3(i<m_functions.size());
			return m_functions[ i ].m_first;
		}
		
		void * getFunctionPtr( u32 i ){
			GT_ASSERT3(i<m_functions.size());
			return m_functions[ i ].m_second;
		}
		
		void setFunctionPtr( u32 i, void * ptr ){
			GT_ASSERT3(i<m_functions.size());
			m_functions[ i ].m_second = ptr;
		}
		
	};
	
#define gtPlugin_addFunction(x,y) m_functions.push_back(gtPair<gtStringA,void*>(x,y))
	
	class gtPluginPhysics : public gtPluginImpl{
	public:
		gtPluginPhysics(){
			gtPlugin_addFunction("gtLoadPhysicsPlugin", nullptr);
		}
	
		//gtLoadPhysicsPlugin_t      loadPhysicsProc = nullptr/*(const gtGraphicsSystemInfo& params)*/;
		gtPhysicsSystem *          loadPhysics( gtPhysicsSystemInfo* params ){
			auto sz = getFunctionsCount();
			for( u32 i = 0u; i < sz; ++i ){
				auto fn = getFunctionName( i );
				if( fn == "gtLoadPhysicsPlugin" ){
					gtLoadPhysicsPlugin_t loadPhysicsProc = (gtLoadPhysicsPlugin_t)getFunctionPtr( i );
					return loadPhysicsProc( params );
				}
			}
			return nullptr;
		}
	};

	class gtPluginRender  : public gtPluginImpl{
	public:
		gtPluginRender(){
			gtPlugin_addFunction("gtLoadGPUDriver", nullptr);
		}
		
	//	gtLoadGPUDriver_t          loadDriverProc = nullptr/*(const gtGraphicsSystemInfo& params)*/;
		gtGraphicsSystem *         loadDriver( gtGraphicsSystemInfo* params ){
			auto sz = getFunctionsCount();
			for( u32 i = 0u; i < sz; ++i ){
				auto fn = getFunctionName( i );
				if( fn == "gtLoadGPUDriver" ){
					gtLoadGPUDriver_t loadDriverProc = (gtLoadGPUDriver_t)getFunctionPtr( i );
					return loadDriverProc( params );
				}
			}
			return nullptr;
		}
	};

	class gtPluginAudio  : public gtPluginImpl{
	public:
		gtPluginAudio(){
			gtPlugin_addFunction("gtLoadAudioPlugin", nullptr);
		}
		
	//	gtLoadAudioDriver_t    loadAudioDriverProc = nullptr;
		gtAudioSystem*         loadAudioDriver(){
			auto sz = getFunctionsCount();
			for( u32 i = 0u; i < sz; ++i ){
				auto fn = getFunctionName( i );
				if( fn == "gtLoadAudioPlugin" ){
					gtLoadAudioDriver_t loadAudioDriverProc = (gtLoadAudioDriver_t)getFunctionPtr( i );
					return loadAudioDriverProc();
				}
			}
			return nullptr;
		}
	};

	class gtPluginInput  : public gtPluginImpl{
	public:
		gtPluginInput(){
			gtPlugin_addFunction("gtLoadInputDriver", nullptr);
		}
		
	//	gtLoadInputDriver_t        loadInputDriverProc = nullptr;
		gtInputController*         loadInputDriver(){
			auto sz = getFunctionsCount();
			for( u32 i = 0u; i < sz; ++i ){
				auto fn = getFunctionName( i );
				if( fn == "gtLoadInputDriver" ){
					gtLoadInputDriver_t loadInputDriverProc = (gtLoadInputDriver_t)getFunctionPtr( i );
					return loadInputDriverProc();
				}
			}
			return nullptr;
		}
	};

	class gtPluginImportModel  : public gtPluginImpl{
	public:
		gtPluginImportModel(){
			gtPlugin_addFunction("PluginLoadModel", nullptr);
			gtPlugin_addFunction("PluginGetExtCount", nullptr);
			gtPlugin_addFunction("PluginGetExtension", nullptr);
		}
		
	//	gtPluginLoadModel_t   f_loadModel = nullptr;
		gtArray<gtString>     m_extensions;
		gtModel *             loadModel( gtString* fileName ){
			auto sz = getFunctionsCount();
			for( u32 i = 0u; i < sz; ++i ){
				auto fn = getFunctionName( i );
				if( fn == "PluginLoadModel" ){
					gtPluginLoadModel_t f_loadModel = (gtPluginLoadModel_t)getFunctionPtr( i );
					return f_loadModel(fileName);
				}
			}
			return nullptr;
		}
	};


	class gtPluginImportImage  : public gtPluginImpl{
	public:
		gtPluginImportImage(){
			gtPlugin_addFunction("PluginLoadImage", nullptr);
			gtPlugin_addFunction("PluginGetExtCount", nullptr);
			gtPlugin_addFunction("PluginGetExtension", nullptr);
		}
		
		//gtPluginLoadImage_t   f_loadImage = nullptr/*(gtImage*im,gtString*fileName)*/;
		gtArray<gtString>     m_extensions;
		void                  loadImage( gtString* fileName, gtImage** im ){
			auto sz = getFunctionsCount();
			for( u32 i = 0u; i < sz; ++i ){
				auto fn = getFunctionName( i );
				if( fn == "PluginLoadImage" ){
					gtPluginLoadImage_t f_loadImage = (gtPluginLoadImage_t)getFunctionPtr( i );
					f_loadImage(*im,fileName);
				}
			}
		}
		
	};


	class gtPluginCommon : public gtPlugin{
		gtPluginImpl * m_pl;
	public:
		gtPluginCommon();
		virtual ~gtPluginCommon();
	
		gtPluginImpl * getImplementation(){ 
			return m_pl; 
		}
		
		void           setImplementation( gtPluginImpl * pl ){
			GT_ASSERT3( m_pl == nullptr );
			m_pl = pl;
		}
	
		// общая реализация
		void  load();
		void  unload();
		
		const gtPluginInfoDL& getInfo(){
			return m_info;
		}
		
		void setInfo( const gtPluginInfoDL& i ){
			m_info = i;
		}
		bool  checkLibraryFunctions();
	};
	

	class gtPluginSystem : public gtRefObject{
	public:
		virtual u32	        getNumOfPlugins() = 0;
		virtual gtPlugin*	getPlugin( const GT_GUID& uid ) = 0;
		virtual gtPlugin*	getPlugin( u32 id ) = 0;
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