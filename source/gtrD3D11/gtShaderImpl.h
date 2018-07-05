//GoST

#pragma once
#ifndef __GT_SHADER_IMPL_H__
#define __GT_SHADER_IMPL_H__

#include "gtShaderCallbacks.h"

namespace gost{

	class gtDriverD3D11;
	class gtShaderImpl GT_FINAL: public gtShader{

		gtShaderModel m_shaderModel;

		gtDriverD3D11 * m_gs;

	public:
		gtShaderImpl( gtDriverD3D11* );
		virtual ~gtShaderImpl();

		bool			compileShader(
			gtShaderCallback * callback,
			const gtStringA& vertexTarget,
			const gtStringA& pixelTarget,
			s8 * vertexShader,
			s8 * pixelShader,
			s8 * vertexEntryPoint,
			s8 * pixelEntryPoint,
			gtVertexType *);

		gtShaderModel	getShaderModel();

		bool	createShaderObject( u32 byteSize );
		
		ID3D11VertexShader*	m_vShader;
		ID3D11PixelShader*	m_pShader;
		ID3D11InputLayout*	m_vLayout;
		gtShaderCallback * m_callback;

		gtArray<ID3D11Buffer*> m_constantBuffers;
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