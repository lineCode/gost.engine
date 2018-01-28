//GoST

#include "common.h"

gtShaderImpl::gtShaderImpl( gtDriverD3D11 * driver ):
	m_vShader( nullptr ),
	m_pShader( nullptr ),
	m_vLayout( nullptr ),
	m_driver( driver )
{}


gtShaderImpl::~gtShaderImpl( void ){

	auto sz = m_constantBuffers.size();
	for( auto i = 0u; i < sz; ++i ){
		m_constantBuffers[ i ]->Release();
	}

	if( m_vLayout )
		m_vLayout->Release();

	if( m_vShader )
		m_vShader->Release();

	if( m_pShader )
		m_pShader->Release();

}

bool gtShaderImpl::compileShader(
			gtShaderCallback * callback,
			const gtStringA& vertexTarget,
			const gtStringA& pixelTarget,
			s8 * vertexShader,
			s8 * pixelShader,
			s8 * vertexEntryPoint,
			s8 * pixelEntryPoint,
			gtVertexType * vertexType ){

	m_callback = callback;

	s32 d3dcompiler_num = 47;
	
	gtString compilerPath;

	for(;;){
		compilerPath = gtFileSystem::getSystemPath();
		compilerPath += u"D3dcompiler_";
		compilerPath += d3dcompiler_num;
		compilerPath += u".dll";

		if( !gtFileSystem::existFile( compilerPath ) ) --d3dcompiler_num;
		else break;

		if( d3dcompiler_num == 42 ){
			gtString msg = u"D3dcompiler_";
			msg += d3dcompiler_num;
			msg += u".dll (or high) not exist";
			gtLogWriter::printError( u"%s", msg.data() );
			MessageBox( (HWND)m_driver->getParams().m_outWindow->getHandle(),
				(wchar_t*)msg.data(),
				L"Error", MB_ICONERROR | MB_OK );
			return false;
		}
	}

	HMODULE lib = LoadLibrary( (wchar_t*)compilerPath.data() );
	if( !lib ){
		gtString msg = u"Can not load D3dcompiler_";
		msg += d3dcompiler_num;
		msg += u".dll";
		gtLogWriter::printError( u"%s Error code[%u]", msg.data(), GetLastError() );
		MessageBox( (HWND)m_driver->getParams().m_outWindow->getHandle(),
			(wchar_t*)msg.data(),
			L"Error", MB_ICONERROR | MB_OK );
		return false;
	}

	//gtD3DCompile_t	CompileShader = (gtD3DCompile_t)GetProcAddress( lib, "D3DCompile" );
	gtD3DCompile_t	CompileShader = GT_LOAD_FUNCTION_SAFE_CAST(gtD3DCompile_t,lib, "D3DCompile" );
	if( !CompileShader ){
		gtString msg = u"Can not get procedure address \"D3DCompile\" from D3dcompiler_";
		msg += d3dcompiler_num;
		msg += u".dll";
		gtLogWriter::printError( u"%s", msg.data() );
		MessageBox( (HWND)m_driver->getParams().m_outWindow->getHandle(),
			(wchar_t*)msg.data(),
			L"Error", MB_ICONERROR | MB_OK );
		FreeLibrary( lib );
		return false;
	}

	ID3D10Blob*		m_VsBlob = nullptr;
	ID3D10Blob*		m_PsBlob = nullptr;
	ID3D10Blob*		m_errorBlob = nullptr;

	HRESULT hr = CompileShader(
		vertexShader,
		strlen( vertexShader ),
		0, 0, 0,
		vertexEntryPoint,
		vertexTarget.data(),
		0,
		0,
		&m_VsBlob,
		&m_errorBlob
	);
	
	if( FAILED( hr ) ){
		gtString errorMessage;
		s8 * message = (s8*)m_errorBlob->GetBufferPointer();
		u32 len = strlen(message);
		for( u32 i = 0u; i < len; ++i )
			errorMessage += (char16_t)message[ i ];
		gtLogWriter::printError( u"Vertex shader compile error: %s", errorMessage.data() );
		FreeLibrary( lib );
		return false;
	}


	hr = CompileShader(
		pixelShader,
		strlen( pixelShader ),
		0, 0, 0,
		pixelEntryPoint,
		pixelTarget.data(),
		0,
		0,
		&m_PsBlob,
		&m_errorBlob
	);
	
	

	if( FAILED( hr ) ){
		gtString errorMessage;
		s8 * message = (s8*)m_errorBlob->GetBufferPointer();
		u32 len = strlen(message);
		for( u32 i = 0u; i < len; ++i )
			errorMessage += (char16_t)message[ i ];
		gtLogWriter::printError( u"Pixel shader compile error: %s", errorMessage.data() );
		FreeLibrary( lib );
		return false;
	}

	auto * d3ddevice = m_driver->getD3DDevice();

	hr =  d3ddevice->CreateVertexShader( 
		m_VsBlob->GetBufferPointer(), 
		m_VsBlob->GetBufferSize(),
		0,
		&m_vShader );
	if( FAILED( hr ) ){
		gtLogWriter::printError( u"Can't create vertex shader. Error code [%u]", hr );
		FreeLibrary( lib );
		return false;
	}

	hr =  d3ddevice->CreatePixelShader( 
		m_PsBlob->GetBufferPointer(), 
		m_PsBlob->GetBufferSize(),
		0,
		&m_pShader );
	if( FAILED( hr ) ){
		gtLogWriter::printError( u"Can't create pixel shader. Error code [%u]", hr );
		FreeLibrary( lib );
		return false;
	}

	u32 vertexLayoutComponentSize = 0u;

	for( u32 i = 0u; i < 1024u; ++i ){
		if( vertexType[ i ] == gtVertexType::end )
			break;
		else
			vertexLayoutComponentSize++;

		if( i == 100u ){
			gtLogWriter::printError( u"Can't create shader. Bad vertex type" );
			FreeLibrary( lib );
			return false;
		}
	}

	//	пока нет возможности это протестировать
	std::unique_ptr< D3D11_INPUT_ELEMENT_DESC[] > layout( new D3D11_INPUT_ELEMENT_DESC[ vertexLayoutComponentSize ] );

	UINT offset = 0u;

	for( u32 i = 0u; i < vertexLayoutComponentSize; ++i ){
		if( vertexType[ i ] == gtVertexType::position ){

			layout.get()[ i ] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offset,  D3D11_INPUT_PER_VERTEX_DATA, 0 };
			offset += 16;

		}else if( vertexType[ i ] == gtVertexType::uv ){

			layout.get()[ i ] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			offset += 8;

		}else if( vertexType[ i ] == gtVertexType::normal ){

			layout.get()[ i ] = { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			offset += 12;

		}else if( vertexType[ i ] == gtVertexType::binormal ){

			layout.get()[ i ] = { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			offset += 12;

		}else if( vertexType[ i ] == gtVertexType::tangent ){

			layout.get()[ i ] = { "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			offset += 12;

		}else if( vertexType[ i ] == gtVertexType::color ){

			layout.get()[ i ] = { "COLOR",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0 };
			offset += 16;

		}
	}

	hr = d3ddevice->CreateInputLayout(
		layout.get(),
		vertexLayoutComponentSize,
		m_VsBlob->GetBufferPointer(),
		m_VsBlob->GetBufferSize(),
		&m_vLayout );
	if( FAILED( hr ) ){
		gtLogWriter::printError( u"Can't create input layout. Error code [%u]", hr );
		FreeLibrary( lib );
		return false;
	}

	if( m_VsBlob )
		m_VsBlob->Release();
		m_VsBlob = nullptr;

	if( m_PsBlob )
		m_PsBlob->Release();
		m_PsBlob = nullptr;

	if( m_errorBlob )
		m_errorBlob->Release();
		m_errorBlob = nullptr;

	FreeLibrary( lib );
	return true;
}

gtShaderModel	gtShaderImpl::getShaderModel( void ){
	return m_shaderModel;
}

bool gtShaderImpl::createShaderObject( u32 byteSize ){
	D3D11_BUFFER_DESC mbd;
	memset( &mbd, 0, sizeof( mbd ) );
	mbd.Usage	=	D3D11_USAGE_DYNAMIC;
	mbd.ByteWidth	=	byteSize;
	mbd.BindFlags	=	D3D11_BIND_CONSTANT_BUFFER;
	mbd.CPUAccessFlags	=	D3D11_CPU_ACCESS_WRITE;
	mbd.MiscFlags	=	0;
	mbd.StructureByteStride	=	0;

	ID3D11Buffer* buffer = nullptr;

	HRESULT hr	=	m_driver->getD3DDevice()->CreateBuffer( &mbd, 0, &buffer );
	if( FAILED( hr ) ){
		gtLogWriter::printError( u"Can't create constant buffer. Error code [%u]", hr );
		return false;
	}

	this->m_constantBuffers.push_back( buffer );

	return true;
}

/*
Copyright (c) 2017 532235

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