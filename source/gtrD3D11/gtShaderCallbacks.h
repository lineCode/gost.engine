#pragma once
#ifndef __GT_SHADER_CALLBACKS_H__
#define __GT_SHADER_CALLBACKS_H__

namespace gost{

	class gtD3D11StandartShaderCallback : public gtShaderCallback{

		gtMainSystem * m_system;
		gtSceneSystem * m_scene;

	public:
		gtD3D11StandartShaderCallback();
		virtual ~gtD3D11StandartShaderCallback();

		void onShader( const gtMaterial&, gtShaderProcessing* );

	};

	class gtD3D11GUIShaderCallback : public gtShaderCallback{
		gtMainSystem * m_system;
	public:
		gtD3D11GUIShaderCallback();
		virtual ~gtD3D11GUIShaderCallback();
		void onShader( const gtMaterial&, gtShaderProcessing* );
	};

	class gtD3D11SpriteShaderCallback : public gtShaderCallback{

		gtMainSystem * m_system;

	public:
		gtD3D11SpriteShaderCallback();
		virtual ~gtD3D11SpriteShaderCallback();

		void onShader( const gtMaterial&, gtShaderProcessing* );

	};

	class gtD3D11LineShaderCallback : public gtShaderCallback{

		gtMainSystem * m_system;

	public:
		gtD3D11LineShaderCallback();
		virtual ~gtD3D11LineShaderCallback();

		void onShader( const gtMaterial&, gtShaderProcessing* );

		v4f s, e, c;
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