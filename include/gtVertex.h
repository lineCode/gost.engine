/*!	GOST
	\file gtVertex.h
	\brief gtVertex

*/

#pragma once
#ifndef __GT_VERTEX_H__
#define __GT_VERTEX_H__ ///< include guard

namespace gost{



	/**
		\brief Stride - количество байт на 1 вершину.

		минимум
		4 координаты позиции
		3 нормали
		2 текстурных координат
		итого sizeof( gtReal ) * 9u;
	*/
	constexpr u32 gtStrideStandart = sizeof( gtReal ) * 9u;

		/// вроде не используется. надо проверить и удалить.
	struct gtVertex{

			//	данные
		u8	* data;

		gtVertex( void ):
			data( nullptr )
		{}

		~gtVertex( void ){
			if( data ) delete []data;
		}


			//	получить данные в удобном для работы виде
		/*gtReal	*	getFPData( void ){
			return reinterpret_cast<gtReal*>(data);
		}*/

		gtVertex*	allocate( u32 stride ){
			data = new u8[ stride ];
			return this;
		}
	};

	/**
		В структуре с моделью рядом с буфером вершин должно стоять это.
		Он описывает в каком порядке что стоит в вершине.
	*/
	enum class gtVertexType{
		position,	///< 4 * sizeof( gtReal )
		normal,		///< 3 * sizeof( gtReal )
		uv,			///< 2 * sizeof( gtReal )
		color,		///< 4 * sizeof( gtReal )
		binormal,	///< 3 * sizeof( gtReal )
		tangent,	///< 3 * sizeof( gtReal )
		end			///< типа завершающий ноль. должен стоять в конце.
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