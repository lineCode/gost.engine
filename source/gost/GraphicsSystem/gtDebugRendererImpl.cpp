#include "common.h"

gtDebugRendererImpl::gtDebugRendererImpl():
	m_gs( nullptr )
{}

gtDebugRendererImpl::~gtDebugRendererImpl()
{}
		
void gtDebugRendererImpl::draw( gtGameObject* object, u32 flags ){
	const auto& pos = object->getPositionInSpace();
			
	gtColor red( 1.f, 0.f, 0.f );
	gtColor green( 0.f, 1.f, 0.f );
	gtColor blue( 0.f, 0.f, 1.f );
	
	if( flags & debug::BV_OBB ){
		auto obb = object->getObb();
		if( obb ){

			m_gs->drawLineBox(
				obb->v1,
				obb->v2,
				pos,
				red
			);
		}
	}
	
	if( flags & debug::BV_AABB ){
		auto aabb = object->getAabb();
		if( aabb ){
			m_gs->drawLineBox( aabb->m_min, aabb->m_max, pos, green  );
		}
	}
	
	if( flags & debug::BV_sphere ){
		m_gs->drawLineSphere( 
			pos, 
			object->getBVSphereRadius(), 
			1u, 
			red, 
			green, 
			blue );
	}
	
	if( flags & debug::normal ){
		if( object->getType() == gtGameObjectType::Static ){
			auto m = ((gtStaticObject*)object)->getSoftwareModel();
			if( m ){
				auto smc = m->getSubmodelsCount();
				for( u32 i = 0u; i < smc; ++i ){
					auto   sm = m->getSubModel( i );
					
					auto * vs = sm->m_vertices;
					
					
					for( u32 i2 = 0u; i2 < sm->m_vCount; ){
						
						f32  * data32 = reinterpret_cast<f32*>( vs );
						
						v4f p;
						p.x  = data32[sm->m_vertexPosition];
						p.y  = data32[sm->m_vertexPosition+1];
						p.z  = data32[sm->m_vertexPosition+2];
						p.w  = data32[sm->m_vertexPosition+3];
						
						v3f n;
						n.x  = data32[sm->m_normalPosition];
						n.y  = data32[sm->m_normalPosition+1];
						n.z  = data32[sm->m_normalPosition+2];
						
						p *= object->getScale();
						
						m_gs->drawLine( p + pos, p + pos + n, green  );
						
						vs += sm->m_stride;
						
						++i2;
					}
					
				}
			}
		}
	}
}

void gtDebugRendererImpl::setGS( gtGraphicsSystem * gs ){
	m_gs = gs;
}