#define GT_EXPORTS

#include "common.h"
#include <memory>
#include <cctype>

#include "gost.h"
using namespace gost;

#ifdef _DEBUG
#pragma comment(lib, "gost_d.lib")
#else 
#pragma comment(lib, "gost.lib")
#endif

#define GT_EXT_COUNT 1u

extern "C"{
	
	GT_API void	GetPluginInfo( gtPluginInfo& info ){
		info.m_author.assign( u"532235" );
		info.m_description.assign( u"Import .obj files" );
		info.m_GUID=GT_UID_IMPORT_MODEL_OBJ;
		info.m_name.assign( u"OBJ file importer" );
		info.m_type = gtPluginType::import_model;
		info.m_version = 1;
		info.m_build = 31;
	}

	GT_API u32	PluginGetExtCount(){
		return GT_EXT_COUNT;
	}


	GT_API s8*	PluginGetExtension( u32 id ){
		GT_ASSERT1( id < GT_EXT_COUNT, "Bad argument", "id < 1"  );

		s8 * exts[ GT_EXT_COUNT ] = {
			"obj"
		};

		return exts[ id ];
	}



	struct face{
		/*face - треугольник, по этому v3i*/
		v3i m_pos;
		v3i m_nor;
		v3i m_tcoord;
	};

	struct VERTEX{
		v4f position;
		v2f texcoord;
		v3f normal;
	};

	struct CacheEntry{
		u32 index;
		CacheEntry* pNext;
	};

	u8 * nextLine( u8 * ptr );
	u8 * readVec3( u8 * ptr, v3f& vec3 );
	u8 * readVec2( u8 * ptr, v2f& vec2 );
	u8 * readWord( u8 * ptr, gtStringA& str );
	u8 * readFace( u8 * ptr, face& f );

	u8 * printComment( u8 * ptr ){
		gtString str;
		while( *ptr ){
			if( *ptr == '\n' ){
				ptr++;
				if( str.size() )
					gtLogWriter::printInfo( u"%s", str.data() );

				return ptr;
			}
			str += static_cast<char16_t>(*ptr);
			ptr++;
		}

		return ptr;
	}

	GT_API gtModel * PluginLoadModel( gtString* fileName ){


		if( !gtFileSystem::existFile( *fileName ) ){
			gtLogWriter::printWarning( u"OBJ Plugin: File not exist [%s]", fileName->data() );
			return nullptr;
		}

		gtLogWriter::printInfo( u"Loading model: %s", fileName->data() );

		u32 fileSz = 0u;
		std::unique_ptr<u8[]> data;
		{
			gtFile_t file = util::openFileForReadBin( *fileName );
			fileSz = file->size();
			data.reset( new u8[ fileSz+1u ] );
			file->read( data.get(), fileSz );
		}


		gtArray<v3f> position;
		gtArray<v2f> uv;
		gtArray<v3f> normal;

		u8 * ptr = data.get();
		ptr[fileSz] = 0;

		v2f tcoords;
		v3f pos, norm;

		gtStringA word;

		/*символ g может встретится как перед вершинами, так и после, рядом с группой f*/
		/*если группа одна, то может быть что g вовсе нет*/
		/*когда встречается f, наступает время создания суб модели.*/
		/*нужно уметь разделять, когда f идёт для одной группы, когда для следующей*/
		bool grpFound = false;

		gtVertexType vt[ 4 ] = {
			gtVertexType::position,
			gtVertexType::uv,
			gtVertexType::normal,
			gtVertexType::end
		};

		auto model = gtMainSystem::getInstance()->getModelSystem()->createEmpty( gtStrideStandart, &vt[ 0u ] );

		/*для удобства*/
		struct vert_t{
			v4f pos;
			v2f uv;
			v3f nor;
		};


		gtSubModel * sub = new gtSubModel;
		sub->m_stride = gtStrideStandart;
		
		/* сюда будут накапливаться вершины */
		gtArray<vert_t> buf_verts;
		gtArray<u16> buf_inds;
		
		u32 indexOld = 0;	/*если новая группа, новая суб модель, то нужно начинать отсчёт индексов сначала*/
							/*она учитывает старый индекс, и в новой субмодели просто вычитается */

		
		/*это всё для того чтобы правильно получить индексы и т.д.*/
		gtArray<CacheEntry*>	m_VertexCache;
		gtArray<CacheEntry*>	m_VertexCache_trash;
		gtArray<VERTEX> m_Vertices;      // Filled and copied to the vertex buffer
		gtArray<u32>	m_Indices;       // Filled and copied to the index buffer
		u32 index = 0u;

		bool groupBegin = false;

		while( *ptr ){
			switch( *ptr ){

			case '#':
				ptr = printComment( ptr );
				break;

			case 's':
			case 'l':
			case 'u'://usemtl
			case 'c'://curv
			case 'm'://mtllib
			case 'p'://parm
			case 'd'://deg
			case 'e'://end
				ptr = nextLine( ptr );
				break;

			case 'v':{
				++ptr;

				if( groupBegin )
					groupBegin = false;

				switch( *ptr ){
				
				case 't':
					ptr = readVec2( ++ptr, tcoords );
					uv.push_back( tcoords );
					break;
				
				case 'n':
					ptr = readVec3( ++ptr, norm );
					normal.push_back( norm );
					break;
				
				default:
					ptr = readVec3( ptr, pos );
					position.push_back( pos );
					break;
				}
			}break;

			case 'f':{
				
				face f;
				ptr = readFace( ++ptr, f );

				VERTEX vertex;
				u32 iPosition, iTexCoord, iNormal;

				for( u32 iFace = 0; iFace < 3u; iFace++ ){
					ZeroMemory( &vertex, sizeof( VERTEX ) );


					iPosition = f.m_pos.getComponent( iFace  );
					iTexCoord = f.m_tcoord.getComponent( iFace  );
					iNormal   = f.m_nor.getComponent( iFace );


					vertex.position.set( position[ iPosition - 1 ] );
					
					if( uv.size() )
						vertex.texcoord = uv[ iTexCoord - 1 ];

					if( normal.size() )
						vertex.normal	= normal[ iNormal - 1 ];
					else{
						auto * pv = &position[ iPosition - 1];
						vertex.normal = math::cross((pv->y-pv->x),(pv->z-pv->x));
					}

					// If a duplicate vertex doesn't exist, add this vertex to the Vertices
					// list. Store the index in the Indices array. The Vertices and Indices
				    // lists will eventually become the Vertex Buffer and Index Buffer for
				    // the mesh.

					// If this vertex doesn't already exist in the Vertices list, create a new entry.
					// Add the index of the vertex to the Indices list.
					bool bFoundInList = false;

					u32 hash = iPosition;
					VERTEX* pVertex = &vertex;

					// Since it's very slow to check every element in the vertex list, a hashtable stores
					// vertex indices according to the vertex position's index as reported by the OBJ file
					if( ( u32 )m_VertexCache.size() > hash ){
						CacheEntry* pEntry = m_VertexCache.at( hash );
						while( pEntry != NULL ){
							VERTEX* pCacheVertex = m_Vertices.data() + pEntry->index;

							// If this vertex is identical to the vertex already in the list, simply
							// point the index buffer to the existing vertex
							if( 0 == memcmp( pVertex, pCacheVertex, sizeof( VERTEX ) ) ){
								bFoundInList = true;
								index = pEntry->index;
								break;
							}

							pEntry = pEntry->pNext;
						}
					}

					// Vertex was not found in the list. Create a new entry, both within the Vertices list
					// and also within the hashtable cache
					if( !bFoundInList ){
						// Add to the Vertices list
						index = m_Vertices.size();
						m_Vertices.push_back( *pVertex );

						vert_t vrtx;
						vrtx.pos = pVertex->position;
						vrtx.nor = pVertex->normal;
						vrtx.uv  = pVertex->texcoord;

						buf_verts.push_back( vrtx );

						// Add this to the hashtable
						CacheEntry* pNewEntry = new CacheEntry;
						if( pNewEntry == NULL )
							return nullptr;

						m_VertexCache_trash.push_back( pNewEntry );

						pNewEntry->index = index;
						pNewEntry->pNext = NULL;
						
						
						// Grow the cache if needed
						while( ( u32 )m_VertexCache.size() <= hash ){
							m_VertexCache.push_back( NULL );
						}

						// Add to the end of the linked list
						CacheEntry* pCurEntry = m_VertexCache.at( hash );
						if( pCurEntry == NULL ){
							// This is the head element
							m_VertexCache.at( hash ) = pNewEntry;
						}
						else{
							// Find the tail
							while( pCurEntry->pNext != NULL ){
								pCurEntry = pCurEntry->pNext;
							}

							pCurEntry->pNext = pNewEntry;
						}
					}

					if( index == (DWORD)-1 ){
						auto sz = m_VertexCache_trash.size();
						for( u32 vi = 0; vi < sz; ++vi ){
							delete m_VertexCache_trash[vi];
						}
						return nullptr;
					}

					m_Indices.push_back( index  );
					buf_inds.push_back( (u16)(index - indexOld));
				}

			}break;

			case 'o':
			case 'g':
				
				if( !groupBegin ){
					groupBegin = true;
				}else{
					++ptr;
					break;
				}

				ptr = readWord( ++ptr, word );
				//if( !word.size() )break;
			
				if( grpFound ){

					sub->m_iCount = buf_inds.size();
					sub->m_vCount = buf_verts.size();
					sub->allocate( sub->m_stride );
					sub->m_name = word;
					memcpy( sub->m_vertices, buf_verts.data(), sub->m_stride * sub->m_vCount );
					memcpy( sub->m_indices, buf_inds.data(), sizeof(u16) * sub->m_iCount );
					model->addSubModel( sub );

					delete sub;
					buf_inds.clear();
					buf_verts.clear();
					sub = new gtSubModel;
					sub->m_stride = gtStrideStandart;

					indexOld = m_Vertices.size();
				}
				grpFound = true;
				break;

			default:
				++ptr;
				break;
			}
		}

		sub->m_iCount = buf_inds.size();
		sub->m_vCount = buf_verts.size();
		sub->allocate( sub->m_stride );
		sub->m_name = word;
		memcpy( sub->m_vertices, buf_verts.data(), sub->m_stride * sub->m_vCount );
		memcpy( sub->m_indices, buf_inds.data(), sizeof(u16) * sub->m_iCount );
		model->addSubModel( sub );

		delete sub;
		

		auto sz = m_VertexCache_trash.size();
		for( u32 vi = 0; vi < sz; ++vi ){
			delete m_VertexCache_trash[vi];
		}
		
		model->updateBoundingVolume();

		model->addRef();
		return model.data();
	}

	u8 * nextLine( u8 * ptr ){
		while( *ptr ){
			if( *ptr == '\n' ){
				ptr++;
				return ptr;
			}
			ptr++;
		}

		return ptr;
	}

		/*пропуск пробела и т.д.*/
	u8 * skipSpaces( u8 * ptr ){

		while( *ptr ){
			if( !isspace(*ptr) ) break;
			ptr++;
		}

		return ptr;
	}

	u8 * readWord( u8 * ptr, gtStringA& str ){
		
		ptr = skipSpaces( ptr );

		str.clear();

		while( *ptr ){
			if( isspace(*ptr) ) break;

			str += (char)*ptr;

			ptr++;

		}

		return ptr;
	}

	u8 * readFloat( u8 * ptr, f32& value ){
		s8 str[ 32u ];
		memset( str, 0, 32 );

		s8 * p = &str[ 0u ];
		while( *ptr ){
			if( !isdigit(*ptr) && (*ptr != '-') && (*ptr != '.')  ) break;

			*p = *ptr;

			p++;
			ptr++;
		}

		value = (f32)atof( str );

		return ptr;
	}

	u8 * readVec3( u8 * ptr, v3f& vec3 ){
		
		ptr = skipSpaces( ptr );

		f32 x, y, z;

		if( *ptr == '\n' ){
			ptr++;
		}else{
			ptr = readFloat( ptr, x );
			ptr = skipSpaces( ptr );
			ptr = readFloat( ptr, y );
			ptr = skipSpaces( ptr );
			ptr = readFloat( ptr, z );
			ptr = nextLine( ptr );
			
			vec3.x = x;
			vec3.y = y;
			vec3.z = z;
		}

		return ptr;
	}

	u8 * readVec2( u8 * ptr, v2f& vec2 ){
		
		ptr = skipSpaces( ptr );

		f32 x, y;

		if( *ptr == '\n' ){
			ptr++;
		}else{
			ptr = readFloat( ptr, x );
			ptr = skipSpaces( ptr );
			ptr = readFloat( ptr, y );
			ptr = nextLine( ptr );

			vec2.x = x;
			vec2.y = 1.f-y;

		}

		return ptr;
	}

	u8 * getIds( u8 * ptr, gtStringA& str ){
		while( *ptr ){
			if( std::isdigit( *ptr ) || (*ptr == '/') ){
				str += (char)*ptr;
				++ptr;
			}else break;

		}
		return ptr;
	}

	bool isQuad( u8 * ptr ){
		s8 n = 0;
		if( *ptr != '\n' ){

				gtStringA str;

				ptr = getIds( ptr, str );

				++n;
				++ptr;
		}

		return n > 3;
	}

	u8 * getInt( u8 * p, int& i ){
		s8 str[ 8u ];
		memset( str, 0, 8 );

		s8 * pi = &str[ 0u ];

		while( *p ){

			if( !std::isdigit( *p ) ) break;

			*pi = *p;

			pi++;
			p++;
		}

		i = atoi( str );

		p++;

		return p;
	}

	void fillFace( gtStringA& ids1, gtStringA& ids2, gtStringA& ids3, face& f ){

		u8 * p1 = (u8*)ids1.data();
		u8 * p2 = (u8*)ids2.data();
		u8 * p3 = (u8*)ids3.data();

		s32 p, n, t;

		u32 id1 = 0u; /*проба изменить порядок обхода вершин*/
		u32 id2 = 2u; /*сделать по часовой стрелке*/
		u32 id3 = 1u; /*оригинал 0 1 2*/

		p1 = getInt( p1, p );
		p1 = getInt( p1, t );
		p1 = getInt( p1, n );

		f.m_pos[ id1 ] = p;
		f.m_tcoord[ id1 ] = t;
		f.m_nor[ id1 ] = n;

		p2 = getInt( p2, p );
		p2 = getInt( p2, t );
		p2 = getInt( p2, n );

		f.m_pos[ id2 ] = p;
		f.m_tcoord[ id2 ] = t;
		f.m_nor[ id2 ] = n;

		p3 = getInt( p3, p );
		p3 = getInt( p3, t );
		p3 = getInt( p3, n );

		f.m_pos[ id3 ] = p;
		f.m_tcoord[ id3 ] = t;
		f.m_nor[ id3 ] = n;

	}

	u8 * readFace( u8 * ptr, face& f ){
		
		ptr = skipSpaces( ptr );
		
		if( *ptr == '\n' ){
			ptr++;
		}else{

			gtStringA ids1, ids2, ids3;

			ptr = getIds( ptr, ids1 );
			ptr = skipSpaces( ptr );

			ptr = getIds( ptr, ids2 );
			ptr = skipSpaces( ptr );

			ptr = getIds( ptr, ids3 );
			ptr = skipSpaces( ptr );

			fillFace( ids1, ids2, ids3, f );

		}

		return ptr;
	}
}

/*
Copyright (c) 2018 532235

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