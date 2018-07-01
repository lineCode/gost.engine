#pragma once
#ifndef __GT_PLUGIN_H__
#define __GT_PLUGIN_H__ 

namespace gost{

	enum class gtPluginType{
		Unknown,
		Render,
		Import_image,
		Import_model,
		Audio,
		Export_image,
		Export_model,
		Input
	};

	struct gtPluginInfo{

			// c-tor
		gtPluginInfo():
			m_type( gtPluginType::Unknown ),
			m_version( 0u ),
			m_build( 0u )
		{
			m_name.reserve( 64u );
			m_author.reserve( 64u );
			m_url.reserve( 64u );
			m_email.reserve( 64u );
			m_company.reserve( 64u );
			m_description.reserve( 512u );
		}

		gtPluginType m_type;

		gtString m_name;

		gtString m_author;

		gtString m_url;

		gtString m_email;

		gtString m_company;

		gtString m_description;

		GT_GUID m_GUID;

		u32 m_version;

		u32 m_build;

	};

	/**	
	*/
	struct gtPluginInfoDL{

			// c-tor
		gtPluginInfoDL():
			m_handle( nullptr )
		{}

			// d-tor
		~gtPluginInfoDL(){}

		gtString m_path;
		
	
#ifdef GT_EXPORTS
		GT_LIBRARY_HANDLE
#else
		void*
#endif
			m_handle;

		gtPluginInfo m_info;
	};
	
	class gtPlugin : public gtRefObject{
	protected:

		gtPluginInfoDL	m_info;

		bool m_isLoad;

	public:

			// c-tor
		gtPlugin():
			m_isLoad( false )
		{}

			// d-tor
		virtual ~gtPlugin(){	}

		bool isLoad() const { return m_isLoad; }

		virtual void load() = 0;

		virtual void unload() = 0;

		virtual const gtPluginInfoDL&	getInfo() = 0;

		virtual bool checkLibraryFunctions() = 0;

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