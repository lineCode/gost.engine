#include "common.h"

gtGUIShapeImpl::gtGUIShapeImpl( gtGraphicsSystem * d ):
m_gs( d ),
m_mainSystem( nullptr ),
m_modelSystem( nullptr ),
m_material( nullptr ){
	vt[ gtConst0U ] = gtVertexType::Position;
	vt[ gtConst1U ] = gtVertexType::UV;
	vt[ gtConst2U ] = gtVertexType::Normal;
	vt[ gtConst3U ] = gtVertexType::End;
	m_mainSystem = gtMainSystem::getInstance();
	m_modelSystem = m_mainSystem->getModelSystem();
	m_type = gtGUIObjectType::Shape;
	m_shapeType = gtGUIShapeType::Rectangle;
}

gtGUIShapeImpl::~gtGUIShapeImpl(){
	if( m_model ) m_model->release();
}

void gtGUIShapeImpl::render(){
	if( m_model )
	if( m_gs ){
		m_gs->drawModel( m_model );
	}
}

void gtGUIShapeImpl::setTransparent( f32 transparent ){
	m_material->transparent = transparent;
}

f32  gtGUIShapeImpl::getTransparent(){
	return m_material->transparent;
}

void gtGUIShapeImpl::setColor( const gtColor& color ){
	m_material->textureLayer[ gtConst0U ].diffuseColor = color;
}

gtMaterial* gtGUIShapeImpl::getMaterial(){
	return m_material;
}

bool gtGUIShapeImpl::initRectangle( const v4i& rect, const gtColor& color ){
	const u16 u[gtConst6U] = {0U,1U,2U,0U,2U,3U};
	gtPtr<gtModel> soft = m_modelSystem->createEmpty( gtStrideStandart, &vt[ gtConst0U ] );
	
	auto ssz = m_gs->getParams().m_outWindow->getWindowInfo().m_borderSize;
	auto offset_x = ssz.x + ssz.x;
	auto offset_y = ssz.y;


	if( soft.data() ){
		auto * sub = soft->addSubModel( gtConst4U, gtConst6U, gtStrideStandart );
		sub->m_material.type = gtMaterialType::GUI;
		sub->m_material.flags |= (u32)gtMaterialFlag::AlphaBlend;
		sub->m_material.textureLayer[ gtConst0U ].diffuseColor = color;
		//sub->m_material.textureLayer[ gtConst0U ].texture = m_font->getTexture( id );
		u8* v = &sub->m_vertices[ gtConst0U ];
		sub->fillIndices( u );

		gtStandartVertex * v1 = (gtStandartVertex*)v;
		gtStandartVertex * v2 = (gtStandartVertex*)&v[sub->m_stride];
		gtStandartVertex * v3 = (gtStandartVertex*)&v[sub->m_stride*2];
		gtStandartVertex * v4 = (gtStandartVertex*)&v[sub->m_stride*3];
		v1->pos.zero();
		v2->pos.zero();
		v3->pos.zero();
		v4->pos.zero(); 

		auto params = m_gs->getParams();
		auto bbsz = params.m_outWindow->getClientRect();

		f32 px = (2.f/bbsz.getWidth());
		f32 py = (2.f/bbsz.getHeight());
		f32 centerx = (bbsz.getWidth()*0.5f);
		f32 centery = (bbsz.getHeight()*0.5f);

		auto width = rect.z - rect.x;
		auto height = f32(rect.w - rect.y);

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

		auto rm = m_gs->createModel( soft.data() );
		if( rm.data() ){
			rm->addRef();
			m_model = rm.data();
			m_material = m_model->getMaterial( gtConst0U );
		}else{
			gtLogWriter::printWarning( u"Can not create static text" );
		}
	}else{
		gtLogWriter::printWarning( u"Can not create static text" );
	}

	m_activeArea = rect;

	return true;
}

void gtGUIShapeImpl::setTexture( gtTexture* texture ){
	m_material->textureLayer[ gtConst0U ].texture = texture;
}

gtTexture* gtGUIShapeImpl::getTexture(){
	return m_material->textureLayer[ gtConst0U ].texture;
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