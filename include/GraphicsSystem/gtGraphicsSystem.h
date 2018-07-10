#pragma once
#ifndef __GT_GRAPHICS_SYSTEM_INFO_H__
#define __GT_GRAPHICS_SYSTEM_INFO_H__

namespace gost{

	struct gtGraphicsSystemInfo{

		gtGraphicsSystemInfo():
			m_fullScreen( false ),
			m_stencilBuffer( true ),
			m_doubleBuffer( true ),
			m_vSync( false ),
			m_colorDepth( gtConst32U ),
			m_adapterID( gtConst0U ),
			m_textureFilterType( gtTextureFilterType::Anisotropic ),
			m_outWindow( nullptr )
		{
			m_backBufferSize.set( 800, 600 );
		}

		gtGraphicsSystemInfo( gtWindowInfo * wi ):
			m_fullScreen( false ),
			m_stencilBuffer( true ),
			m_doubleBuffer( true ),
			m_vSync( false ),
			m_colorDepth( gtConst32U ),
			m_adapterID( gtConst0U ),
			m_textureFilterType( gtTextureFilterType::Anisotropic ),
			m_outWindow( wi->m_owner )
		{
			m_backBufferSize.set( wi->m_rect.getWidth(), wi->m_rect.getHeight() );
		}


		v2i		m_backBufferSize;
		bool	m_fullScreen;
		bool	m_stencilBuffer;
		bool	m_doubleBuffer;
		bool	m_vSync;
		u8		m_colorDepth;

			// only for Direct3D
		u32		m_adapterID;

		gtTextureFilterType m_textureFilterType;

		gtWindow * m_outWindow;
	};


	class gtGraphicsSystem : public gtRefObject{
	public:

		virtual void clearModelCache() = 0;
		virtual void clearTextureCache() = 0;

		virtual gtPtr<gtRenderModel> createModel( gtModel* software_model ) = 0;
		virtual gtPtr<gtTexture>     createTexture( gtImage* sourceImage ) = 0;
		virtual gtPtr<gtTexture>     createRenderTargetTexture( const v2u& size, gtImageFormat pixelFormat ) = 0;

		virtual void draw2DImage( const v4i& rect, const gtMaterial& m ) = 0;
		virtual void draw2DImage( const v4i& rect, gtTexture* texture ) = 0;
		virtual void draw2DImage( const v4i& rect, const v4i& region, const gtMaterial& m ) = 0;
		virtual void draw2DImage( const v4i& rect, const v4i& region, gtTexture* texture ) = 0;
		virtual void drawLine( const v4f& start, const v4f& end, const gtColor& color = gtColor( gtColorWhite ) ) = 0;
		/*
			//       v6-----------v2
			//      /|			/ |
			//	   / |         /  |
			//	  /  |        /   |
			//	 /   v3______/____v7
			//	v5--/-------v8   /
			//	|  /		|   /
			//	| /			|  /
			//	|/			| /
			//	/			|/
			//	v1----------v4
		*/
		virtual void drawLineBox( const v4f& v1, const v4f& v2, const v4f& v3, const v4f& v4,
			const v4f& v5, const v4f& v6, const v4f& v7, const v4f& v8,
			const v4f& positionOffset = v4f(), const gtColor& color = gtColor( gtColorWhite ) ) = 0;

		virtual void drawLineSphere( const v4f& position, f32 radius, u32 smoothLevel = gtConst1U, const gtColor& color1 = gtColor( gtColorWhite ),
			const gtColor& color2 = gtColor( gtColorWhite ),
			const gtColor& color3 = gtColor( gtColorWhite )) = 0;

		virtual void drawModel( gtRenderModel* model, gtArray<gtMaterial>* materials = nullptr ) = 0;
		virtual void beginRender( bool clearRenderTarget = true, const gtColor& color = gtColor( gtColorBlack ) ) = 0;
		virtual void endRender() = 0;

		virtual gtShader *	getShader(
			gtShaderCallback * callback,
			const gtString& vertexShader,
			const gtStringA& vertexShaderMain,
			const gtString& pixelShader,
			const gtStringA& pixelShaderMain,
			gtShaderModel shaderModel,
			gtVertexType * vertexType
			) = 0;

		virtual const gtGraphicsSystemInfo&	getParams() = 0;

		virtual gtTexture*		getTexture( const gtString& fileName, gtImage** outImage = nullptr ) = 0;
		virtual gtRenderModel*	getModel( const gtString& fileName, gtModel** software_model = nullptr ) = 0;

		virtual bool	removeModel( gtRenderModel* model ) = 0;
		virtual bool	removeTexture( gtTexture* texture ) = 0;
		virtual void	scissorAdd( const v4i& rect ) = 0;
		virtual void	scissorClear( bool setOriginal = true ) = 0;
		virtual void	setDepthState( bool state = true ) = 0;
		virtual void	setRenderTarget( gtTexture * rtt, bool clearDepth = true, bool clearTarget = true, const gtColor& clearTargetColor = gtColorBlack ) = 0;
		virtual void	setTextureFilterType( gtTextureFilterType ) = 0;
		virtual void    setViewport( const v2f& viewportSize ) = 0;

		//автоматически вызывается через sceneSystem
		//рендерит стандартные эффекты
		//если сцена рисуется не через sceneSystem, то нужно вызывать этот метод вручную
		virtual void	renderEffects() = 0;
	};


	class gtGraphicsSystemCommon : public gtGraphicsSystem{
	protected:


		gtGraphicsSystemInfo m_params;
		gtArray<v4i> m_scissorRects;

		v2i			m_currentWindowSize;

		template<typename type>
		struct cache{
			cache(void):m_object(nullptr){}
			cache(type obj,const gtString&path):m_object(obj),m_path(path){}
			~cache(){
			}

			type		m_object;
			gtString	m_path;


		};

		gtArray<cache<gtPtr<gtTexture>>>		m_textures;
		gtArray<cache<gtPtr<gtRenderModel>>>	m_models;

		gtTexture* get_texture( const gtString& path, gtImage** im ){
			u32 sz = m_textures.size();
			for( u32 i = gtConst0U; i < sz; ++i ){
				if( path == m_textures[ i ].m_path )
					return m_textures[ i ].m_object.data();
			}

			auto img  = gtMainSystem::getInstance()->loadImage( path );
			if( !img.data() ){
				gtLogWriter::printWarning( u"Can not load image. %s", path.data() );
				return nullptr;
			}

			auto texture = createTexture( img.data() );
			if( !texture.data() ){
				gtLogWriter::printWarning( u"Can not create texture. %s", path.data() );
				return nullptr;
			}

			if( im ){
				*im = img.data();
				img->addRef();
			}

			//texture->addRef();
			m_textures.push_back(cache<gtPtr<gtTexture>>(texture,path));

			gtString pth = path;
			texture->setName( gtStringA( pth.to_ansiString() ) );

			return texture.data();
		}

		gtRenderModel* get_model( const gtString& path, gtModel** m ){
			u32 sz = m_models.size();
			for( u32 i = gtConst0U; i < sz; ++i ){
				if( path == m_models[ i ].m_path )
					return m_models[ i ].m_object.data();
			}

			auto mdl = gtMainSystem::getInstance()->getModelSystem()->createFromFile( path );
			if( !mdl.data() ){
				gtLogWriter::printWarning( u"Can not load model. %s", path.data() );
				return nullptr;
			}

			auto model = createModel( mdl.data() );
			if( !model.data() ){
				gtLogWriter::printWarning( u"Can not create model. %s", path.data() );
				return nullptr;
			}

			if( m ){
				*m = mdl.data();
				mdl->addRef();
			}
//
	//		model->addRef();
			m_models.push_back(cache<gtPtr<gtRenderModel>>(model,path));

			return model.data();
		}

		bool remove_model( gtRenderModel* model ){
			u32 sz = m_models.size();
			for( u32 i = gtConst0U; i < sz; ++i ){
				if( model == m_models[ i ].m_object.data() ){
			//		model->release();
					m_models.erase(i);
					return true;
				}
			}
			return false;
		}

		bool remove_texture( gtTexture* texture ){
			u32 sz = m_textures.size();
			for( u32 i = gtConst0U; i < sz; ++i ){
				if( texture == m_textures[ i ].m_object.data() ){
				//	texture->release();
					m_textures.erase(i);
					return true;
				}
			}
			return false;
		}

		virtual void applyScissor() = 0;

	public:

			// c-tor
		gtGraphicsSystemCommon()
		{
		}

			// d-tor
		virtual ~gtGraphicsSystemCommon(){
		}

		virtual void	scissorClear( bool setOriginal ){
			m_scissorRects.clear();
			if( setOriginal )
				m_scissorRects.push_back( v4i( gtConst0, gtConst0, m_params.m_backBufferSize.x, m_params.m_backBufferSize.y ) );
			applyScissor();
		}

		virtual void	scissorAdd( const v4i& rect ){
			m_scissorRects.push_back( rect );
			applyScissor();
		}

		virtual const gtGraphicsSystemInfo&	getParams(){
			return m_params;
		}

		virtual const v2i&	getCurrentWindowSize() const {
			return m_currentWindowSize;
		}

		virtual gtTexture*		getTexture( const gtString& fileName, gtImage** outImage ){
			gtString fullPath = fileName;
			if( !gtFileSystem::existFile( fullPath ) ){
				fullPath = gtFileSystem::getRealPath( fileName );

				if( !gtFileSystem::existFile( fullPath ) ){
					gtLogWriter::printWarning( u"Can not load texture. File not exist. %s", fullPath.data() );
					return nullptr;
				}
			}
			return get_texture( fileName, outImage );
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

		virtual void	clearTextureCache(){
			m_textures.clear();
		}

		virtual void	clearModelCache(){
			m_models.clear();
		}

		virtual bool	removeModel( gtRenderModel* model ){
			if( !model ){
				return false;
			}
			return remove_model( model );
		}

		virtual bool	removeTexture( gtTexture* texture ){
			if( !texture ){
				return false;
			}
			return remove_texture( texture );
		}

		virtual void drawLineBox( const v4f& v1, const v4f& v2, const v4f& v3, const v4f& v4,
			const v4f& v5, const v4f& v6, const v4f& v7, const v4f& v8,
			const v4f& positionOffset, const gtColor& color ){
				drawLine( v1 + positionOffset, v4 + positionOffset, color );
				drawLine( v5 + positionOffset, v8 + positionOffset, color );
				drawLine( v1 + positionOffset, v5 + positionOffset, color );
				drawLine( v4 + positionOffset, v8 + positionOffset, color );
				drawLine( v3 + positionOffset, v7 + positionOffset, color );
				drawLine( v6 + positionOffset, v2 + positionOffset, color );
				drawLine( v3 + positionOffset, v6 + positionOffset, color );
				drawLine( v7 + positionOffset, v2 + positionOffset, color );
				drawLine( v2 + positionOffset, v8 + positionOffset, color );
				drawLine( v4 + positionOffset, v7 + positionOffset, color );
				drawLine( v5 + positionOffset, v6 + positionOffset, color );
				drawLine( v1 + positionOffset, v3 + positionOffset, color );
		}

		virtual void drawLineSphere( const v4f& position, f32 radius, u32 smoothLevel, const gtColor& color1, const gtColor& color2, const gtColor& color3 ){

			gtArray< v4f > points1;
			gtArray< v4f > points2;
			gtArray< v4f > points3;
			f32 x, y;

			u32 q = smoothLevel;

			if( !q ) q = gtConst1U;

			u32 s = gtConst9U * q + gtConst1U;
			f32 m = gtConst40F / q;
			for( u32 i = gtConst0U; i < s; ++i ){
				f32 a = (f32)i * m / gtConst180F * math::PI;
				x = std::sin( a );
				y = std::cos( a );
				points1.push_back( v4f( x * radius, gtConst0F, y * radius ) + position );
			}

			for( u32 i = gtConst0U; i < s; ++i ){
				f32 a = (f32)i * m / gtConst180F * math::PI;
				x = std::sin( a );
				y = std::cos( a );
				points2.push_back( v4f( x * radius, y * radius, gtConst0F ) + position );
			}

			for( u32 i = gtConst0U; i < s; ++i ){
				f32 a = (f32)i * m / gtConst180F * math::PI;
				x = std::sin( a );
				y = std::cos( a );
				points3.push_back( v4f( gtConst0F, x * radius, y * radius ) + position );
			}

			u32 sz = points1.size();
			for( u32 i = gtConst1U; i < sz; ++i ){
				drawLine( points1[ i ], points1[ i - gtConst1U ], color1 );
			}

			sz = points2.size();
			for( u32 i = gtConst1U; i < sz; ++i ){
				drawLine( points2[ i ], points2[ i - gtConst1U ], color2 );
			}

			sz = points3.size();
			for( u32 i = gtConst1U; i < sz; ++i ){
				drawLine( points3[ i ], points3[ i - gtConst1U ], color3 );
			}

		}

		virtual void	setTextureFilterType( gtTextureFilterType f ){
			m_params.m_textureFilterType = f;
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
