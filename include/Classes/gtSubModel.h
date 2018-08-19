#pragma once
#ifndef __GT_MESH_BUFFER_H__
#define __GT_MESH_BUFFER_H__

namespace gost{

		// Структура описывает стандартную вершину
	struct gtStandartVertex{
		v4f pos;
		v2f uv; 
		v3f nor;
	};

		// Тут уже с цветом
	struct gtStandartColorVertex{
		v4f pos;
		v2f uv; 
		v3f nor;
		gtColor col;
	};

		// Мешбуфер. gtModel может хранить множество мешбуферов.
	struct gtSubModel{

			// c-tor
		gtSubModel():
			m_vertices( nullptr ),
			m_indices( nullptr ),
			m_vCount( 0u ),
			m_iCount( 0u ),
			m_stride( 0u ),
			
			m_vertexPosition( 0 ),
			m_uvPosition( 4 ),
			m_normalPosition( 6 ) // pos[0,1,2,3] uv[4,5] normal[6,7,8]
		{}

			// d-tor
		~gtSubModel(){
			if( m_vertices )
				delete []m_vertices;
			if( m_indices )
				delete []m_indices;
		}

		u8	*			m_vertices;

		u16	*			m_indices;

		u32 m_vCount;

		u32 m_iCount;

		gtMaterial m_material;

		gtStringA m_name;

		gtAabb m_aabb;
		gtObb  m_obb;
		
		u32 m_stride;

			//	Index of the first position coordinate in the vertex.
			// формат вершины заранее неизвестен
		s32 m_vertexPosition;
		s32 m_uvPosition;
		s32 m_normalPosition;


			// \param array: index array
		void fillIndices( const u16* array ){
			for(u32 i = 0u; i < m_iCount; ++i){
				m_indices[ i ] = array[ i ];
			}
		}

		void	allocate( u32 stride ){
			m_stride = stride;
			if( !m_vertices )
				m_vertices = new u8[ stride * m_vCount ];
			if( !m_indices )
				m_indices  = new u16[ m_iCount ];
		}

		void	buildObb(){
			m_obb.reset();

			u8 * p8 = &m_vertices[0u];
			for(u32 i = 0u; i < m_vCount; ++i){
				f32 * p32 = reinterpret_cast<f32*>(p8);

				m_obb.add( v3f( p32[ m_vertexPosition ], p32[ m_vertexPosition + 1 ], p32[ m_vertexPosition + 2 ] ) );
				p8 += m_stride;
			}

		}

		void	updateAabb(){
			m_aabb.reset();

			m_aabb.add( m_obb.v1 );
			m_aabb.add( m_obb.v2 );
			m_aabb.add( m_obb.v3 );
			m_aabb.add( m_obb.v4 );
			m_aabb.add( m_obb.v5 );
			m_aabb.add( m_obb.v6 );
			m_aabb.add( m_obb.v7 );
			m_aabb.add( m_obb.v8 );
		}

		void	append( gtSubModel * model ){
			GT_ASSERT3(model);
			if( model ){

				u16 maxIndex = 0u;
				for( u32 i = 0; i < m_iCount; ++i ){
					if( m_indices[ i ] > maxIndex ) maxIndex = m_indices[ i ];
				}

			///	maxIndex--;

				u32 newIndexCount = m_iCount + model->m_iCount;
				u16 * newInds = new u16[ newIndexCount ];

				memcpy( newInds, m_indices, m_iCount * sizeof( u16 ) );
				for( u32 i = 0; i < model->m_iCount; ++i ){
					newInds[ m_iCount + i ] = model->m_indices[ i ] + maxIndex + 1u;
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

			//	Rotate mesh
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

		void	calculate_normals(){
			
			for(u32 i = 0u; i < m_iCount; ){
				
				u16 i0 = m_indices[  i  ];
				u16 i1 = m_indices[ i+1 ];
				u16 i2 = m_indices[ i+2 ];
				
				gtLogWriter::printInfo( u"%u %u %u", i0, i1, i2 );
				
				v3f v1, v2, v3;
				
				u8 * p8 = &m_vertices[0u];
				
				f32 * p32_v1 = reinterpret_cast<f32*>(p8 + (i0*m_stride));
				f32 * p32_v2 = reinterpret_cast<f32*>(p8 + (i1*m_stride));
				f32 * p32_v3 = reinterpret_cast<f32*>(p8 + (i2*m_stride));
				
				f32 * p32_n1 = p32_v1;
				f32 * p32_n2 = p32_v2;
				f32 * p32_n3 = p32_v3;
				
				p32_v1 += m_vertexPosition;
				p32_v2 += m_vertexPosition;
				p32_v3 += m_vertexPosition;
				
				p32_n1 += m_normalPosition;
				p32_n2 += m_normalPosition;
				p32_n3 += m_normalPosition;
				
				v1.x = *p32_v1; ++p32_v1;
				v1.y = *p32_v1; ++p32_v1;
				v1.z = *p32_v1;
								
				v2.x = *p32_v2; ++p32_v2;
				v2.y = *p32_v2; ++p32_v2;
				v2.z = *p32_v2;
								
				v3.x = *p32_v3; ++p32_v3;
				v3.y = *p32_v3; ++p32_v3;
				v3.z = *p32_v3;
				
				v3f e0 = v2 - v1;
				v3f e1 = v3 - v1;
				v3f fn = math::cross(e0,e1);
				
				*p32_n1 += fn.x; ++p32_n1;
				*p32_n1 += fn.y; ++p32_n1;
				*p32_n1 += fn.z;
				
				*p32_n2 += fn.x; ++p32_n2;
				*p32_n2 += fn.y; ++p32_n2;
				*p32_n2 += fn.z;
				
				*p32_n3 += fn.x; ++p32_n3;
				*p32_n3 += fn.y; ++p32_n3;
				*p32_n3 += fn.z;
				
				i += 3u;
			}
			
			for(u32 i = 0u, i2 = 0u; i < m_vCount; ){
				u8 * p8 = &m_vertices[i2];
				f32 * p32 = reinterpret_cast<f32*>(p8);
				
				p32 += m_normalPosition;
				
				v3f * v = reinterpret_cast<v3f*>(p32);
				
				v->normalize();
				
				i2 += m_stride;
				++i;
			}
		}
		
		
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