#include <gost.h>

#ifdef _DEBUG
#pragma comment(lib, "gost_d.lib")
#else 
#pragma comment(lib, "gost.lib")
#endif

using namespace gost;

///	Scan 'media' folder for search images
void getImagesFromMediaFolder( gtArray<gtString>& out_array, gtArray<gtString>& extensions, gtString dir ){

	/// Need collect all dir objects(folders and files).
	gtArray<gtFileSystem::DirObject> objs;

	///	Before scan, need call this function.
	gtFileSystem::scanDirBegin( dir );

	///	Get all dir objects and push back in to the array
 	gtFileSystem::DirObject ob;
 	while( gtFileSystem::getDirObject( &ob ) ){
 		objs.push_back( ob );
 	}
 	gtFileSystem::scanDirEnd(); /// call this for end scan

	///	Get all collected dir objects
	u32 sz = objs.size();
 	for( u32 i = 0u; i < sz; ++i ){
 		auto * o = &objs[ i ];
  		if( o->type == gtFileSystem::DirObjectType::info )  /// .. .
  			continue;

		else if( o->type == gtFileSystem::DirObjectType::folder )
			getImagesFromMediaFolder( out_array, extensions, (char16_t*)o->path ); /// if object is folder, scan this folder.

		else if( o->type == gtFileSystem::DirObjectType::file ){
			
			gtString path( (char16_t*)o->path ); /// push path in to the string
			gtString ext = util::stringGetExtension( path ); /// return extension without dot

			u32 esz = extensions.size(); /// supported image extensions
			for( u32 ei = 0u; ei < esz; ++ei ){
				if( ext == extensions[ ei ] ){ /// if extension supported, push back
					out_array.push_back( path );
					break;
				}
			}

		}
	}
}

#if defined( GT_PLATFORM_WIN32 )
#include <Windows.h>
int WINAPI WinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/ ){
#endif

	gtDeviceCreationParameters params;

	gtPtr_t(gtMainSystem,mainSystem,InitializeGoSTEngine(params));

	///	Create main window.
	gtWindowInfo wi;
    wi.m_style |= gtWindowInfo::maximize;
    wi.m_style |= gtWindowInfo::resize;
	
	gtPtr_t(gtWindow,window,mainSystem->createSystemWindow( wi ));


	gtPluginSystem * pluginSystem = mainSystem->getPluginSystem();

	///	Another way
	gtPluginRender * d3d11Plugin = pluginSystem->getAsPluginRender( pluginSystem->getPlugin( GT_UID_RENDER_D3D11 ) );
	gtDriverInfo di;
	di.m_outWindow = window.data(); /// Set output window for rendering.
	gtPtr_t(gtDriver,driver,d3d11Plugin->loadDriver( di ));

	///	Get `import image` plugins
	gtPluginImportImage * bmpPlugin = pluginSystem->getAsPluginImportImage( pluginSystem->getPlugin( GT_UID_IMPORT_IMAGE_BMP ) );
	gtPluginImportImage * pngPlugin = pluginSystem->getAsPluginImportImage( pluginSystem->getPlugin( GT_UID_IMPORT_IMAGE_PNG ) );

	///	Collect supported extension
	gtArray<gtString> supported_extensions;

	///	load plugin and get extensions
	bmpPlugin->load();
	u32 sz = bmpPlugin->m_extensions.size();
	for( u32 i = 0u; i < sz; ++i )
		supported_extensions.push_back( bmpPlugin->m_extensions[ i ] );

	///	load plugin and get extensions
	pngPlugin->load();
	sz = pngPlugin->m_extensions.size();
	for( u32 i = 0u; i < sz; ++i )
		supported_extensions.push_back( pngPlugin->m_extensions[ i ] );
	

	///	Collect images paths in `media` folder
	gtArray<gtString> images;
	getImagesFromMediaFolder( images, supported_extensions, u"../media/" );


	u32 num_of_images		= images.size();
	u32 current_image_index = 0u;
	gtImage * image			= nullptr;
	gtTexture*texture		= nullptr;

	///	for pollEvent
	gtEvent event;

	gtTextureFilterType filter = gtTextureFilterType::FILTER_ANISOTROPIC;

	while( mainSystem->update() ){


		while( mainSystem->pollEvent( event ) ){
			switch( event.type ){
				case gtEventType::keyboard:

				if( event.keyboardEvent.isPressed( gtKey::K_LEFT ) ){

					if( current_image_index == 0u )
						current_image_index = num_of_images;

					--current_image_index;
					
					if( texture )
						texture->release();
					texture = nullptr;

				}else if( event.keyboardEvent.isPressed( gtKey::K_RIGHT ) ){

					++current_image_index;

					if( current_image_index == num_of_images )
						current_image_index = 0u;

					if( texture )	texture->release();
					texture = nullptr;

				}else if( event.keyboardEvent.isPressed( gtKey::K_1 ) ){
					filter = gtTextureFilterType::FILTER_ANISOTROPIC;
					if( texture )	texture->release();
					texture = nullptr;

				}else if( event.keyboardEvent.isPressed( gtKey::K_2 ) ){
					filter = gtTextureFilterType::FILTER_PPP;
					if( texture )	texture->release();
					texture = nullptr;

				}else if( event.keyboardEvent.isPressed( gtKey::K_3 ) ){
					filter = gtTextureFilterType::FILTER_LLL;
					if( texture )	texture->release();
					texture = nullptr;

				}
				break;
			}
		}

		if( mainSystem->isRun() ){

			if( mainSystem->isKeyPressed( gtKey::K_ESCAPE ) ){
				mainSystem->shutdown();
			}

			if( !texture ){
				if( !image ){
					window->setWindowTitle( gtString(u"2D image: ") + images[ current_image_index ] );
					image	= mainSystem->loadImage( images[ current_image_index ] );
					texture = driver->createTexture( image, filter );
					image->release();
					image = nullptr;
				}
			}

			driver->beginRender( true, gtColor( 0.7372549019607843f, 0.8901960784313725f, 1.f, 1.f ) ); /// RGBA.

			if( texture ){
				f32 tw = texture->getWidth();
				f32 th = texture->getHeight();

				f32 x2 = tw * (600.f/th);
				f32 y2 = th * (600.f/th);
				driver->draw2DImage( v4i_t( 0, 0, (s32)x2, (s32)y2 ), texture );

				/// You can draw region of image. Use overloaded function for this.
				/// driver->draw2DImage( v4i_t( 0, 0, (s32)x2, (s32)y2 ), v4i_t( 30, 30, 60, 60 ), texture );
			}

			driver->endRender();
		}
	}

	if( texture )
		texture->release();

	return 0;
}