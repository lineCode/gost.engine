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
			m_vertexPosition( 0 ),
			m_uvPosition( 4 ),
			m_normalPosition( 6 ) // в стандартном stride, pos[0,1,2,3] uv[4,5] normal[6,7,8]
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

			// aabb
		gtAabb m_aabb;
		gtObb  m_obb;
		
			/// размер вершины в байтах
		u32 m_stride;

			/// Индекс первой координаты позиции в вершине
			///	Если, подразумевается что позиция в вершине стоит не на первом месте, то нужно изменить это значение
			///	Index of the first position coordinate in the vertex.
		s32 m_vertexPosition;

		s32 m_uvPosition;

			/// Индекс первой координаты нормали в вершине
			/// Аналогия как и с m_vertexPosition
		s32 m_normalPosition;


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

			//	Построит obb. Изначально выглядит как aabb, но повернётся вместе с объектом.
		void	buildObb( void ){
			m_obb.reset();

			u8 * p8 = &m_vertices[0u];
			for(u32 i = 0u; i < m_vCount; ++i){
				f32 * p32 = reinterpret_cast<f32*>(p8);

				m_obb.add( v3f( p32[ m_vertexPosition ], p32[ m_vertexPosition + 1 ], p32[ m_vertexPosition + 2 ] ) );
				p8 += m_stride;
			}

		}

			//	Обновит aabb. Должна делаться из obb
		void	updateAabb( void ){
			m_aabb.reset();

			u8 * p8 = &m_vertices[0u];
			for(u32 i = 0u; i < m_vCount; ++i){
				f32 * p32 = reinterpret_cast<f32*>(p8);

				m_aabb.add( v3f( p32[ m_vertexPosition ], p32[ m_vertexPosition + 1 ], p32[ m_vertexPosition + 2 ] ) );
				p8 += m_stride;
			}
		}

		void	append( gtSubModel * model ){
			GT_ASSERT3(model);
			if( model ){

				u16 maxIndex = 0u;
				for( u32 i = 0; i < m_iCount; ++i ){
					if( m_indices[ i ] > maxIndex ) maxIndex = m_indices[ i ];
				}

			////	maxIndex--;

				u32 newIndexCount = m_iCount + model->m_iCount;
				u16 * newInds = new u16[ newIndexCount ];

				memcpy( newInds, m_indices, m_iCount * sizeof( u16 ) );
				for( u32 i = 0; i < model->m_iCount; ++i ){
					newInds[ m_iCount + i ] = model->m_indices[ i ] + maxIndex + 1u; /// < возможно ошибка
				}

				delete []m_indices;
				m_indices = newInds;
				m_iCount = newIndexCount;

				u32 newVertsCount = m_vCount + model->m_vCount;
				u8 * newVerts = new u8[ m_stride * newVertsCount ];
				memcpy( newVerts, m_vertices, m_stride * m_vCount );

				
				u8 * p8 = &newVerts[m_vCount*m_stride];

				u8 * otherp8 = &model->m_vertices[0u];
				for( u32 i = 0; i < model->m_vCount; ++i ){
					f32 * p32 = reinterpret_cast<f32*>(p8);
					f32 * otherp32 = reinterpret_cast<f32*>(otherp8);

					p32[ m_vertexPosition	] = otherp32[ model->m_vertexPosition	];
					p32[ m_vertexPosition+1 ] = otherp32[ model->m_vertexPosition+1 ];
					p32[ m_vertexPosition+2 ] = otherp32[ model->m_vertexPosition+2 ];
					p32[ m_uvPosition		] = otherp32[ model->m_uvPosition		];
					p32[ m_uvPosition+1		] = otherp32[ model->m_uvPosition+1     ];
					p32[ m_normalPosition	] = otherp32[ model->m_normalPosition   ];
					p32[ m_normalPosition+1 ] = otherp32[ model->m_normalPosition+1 ];
					p32[ m_normalPosition+2 ] = otherp32[ model->m_normalPosition+2 ];


					p8 += m_stride;
					otherp8 += model->m_stride;
				}

				delete []m_vertices;
				m_vertices = newVerts;
				m_vCount = newVertsCount;

			}
		}

			///	Rotate mesh
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

				v3f vector( p32[ i1 ], p32[ i2 ], p32[ i3 ] );

				vector = math::mul( vector, m );

				p32[ i1 ] = vector.x;
				p32[ i2 ] = vector.y;
				p32[ i3 ] = vector.z;

				p8 += m_stride;
			}

			calculate_normals();
		}

		void	move( const v3f& newPos ){
			u32 i1 = m_vertexPosition;
			u32 i2 = m_vertexPosition + 1u;
			u32 i3 = m_vertexPosition + 2u;

			u8 * p8 = &m_vertices[0u];
			for(u32 i = 0u; i < m_vCount; ++i){
				f32 * p32 = reinterpret_cast<f32*>(p8);

				p32[ i1 ] += newPos.x;
				p32[ i2 ] += newPos.y;
				p32[ i3 ] += newPos.z;

				p8 += m_stride;
			}
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

				v3f vector_p1( p32[ i1 ], p32[ i2 ], p32[ i3 ] );
				v3f vector_p2( p32[ i4 ], p32[ i5 ], p32[ i6 ] );
				v3f vector_p3( p32[ i7 ], p32[ i8 ], p32[ i9 ] );

				v3f vector_U( vector_p2 - vector_p1 );
				v3f vector_V( vector_p3 - vector_p1 );


				v3f normal;

				normal.x = (vector_U.y * vector_V.z) - (vector_U.z * vector_V.y);
				normal.y = (vector_U.z * vector_V.x) - (vector_U.x * vector_V.z);
				normal.z = (vector_U.x * vector_V.y) - (vector_U.y * vector_V.x);
				
				normal.normalize();

				p32[ in1 ] = normal.x;
				p32[ in2 ] = normal.y;
				p32[ in3 ] = normal.z;

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