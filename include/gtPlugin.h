/*!	GOST
	\file gtPlugin.h
	\brief work with plugins
*/
#pragma once
#ifndef __GT_PLUGIN_H__
#define __GT_PLUGIN_H__ //< include guard

namespace gost{

		//	тип плагина
	enum class gtPluginType{

			//	плагины должны быть правильно инициализированы
		unknown,

			//	рендер
		render,

			//	импортёр картинок
		import_image,

			//	импортёр моделей
		import_model,

			// аудио плагин
		audio,

			// экспортёр картинок
		export_image,

			// экспортёр моделей
		export_model,

			// устройства ввода
		input

	};

	struct gtPluginInfo{

			// c-tor
		gtPluginInfo( void ):
			m_type( gtPluginType::unknown ),
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
		gtPluginInfoDL( void ):
			m_handle( nullptr )
		{}

			// d-tor
		~gtPluginInfoDL( void ){}

			//	путь к файлу
		gtString m_path;
		
			//	handle плагина
#ifdef GT_EXPORTS
		GT_LIBRARY_HANDLE
#else
		void*
#endif
			m_handle;

			//	общая информация
		gtPluginInfo m_info;
	};
	
		// Плагин
	class gtPlugin : public gtRefObject{
	protected:

			//	информация
		gtPluginInfoDL	m_info;

			//	загружен ли плагин
		bool m_isLoad;

	public:

			// c-tor
		gtPlugin( void ):
			m_isLoad( false )
		{}

			// d-tor
		virtual ~gtPlugin( void ){	}

			// Загружен ли плагин
			// \return \b true если загружен
		bool isLoad( void ) const { return m_isLoad; }

			//	Загрузить плагин
		virtual void load( void ) = 0;

			//	Выгрузить плагин
		virtual void unload( void ) = 0;

			// Получить информацию о плагине
			// \return Вернёт информацию о плагиен
		virtual const gtPluginInfoDL&	getInfo( void ) = 0;

			//	Проверить, есть ли нужные функции в плагине
			// \return \b true если есть все нужные функции
		virtual bool checkLibraryFunctions( void ) = 0;

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