#pragma once
#ifndef __GT_DRIVER_H__
#define __GT_DRIVER_H__ //< include guard

namespace gost{

	struct gtDriverInfo{

			// c-tor
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

		gtDriverInfo( gtWindowInfo * wi ):
			m_fullScreen( false ),
			m_stencilBuffer( true ),
			m_doubleBuffer( true ),
			m_vSync( false ),
			m_colorDepth( 32u ),
			m_adapterID( 0u ),
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


		gtWindow * m_outWindow;
	};


	class gtDriver : public gtRefObject{
	public:


			// \return gtDriverInfo
		virtual const gtDriverInfo&	getParams( void ) = 0;


		virtual void beginRender( bool clearRenderTarget = true, const gtColor& color = gtColor(0.f) ) = 0;


		virtual void endRender( void ) = 0;


		virtual void draw2DImage( const v4i& rect, const gtMaterial& m ) = 0;


		virtual void draw2DImage( const v4i& rect, gtTexture* texture ) = 0;


		virtual void drawModel( gtRenderModel* model ) = 0;


		virtual void draw2DImage( const v4i& rect, const v4i& region, const gtMaterial& m ) = 0;
		virtual void draw2DImage( const v4i& rect, const v4i& region, gtTexture* texture ) = 0;

		virtual void drawLine( const v3f& start, const v3f& end, const gtColor& color = gtColor( 1.f, 1.f, 1.f, 1.f ) ) = 0;

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
		virtual void drawLineBox( const v3f& v1, const v3f& v2, const v3f& v3, const v3f& v4,
			const v3f& v5, const v3f& v6, const v3f& v7, const v3f& v8,
			const v3f& positionOffset = v3f(), const gtColor& color = gtColor( 1.f, 1.f, 1.f, 1.f ) ) = 0;

		virtual void drawLineSphere( const v3f& position, f32 radius, u32 smoothLevel = 1u, const gtColor& color1 = gtColor( 1.f, 1.f, 1.f, 1.f ),
			const gtColor& color2 = gtColor( 1.f, 1.f, 1.f, 1.f ),
			const gtColor& color3 = gtColor( 1.f, 1.f, 1.f, 1.f )) = 0;



		virtual gtShader *	getShader(
			gtShaderCallback * callback,
			const gtString& vertexShader,
			const gtStringA& vertexShaderMain,
			const gtString& pixelShader,
			const gtStringA& pixelShaderMain,
			gtShaderModel shaderModel,
			gtVertexType * vertexType
			) = 0;



		virtual gtPtr<gtTexture>	createTexture( gtImage* sourceImage, gtTextureFilterType filter = gtTextureFilterType::FILTER_ANISOTROPIC ) = 0;


		virtual gtPtr<gtRenderModel>	createModel( gtModel* software_model ) = 0;

			//	Get texture from texture cache, or load new texture to the cache
			//	\param fileName: path to the file
			//	\param filter: filter type
			//	\param outImage: Loaded gtImage. If set nullptr, gtImage will be removed.
			//	\return texture
			//	\remark If you get outImage, don't forget call outImage->release() for delete.
		virtual gtTexture*		getTexture( const gtString& fileName, gtTextureFilterType filter = gtTextureFilterType::FILTER_ANISOTROPIC, gtImage** outImage = nullptr ) = 0;

			//	Get model from model cache, or load new model to the cache
			//	\param fileName: path to the file
			//	\param software_model: Loaded software_model. If set nullptr, gtMoel will be removed.
			//	\return render model
			//	\remark If you get software_model, don't forget call software_model->release() for delete.
		virtual gtRenderModel*	getModel( const gtString& fileName, gtModel** software_model = nullptr ) = 0;

			//	clearTextureCache
		virtual void	clearTextureCache( void ) = 0;

			// clearModelCache
		virtual void	clearModelCache( void ) = 0;

		virtual bool	removeModel( gtRenderModel* model ) = 0;

		virtual bool	removeTexture( gtTexture* texture ) = 0;

		virtual void	setDepthState( bool state = true ) = 0;

		virtual void	scissorClear( bool setOriginal = true ) = 0;

		virtual void	scissorAdd( const v4i& rect ) = 0;
	};


	class gtDriverCommon : public gtDriver{
	protected:


		gtDriverInfo m_params;
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

		gtTexture* get_texture( const gtString& path, gtTextureFilterType filter, gtImage** im ){
			u32 sz = m_textures.size();
			for( u32 i = 0u; i < sz; ++i ){
				if( path == m_textures[ i ].m_path )
					return m_textures[ i ].m_object.data();
			}

			auto img  = gtMainSystem::getInstance()->loadImage( path );
			if( !img.data() ){
				gtLogWriter::printWarning( u"Can not load image. %s", path.data() );
				return nullptr;
			}

			auto texture = createTexture( img.data(), filter );
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

			return texture.data();
		}

		gtRenderModel* get_model( const gtString& path, gtModel** m ){
			u32 sz = m_models.size();
			for( u32 i = 0u; i < sz; ++i ){
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
			for( u32 i = 0u; i < sz; ++i ){
				if( model == m_models[ i ].m_object.data() ){
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
				if( texture == m_textures[ i ].m_object.data() ){
				//	texture->release();
					m_textures.erase(i);
					return true;
				}
			}
			gtLogWriter::printWarning( u"Can not remove texture from cache. Texture not found." );
			return false;
		}

		virtual void applyScissor( void ) = 0;

	public:

			// c-tor
		gtDriverCommon( void )
		{
#ifdef GT_DEBUG
			m_debugName.assign(u"gtDriver");
#endif
		}

			// d-tor
		virtual ~gtDriverCommon( void ){
		}

		virtual void	scissorClear( bool setOriginal ){
			m_scissorRects.clear();
			if( setOriginal )
				m_scissorRects.push_back( v4i( 0, 0, m_params.m_backBufferSize.x, m_params.m_backBufferSize.y ) );
			applyScissor();
		}

		virtual void	scissorAdd( const v4i& rect ){
			m_scissorRects.push_back( rect );
			applyScissor();
		}

			// \return gtDriverInfo
		virtual const gtDriverInfo&	getParams( void ){
			return m_params;
		}

		virtual const v2i&	getCurrentWindowSize( void ) const {
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

		virtual void drawLineBox( const v3f& v1, const v3f& v2, const v3f& v3, const v3f& v4,
			const v3f& v5, const v3f& v6, const v3f& v7, const v3f& v8,
			const v3f& positionOffset, const gtColor& color ){
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

		virtual void drawLineSphere( const v3f& position, f32 radius, u32 smoothLevel, const gtColor& color1, const gtColor& color2, const gtColor& color3 ){

			gtArray< v3f > points1;
			gtArray< v3f > points2;
			gtArray< v3f > points3;
			f32 x, y;

			u32 q = smoothLevel;

			if( !q ) q = 1u;

			u32 s = 9u * q + 1u;
			f32 m = 40.f / q;
			for( u32 i = 0u; i < s; ++i ){
				f32 a = (f32)i * m / 180.f * PI;
				x = std::sin( a );
				y = std::cos( a );
				points1.push_back( v3f( x * radius, 0.f, y * radius ) + position );
			}

			for( u32 i = 0u; i < s; ++i ){
				f32 a = (f32)i * m / 180.f * PI;
				x = std::sin( a );
				y = std::cos( a );
				points2.push_back( v3f( x * radius, y * radius, 0.f ) + position );
			}

			for( u32 i = 0u; i < s; ++i ){
				f32 a = (f32)i * m / 180.f * PI;
				x = std::sin( a );
				y = std::cos( a );
				points3.push_back( v3f( 0.f, x * radius, y * radius ) + position );
			}

			u32 sz = points1.size();
			for( u32 i = 1u; i < sz; ++i ){
				drawLine( points1[ i ], points1[ i - 1u ], color1 );
			}

			sz = points2.size();
			for( u32 i = 1u; i < sz; ++i ){
				drawLine( points2[ i ], points2[ i - 1u ], color2 );
			}

			sz = points3.size();
			for( u32 i = 1u; i < sz; ++i ){
				drawLine( points3[ i ], points3[ i - 1u ], color3 );
			}

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
