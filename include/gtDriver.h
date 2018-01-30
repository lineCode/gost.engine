/*!	GOST
	\file gtDriver.h
	\brief video driver
*/
#pragma once
#ifndef __GT_DRIVER_H__
#define __GT_DRIVER_H__ ///< include guard

namespace gost{

	class gtWindow;


		///	параметры запуска драйвера
	struct gtDriverInfo{

			/// c-tor
		gtDriverInfo( void ):
			m_fullScreen( false ),
			m_stencilBuffer( true ),
			m_doubleBuffer( true ),
			m_vSync( false ),
			m_colorDepth( 32u ),
			m_adapterID( 0u ),
			m_outWindow( nullptr )
		{
			m_backBufferSize.set({800,600});
		}

			///	разрешение экрана
		v2i		m_backBufferSize;

			///	запускать ли программу в полноэкранном режиме
		bool	m_fullScreen;

			///	использовать ли стенсильный буффер. Нужен для OGL, в D3D по умолчанию есть.
		bool	m_stencilBuffer;

			///	двойная буферизация
		bool	m_doubleBuffer;

			///	вертикльная синхронизация
		bool	m_vSync;

			///	глубина цвета - биты на пиксель
		u8		m_colorDepth;

			/// only for Direct3D
		u32		m_adapterID;

			///	окно для рисования
		gtWindow * m_outWindow;
	};

		///	видео драйвер
	class gtDriver : public gtRefObject{
	public:

			///	получить параметры
			/// \return gtDriverInfo
		virtual const gtDriverInfo&	getParams( void ) = 0;
			
			/// начало рисования. Перед рисованием нужно вызвать этот метод. в конце рисования нужно вызвать метод endRender
			/// \param clearRenderTarget: \b true если нужно очистить render target
			/// \param color: цвет очистки render target
		virtual void beginRender( bool clearRenderTarget = true, const gtColor& color = gtColor(0.f) ) = 0;

			/// завершение рисования. Нужно вызывать после вызова beginRender
		virtual void endRender( void ) = 0;

			///	нарисует картинку. В материале можно указать шейдер
			///	\param rect: координаты левого верхнего и правого нижнего углов
			///	\param m: материал с текстурой
		virtual void draw2DImage( const v4i& rect, const gtMaterial& m ) = 0;

			///	нарисует картинку. Используется стандартный шейдер
			///	\param rect: координаты левого верхнего и правого нижнего углов
			///	\param m: материал с текстурой
		virtual void draw2DImage( const v4i& rect, gtTexture* texture ) = 0;

			///	нарисует gtRenderModel
			/// \param model: модель для рисования
		virtual void drawModel( gtRenderModel* model ) = 0;

			///	Render 2d image using region of texture. Нарисует картинку с выбранной областью текстуры
			/// \param rect: координаты левого верхнего и правого нижнего углов
			/// \param region: координаты левого верхнего и правого нижнего углов области картинки которую нужно нарисовать
			/// \param m: материал с текстурой
		virtual void draw2DImage( const v4i& rect, const v4i& region, const gtMaterial& m ) = 0;
		virtual void draw2DImage( const v4i& rect, const v4i& region, gtTexture* texture ) = 0;

			///	компилировать либо получить ранее скомпилированный шейдер
			/// \param callback: callback для установки параметров шейдера
			/// \param vertexShader: путь к файлу хранящем вершинный шейдер
			/// \param vertexShaderMain: главная функция вершинного шейдера, точка входа
			/// \param pixelShader: путь к файлу хранящем пиксельный/фрагментный шейдер
			/// \param pixelShaderMain: главная функция пиксельного/фрагментного шейдера, точка входа
			/// \param shaderModel: тип шейдерного языка
			/// \param vertexType: тип вершины (должен быть массив)
			/// \return созданный шейдер
		virtual gtShader *	getShader(
			gtShaderCallback * callback,
			const gtString& vertexShader,
			const gtStringA& vertexShaderMain,
			const gtString& pixelShader,
			const gtStringA& pixelShaderMain,
			gtShaderModel shaderModel,
			gtVertexType * vertexType
			) = 0;


			///	Создаёт текстуру из gtImage. Обязательно нужно в ручную удалить вызвав release()
			/// \param sourceImage: картинка из которой создаётся текстура
			/// \param filter: фильтр
			/// \return созданная текстура
		virtual gtTexture*	createTexture( gtImage* sourceImage, gtTextureFilterType filter ) = 0;

			///	Создаёт модель для рисования
			/// \param software_model: обычная модель
			/// \return созданная модель
		virtual gtRenderModel*	createModel( gtModel* software_model ) = 0;
	};

		///	реализация драйвера в отдельных dll
		///	по этому создавать общий класс вижу только таким способом
	class gtDriverCommon : public gtDriver{
	protected:

			///	параметры запуска
		gtDriverInfo m_params;
		
			///	текущий размер окна
		v2i			m_currentWindowSize;

	public:

			/// c-tor
		gtDriverCommon( void )
		{
#ifdef GT_DEBUG
			m_debugName.assign(u"gtDriver");
#endif
		}

			/// d-tor
		virtual ~gtDriverCommon( void ){
		}


			///	получить параметры
			/// \return gtDriverInfo
		virtual const gtDriverInfo&	getParams( void ){
			return m_params;
		}

			///	получить текущий размер окна
			/// \return размер окна
		virtual const v2i&	getСurrentWindowSize( void ) const {
			return m_currentWindowSize;
		}

	};

}

#endif

/*
Copyright (c) 2017, 2018 532235

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