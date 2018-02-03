/*!	GOST
	\file gtModelSystem.h
	\brief model system
*/
#pragma once
#ifndef __GT_MODEL_SYSTEM_H__
#define __GT_MODEL_SYSTEM_H__ ///< include guard

namespace gost{

		/// выполняет функции связанные с моделями
	class gtModelSystem : public gtRefObject{
	public:
			
			/// создаёт пустую, выделяя память для gtVertexType
			/// \param stride: размер вершины в байтах
			/// \param type: массив gtVertexType
			/// \return пустая модель
		virtual gtModel*	createEmpty( u32 stride, gtVertexType* type ) = 0;

			/// создаёт плоскость
			/// \param x: ширина
			/// \param y: высота
			/// \return плоскость
		virtual gtModel*	createPlane( f32 x = 1.f, f32 y = 1.f, gtSide side = gtSide::DOWN ) = 0;

			/// загружает модель из файла
			/// \param fileName: путь к файлу
			/// \return модель
		virtual gtModel*	createFromFile( const gtString& fileName ) = 0;

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