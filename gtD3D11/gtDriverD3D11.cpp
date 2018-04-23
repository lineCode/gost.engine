//GoST

#include "common.h"

gtDriverD3D11::gtDriverD3D11( /*gtMainSystem* System,*/ gtDriverInfo params ):
	m_beginRender( false ),
	m_D3DLibrary( nullptr ),
	m_SwapChain( nullptr ),
	m_d3d11Device( nullptr ),
	m_d3d11DevCon( nullptr ),
	m_MainTargetView( nullptr ),
	m_depthStencilBuffer( nullptr ),
	m_depthStencilStateEnabled( nullptr ),
	m_depthStencilStateDisabled( nullptr ),
	m_depthStencilView( nullptr ),
	m_RasterizerSolid( nullptr ),
	m_RasterizerSolidNoBackFaceCulling( nullptr ),
	m_RasterizerWireframeNoBackFaceCulling( nullptr ),
	m_RasterizerWireframe( nullptr ),
	m_blendStateAlphaEnabled( nullptr ),
	m_blendStateAlphaEnabledWithATC( nullptr ),
	m_blendStateAlphaDisabled( nullptr ),
	m_shader2DStandart( nullptr ),
	m_shader3DStandart( nullptr ),
	m_shaderGUI( nullptr ),
	m_shaderSprite( nullptr ),
	m_shaderLine( nullptr )
{

	m_system = gtMainSystem::getInstance();


	m_params =  params;
	
	if( params.m_outWindow ){
		m_currentWindowSize.x = params.m_outWindow->getRect().z;
		m_currentWindowSize.y = params.m_outWindow->getRect().w;
	}

#ifdef GT_DEBUG
	this->setDebugName( u"DriverD3D11" );
#endif

	s_instance = this;
}

gtDriverD3D11* gtDriverD3D11::s_instance = nullptr;

gtDriverD3D11*	gtDriverD3D11::getInstance( void ){
	return s_instance;
}
gtTexture * gtDriverD3D11::getStandartTexture( void ){
	return m_standartTexture.data();
}
gtTexture * gtDriverD3D11::getStandartTextureWhiteColor( void ){
	return m_standartTextureWhiteColor.data();
}

gtDriverD3D11::~gtDriverD3D11( void ){

	clearTextureCache();
	clearModelCache();

	if( m_standartTextureWhiteColor.data() )
		m_standartTextureWhiteColor->release();

	if( m_standartTexture.data() )
		m_standartTexture->release();

	if( m_shaderLine )
		m_shaderLine->release();

	if( m_shaderSprite )
		m_shaderSprite->release();

	if( m_shader3DStandart )
		m_shader3DStandart->release();

	if( m_shaderGUI )
		m_shaderGUI->release();

	if( m_shader2DStandart )
		m_shader2DStandart->release();

	if( m_blendStateAlphaDisabled )
		m_blendStateAlphaDisabled->Release();

	
	if( m_blendStateAlphaEnabledWithATC )
		m_blendStateAlphaEnabledWithATC->Release();

	if( m_blendStateAlphaEnabled )
		m_blendStateAlphaEnabled->Release();

	if( m_RasterizerWireframeNoBackFaceCulling )
		m_RasterizerWireframeNoBackFaceCulling->Release();

	if( m_RasterizerWireframe )
		m_RasterizerWireframe->Release();

	if( m_RasterizerSolidNoBackFaceCulling )
		m_RasterizerSolidNoBackFaceCulling->Release();

	if( m_RasterizerSolid )
		m_RasterizerSolid->Release();

	if( m_depthStencilView )
		m_depthStencilView->Release();

	if( m_depthStencilStateDisabled )
		m_depthStencilStateDisabled->Release();

	if( m_depthStencilStateEnabled )
		m_depthStencilStateEnabled->Release();

	if( m_depthStencilBuffer )
		m_depthStencilBuffer->Release();

	if( m_MainTargetView )
		m_MainTargetView->Release();

	if( m_d3d11DevCon )
		m_d3d11DevCon->Release();

	if( m_SwapChain )
		m_SwapChain->Release();
	
	if( m_d3d11Device )
		m_d3d11Device->Release();

	if( m_D3DLibrary )
		FreeLibrary( m_D3DLibrary );
	m_D3DLibrary = NULL;

}

HMODULE gtDriverD3D11::getD3DLibraryHandle( void ){
	return m_D3DLibrary;
}

ID3D11Device * gtDriverD3D11::getD3DDevice( void ){
	return m_d3d11Device;
}

void	gtDriverD3D11::setDepthState( bool state ){
	if( state ){
		m_d3d11DevCon->OMSetDepthStencilState( this->m_depthStencilStateEnabled, 0 );
	}else{
		m_d3d11DevCon->OMSetDepthStencilState( this->m_depthStencilStateDisabled, 0 );
	}
}

ID3D11DeviceContext * gtDriverD3D11::getD3DDeviceContext( void ){
	return m_d3d11DevCon;
}

void gtDriverD3D11::setActiveShader( gtShader* shader ){
	m_d3d11DevCon->IASetInputLayout( ((gtShaderImpl*)shader)->m_vLayout );
	m_d3d11DevCon->VSSetShader( ((gtShaderImpl*)shader)->m_vShader, 0, 0 );
	m_d3d11DevCon->PSSetShader( ((gtShaderImpl*)shader)->m_pShader, 0, 0 );
}

bool gtDriverD3D11::initialize( void ){

	if( !m_params.m_outWindow ){
		gtLogWriter::printError( u"No render out window." );
		return false;
	}

	HWND outWindow = (HWND)m_params.m_outWindow->getHandle();

	DXGI_MODE_DESC	bufferDesc;
	ZeroMemory( &bufferDesc, sizeof(bufferDesc) );
	bufferDesc.Width	=	m_params.m_backBufferSize.x;
	bufferDesc.Height	=	m_params.m_backBufferSize.y;
	if( m_params.m_vSync )
		bufferDesc.RefreshRate.Numerator	=	60;
	else bufferDesc.RefreshRate.Numerator	=	0;
	bufferDesc.RefreshRate.Denominator	=	1;
	
	bufferDesc.Format	=	DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering	=	DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling	=	DXGI_MODE_SCALING_UNSPECIFIED;
	
	DXGI_SWAP_CHAIN_DESC	swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof(swapChainDesc) );
	swapChainDesc.BufferDesc	=	bufferDesc;
	swapChainDesc.BufferUsage	=	DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow	=	outWindow;
	swapChainDesc.SampleDesc.Count	=	1;
	swapChainDesc.SampleDesc.Quality	=	0;
	swapChainDesc.BufferCount	=	1;
	swapChainDesc.Windowed	=	m_params.m_fullScreen ? false : true;
	swapChainDesc.SwapEffect	=	DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags	=	0;

	gtString d3dlib_str = gtFileSystem::getSystemPath();
	d3dlib_str += u"d3d11.dll";

	m_D3DLibrary = LoadLibrary( (wchar_t*)d3dlib_str.data() );
	if( !m_D3DLibrary ){
		gtLogWriter::printError( u"Could not load d3d11.dll" );
		return false;
	}

	gtD3D11CreateDevice_t D3D11CreateDevice = GT_LOAD_FUNCTION_SAFE_CAST(gtD3D11CreateDevice_t,m_D3DLibrary, "D3D11CreateDevice");
	if( !D3D11CreateDevice ){
		gtLogWriter::printError( u"Could not get proc adress of D3D11CreateDevice");
		return false;
	}

	gtD3D11CreateDeviceAndSwapChain_t D3D11CreateDeviceAndSwapChain =
		GT_LOAD_FUNCTION_SAFE_CAST(gtD3D11CreateDeviceAndSwapChain_t,m_D3DLibrary, "D3D11CreateDeviceAndSwapChain");
	if( !D3D11CreateDeviceAndSwapChain ){
		gtLogWriter::printError( u"Could not get proc adress of D3D11CreateDeviceAndSwapChain");
		return false;
	}

	D3D_FEATURE_LEVEL FeatureLevels[] = {
    D3D_FEATURE_LEVEL_11_1,
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
    D3D_FEATURE_LEVEL_9_3,
    D3D_FEATURE_LEVEL_9_2,
    D3D_FEATURE_LEVEL_9_1
    };
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;
	if( FAILED(D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		&FeatureLevels[0],
		ARRAYSIZE( FeatureLevels ),
		D3D11_SDK_VERSION,
		nullptr,
		&featureLevel,
		nullptr ))){
			gtLogWriter::printError( u"Can not get D3D Feature Level");
		return false;
	}

	if( featureLevel == D3D_FEATURE_LEVEL_11_0 ){
		gtLogWriter::printInfo( u"D3D feature level 11.0" );
	}else if( featureLevel == D3D_FEATURE_LEVEL_11_1 ){
		gtLogWriter::printInfo( u"D3D feature level 11.1" );
	}

	if( FAILED( D3D11CreateDeviceAndSwapChain( 
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE, 
		nullptr,
		0, 
		&featureLevel, 
		1,
		D3D11_SDK_VERSION, 
		&swapChainDesc,
		&m_SwapChain,
		&m_d3d11Device, 
		nullptr, 
		&m_d3d11DevCon ) ) ){
			gtLogWriter::printError( u"Can't create Direct3D 11 Device" );
		return false;
	}

	ID3D11Texture2D* BackBuffer;
	if( FAILED( m_SwapChain->GetBuffer( 
		0,
		__uuidof( ID3D11Texture2D ), 
		(void**)&BackBuffer ) ) ){
		gtLogWriter::printError( u"Can't create Direct3D 11 back buffer" );
		return false;
	}


	if( FAILED( this->m_d3d11Device->CreateRenderTargetView( 
		BackBuffer, 0, &m_MainTargetView ) ) ){
		gtLogWriter::printError( u"Can't create Direct3D 11 render target" );
		if( BackBuffer ) BackBuffer->Release();
		return false;
	}
	
	if( BackBuffer ) BackBuffer->Release();

	D3D11_TEXTURE2D_DESC	DSD;
	ZeroMemory( &DSD, sizeof(DSD) );
	DSD.Width	=	m_params.m_backBufferSize.x;
	DSD.Height	=	m_params.m_backBufferSize.y;
	DSD.MipLevels	=	1;
	DSD.ArraySize	=	1;
	DSD.Format	=	DXGI_FORMAT_D32_FLOAT;
	DSD.SampleDesc.Count	=	1;
	DSD.SampleDesc.Quality	=	0;
	DSD.Usage	=	D3D11_USAGE_DEFAULT;
	DSD.BindFlags	=	D3D11_BIND_DEPTH_STENCIL;
	DSD.CPUAccessFlags	=	0;
	DSD.MiscFlags	=	0;
	if( FAILED( m_d3d11Device->CreateTexture2D( &DSD, 0, &m_depthStencilBuffer ) ) ){
		gtLogWriter::printError( u"Can't create Direct3D 11 depth stencil buffer" );
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC	depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof(depthStencilDesc) );
	depthStencilDesc.DepthEnable	=	true;
	depthStencilDesc.DepthWriteMask	=	D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc		=	D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable	=	true;
	depthStencilDesc.StencilReadMask=	0xFF;
	depthStencilDesc.StencilWriteMask=	0xFF;
	depthStencilDesc.FrontFace.StencilFailOp	=	D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp=	D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp	=	D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc		=	D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp		=	D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp=	D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp		=	D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc		=	D3D11_COMPARISON_ALWAYS;

	if( FAILED( m_d3d11Device->CreateDepthStencilState( &depthStencilDesc, &m_depthStencilStateEnabled ) ) ){
		gtLogWriter::printError( u"Can't create Direct3D 11 depth stencil state" );
		return false;
	}

	m_d3d11DevCon->OMSetDepthStencilState( this->m_depthStencilStateEnabled, 0 );

	depthStencilDesc.DepthEnable	=	false;
	if( FAILED( m_d3d11Device->CreateDepthStencilState( &depthStencilDesc, &this->m_depthStencilStateDisabled ) ) ){
		gtLogWriter::printError( u"Can't create Direct3D 11 depth stencil state" );
		return false;
	}

	ZeroMemory( &m_depthStencilViewDesc, sizeof( m_depthStencilViewDesc ) );
	m_depthStencilViewDesc.Format		=	DXGI_FORMAT_D32_FLOAT;
	m_depthStencilViewDesc.ViewDimension=	D3D11_DSV_DIMENSION_TEXTURE2D;
	m_depthStencilViewDesc.Texture2D.MipSlice	=	0;
	if( FAILED( m_d3d11Device->CreateDepthStencilView( m_depthStencilBuffer, &m_depthStencilViewDesc, &m_depthStencilView ) ) ){
		gtLogWriter::printError( u"Can't create Direct3D 11 depth stencil view" );
		return false;
	}

	m_d3d11DevCon->OMSetRenderTargets( 1, &m_MainTargetView, m_depthStencilView );

	D3D11_RASTERIZER_DESC	rasterDesc;
	ZeroMemory( &rasterDesc, sizeof( D3D11_RASTERIZER_DESC ) );
	rasterDesc.AntialiasedLineEnable = true;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = true;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	if( FAILED( m_d3d11Device->CreateRasterizerState( &rasterDesc, &m_RasterizerSolid ))){
		gtLogWriter::printError( u"Can not create rasterizer state" );
		return false;
	}

	rasterDesc.CullMode = D3D11_CULL_NONE;
	m_d3d11Device->CreateRasterizerState( &rasterDesc, &m_RasterizerSolidNoBackFaceCulling );
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	m_d3d11Device->CreateRasterizerState( &rasterDesc, &m_RasterizerWireframeNoBackFaceCulling );
	rasterDesc.CullMode = D3D11_CULL_BACK;
	m_d3d11Device->CreateRasterizerState( &rasterDesc, &m_RasterizerWireframe );

	m_d3d11DevCon->RSSetState( m_RasterizerSolid );

	


	D3D11_BLEND_DESC  bd;
	memset( &bd, 0, sizeof(bd) );
	bd.AlphaToCoverageEnable = 0;
	bd.RenderTarget[0].BlendEnable = TRUE;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	

	if( FAILED( m_d3d11Device->CreateBlendState( &bd, &m_blendStateAlphaEnabled ) ) ){
		gtLogWriter::printError( u"Can't create Direct3D 11 blend state" );
		return false;
	}

	bd.AlphaToCoverageEnable = 1;
	if( FAILED( m_d3d11Device->CreateBlendState( &bd, &m_blendStateAlphaEnabledWithATC ) ) ){
		gtLogWriter::printError( u"Can't create Direct3D 11 blend state" );
		return false;
	}


	bd.RenderTarget[0].BlendEnable = FALSE;
	if( FAILED( m_d3d11Device->CreateBlendState( &bd, &m_blendStateAlphaDisabled ) ) ){
		gtLogWriter::printError( u"Can't create Direct3D 11 blend state" );
		return false;
	}

	enableBlending( true );
	scissorClear( true );


	D3D11_VIEWPORT viewport;
	viewport.Width		=	(f32)m_params.m_backBufferSize.x;
	viewport.Height		=	(f32)m_params.m_backBufferSize.y;
	viewport.MinDepth	=	0.0f;
	viewport.MaxDepth	=	1.0f;
	viewport.TopLeftX	=	0.0f;
	viewport.TopLeftY	=	0.0f;
	m_d3d11DevCon->RSSetViewports( 1, &viewport );

	createStandartTexture();

	m_shaderProcessing = gtPtrNew<gtShaderProcessingD3D11>( new gtShaderProcessingD3D11(m_d3d11DevCon) );

	
	return createShaders();
}

void	gtDriverD3D11::createStandartTexture( void ){
	{
		gtImage * i = new gtImage;
		i->bits = 24u;
		i->width = 8u;
		i->height = 8u;
		i->pitch = i->width * 3u;
		i->dataSize = i->pitch * i->height;
		i->format = gtImage::Format::FMT_R8G8B8;
		i->frames = 1u;
		i->mipCount = 1u;
		gtMainSystem::getInstance()->allocateMemory( (void**)&i->data, i->dataSize );

		image::fillCheckerBoard( i, false, gtColor(u8(255),48,224), gtColor(u8(0),0,0) );

		m_standartTexture = this->createTexture( i, gtTextureFilterType::FILTER_PPP );

		gtMainSystem::getInstance()->freeMemory( (void**)&i->data );

		delete i;
	}
	{
		gtImage * i = new gtImage;
		i->bits = 24u;
		i->width = 8u;
		i->height = 8u;
		i->pitch = i->width * 3u;
		i->dataSize = i->pitch * i->height;
		i->format = gtImage::Format::FMT_R8G8B8;
		i->frames = 1u;
		i->mipCount = 1u;
		gtMainSystem::getInstance()->allocateMemory( (void**)&i->data, i->dataSize );

		image::fillSolid( i, false, gtColor( 1.f ) );

		m_standartTextureWhiteColor = this->createTexture( i, gtTextureFilterType::FILTER_PPP );

		gtMainSystem::getInstance()->freeMemory( (void**)&i->data );

		delete i;
	}
}

void	gtDriverD3D11::enableBlending( bool b, bool atc ){
	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	if( b ){
		if( atc )
			m_d3d11DevCon->OMSetBlendState( m_blendStateAlphaEnabledWithATC, blendFactor, 0xffffffff );
		else
			m_d3d11DevCon->OMSetBlendState( m_blendStateAlphaEnabled, blendFactor, 0xffffffff );
	}else{
		m_d3d11DevCon->OMSetBlendState( m_blendStateAlphaDisabled, blendFactor, 0xffffffff );
	}
}

void gtDriverD3D11::clearRenderTarget( const gtColor& color ){
	m_d3d11DevCon->ClearRenderTargetView( m_MainTargetView, color.getData() );
	m_d3d11DevCon->ClearDepthStencilView( m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

void gtDriverD3D11::beginRender( bool _clearRenderTarget, const gtColor& color ){
	if( !m_beginRender ){
		m_beginRender = true;

		if( _clearRenderTarget )
			clearRenderTarget( color );

	}
}

void gtDriverD3D11::endRender( void ){
	if( m_beginRender ){
		
		if( m_params.m_vSync )
			this->m_SwapChain->Present( 1, 0 );
		else
			this->m_SwapChain->Present( 0, 0 );

		m_beginRender = false;
	}
}

void gtDriverD3D11::draw2DImage( const v4i& rect, gtTexture* texture ){
	gtMaterial m;
	m.textureLayer[ 0u ].texture = texture;
	draw2DImage( rect, v4i(), m );
}

void gtDriverD3D11::draw2DImage( const v4i& rect, const v4i& region, gtTexture* texture ){
	gtMaterial m;
	m.textureLayer[ 0u ].texture = texture;
	draw2DImage( rect, region, m );
}

void gtDriverD3D11::draw2DImage( const v4i& rect, const gtMaterial& m ){
	draw2DImage( rect, v4i(), m );
}
	//	Render 2d image using region of texture
	//	нарисует картинку с выбранной областью текстуры
	//	rect - координаты левого верхнего и правого нижнего углов
	//	region - координаты левого верхнего и правого нижнего углов области картинки которую нужно нарисовать
void gtDriverD3D11::draw2DImage( const v4i& rect, const v4i& region, const gtMaterial& m ){

	v2i center( m_currentWindowSize.x / 2, m_currentWindowSize.y / 2  );

	v4f realRect;
	realRect.x = f32(rect.x - center.x ) / (f32)center.x;
	realRect.y = (f32(rect.y - center.y ) * -1.f )/(f32)center.y;
	realRect.z = (rect.z - center.x ) / (f32)center.x;
	realRect.w = (f32(rect.w - center.y ) * -1.f )/(f32)center.y;


	/*
	2-------3
	|		|
	|		|
	1-------4
	*/

	
	v2f lt, rb;

	if( v4i() == region ){ // сравнение с пустым вектором. если пустой то координаты по умолчанию.
		lt.x = 0.f;
		lt.y = 0.f;
		rb.x = 1.f;
		rb.y = 1.f;

	}else{
		GT_ASSERT2( m.textureLayer[ 0u ].texture, "texture != nullptr" );

		u32 width = 1u, height = 1u;
		if( m.textureLayer[ 0u ].texture ){
			width = m.textureLayer[ 0u ].texture->getWidth();
			height = m.textureLayer[ 0u ].texture->getHeight();
		}

		f32 mulX = 1.f / (f32)width;
		f32 mulY = 1.f / (f32)height;

		lt.x = region.x * mulX;
		lt.y = region.y * mulY;
		rb.x = region.z * mulX;
		rb.y = region.w * mulY;

	
	}

	v8f uvs;
	uvs.x = lt.x; // 1
	uvs.y = rb.y;

	uvs.z = lt.x; // 2
	uvs.w = lt.y;

	uvs.a = rb.x; // 3
	uvs.b = lt.y;

	uvs.c = rb.x; // 4
	uvs.d = rb.y;
	
	_draw2DImage( realRect, uvs, m );
}

	//	непосредственно рисует картинку
void gtDriverD3D11::_draw2DImage( const v4f& rect, const v8f& region, const gtMaterial& material ){

	gtShader * shader = material.shader;
	if( !shader ){
		shader = m_shader2DStandart;
	}

	setActiveShader( shader );

	struct cbVerts{
		v4f v1;
		v4f v2;
		v4f v3;
		v4f v4;
		v2f t1;
		v2f t2;
		v2f t3;
		v2f t4;
	}cb;

	cb.v1.x = rect.x;	//x		
	cb.v1.y = rect.w;	//y		
	cb.v1.z = 0.f;	//z		*
	cb.v1.w = 1.f;
	cb.t1.x = region.x;	//u
	cb.t1.y = region.y;	//v

	cb.v2.x = rect.x;	//x		*
	cb.v2.y = rect.y;	//y		|
	cb.v2.z = 0.f;	//z			*
	cb.v2.w = 1.f;
	cb.t2.x = region.z;	//u
	cb.t2.y = region.w;	//v

	cb.v3.x = rect.z;	//x		*-----*
	cb.v3.y = rect.y;	//y		|	/
	cb.v3.z = 0.f;	//z			*/
	cb.v3.w = 1.f;
	cb.t3.x = region.a;	//u
	cb.t3.y = region.b;	//v

	cb.v4.x = rect.z;	//x		*-----*
	cb.v4.y = rect.w;	//y		|	/
	cb.v4.z = 0.f;		//z		*/    *
	cb.v4.w = 1.f;
	cb.t4.x = region.c;	//u
	cb.t4.y = region.d;	//v

	m_d3d11DevCon->IASetInputLayout( 0 );
	m_d3d11DevCon->VSSetShader( ((gtShaderImpl*)shader)->m_vShader, 0, 0 );
	m_d3d11DevCon->PSSetShader( ((gtShaderImpl*)shader)->m_pShader, 0, 0 );
	m_d3d11DevCon->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	u32 sz = ((gtShaderImpl*)shader)->m_constantBuffers.size();

	struct cbPixel{
		float opacity;
		float padding[3];
	}cb_pixel;

	cb_pixel.opacity = 1.f - material.opacity;

	void * cbs[2] = 
	{
		&cb,
		&cb_pixel
	};

	for( u32 i = 0u; i < sz; ++i ){
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		
		m_d3d11DevCon->Map(
			((gtShaderImpl*)shader)->m_constantBuffers[ i ],
			0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

		D3D11_BUFFER_DESC d;
		((gtShaderImpl*)shader)->m_constantBuffers[ i ]->GetDesc( &d );

		memcpy( mappedResource.pData, cbs[ i ], d.ByteWidth );
		m_d3d11DevCon->Unmap( ((gtShaderImpl*)shader)->m_constantBuffers[ i ], 0 );

	}
	m_d3d11DevCon->VSSetConstantBuffers( 0, 1, &((gtShaderImpl*)shader)->m_constantBuffers[ 0u ] );
	m_d3d11DevCon->PSSetConstantBuffers( 0, 1, &((gtShaderImpl*)shader)->m_constantBuffers[ 1u ] );

	for( u32 i = 0u; i < 16u; ++i ){
		if( !material.textureLayer[ i ].texture ) break;

		gtTextureD3D11* texture = (gtTextureD3D11*)material.textureLayer[ i ].texture;

		m_d3d11DevCon->PSSetShaderResources( i, 1, texture->getResourceView() );
		m_d3d11DevCon->PSSetSamplers( i, 1, texture->getSamplerState() );
	}

	/* так делать плохо, но пусть пока будет так */
	if( material.flags & gtMaterialFlag::MF_BLEND )
		enableBlending( true );
	else
		enableBlending( false );

	
	m_d3d11DevCon->Draw( 6, 0 );
}

	//	нарисует gtRenderModel
void gtDriverD3D11::drawModel( gtRenderModel* model ){
	//auto * soft = model->getModel();

	gtRenderModelD3D11 * rm = (gtRenderModelD3D11*)model;

	u32 smc = rm->m_subs.size();
	
	u32 stride = rm->m_stride;

	gtRenderModelD3D11* d3dm = (gtRenderModelD3D11*)model;

	u32 offset = 0u;

	for( u32 i( 0u ); i < smc; ++i ){

		//auto * sub = soft->getSubModel( i );
		
		gtMaterial * material = rm->getMaterial( i );

		gtShader * shader = material->shader;
		if( !shader ){

			switch( material->type ){
			case gtMaterialType::GUI:
				shader = m_shaderGUI;
				m_shaderProcessing->setStandartTexture( m_standartTextureWhiteColor.data() );
				break;
			case gtMaterialType::Standart:
			default:
				shader = m_shader3DStandart;
				m_shaderProcessing->setStandartTexture( m_standartTexture.data() );
				break;
			case gtMaterialType::Sprite:
				shader = m_shaderSprite;
				m_shaderProcessing->setStandartTexture( m_standartTexture.data() );
				break;
			}
		}

		m_shaderProcessing->setShader( shader );
		m_shaderProcessing->setMaterial( material );
		
		gtShaderImpl * shaderD3D11 = ((gtShaderImpl*)shader);

		m_d3d11DevCon->IASetInputLayout( ((gtShaderImpl*)shader)->m_vLayout );
		m_d3d11DevCon->VSSetShader( ((gtShaderImpl*)shader)->m_vShader, 0, 0 );
		m_d3d11DevCon->PSSetShader( ((gtShaderImpl*)shader)->m_pShader, 0, 0 );
		m_d3d11DevCon->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		m_d3d11DevCon->IASetVertexBuffers( 0, 1, &d3dm->m_vBuffers[ i ], &stride, &offset );
		m_d3d11DevCon->IASetIndexBuffer( d3dm->m_iBuffers[ i ], DXGI_FORMAT_R16_UINT, 0);

		if( shaderD3D11->m_callback )
			shaderD3D11->m_callback->onShader( *material, m_shaderProcessing.data() );

		if( material->flags & gtMaterialFlag::MF_BLEND )
			enableBlending( true, material->alphaToCoverage );
		else
			enableBlending( false );

		if( material->flags & gtMaterialFlag::MF_BACKFACE ){
			if( material->flags & gtMaterialFlag::MF_WIREFRAME )
				m_d3d11DevCon->RSSetState( m_RasterizerWireframeNoBackFaceCulling );
			else
				m_d3d11DevCon->RSSetState( m_RasterizerSolidNoBackFaceCulling );
		}else{
			if( material->flags & gtMaterialFlag::MF_WIREFRAME )
				m_d3d11DevCon->RSSetState( m_RasterizerWireframe );
			else
				m_d3d11DevCon->RSSetState( m_RasterizerSolid );
		}
		m_d3d11DevCon->DrawIndexed( rm->m_subs[ i ].iCount, 0, 0 );
	}
}

void gtDriverD3D11::drawLine( const v3f& start, const v3f& end, const gtColor& color ){
	gtShader * shader = m_shaderLine;
	m_shaderProcessing->setShader( shader );
	m_shaderProcessing->setMaterial( nullptr );

	m_shaderLineCallback->s = start;
	m_shaderLineCallback->e = end;
	m_shaderLineCallback->c.x = color.getRed();
	m_shaderLineCallback->c.y = color.getGreen();
	m_shaderLineCallback->c.z = color.getBlue();
	m_shaderLineCallback->c.w = color.getAlpha();
	m_shaderLineCallback->s.w = 1.f;
	m_shaderLineCallback->e.w = 1.f;

	if( ((gtShaderImpl*)shader)->m_callback )
			((gtShaderImpl*)shader)->m_callback->onShader( gtMaterial(), m_shaderProcessing.data() );

	m_d3d11DevCon->IASetInputLayout( 0 );
	m_d3d11DevCon->VSSetShader( ((gtShaderImpl*)shader)->m_vShader, 0, 0 );
	m_d3d11DevCon->PSSetShader( ((gtShaderImpl*)shader)->m_pShader, 0, 0 );
	m_d3d11DevCon->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST  );
	m_d3d11DevCon->Draw( 2, 0 );
}


gtShader *	gtDriverD3D11::getShader( 
	gtShaderCallback * callback,
	const gtString& vertexShader,
	const gtStringA& vertexShaderMain,
	const gtString& pixelShader,
	const gtStringA& pixelShaderMain,
	gtShaderModel shaderModel,

	gtVertexType * vertexType
){
	std::unique_ptr<s8[]> vertexBuffer;
	std::unique_ptr<s8[]> pixelBuffer;

	gtString fullPathVS = gtFileSystem::getRealPath( vertexShader );
	gtString fullPathPS = gtFileSystem::getRealPath( pixelShader );

	if( gtFileSystem::existFile( fullPathVS ) ){

		gtFile_t file = util::openFileForReadText( fullPathVS );

		u32 sz = file->size();
		
		if( !sz ){
			gtLogWriter::printError( u"Empty shader file [%s]", fullPathVS.data() );
			return nullptr;
		}

		vertexBuffer.reset( new s8[ sz+1u ] );
		vertexBuffer.get()[sz] = 0;

		file->read( (u8*)vertexBuffer.get(), sz );


	}else{
		u32 sz = vertexShader.size();
		if( !sz ){
			gtLogWriter::printError( u"Empty shader file [%s]", vertexShader.data() );
			return nullptr;
		}

		vertexBuffer.reset( new s8[ sz+1u ] );
		vertexBuffer.get()[sz] = 0;
		auto * data = vertexShader.data();

		for( u32 i = 0u; i < sz; ++i ){
			auto * b = vertexBuffer.get();
			b[ i ] = (s8)data[ i ];
		}
	}

	if( gtFileSystem::existFile( fullPathPS ) ){
		gtFile_t file = util::openFileForReadText( fullPathPS );
		u32 sz = file->size();
		if( !sz ){
			gtLogWriter::printError( u"Empty shader file [%s]", fullPathPS.data() );
			return nullptr;
		}
		pixelBuffer.reset( new s8[ sz+1 ] );
		pixelBuffer.get()[sz] = 0;

		file->read( (u8*)pixelBuffer.get(), sz );
	}else{
		u32 sz = pixelShader.size();
		if( !sz ){
			gtLogWriter::printError( u"Empty shader file [%s]", pixelShader.data() );
			return nullptr;
		}
		pixelBuffer.reset( new s8[ sz+1 ] );
		pixelBuffer.get()[sz] = 0;
		auto * data = pixelShader.data();
		for( u32 i = 0u; i < sz; ++i ){
			auto * b = pixelBuffer.get();
			b[ i ] = (s8)data[ i ];
		}
	}

	gtStringA v_target;

	switch( shaderModel.vertexShaderModel ){
	case gtShaderModel::shaderModel::_1_1:
	case gtShaderModel::shaderModel::_1_2:
	case gtShaderModel::shaderModel::_1_3:
	case gtShaderModel::shaderModel::_1_4:
	case gtShaderModel::shaderModel::_1_5:
		v_target = "vs_1_1";
		break;
	case gtShaderModel::shaderModel::_2_0:
		v_target = "vs_2_0";
		break;
	case gtShaderModel::shaderModel::_3_0:
	case gtShaderModel::shaderModel::_3_3:
		v_target = "vs_3_0";
		break;
	case gtShaderModel::shaderModel::_4_0:
		v_target = "vs_4_0";
		break;
	case gtShaderModel::shaderModel::_4_1:
	case gtShaderModel::shaderModel::_4_2:
	case gtShaderModel::shaderModel::_4_3:
	case gtShaderModel::shaderModel::_4_5:
		v_target = "vs_4_1";
		break;
	case gtShaderModel::shaderModel::_5_0:
		v_target = "vs_5_0";
		break;
	case gtShaderModel::shaderModel::_6_0:
		v_target = "vs_6_0";
		break;
	}

	gtStringA p_target;

	switch( shaderModel.pixelShaderModel ){
	case gtShaderModel::shaderModel::_1_1:
	case gtShaderModel::shaderModel::_1_2:
	case gtShaderModel::shaderModel::_1_3:
	case gtShaderModel::shaderModel::_1_4:
	case gtShaderModel::shaderModel::_1_5:
		p_target = "ps_1_1";
		break;
	case gtShaderModel::shaderModel::_2_0:
		p_target = "ps_2_0";
		break;
	case gtShaderModel::shaderModel::_3_0:
	case gtShaderModel::shaderModel::_3_3:
		p_target = "ps_3_0";
		break;
	case gtShaderModel::shaderModel::_4_0:
		p_target = "ps_4_0";
		break;
	case gtShaderModel::shaderModel::_4_1:
	case gtShaderModel::shaderModel::_4_2:
	case gtShaderModel::shaderModel::_4_3:
	case gtShaderModel::shaderModel::_4_5:
		p_target = "ps_4_1";
		break;
	case gtShaderModel::shaderModel::_5_0:
		p_target = "ps_5_0";
		break;
	case gtShaderModel::shaderModel::_6_0:
		p_target = "ps_6_0";
		break;
	}

	gtPtr< gtShaderImpl > shader = gtPtrNew< gtShaderImpl >( new gtShaderImpl(this) );

	if( !shader->compileShader( 
		callback,
		v_target, 
		p_target,
		vertexBuffer.get(),
		pixelBuffer.get(),
		(s8*)vertexShaderMain.data(),
		(s8*)pixelShaderMain.data(),
		vertexType ) ){
		return nullptr;
	}

	shader->addRef();
	return shader.data();
}

gtPtr<gtTexture>	gtDriverD3D11::createTexture( gtImage* image, gtTextureFilterType filter ){
	GT_ASSERT2( image, "gtImage != nullptr" );

	auto ptr = new gtTextureD3D11( this );
	gtPtr<gtTexture> texture = gtPtrNew<gtTexture>( ptr );

	if( !ptr->init( image, filter ) ){
		gtLogWriter::printWarning( u"Can not init D3D11 texture" );
		return nullptr;
	}

	return texture;
}

gtPtr<gtRenderModel>	gtDriverD3D11::createModel( gtModel* m ){
	GT_ASSERT2( m, "gtModel != nullptr" );

	auto ptr = new gtRenderModelD3D11( this );
	gtPtr<gtRenderModel> model = gtPtrNew<gtRenderModel>( ptr );

	if( !ptr->init( m ) ){
		gtLogWriter::printWarning( u"Can not init D3D11 model" );
		return nullptr;
	}

	return model;
}


bool	gtDriverD3D11::createShaders( void ){
	gtShaderModel shaderModel;
	shaderModel.pixelShaderModel = gtShaderModel::shaderModel::_5_0;
	shaderModel.vertexShaderModel = gtShaderModel::shaderModel::_5_0;

	gtVertexType vertexType2D[] = 
	{
		{ gtVertexType::position },
		{ gtVertexType::end }
	};

	m_shader2DStandart = getShader(
		nullptr,
		u"../shaders/2d_basic.hlsl",
		"VSMain",
		u"../shaders/2d_basic.hlsl",
		"PSMain",
		shaderModel,
		vertexType2D
		);
	if( m_shader2DStandart ){
		if( !m_shader2DStandart->createShaderObject( 96u ) ) return false;
		if( !m_shader2DStandart->createShaderObject( 16u ) ) return false;
	}

	gtVertexType vertexType3D[] = 
	{
		{ gtVertexType::position },
		{ gtVertexType::uv },
		{ gtVertexType::normal },
		{ gtVertexType::end }
	};
	
	m_shader3DStandartCallback = gtPtrNew<gtD3D11StandartShaderCallback>( new gtD3D11StandartShaderCallback );
	m_shaderGUICallback = gtPtrNew<gtD3D11GUIShaderCallback>( new gtD3D11GUIShaderCallback );
	m_shaderSpriteCallback = gtPtrNew<gtD3D11SpriteShaderCallback>( new gtD3D11SpriteShaderCallback );
	m_shaderLineCallback = gtPtrNew<gtD3D11LineShaderCallback>( new gtD3D11LineShaderCallback );

	m_shader3DStandart = getShader( m_shader3DStandartCallback.data(), u"../shaders/3d_basic.hlsl", "VSMain",
		u"../shaders/3d_basic.hlsl", "PSMain", shaderModel, vertexType3D );
	
	m_shaderGUI = getShader( m_shaderGUICallback.data(), u"../shaders/GUI.hlsl", "VSMain",
		u"../shaders/GUI.hlsl", "PSMain", shaderModel, vertexType3D );

	m_shaderSprite = getShader( m_shaderSpriteCallback.data(), u"../shaders/sprite.hlsl", "VSMain",
		u"../shaders/sprite.hlsl", "PSMain", shaderModel, vertexType3D );

	m_shaderLine = getShader( m_shaderLineCallback.data(), u"../shaders/line.hlsl", "VSMain",
		u"../shaders/line.hlsl", "PSMain", shaderModel, vertexType2D );

	if( m_shader3DStandart ) if( !m_shader3DStandart->createShaderObject( 16u * sizeof(f32) ) ) return false;
	if( m_shaderSprite ) if( !m_shaderSprite->createShaderObject( 24u * sizeof(f32) ) ) return false;
	if( m_shaderLine ) if( !m_shaderLine->createShaderObject( 28u * sizeof(f32) ) ) return false;
	if( m_shaderGUI ) if( !m_shaderGUI->createShaderObject( 4u * sizeof(f32) + 128u ) ) return false;

	return true;
}

void	gtDriverD3D11::applyScissor( void ){
	m_d3d11DevCon->RSSetScissorRects( m_scissorRects.size(), (D3D11_RECT*)m_scissorRects.data() );
}

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