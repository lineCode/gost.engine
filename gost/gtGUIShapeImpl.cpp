#include "common.h"

gtGUIShapeImpl::gtGUIShapeImpl( gtDriver * d ):
m_driver( d ),
m_mainSystem( nullptr ),
m_modelSystem( nullptr ),
m_material( nullptr ){
	vt[ 0u ] = gtVertexType::position;
	vt[ 1u ] = gtVertexType::uv;
	vt[ 2u ] = gtVertexType::normal;
	vt[ 3u ] = gtVertexType::end;
	m_mainSystem = gtMainSystem::getInstance();
	m_modelSystem = m_mainSystem->getModelSystem();
	m_type = gtGUIObjectType::Shape;
	m_shapeType = gtGUIShapeType::Rectangle;
}

gtGUIShapeImpl::~gtGUIShapeImpl( void ){
	if( m_model ) m_model->release();
}

void gtGUIShapeImpl::render( void ){
	if( m_model )
	if( m_driver ){
		m_driver->drawModel( m_model );
	}
}

void gtGUIShapeImpl::setOpacity( f32 opacity ){
	m_material->opacity = opacity;
}

f32  gtGUIShapeImpl::getOpacity( void ){
	return m_material->opacity;
}

void gtGUIShapeImpl::setColor( const gtColor& color ){
	m_material->textureLayer[ 0u ].diffuseColor = color;
}

bool gtGUIShapeImpl::initRectangle( const v4i& rect, const gtColor& color ){
	const u16 u[6u] = {0U,1U,2U,0U,2U,3U};
	gtPtr<gtModel> soft = m_modelSystem->createEmpty( gtStrideStandart, &vt[ 0u ] );

	if( soft.data() ){
		auto * sub = soft->addSubModel( 4u, 6u, gtStrideStandart );
		sub->m_material.type = gtMaterialType::GUI;
		sub->m_material.flags |= gtMaterialFlag::MF_BLEND;
		sub->m_material.textureLayer[ 0u ].diffuseColor = color;
		//sub->m_material.textureLayer[ 0u ].texture = m_font->getTexture( id );
		u8* v = &sub->m_vertices[ 0u ];
		sub->fillIndices( u );

		gtStandartVertex * v1 = (gtStandartVertex*)v;
		gtStandartVertex * v2 = (gtStandartVertex*)&v[sub->m_stride];
		gtStandartVertex * v3 = (gtStandartVertex*)&v[sub->m_stride*2];
		gtStandartVertex * v4 = (gtStandartVertex*)&v[sub->m_stride*3];
		v1->pos.zero();
		v2->pos.zero();
		v3->pos.zero();
		v4->pos.zero(); 

		auto params = m_driver->getParams();
		auto * bbsz = &params.m_backBufferSize;

		f32 px = (2.f/bbsz->x);
		f32 py = (2.f/bbsz->y);
		f32 centerx = (bbsz->x*0.5f);
		f32 centery = (bbsz->y*0.5f);

		auto width = rect.z - rect.x;
		auto height = rect.w - rect.y;

		v1->pos.y -= height * py;
		v1->uv.set( 0.f, 1.f );

		v3->pos.x += width * px;
		v3->uv.set( 1.f, 0.f );

		v4->pos.x += v3->pos.x;
		v4->pos.y += v1->pos.y;
		v4->uv.set( 1.f, 1.f );
		
		v2->uv.set( 0.f, 0.f );



		f32 mulx = 1.f / centerx;
		f32 muly = 1.f / centery;
		f32 move_x = rect.x * mulx - 1.f;
		f32 move_y = 1.f - rect.y * muly;

		sub->move( v3f( move_x, move_y, 0.f ) );

		auto rm = m_driver->createModel( soft.data() );
		if( rm.data() ){
			rm->addRef();
			m_model = rm.data();
			m_material = m_model->getMaterial( 0u );
		}else{
			gtLogWriter::printWarning( u"Can not create static text" );
		}
	}else{
		gtLogWriter::printWarning( u"Can not create static text" );
	}
	return true;
}

void gtGUIShapeImpl::setTexture( gtTexture* texture ){
	m_material->textureLayer[ 0u ].texture = texture;
}

gtTexture* gtGUIShapeImpl::getTexture( void ){
	return m_material->textureLayer[ 0u ].texture;
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