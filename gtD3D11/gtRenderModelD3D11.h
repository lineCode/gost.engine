//	GOST

#pragma once
#ifndef __GT_RENDER_MODEL_D3D11_H__
#define __GT_RENDER_MODEL_D3D11_H__

/*
*/

namespace gost{
	
		//	hardware модель
		//	реализуется в плагинах.
		//	создаёт hardware буферы
	class gtRenderModelD3D11 : public gtRenderModel{

		/*gtModel	*	m_sModel;*/

		gtDriverD3D11* m_driver;

	public:

		gtRenderModelD3D11( gtDriverD3D11* );
		~gtRenderModelD3D11( void );
		

		bool	init( gtModel* );


		//gtModel*	getModel( void );

		u32 getSubModelCount( void );

		gtMaterial*	getMaterial( u32 id );

			//	нарисует gtRenderModel
		void		render( void );
		
		
		gtArray<ID3D11Buffer*> m_vBuffers;
		gtArray<ID3D11Buffer*> m_iBuffers;

		struct gtRenderModelD3D11_sub{
			u32 iCount, vCount;
			gtMaterial material;
		};
		gtArray<gtRenderModelD3D11_sub> m_subs;

		u32 m_stride;
	};

}

#endif

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