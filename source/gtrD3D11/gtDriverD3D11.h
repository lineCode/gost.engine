//GoST

#pragma once

#ifndef __GT_DRIVER_D3D11_H__
#define __GT_DRIVER_D3D11_H__

#define GT_D3D11_CALL

using gtD3D11CreateDevice_t = HRESULT(GT_D3D11_CALL*)(
	_In_opt_ IDXGIAdapter* pAdapter,
	D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	_In_reads_opt_( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	_Out_opt_ ID3D11Device** ppDevice,
	_Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
	_Out_opt_ ID3D11DeviceContext** ppImmediateContext);

using gtCreateDXGIFactory_t = HRESULT(GT_D3D11_CALL*)(
	REFIID riid, _COM_Outptr_ void **ppFactory );

using gtD3D11CreateDeviceAndSwapChain_t = HRESULT(GT_D3D11_CALL*)(
	__in_opt IDXGIAdapter* pAdapter,
	D3D_DRIVER_TYPE DriverType,
	HMODULE Software,
	UINT Flags,
	__in_ecount_opt( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
	UINT FeatureLevels,
	UINT SDKVersion,
	__in_opt CONST DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
	__out_opt IDXGISwapChain** ppSwapChain,
	__out_opt ID3D11Device** ppDevice,
	__out_opt D3D_FEATURE_LEVEL* pFeatureLevel,
	__out_opt ID3D11DeviceContext** ppImmediateContext );

//	D3dcompiler_47.dll
using gtD3DCompile_t = HRESULT (__stdcall*)(
	LPCVOID pSrcData,
	SIZE_T SrcDataSize,
    LPCSTR pSourceName,
    CONST D3D_SHADER_MACRO* pDefines,
    ID3DInclude* pInclude,
    LPCSTR pEntrypoint,
    LPCSTR pTarget,
    UINT Flags1,
    UINT Flags2,
    ID3DBlob** ppCode,
    ID3DBlob** ppErrorMsgs);

namespace gost{

	class gtDriverD3D11 GT_FINAL : public gtGraphicsSystemCommon{

		static gtDriverD3D11* s_instance;
	
		gtMainSystem* m_system;
		HMODULE m_D3DLibrary;
		//HMODULE m_DXGILibrary;

		IDXGIFactory1*          m_dxgiFactory;
		IDXGISwapChain*			m_SwapChain;
		ID3D11Device*			m_d3d11Device;
		ID3D11DeviceContext*	m_d3d11DevCon;
		ID3D11RenderTargetView* m_MainTargetView;
		ID3D11Texture2D*		m_depthStencilBuffer;
		ID3D11DepthStencilState*m_depthStencilStateEnabled;
		ID3D11DepthStencilState*m_depthStencilStateDisabled;
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11RasterizerState*	m_RasterizerSolid;
		ID3D11RasterizerState*	m_RasterizerSolidNoBackFaceCulling;
		ID3D11RasterizerState*	m_RasterizerWireframeNoBackFaceCulling;
		ID3D11RasterizerState*	m_RasterizerWireframe;
		ID3D11BlendState*		m_blendStateAlphaEnabled;
		ID3D11BlendState*		m_blendStateAlphaEnabledWithATC;
		ID3D11BlendState*		m_blendStateAlphaDisabled;
		
		gtShader*			m_shader2DStandart;
		gtShader*			m_shader3DStandart;
		gtShader*			m_shaderGUI;
		gtShader*			m_shaderSprite;
		gtShader*			m_shaderLine;
		

		bool m_beginRender;
		
		D3D11_DEPTH_STENCIL_VIEW_DESC	m_depthStencilViewDesc;

		

		gtPtr<gtShaderProcessingD3D11> m_shaderProcessing;

		gtPtr<gtTexture>	m_standartTexture;
		gtPtr<gtTexture>	m_standartTextureWhiteColor;
		
		gtPtr<gtTexture>	   m_CSMTexture;
		gtPtr<gtCamera>        m_CSMLightCamera;
		
		gtPtr<gtD3D11StandartShaderCallback> m_shader3DStandartCallback;
		gtPtr<gtD3D11GUIShaderCallback>		 m_shaderGUICallback;
		gtPtr<gtD3D11SpriteShaderCallback>	 m_shaderSpriteCallback;
		gtPtr<gtD3D11LineShaderCallback>	 m_shaderLineCallback;
		
		void    clearRenderTarget( const gtColor& );
		
		void	setActiveShader( gtShader* );
		void	_draw2DImage( const v4f& rect, const v8f& region, const gtMaterial& );
		void	enableBlending( bool, bool atc = false );
		bool	createShaders();
		void	createStandartTexture();
		void	applyScissor();
		void    initCSM();
	public:
		gtDriverD3D11( /*gtMainSystem* System, */gtGraphicsSystemInfo params );
		virtual ~gtDriverD3D11();

		bool initialize();

		HMODULE getD3DLibraryHandle();
		ID3D11Device * getD3DDevice();
		ID3D11DeviceContext * getD3DDeviceContext();


		void beginRender( bool clearRenderTarget = true, const gtColor& color = gtColor(0.f) );
		void endRender();
		
		void draw2DBox( const v4i& rect, const gtColor& color );
		void draw2DImage( const v4i& rect, const gtMaterial& );
		void draw2DImage( const v4i& rect, const v4i& region, const gtMaterial& );
		void draw2DImage( const v4i& rect, gtTexture* texture );
		void draw2DImage( const v4i& rect, const v4i& region, gtTexture* texture );
		void drawModel( gtRenderModel*, gtArray<gtMaterial>* materials );
		void drawLine( const v4f& start, const v4f& end, const gtColor& color );
		
		gtTexture*		getDefaultTexture(); 

		gtShader *	getShader( 
			gtShaderCallback * callback,
				//	путь к файлу хранящем вершинный шейдер
			const gtString& vertexShader,
				//	главная функция вершинного шейдера, точка входа
			const gtStringA& vertexShaderMain,
				//	путь к файлу хранящем пиксельный/фрагментный шейдер
			const gtString& pixelShader,
				//	главная функция пиксельного/фрагментного шейдера, точка входа
			const gtStringA& pixelShaderMain,
				//	тип шейдерного языка
			gtShaderModel shaderModel,
				//	тип вершины (должен быть массив)
			gtVertexType * vertexType
			);

		gtPtr<gtTexture>	createTexture( gtImage* );
		gtPtr<gtTexture>    createRenderTargetTexture( const v2u& size, gtImageFormat pixelFormat );
		gtPtr<gtRenderModel>	createModel( gtModel* );


		static gtDriverD3D11*	getInstance();
		gtTexture * getStandartTexture();
		gtTexture * getStandartTextureWhiteColor();

		void	setDepthState( bool state = true );
		void	setRenderTarget( gtTexture * rtt, bool clearDepth, bool clear, const gtColor& crearColor );
		void    setViewport( const v2f& viewportSize );

		

		void	renderEffects();
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