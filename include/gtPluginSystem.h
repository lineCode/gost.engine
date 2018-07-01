#pragma once
#ifndef __GT_PLUGIN_SYSTEM_H__
#define __GT_PLUGIN_SYSTEM_H__

#include <gtPlugin.h>

namespace gost{
	
	class gtMainSystem;


	GT_DEFINE_GUID(GT_UID_RENDER_D3D11, 0xd3a6e16b, 0x65d5, 0x4e44, 0x99, 0x90, 0x44, 0x6, 0x79, 0x13, 0x2e, 0x71);
	GT_DEFINE_GUID(GT_UID_AUDIO_XADUDIO2, 0x3f6d55c3, 0x4479, 0x4e50, 0x94, 0x26, 0x99, 0xc8, 0x75, 0x5, 0x3f, 0x9a);
	GT_DEFINE_GUID(GT_UID_INPUT_DINPUT, 0x27450a9f, 0x8272, 0x4b8f, 0xa2, 0xdf, 0x1d, 0xaa, 0x95, 0x80, 0x45, 0xe7);
	GT_DEFINE_GUID(GT_UID_IMPORT_IMAGE_BMP, 0xcb2c9c1, 0x233a, 0x4570, 0x8d, 0x1e, 0x31, 0xe, 0xb1, 0xa9, 0xf4, 0xf7);
	GT_DEFINE_GUID(GT_UID_IMPORT_IMAGE_PNG, 0x52b5e4a2, 0xcbda, 0x4492, 0x90, 0xb9, 0x97, 0xaf, 0xeb, 0xec, 0xf4, 0x90);
	GT_DEFINE_GUID(GT_UID_IMPORT_MODEL_OBJ, 0xddb35bd5, 0x41ca, 0x48d8, 0x9d, 0xf5, 0x5b, 0x67, 0xa3, 0xa5, 0xa1, 0x55);


	using gtGetPluginInfo			= void(GT_CDECL*)		(gtPluginInfo&);		
	using gtLoadGPUDriver_t			= gtDriver*(GT_CDECL*)	(gtDriverInfo);			
	using gtLoadAudioDriver_t		= gtAudioSystem*(GT_CDECL*)(void);
	using gtLoadInputDriver_t		= gtGameController*(GT_CDECL*)(void);
	using gtPluginGetExtCount_t		= u32(GT_CDECL*)		();				
	using gtPluginGetExtension_t	= s8*(GT_CDECL*)		( u32 id );				
	using gtPluginLoadImage_t		= bool(GT_CDECL*)		(gtImage*,gtString*);	
	using gtPluginLoadModel_t		= gtModel*(GT_CDECL*)	(gtString*);			
	
	class gtPluginRender;
	class gtPluginImportImage;
	class gtPluginImportModel;
	class gtPluginAudio;
	class gtPluginInput;


	class gtPluginSystem : public gtRefObject{
	public:

		virtual u32	getNumOfPlugins() = 0;

		virtual gtPlugin*	getPlugin( const GT_GUID& uid ) = 0;

		virtual gtPlugin*	getPlugin( u32 id ) = 0;

		gtPluginImportImage * getAsPluginImportImage( gtPlugin * plugin ){
			if( plugin->getInfo().m_info.m_type == gtPluginType::Import_image )
				return reinterpret_cast<gtPluginImportImage*>(plugin);
			return nullptr;
		}

		gtPluginImportModel * getAsPluginImportModel( gtPlugin * plugin ){
			if( plugin->getInfo().m_info.m_type == gtPluginType::Import_model )
				return reinterpret_cast<gtPluginImportModel*>(plugin);
			return nullptr;
		}

		gtPluginRender * getAsPluginRender( gtPlugin * plugin ){
			if( plugin->getInfo().m_info.m_type == gtPluginType::Render )
				return reinterpret_cast<gtPluginRender*>(plugin);
			return nullptr;
		}

		gtPluginAudio * getAsPluginAudio( gtPlugin * plugin ){
			if( plugin->getInfo().m_info.m_type == gtPluginType::Audio )
				return reinterpret_cast<gtPluginAudio*>(plugin);
			return nullptr;
		}

		gtPluginInput * getAsPluginInput( gtPlugin * plugin ){
			if( plugin->getInfo().m_info.m_type == gtPluginType::Input )
				return reinterpret_cast<gtPluginInput*>(plugin);
			return nullptr;
		}

	};

	class gtPluginRender : public gtPlugin{
	public:
		gtPluginRender( gtPluginInfoDL* info );

			// d-tor
		~gtPluginRender();

		gtLoadGPUDriver_t loadDriverProc/*(const gtDriverInfo& params)*/;

		virtual gtDriver * loadDriver( const gtDriverInfo& params );

		void load();

		void unload();

		const gtPluginInfoDL&	getInfo();

		bool checkLibraryFunctions();
	};

		// audio plugin
	class gtPluginAudio : public gtPlugin{
	public:
			// c-tor
		gtPluginAudio( gtPluginInfoDL* info );

			// d-tor
		~gtPluginAudio();

		gtLoadAudioDriver_t loadAudioDriverProc;

		virtual gtAudioSystem* loadAudioDriver();

		void load();

		void unload();

		const gtPluginInfoDL&	getInfo();

		bool checkLibraryFunctions();
	};

	class gtPluginInput : public gtPlugin{
	public:
			// c-tor
		gtPluginInput( gtPluginInfoDL* info );

			// d-tor
		~gtPluginInput();

		gtLoadInputDriver_t loadInputDriverProc;

		virtual gtGameController* loadInputDriver();

		void load();

		void unload();

		const gtPluginInfoDL&	getInfo();

		bool checkLibraryFunctions();
	};

	class gtPluginImportModel : public gtPlugin{
	public:

			// c-tor
		gtPluginImportModel( gtPluginInfoDL* info );

			// d-tor
		~gtPluginImportModel();
		
		gtPluginLoadModel_t f_loadModel;

		void load();

		void unload();

		const gtPluginInfoDL&	getInfo();

		bool checkLibraryFunctions();

		gtArray<gtString> m_extensions;

		virtual gtModel * loadModel( gtString* fileName );
	};


	class gtPluginImportImage : public gtPlugin{
	public:

			// c-tor
		gtPluginImportImage( gtPluginInfoDL* info );

			// d-tor
		~gtPluginImportImage();

		gtPluginLoadImage_t f_loadImage/*(gtImage*im,gtString*fileName)*/;
		
		void load();

		void unload();

		const gtPluginInfoDL&	getInfo();

		bool checkLibraryFunctions();

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