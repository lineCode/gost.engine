#pragma once
#ifndef __GT_MODEL_H__
#define __GT_MODEL_H__

namespace gost{
	
	class gtModel : public gtRefObject{
	public:

		virtual gtSubModel*		addSubModel( u32 v_count, u32 i_count, u32 stride ) = 0;
		virtual gtSubModel*		addSubModel( gtSubModel* subModel ) = 0;
		virtual const gtAabb&	getAabb() = 0;
		virtual const gtObb&	getObb() = 0;
		virtual u32				getStride() = 0;
		virtual gtSubModel*		getSubModel( u32 id ) = 0;
		virtual u32				getSubmodelsCount() = 0;
		virtual gtVertexType*	getTypeArray() = 0;
		virtual void			setAabb( const gtAabb& aabb ) = 0;
		virtual void			setObb( const gtObb& obb ) = 0;
		virtual void			updateBoundingVolume() = 0;
	};

}

#endif

/*
Copyright (c) 2017, 2018 532235

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