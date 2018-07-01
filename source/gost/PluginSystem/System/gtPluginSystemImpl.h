//	GOST

#pragma once
#ifndef __GT_PLUGIN_SYSTEM_IMPL_H__
#define __GT_PLUGIN_SYSTEM_IMPL_H__

namespace gost{

	class gtPluginRender;
	class gtPluginImportImage;
	class gtPluginImportModel;
	class gtPluginSystemImpl GT_FINAL : public gtPluginSystem{

		u32									m_numOfPlugins;

		gtArray<gtPlugin*>					m_plugins;
		gtArray<gtPtr<gtPluginRender>>		m_renderPluginCache;
		gtArray<gtPtr<gtPluginAudio>>		m_audioPluginCache;
		gtArray<gtPtr<gtPluginInput>>		m_inputPluginCache;
		gtArray<gtPtr<gtPluginImportImage>> m_importImagePluginCache;
		gtArray<gtPtr<gtPluginImportModel>> m_importModelPluginCache;
		
	public:
		gtPluginSystemImpl();
		virtual ~gtPluginSystemImpl();

		bool		init();
		void		scanFolder( const gtString& );
		u32			getNumOfPlugins();
		gtPlugin*	getPlugin( const GT_GUID& uid );
		gtPlugin*	getPlugin( u32 id );
		gtImage *	importImage( const gtString& fileName, const GT_GUID& guid = GT_GUID(), bool useguid = false );
		gtModel *	importModel( const gtString& fileName, const GT_GUID& guid = GT_GUID(), bool useguid = false );

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