#pragma once
#ifndef __GT_PLUGIN_SYSTEM_H__
#define __GT_PLUGIN_SYSTEM_H__ //< include guard

#include <gtPlugin.h>

namespace gost{
	
	class gtMainSystem;


	#define GT_UID_RENDER_D3D11 u"{41B20362-9FC0-4C40-9903-B8D2FF98CF88}"

	#define GT_UID_AUDIO_XADUDIO2 u"{220F7289-38BA-415C-A8F7-4583F73C6091}"

	#define GT_UID_INPUT_DINPUT u"{4671CE44-D4D9-4AE2-9EE0-901296CAF7DA}"
	
	#define GT_UID_IMPORT_IMAGE_BMP u"{B0904D7B-5AA3-4023-BC99-ECA2232E1EBE}"
	#define GT_UID_IMPORT_IMAGE_PNG u"{C6FFFD23-DA18-4D78-9DF2-65B5DA0EF41E}"
	#define GT_UID_IMPORT_MODEL_OBJ u"{34C557D4-694F-477A-884E-ADE63D3E774B}"

	using gtGetPluginInfo			= void(GT_CDECL*)		(gtPluginInfo&);		
	using gtLoadGPUDriver_t			= gtDriver*(GT_CDECL*)	(gtDriverInfo);			
	using gtLoadAudioDriver_t		= gtAudioSystem*(GT_CDECL*)(void);
	using gtLoadInputDriver_t		= gtGameController*(GT_CDECL*)(void);
	using gtPluginGetExtCount_t		= u32(GT_CDECL*)		( void );				
	using gtPluginGetExtension_t	= s8*(GT_CDECL*)		( u32 id );				
	using gtPluginLoadImage_t		= bool(GT_CDECL*)		(gtImage*,gtString*);	
	using gtPluginLoadModel_t		= gtModel*(GT_CDECL*)	(gtString*);			
	
	class gtPluginRender;
	class gtPluginImportImage;
	class gtPluginImportModel;
	class gtPluginAudio;


	class gtPluginSystem : public gtRefObject{
	public:

		virtual u32	getNumOfPlugins( void ) = 0;

		virtual gtPlugin*	getPlugin( const gtString& uid ) = 0;

		virtual gtPlugin*	getPlugin( u32 id ) = 0;

		gtPluginImportImage * getAsPluginImportImage( gtPlugin * plugin ){
			if( plugin->getInfo().m_info.m_type == gtPluginType::import_image )
				return reinterpret_cast<gtPluginImportImage*>(plugin);
			return nullptr;
		}

		gtPluginImportModel * getAsPluginImportModel( gtPlugin * plugin ){
			if( plugin->getInfo().m_info.m_type == gtPluginType::import_model )
				return reinterpret_cast<gtPluginImportModel*>(plugin);
			return nullptr;
		}

		gtPluginRender * getAsPluginRender( gtPlugin * plugin ){
			if( plugin->getInfo().m_info.m_type == gtPluginType::render )
				return reinterpret_cast<gtPluginRender*>(plugin);
			return nullptr;
		}

		gtPluginAudio * getAsPluginAudio( gtPlugin * plugin ){
			if( plugin->getInfo().m_info.m_type == gtPluginType::audio )
				return reinterpret_cast<gtPluginAudio*>(plugin);
			return nullptr;
		}


	};

	class gtPluginRender : public gtPlugin{
	public:
		gtPluginRender( gtPluginInfoDL* info );

			// d-tor
		~gtPluginRender( void );

		gtLoadGPUDriver_t loadDriverProc/*(const gtDriverInfo& params)*/;

		virtual gtDriver * loadDriver( const gtDriverInfo& params );

		void load( void );

		void unload( void );

		const gtPluginInfoDL&	getInfo( void );

		bool checkLibraryFunctions( void );
	};

		// audio plugin
	class gtPluginAudio : public gtPlugin{
	public:
			// c-tor
		gtPluginAudio( gtPluginInfoDL* info );

			// d-tor
		~gtPluginAudio( void );

		gtLoadAudioDriver_t loadAudioDriverProc;

		virtual gtAudioSystem* loadAudioDriver();

		void load( void );

		void unload( void );

		const gtPluginInfoDL&	getInfo( void );

		bool checkLibraryFunctions( void );
	};

	class gtPluginInput : public gtPlugin{
	public:
			// c-tor
		gtPluginInput( gtPluginInfoDL* info );

			// d-tor
		~gtPluginInput( void );

		gtLoadInputDriver_t loadInputDriverProc;

		virtual gtGameController* loadInputDriver();

		void load( void );

		void unload( void );

		const gtPluginInfoDL&	getInfo( void );

		bool checkLibraryFunctions( void );
	};

	class gtPluginImportModel : public gtPlugin{
	public:

			// c-tor
		gtPluginImportModel( gtPluginInfoDL* info );

			// d-tor
		~gtPluginImportModel( void );
		
		gtPluginLoadModel_t f_loadModel;

		void load( void );

		void unload( void );

		const gtPluginInfoDL&	getInfo( void );

		bool checkLibraryFunctions( void );

		gtArray<gtString> m_extensions;

		virtual gtModel * loadModel( gtString* fileName );
	};


	class gtPluginImportImage : public gtPlugin{
	public:

			// c-tor
		gtPluginImportImage( gtPluginInfoDL* info );

			// d-tor
		~gtPluginImportImage( void );

		gtPluginLoadImage_t f_loadImage/*(gtImage*im,gtString*fileName)*/;
		
		void load( void );

		void unload( void );

		const gtPluginInfoDL&	getInfo( void );

		bool checkLibraryFunctions( void );

		gtArray<gtString> m_extensions;

		virtual void loadImage( gtString* fileName, gtImage** im );
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