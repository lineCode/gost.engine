#include "common.h"

gtGUIStaticTextImpl::gtGUIStaticTextImpl( gtGraphicsSystem* d ):
	m_font( nullptr ),
	m_mainSystem( nullptr ),
	m_modelSystem( nullptr ),
	m_gs( d ),
	m_gui( nullptr ),
	m_length( 0.f ),
	m_height( 0.f ),
	m_showBackground( true )
{
	m_type = gtGUIObjectType::Text;
	m_mainSystem = gtMainSystem::getInstance();
	m_modelSystem= m_mainSystem->getModelSystem();
	m_gui = m_mainSystem->getGUISystem( d );
	m_bgColor.set( 0.f, 0.f, 0.f, 1.f );
}

gtGUIStaticTextImpl::~gtGUIStaticTextImpl(){
	clear();
}

void gtGUIStaticTextImpl::setFont( gtGUIFont * font ){
	m_font = (gtGUIFontImpl*)font;
	setText( gtString( m_text ) );
}

void gtGUIStaticTextImpl::setFont( const gtPtr<gtGUIFont>& font ){
	m_font = (gtGUIFontImpl*)font.data();
	setText( gtString( m_text ) );
}

bool gtGUIStaticTextImpl::init( const gtString& text, s32 positionX, s32 positionY, gtGUIFont* font ){
	m_position.set( positionX, positionY );
	m_font = (gtGUIFontImpl*)font;
	setText( text );
	updateBackground();
	return true;
}

void gtGUIStaticTextImpl::clear(){
	m_length = 0.f;
	m_text.clear();
	u32 sz = m_buffers.size();
	for( u32 i = gtConst0U; i < sz; ++i ){
		m_buffers[ i ]->release();
	}
	m_buffers.clear();
	m_bufferInfo.clear();
}

void gtGUIStaticTextImpl::updateMaterial(){
	u32 sz = m_buffers.size();
	for( u32 i = gtConst0U; i < sz; ++i ){
		auto * m = m_buffers[ i ]->getMaterial( gtConst0U );
		m->textureLayer[ gtConst0U ].diffuseColor = m_material.textureLayer[ gtConst0U ].diffuseColor;
		m->transparent = m_material.transparent;
	}
}

f32  gtGUIStaticTextImpl::getLength(){
	return m_length;
}

void gtGUIStaticTextImpl::setColor( const gtColor& color ){
	m_material.textureLayer[ gtConst0U ].diffuseColor = color;
	updateMaterial();
}

void gtGUIStaticTextImpl::setPosition( const v2i& position ){
	m_position = position;
	setText( gtString( m_text ) );
}

template<typename rectType>
void uvRectToUV( const rectType& rect, v2f * LT, v2f * RT, v2f * LB, v2f * RB, gtTexture * texture ){
	f32 mulX = 1.f / (f32)texture->getWidth();
	f32 mulY = 1.f / (f32)texture->getHeight();

	v2f lt, rb;
	lt.x = rect.x * mulX;
	lt.y = rect.y * mulY;
	rb.x = rect.z * mulX;
	rb.y = rect.w * mulY;

	*RB = rb;
	*LT = lt;

	LB->x = lt.x;
	LB->y = rb.y;

	RT->x = rb.x;
	RT->y = lt.y;
}

void gtGUIStaticTextImpl::setText( const gtString& text ){
	GT_ASSERT3(m_gs);

	clear();

	m_text = text;

	if( m_font ){

		u32 textSize = m_text.size();
		if( textSize ){

			auto params = m_gs->getParams();
			auto bbsz = params.m_outWindow->getClientRect();

			gtVertexType vt[ 4 ] = {
				gtVertexType::Position,
				gtVertexType::UV,
				gtVertexType::Normal,
				gtVertexType::End
			};

			const u16 u[gtConst6U] = {0U,1U,2U,0U,2U,3U};

			gtPtr<gtModel> soft = m_modelSystem->createEmpty( gtStrideStandart, &vt[ gtConst0U ] );

			if( soft.data() ){

				auto * mainsub = soft->addSubModel( gtConst4U, gtConst6U, gtStrideStandart );
				auto * sub = mainsub;

				u32 interval = gtConst0U;
				u32 line_interval = gtConst0U;

				f32 px = (2.f/(f32)bbsz.getWidth());
				f32 py = (2.f/(f32)bbsz.getHeight());
				f32 centerx = ((f32)bbsz.getWidth()*0.5f);
				f32 centery = ((f32)bbsz.getHeight()*0.5f);
				
				m_height = 0;

				for( u32 i = gtConst0U; i < textSize; ++i ){
				
					auto ch = m_text[i];
			
					u32 id = m_font->getTextureID( ch );

					if( i ){
						sub = new gtSubModel;
						sub->m_iCount = gtConst6U;
						sub->m_vCount = gtConst4U;
						sub->allocate( gtStrideStandart );

						bool notFound = true;

						u32 bisz = m_bufferInfo.size();
						for( u32 o = gtConst0U; o < bisz; ++o ){
							if( m_bufferInfo[ o ].texture_id == id ){
								notFound = false;

								mainsub = m_bufferInfo[ o ].sub;
							}
						}

						if( notFound ){
							mainsub = soft->addSubModel( gtConst4U, gtConst6U, gtStrideStandart );
							delete sub;
							sub = mainsub;
							sub->m_material.type = gtMaterialType::GUI;
							sub->m_material.flags |= (u32)gtMaterialFlag::AlphaBlend;
							sub->m_material.textureLayer[ gtConst0U ].texture = m_font->getTexture( id );
							m_bufferInfo.push_back( bufferInfo( id, mainsub ) );
						}

					}else{
						sub->m_material.type = gtMaterialType::GUI;
						sub->m_material.flags |= (u32)gtMaterialFlag::AlphaBlend;
						sub->m_material.textureLayer[ gtConst0U ].texture = m_font->getTexture( id );
						m_bufferInfo.push_back( bufferInfo( id, mainsub ) );
					}

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


					gtVector4<u16> * rect = nullptr;
					gtVector4<f32> rectf;
					
					if( ch != u' ' && ch != u'\t' )
						rect = m_font->getRect( ch );
				
					f32 width = 0.f;
					f32 height = 0.f;


					if( rect ){
					
						width = (f32)rect->z - (f32)rect->x;
						height = (f32)rect->w - (f32)rect->y;

						if( height > m_height ) m_height = height;

						v3->pos.y += height * py;
					
						v4->pos.x += width * px;
						v4->pos.y += v3->pos.y; 

						v1->pos.x += v4->pos.x;

						rectf = *rect;

						uvRectToUV( rectf, &v3->uv, &v4->uv, &v2->uv, &v1->uv, m_font->getTexture( 0 ) );
					}else{
						if( ch == u' ' ){
							width = 4;
						}else{
							width = 14;
						}
					}


					if( ch == u'\n' ){
						width = 0;
						line_interval += (u32)m_height;
						interval = 0u;
					}

					sub->move( v3f( (f32)interval * px, -((f32)line_interval * py), 0.1f ) );

					if( ch != u'\n' ){
						interval += width + gtConst2U;
						m_length = (f32)(interval);
					}

					if( i ){
						if( sub != mainsub ){
							mainsub->append( sub );
							delete sub;
						}
					}
				} 
				
				f32 mulx = 1.f / centerx;
				f32 muly = 1.f / centery;
				f32 move_x = m_position.x * mulx - 1.f;
				f32 move_y = 1.f - m_position.y * muly;

				u32 sz = m_bufferInfo.size();
				for( u32 i = gtConst0U; i < sz; ++i ){
					auto * sub2 = m_bufferInfo[ i ].sub;
					sub2->move( v3f( move_x, move_y, 0.f ) );
				}

				auto rm = m_gs->createModel( soft.data() );
				if( rm.data() ){
					rm->addRef();
					m_buffers.push_back( rm.data() );
					updateBackground();
					updateMaterial();
				}else{
					gtLogWriter::printWarning( u"Can not create static text" );
				}
			}else{
				gtLogWriter::printWarning( u"Can not create static text" );
			}
		}
	}
}



void gtGUIStaticTextImpl::updateBackground(){
	gtTexture * t1 = nullptr;
	gtColor color;

	if( m_backgroundShape.data() ){
		t1 = m_backgroundShape->getTexture();
	}

	m_activeArea = v4i( 
			m_position.x,
			m_position.y - (s32)m_height,
			(m_position.x + (s32)m_length),
			m_position.y
		);

	m_backgroundShape = m_gui->createShapeRectangle( 
		m_activeArea,
		gtColor( 0.f, 0.f, 0.f, 1.f ) );
	m_backgroundShape->setTexture( t1 );
	m_backgroundShape->setColor( m_bgColor );
}

void gtGUIStaticTextImpl::update(){
}

void gtGUIStaticTextImpl::render(){
	u32 sz = m_buffers.size();
	for( u32 i = gtConst0U; i < sz; ++i ){
		if( m_visible ){
			if( m_showBackground )
				m_backgroundShape->render();
			m_gs->drawModel( m_buffers[ i ] );
		}
	}
}

void gtGUIStaticTextImpl::setTransparent( f32 transparent ){
	m_material.transparent = transparent;
	updateMaterial();
	m_backgroundShape->setTransparent( transparent );
}

f32  gtGUIStaticTextImpl::getTransparent(){
	return m_material.transparent;
}

void gtGUIStaticTextImpl::setBackgroundVisible( bool value ){
	m_showBackground = value;
}

void gtGUIStaticTextImpl::setBackgroundColor( const gtColor& color ){
	m_backgroundShape->setColor( color );
	m_bgColor = color;
}

gtGUIShape* gtGUIStaticTextImpl::getBackgroundShape(){
	return m_backgroundShape.data();
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