/*!	GOST
	\file gtShader.h
	\brief working with shaders
*/

#pragma once
#ifndef __GT_SHADER_H__
#define __GT_SHADER_H__ //< include guard

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

		//	Шейдер
	class gtShader : public gtRefObject {
	public:

			// получить используемую версию шейдера
			// \return shader model
		virtual gtShaderModel	getShaderModel( void ) = 0;

			//	В d3d11 создаёт константный буффер
			// \param byteSize: размер в байтах
			// \return \b true если успех
		virtual bool	createShaderObject( u32 byteSize ) = 0;

	};

		// Устанавливает параметры в шейдерной программе
	class gtShaderProcessing : public gtRefObject{
	public:

			// c-tor
		gtShaderProcessing( void ){}

			// d-tor
		virtual ~gtShaderProcessing( void ){}

			// Послать в шейдер значение float
			// \param un: uniform name
			// \param v: value
		virtual void	setFloat( const gtStringA& /*un*/, f32 /*v*/ ){
		}

			// Послать в шейдер значение int
			// \param un: uniform name
			// \param v: value
		virtual void	setInt( const gtStringA& /*un*/, s32 /*v*/ ){}

			// Послать в шейдер матрицу
			// \param un: uniform name
			// \param v: value
		virtual void	setMatrix4x4( const gtStringA& /*un*/, gtMatrix4& /*v*/ ){}

			// Послать в шейдер вектор3
			// \param un: uniform name
			// \param v: value
		virtual void	setVec3f( const gtStringA& /*un*/, v3f& /*v*/ ){}

			// Послать в шейдер вектор4
			// \param un: uniform name
			// \param v: value
		virtual void	setVec4f( const gtStringA& /*un*/, v3f& /*v*/ ){}

			// Послать в шейдер текстуру
			// \param un: uniform name
			// \param id: номер текстуры в материале
		virtual void	setTexture( const gtStringA& /*un*/, s32 /*id*/ ){}

			/**
				для константных буферов \n
				В буфер можно послать как в VS так и в PS, по этому slot и id могут различаться \n
				напр.	 \n
				sendDataVS(data, 0, 0); \n
				sendDataPS(data, 0, 1); \n
				или \n
				sendDataVS(data, 0, 0);		//	sendDataVS(data, 0, 0); <-\ \n
				sendDataVS(data, 1, 1);		//	sendDataVS(data, 1, 1);		|- 1 буфер и туда и туда \n
				sendDataPS(data, 0, 2);		//	sendDataPS(data, 0, 0); <-/ \n
				sendDataPS(data, 1, 3);		//	sendDataPS(data, 1, 2); \n

				\param data: указатель на структуру равную констунтному буферу в шейдере
				\param slot: номер очереди для вершинного шейдера. Начинается с нуля. если посылается в пиксельный, то там нужно тоже начинать с нуля.
				\param id: это номер буфера (созданного через вызов createShaderObject)
			*/
		virtual void	sendDataVS( void* /**data*/, s32 /**slot*/, u32 /**id*/ ){}

			//	см. описание sendDataVS
		virtual void	sendDataPS( void* /**data*/, s32 /**slot*/, u32 /**id*/ ){}
	};

	class gtMaterial;

		// вызывается в момент усановки параметров в шейдерной программе
	class gtShaderCallback : public gtRefObject{
	public:
			
			// вызывается в момент усановки параметров в шейдерной программе
			// \param m: material
			// \param sp: gtShaderProcessing for set shader constants
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