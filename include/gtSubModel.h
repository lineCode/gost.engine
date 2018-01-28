/*!	GOST
	\file gtSubModel.h
	\brief software model
*/
#pragma once
#ifndef __GT_MESH_BUFFER_H__
#define __GT_MESH_BUFFER_H__ ///< include guard

namespace gost{
	
		///	То, что идёт конкретно на рендеринг.
	struct gtSubModel{

			/// c-tor
		gtSubModel( void ):
			m_vertices( nullptr ),
			m_indices( nullptr ),
			m_vCount( 0u ),
			m_iCount( 0u ),
			m_stride( 0u ),
			m_vertexPosition( 0u ),
			m_normalPosition( 6u ) // в стандартном stride, pos[0,1,2,3] uv[4,5] normal[6,7,8]
		{}

			/// d-tor
		~gtSubModel( void ){
			if( m_vertices )
				delete []m_vertices;
			if( m_indices )
				delete []m_indices;
		}

			///	вершины
		u8	*			m_vertices;

			///	индексы
		u16	*			m_indices;

			///	количество вершин
		u32 m_vCount;

			///	количество индексов
		u32 m_iCount;

			///	свой материал
		gtMaterial m_material;

			/// имя
		gtStringA m_name;

			/// aabb
		gtAabb m_box;
		
			/// размер вершины в байтах
		u32 m_stride;

			/// Индекс первой координаты позиции в вершине
			///	Если, подразумевается что позиция в вершине стоит не на первом месте, то нужно изменить это значение
		u32 m_vertexPosition;

			/// Индекс первой координаты нормали в вершине
			/// Аналогия как и с m_vertexPosition
		u32 m_normalPosition;

			/// заполняет индексный массив индексами
			/// \param array: index array
		void fillIndices( const u16* array ){
			for(u32 i = 0u; i < m_iCount; ++i){
				m_indices[ i ] = array[ i ];
			}
		}

			/// выделяет память
			/// \param stride: размер вершины в байтах
		void	allocate( u32 stride ){
			m_stride = stride;
			if( !m_vertices )
				m_vertices = new u8[ stride * m_vCount ];
			if( !m_indices )
				m_indices  = new u16[ m_iCount ];
		}

			///	обновит aabb
		void	updateAabb( void ){
			m_box.reset();

			u8 * p8 = &m_vertices[0u];
			for(u32 i = 0u; i < m_vCount; ++i){
				f32 * p32 = reinterpret_cast<f32*>(p8);

				m_box.add( v3f( IL_BEGIN p32[ m_vertexPosition ], p32[ m_vertexPosition + 1u ], p32[ m_vertexPosition + 2u ] IL_END ) );
				p8 += m_stride;
			}
		}

		void	rotate( gtQuaternion& new_orientation ){
			new_orientation.normalize();
			gtMatrix4 m;
			math::makeRotationMatrix( m, new_orientation );

			u32 i1 = m_vertexPosition;
			u32 i2 = m_vertexPosition + 1u;
			u32 i3 = m_vertexPosition + 2u;

			u8 * p8 = &m_vertices[0u];
			for(u32 i = 0u; i < m_vCount; ++i){
				f32 * p32 = reinterpret_cast<f32*>(p8);

				v3f vector(IL_BEGIN p32[ i1 ], p32[ i2 ], p32[ i3 ] IL_END);

				vector = math::mul( vector, m );

				p32[ i1 ] = vector[ 0u ];
				p32[ i2 ] = vector[ 1u ];
				p32[ i3 ] = vector[ 2u ];

				p8 += m_stride;
			}

			calculate_normals();
		}

			///TEST
		void	calculate_normals( void ){
			u8 * p8 = &m_vertices[0u];

			u32 i1 = m_vertexPosition;
			u32 i2 = m_vertexPosition + 1u;
			u32 i3 = m_vertexPosition + 2u;
			u32 i4 = m_vertexPosition + 3u;
			u32 i5 = m_vertexPosition + 4u;
			u32 i6 = m_vertexPosition + 5u;
			u32 i7 = m_vertexPosition + 6u;
			u32 i8 = m_vertexPosition + 7u;
			u32 i9 = m_vertexPosition + 8u;

			u32 in1 = m_normalPosition;
			u32 in2 = m_normalPosition + 1u;
			u32 in3 = m_normalPosition + 2u;

			for(u32 i = 0u; i < m_vCount; ++i){
				f32 * p32 = reinterpret_cast<f32*>(p8);

				v3f vector_p1(IL_BEGIN p32[ i1 ], p32[ i2 ], p32[ i3 ] IL_END);
				v3f vector_p2(IL_BEGIN p32[ i4 ], p32[ i5 ], p32[ i6 ] IL_END);
				v3f vector_p3(IL_BEGIN p32[ i7 ], p32[ i8 ], p32[ i9 ] IL_END);

				v3f vector_U(IL_BEGIN vector_p2 - vector_p1 IL_END);
				v3f vector_V(IL_BEGIN vector_p3 - vector_p1 IL_END);


				v3f normal;

				normal[ 0u ] = (vector_U[ 1u ] * vector_V[ 2u ]) - (vector_U[ 2u ] * vector_V[ 1u ]);
				normal[ 1u ] = (vector_U[ 2u ] * vector_V[ 0u ]) - (vector_U[ 0u ] * vector_V[ 2u ]);
				normal[ 2u ] = (vector_U[ 0u ] * vector_V[ 1u ]) - (vector_U[ 1u ] * vector_V[ 0u ]);
				
				normal.normalize();

				p32[ in1 ] = normal[ 0u ];
				p32[ in2 ] = normal[ 1u ];
				p32[ in3 ] = normal[ 2u ];

				p8 += m_stride;
			}
		}
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