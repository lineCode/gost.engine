#pragma once
#ifndef __GT_SHADER_H__
#define __GT_SHADER_H__

namespace gost{

		// Shader model
	struct gtShaderModel{

		enum class shaderModel{
			_1_1,		//<	OpenGL + DirectX
			_1_2,		//<	OpenGL
			_1_3,		//<	OpenGL
			_1_4,		//<	OpenGL
			_1_5,		//<	OpenGL
			_2_0,		//<	DirectX
			_3_0,		//<	DirectX
			_3_3,		//<	OpenGL
			_4_0,		//<	OpenGL + DirectX
			_4_1,		//<	OpenGL + DirectX
			_4_2,		//<	OpenGL
			_4_3,		//<	OpenGL
			_4_5,		//<	OpenGL
			_5_0,		//<	DirectX
			_6_0		//<	DirectX
		}vertexShaderModel, pixelShaderModel;

	};

	class gtShader : public gtRefObject {
	public:

		virtual gtShaderModel	getShaderModel() = 0;

		virtual bool	createShaderObject( u32 byteSize ) = 0;

	};

	class gtShaderProcessing : public gtRefObject{
	public:

			// c-tor
		gtShaderProcessing(){}

			// d-tor
		virtual ~gtShaderProcessing(){}

			// \param un: uniform name
			// \param v: value
		virtual void	setFloat( const gtStringA& /*un*/, f32 /*v*/ ){
		}

			// \param un: uniform name
			// \param v: value
		virtual void	setInt( const gtStringA& /*un*/, s32 /*v*/ ){}

			// \param un: uniform name
			// \param v: value
		virtual void	setMatrix4x4( const gtStringA& /*un*/, gtMatrix4& /*v*/ ){}

			// \param un: uniform name
			// \param v: value
		virtual void	setVec3f( const gtStringA& /*un*/, v3f& /*v*/ ){}

			// \param un: uniform name
			// \param v: value
		virtual void	setVec4f( const gtStringA& /*un*/, v3f& /*v*/ ){}

		virtual void	setTexture( const gtStringA& /*un*/, s32 /*id*/ ){}

		virtual void	sendDataVS( void* /**data*/, s32 /**slot*/, u32 /**id*/ ){}

		virtual void	sendDataPS( void* /**data*/, s32 /**slot*/, u32 /**id*/ ){}
	};

	class gtMaterial;

	class gtShaderCallback : public gtRefObject{
	public:
			
		virtual void onShader( const gtMaterial& m, gtShaderProcessing* sp ) = 0;

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