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
			m_backBufferSize.set( 800, 600 );
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

		virtual void drawLine( const v3f& start, const v3f& end ) = 0;

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
		virtual gtTexture*	createTexture( gtImage* sourceImage, gtTextureFilterType filter = gtTextureFilterType::FILTER_ANISOTROPIC ) = 0;

			///	Создаёт модель для рисования
			/// \param software_model: обычная модель
			/// \return созданная модель
		virtual gtRenderModel*	createModel( gtModel* software_model ) = 0;

			///	Get texture from texture cache, or load new texture to the cache
			///	\param fileName: path to the file
			///	\param filter: filter type
			///	\param outImage: Loaded gtImage. If set nullptr, gtImage will be removed.
			///	\return texture
			///	\remark If you get outImage, don't forget call outImage->release() for delete.
		virtual gtTexture*		getTexture( const gtString& fileName, gtTextureFilterType filter = gtTextureFilterType::FILTER_ANISOTROPIC, gtImage** outImage = nullptr ) = 0;

			///	Get model from model cache, or load new model to the cache
			///	\param fileName: path to the file
			///	\param software_model: Loaded software_model. If set nullptr, gtMoel will be removed.
			///	\return render model
			///	\remark If you get software_model, don't forget call software_model->release() for delete.
		virtual gtRenderModel*	getModel( const gtString& fileName, gtModel** software_model = nullptr ) = 0;

			///	clearTextureCache
		virtual void	clearTextureCache( void ) = 0;
		
			/// clearModelCache
		virtual void	clearModelCache( void ) = 0;

		virtual bool	removeModel( gtRenderModel* model ) = 0;
		
		virtual bool	removeTexture( gtTexture* texture ) = 0;


	};

		///	реализация драйвера в отдельных dll
		///	по этому создавать общий класс вижу только таким способом
	class gtDriverCommon : public gtDriver{
	protected:

			///	параметры запуска
		gtDriverInfo m_params;
		
			///	текущий размер окна
		v2i			m_currentWindowSize;

		template<typename type>
		struct cache{
			cache(void):m_object(nullptr){}
			cache(type obj,const gtString&path):m_object(obj),m_path(path){}
			~cache(){if(m_object)m_object->release();}

			type		m_object;
			gtString	m_path;


		};

		gtArray<cache<gtTexture*>>		m_textures;
		gtArray<cache<gtRenderModel*>>	m_models;

		gtTexture* get_texture( const gtString& path, gtTextureFilterType filter, gtImage** im ){
			u32 sz = m_textures.size();
			for( u32 i = 0u; i < sz; ++i ){
				if( path == m_textures[ i ].m_path )
					return m_textures[ i ].m_object;
			}

			gtPtr_t(gtImage,img,gtMainSystem::getInstance()->loadImage( path ));
			if( !img.data() ){
				gtLogWriter::printWarning( u"Can not load image. %s", path.data() );
				return nullptr;
			}

			gtTexture * texture = createTexture( img.data(), filter );
			if( !texture ){
				gtLogWriter::printWarning( u"Can not create texture. %s", path.data() );
				return nullptr;
			}

			if( im ){
				*im = img.data();
				img->addRef();
			}

			texture->addRef();
			m_textures.push_back(cache<gtTexture*>(texture,path));

			return texture;
		}

		gtRenderModel* get_model( const gtString& path, gtModel** m ){
			u32 sz = m_models.size();
			for( u32 i = 0u; i < sz; ++i ){
				if( path == m_models[ i ].m_path )
					return m_models[ i ].m_object;
			}

			gtPtr_t(gtModel,mdl,gtMainSystem::getInstance()->getModelSystem()->createFromFile( path ));
			if( !mdl.data() ){
				gtLogWriter::printWarning( u"Can not load model. %s", path.data() );
				return nullptr;
			}

			gtRenderModel * model = createModel( mdl.data() );
			if( !model ){
				gtLogWriter::printWarning( u"Can not create model. %s", path.data() );
				return nullptr;
			}

			if( m ){
				*m = mdl.data();
				mdl->addRef();
			}

			model->addRef();
			m_models.push_back(cache<gtRenderModel*>(model,path));

			return model;
		}

		bool remove_model( gtRenderModel* model ){
			u32 sz = m_models.size();
			for( u32 i = 0u; i < sz; ++i ){
				if( model == m_models[ i ].m_object ){
			//		model->release();
					m_models.erase(i);
					return true;
				}
			}
			gtLogWriter::printWarning( u"Can not remove model from cache. Model not found." );
			return false;
		}

		bool remove_texture( gtTexture* texture ){
			u32 sz = m_textures.size();
			for( u32 i = 0u; i < sz; ++i ){
				if( texture == m_textures[ i ].m_object ){
					texture->release();
					m_textures.erase(i);
					return true;
				}
			}
			gtLogWriter::printWarning( u"Can not remove texture from cache. Texture not found." );
			return false;
		}

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

		virtual gtTexture*		getTexture( const gtString& fileName, gtTextureFilterType filter, gtImage** outImage ){
			gtString fullPath = fileName;
			if( !gtFileSystem::existFile( fullPath ) ){
				fullPath = gtFileSystem::getRealPath( fileName );

				if( !gtFileSystem::existFile( fullPath ) ){
					gtLogWriter::printWarning( u"Can not load texture. File not exist. %s", fullPath.data() );
					return nullptr;
				}
			}

			return get_texture( fileName, filter, outImage );
		}

		virtual gtRenderModel*		getModel( const gtString& fileName, gtModel** outModel ){
			gtString fullPath = fileName;
			if( !gtFileSystem::existFile( fullPath ) ){
				fullPath = gtFileSystem::getRealPath( fileName );

				if( !gtFileSystem::existFile( fullPath ) ){
					gtLogWriter::printWarning( u"Can not load model. File not exist. %s", fullPath.data() );
					return nullptr;
				}
			}

			return get_model( fileName, outModel );
		}

		virtual void	clearTextureCache( void ){
			m_textures.clear();
		}
		
		virtual void	clearModelCache( void ){
			m_models.clear();
		}

		virtual bool	removeModel( gtRenderModel* model ){
			if( !model ){
				gtLogWriter::printWarning( u"Can not remove model. model == nullptr" );
				return false;
			}
			return remove_model( model );
		}
		
		virtual bool	removeTexture( gtTexture* texture ){
			if( !texture ){
				gtLogWriter::printWarning( u"Can not remove texture. texture == nullptr" );
				return false;
			}
			return remove_texture( texture );
		}

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