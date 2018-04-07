/*!	GOST
	\file gtPluginSystem.h
	\brief work with plugins
*/
#pragma once
#ifndef __GT_PLUGIN_SYSTEM_H__
#define __GT_PLUGIN_SYSTEM_H__ //< include guard

#include <gtPlugin.h>

namespace gost{
	
	class gtMainSystem;


		//	указывается при создании gtDriver
	#define GT_UID_RENDER_D3D11 u"{41B20362-9FC0-4C40-9903-B8D2FF98CF88}"

		//	аудио плагины
	#define GT_UID_AUDIO_XADUDIO2 u"{220F7289-38BA-415C-A8F7-4583F73C6091}"
	
		//	указывается при точном выборе плагина загрузки картинок
	#define GT_UID_IMPORT_IMAGE_BMP u"{B0904D7B-5AA3-4023-BC99-ECA2232E1EBE}"
	#define GT_UID_IMPORT_IMAGE_PNG u"{C6FFFD23-DA18-4D78-9DF2-65B5DA0EF41E}"
	#define GT_UID_IMPORT_MODEL_OBJ u"{34C557D4-694F-477A-884E-ADE63D3E774B}"

		//	для загрузки функций из плагинов
	using gtGetPluginInfo			= void(GT_CDECL*)		(gtPluginInfo&);		//< общая функция для всех плагинов
	using gtLoadGPUDriver_t			= gtDriver*(GT_CDECL*)	(gtDriverInfo);			//< функция запуска GPU драйвера
	using gtLoadAudioDriver_t		= gtAudioSystem*(GT_CDECL*)(void);				//< функция запуска GPU драйвера
	using gtPluginGetExtCount_t		= u32(GT_CDECL*)		( void );				//< ф-я получения количества расширений. импорт/экспрт плагины
	using gtPluginGetExtension_t	= s8*(GT_CDECL*)		( u32 id );				//< ф-я получения расширения. импорт/экспрт плагины
	using gtPluginLoadImage_t		= bool(GT_CDECL*)		(gtImage*,gtString*);	//< ф-я загрузки картинки
	using gtPluginLoadModel_t		= gtModel*(GT_CDECL*)	(gtString*);			//< ф-я загрузки модели
	
	class gtPluginRender;
	class gtPluginImportImage;
	class gtPluginImportModel;
	class gtPluginAudio;


		//	Интерфейс для работы с плагинами
	class gtPluginSystem : public gtRefObject{
	public:

			//	получить количество плагинов в папке plugins
			// \return Вернёт количество доступных плагинов
		virtual u32	getNumOfPlugins( void ) = 0;

			// Получить плагин по уникальному номеру
			// \param uid: уникальный номер
			// \return доступный плагин
			// \remark вернёт nullptr если плагина нет
		virtual gtPlugin*	getPlugin( const gtString& uid ) = 0;

			// Получить плагин по порядковому номеру (see \ref getNumOfPlugins)
			// \param id: порядковый номер
			// \return доступный плагин
			// \remark вернёт nullptr если плагина нет
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

	/**
		Это обёртка к render плагину.
		Хранит информацию.
		Благодоря этому можно грузить и выгружать плагин 
		используя методы load unload
	*/
	class gtPluginRender : public gtPlugin{
	public:
			// c-tor
			// \param info: общая информация
		gtPluginRender( gtPluginInfoDL* info );

			// d-tor
		~gtPluginRender( void );

			//	указатель на функцию запуска render плагина
			// \param params: параметры запуска
			// \return gtDriver*
		gtLoadGPUDriver_t loadDriverProc/*(const gtDriverInfo& params)*/;

			//	загрузить плагин
			// \param params: параметры запуска
			// \return gtDriver*
			// \remark просто автоматически вызывает load и loadDriverProc
		virtual gtDriver * loadDriver( const gtDriverInfo& params );

			//	Загрузить плагин в память
		void load( void );

			//	Выгрузить плагин изпамяти
		void unload( void );

			// Получить информацию о плагине
			// \return Структура с информацией
		const gtPluginInfoDL&	getInfo( void );

			//	Проверить есть ли нужные функции в плагине
			// \return \b true если в плагине есть все необходимые функции
		bool checkLibraryFunctions( void );
	};

		// audio plugin
	class gtPluginAudio : public gtPlugin{
	public:
			// c-tor
			// \param info: общая информация
		gtPluginAudio( gtPluginInfoDL* info );

			// d-tor
		~gtPluginAudio( void );

			//	указатель на функцию запуска audio плагина
			// \return gtAudioSystem*
		gtLoadAudioDriver_t loadAudioDriverProc;

		virtual gtAudioSystem* loadAudioDriver();

			//	Загрузить плагин в память
		void load( void );

			//	Выгрузить плагин изпамяти
		void unload( void );

			// Получить информацию о плагине
			// \return Структура с информацией
		const gtPluginInfoDL&	getInfo( void );

			//	Проверить есть ли нужные функции в плагине
			// \return \b true если в плагине есть все необходимые функции
		bool checkLibraryFunctions( void );
	};

		//	Плагин загрузки моделей
	class gtPluginImportModel : public gtPlugin{
	public:

			// c-tor
			// \param info: общая информация
		gtPluginImportModel( gtPluginInfoDL* info );

			// d-tor
		~gtPluginImportModel( void );
		
			// указатель на функцию загрузки модели
		gtPluginLoadModel_t f_loadModel;

			//	Загрузить плагин в память
		void load( void );

			//	Выгрузить плагин из памяти
		void unload( void );

			// Получить информацию о плагине
			// \return Структура с информацией
		const gtPluginInfoDL&	getInfo( void );

			//	Проверить есть ли нужные функции в плагине
			// \return \b true если в плагине есть все необходимые функции
		bool checkLibraryFunctions( void );

			// Массив с расширениями
		gtArray<gtString> m_extensions;

			// Загрузить модель
			// \param fileName: путь к файлу
			// \return модель
			// \remark плагин попытается загрузить тлько те файлы, расширение которых совпадает с расширениями из массива \b m_extensions
			// \remark для обычной загрузки лучше использовать метод из gtModelSystem
		virtual gtModel * loadModel( gtString* fileName );
	};


		//	Плагин загрузки картинок
	class gtPluginImportImage : public gtPlugin{
	public:

			// c-tor
			// \param info: общая информация
		gtPluginImportImage( gtPluginInfoDL* info );

			// d-tor
		~gtPluginImportImage( void );

			// Указатель на функцию загрузки картинки
			// \param im: пустой укозатель на создаваемую картинку
			// \param fileName: путь к файлу
		gtPluginLoadImage_t f_loadImage/*(gtImage*im,gtString*fileName)*/;
		
			//	Загрузить плагин в память
		void load( void );

			//	Выгрузить плагин из памяти
		void unload( void );

			// Получить информацию о плагине
			// \return Структура с информацией
		const gtPluginInfoDL&	getInfo( void );

			// Проверить есть ли нужные функции в плагине
			// \return \b true если есть все необходимые функции
		bool checkLibraryFunctions( void );

			// Список расширение
		gtArray<gtString> m_extensions;

			// Загрузить картинку
			// \param fileName: путь к файлу
			// \param im: пустой укозатель на создаваемую картинку
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