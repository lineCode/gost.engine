//	GOST

#pragma once
#ifndef __GT_RENDER_MODEL_D3D11_H__
#define __GT_RENDER_MODEL_D3D11_H__


namespace gost{
	
	class gtRenderModelD3D11 : public gtRenderModel{


		gtDriverD3D11*  m_gs;
		ID3D11Buffer*   m_lockedResource;

		gtAabb m_aabb;
		gtObb  m_obb;

		gtRenderModelInfo m_info;
		
	public:

		gtRenderModelD3D11( gtDriverD3D11* );
		~gtRenderModelD3D11();
		

		bool	init( gtModel*, gtRenderModelInfo * );
		u32		getSubModelCount();

		gtMaterial*	getMaterial( u32 id );
		void		render();
		
		
		gtArray<ID3D11Buffer*> m_vBuffers;
		gtArray<ID3D11Buffer*> m_iBuffers;

		struct gtRenderModelD3D11_sub{
			u32 iCount, vCount;
			gtMaterial material;
		};
		gtArray<gtRenderModelD3D11_sub> m_subs;

		u32 m_stride;

		gtAabb*				getAabb();
		gtObb*				getObb();
		
		bool lock( u32 id, void * ptr, lock_type type );
		void unlock();
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