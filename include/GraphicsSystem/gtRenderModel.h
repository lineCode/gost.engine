﻿#pragma once
#ifndef __GT_RENDER_MODEL_H__
#define __GT_RENDER_MODEL_H__

namespace gost{
	
	struct gtRenderModelInfo{
		bool m_dynamic = false;
	};
	
	class gtRenderModel : public gtRefObject{
	public:

			// \return sub models count
		virtual u32 getSubModelCount() = 0;

			// \return material
		virtual gtMaterial*	getMaterial( u32 id ) = 0;

		virtual void render() = 0;

		virtual gtAabb*				getAabb() = 0;
		virtual gtObb*				getObb() = 0;
		
		enum class lock_type{
			index,
			vertex
		};
		
			// Вернёт hardware буфер для редактирования если gtRenderModel создан как dynamic (см. gtRenderModelInfo)
			// id - номер мешбуфера (получить количество getSubModelCount())
		virtual bool lock( u32 id, void ** ptr, lock_type type ) = 0;
		virtual void unlock() = 0;
		
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