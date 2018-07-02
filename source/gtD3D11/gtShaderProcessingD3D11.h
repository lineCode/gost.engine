#pragma once
#ifndef __GT_SHADER_PROCESSING_D3D11_H__
#define __GT_SHADER_PROCESSING_D3D11_H__

namespace gost{


	class gtShaderProcessingD3D11 : public gtShaderProcessing{

		gtShaderImpl * m_shaderD3D11;
		gtMaterial * m_material;

		ID3D11DeviceContext*	m_d3d11DevCon;
		gtTexture* m_standartTexture;

	public:

		gtShaderProcessingD3D11( ID3D11DeviceContext* );
		virtual ~gtShaderProcessingD3D11();

		void	setFloat( const gtStringA& uniformName, f32 a );
		void	setInt( const gtStringA& uniformName, s32 a );
		void	setMatrix4x4( const gtStringA& uniformName, gtMatrix4& matrix );
		void	setVec3f( const gtStringA& uniformName, v3f& vec3 );
		void	setVec4f( const gtStringA& uniformName, v3f& vec4 );
		void	setTexture( const gtStringA& uniformName, s32 id ) override;
		void	sendDataVS( void* data, s32 slot, u32 id ) override;
		void	sendDataPS( void* data, s32 slot, u32 id ) override;

		// ==============================================================
		void setShader( gtShader * );
		void setMaterial( gtMaterial * );
		void mapData( void* data, s32 id );
		void setStandartTexture( gtTexture * t );
	};

}


#endif

/*
Copyright (c) 2018 532235

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