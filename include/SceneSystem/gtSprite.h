#pragma once
#ifndef __GT_SPRITE_H__
#define __GT_SPRITE_H__

namespace gost{

	class gtGraphicsSystem;

	class gtSprite : public gtGameObjectCommon{

			gtSprite(){};

			gtMainSystem *       m_system;
			gtGraphicsSystem *   m_gs;
			gtTexture *          m_texture;
			gtPtr<gtRenderModel> m_rModel;
			u32                  m_width;
			u32                  m_height;
			f32                  m_timer;
			f32                  m_timerLimit;
			gtArray<v8f>         m_frames;
			gtAnimation          m_animation;
			gtMaterial*	         m_material;
			bool                 m_firstFrame;
			bool                 m_inverseHorizontal;
			f32  * m_delta;

	public:

			gtSprite( gtTexture * t, const v2f& size, gtGraphicsSystem * d ):
			m_gs( d ),
			m_rModel( nullptr ),
			m_timer( 0.f ),
			m_timerLimit( 1.f ),
			m_firstFrame( true ),
			m_inverseHorizontal( false )
		{
			m_objectType = gtGameObjectType::Sprite;
			
			m_system = gtMainSystem::getInstance();
			auto model = m_system->getModelSystem()->createPlane( size.y, size.x, gtSide::Front );
			
			m_delta = m_system->getTimer()->getDelta();

			model->getSubModel( 0u )->m_material.textureLayer[ 0u ].texture = t;
			model->getSubModel( 0u )->m_material.type = gtMaterialType::Sprite;
			model->getSubModel( 0u )->m_material.userData = this;
			model->getSubModel( 0u )->m_material.flags = (u32)gtMaterialFlag::AlphaBlend;


			m_texture = t;

			m_rModel = d->createModel( model.data() ).data();
			m_material = m_rModel->getMaterial( 0 );

			m_aabb = *m_rModel->getAabb();
			m_obb = *m_rModel->getObb();

			m_width  = 1u;
			m_height = 1u;

			if( m_material->textureLayer[ 0u ].texture ){
				m_width = m_material->textureLayer[ 0u ].texture->getWidth();
				m_height = m_material->textureLayer[ 0u ].texture->getHeight();
			}

			m_position.z = -1.f;

			resetAnimation( true );
		}

		virtual        ~gtSprite()                 {}
		u32            getCurrentFrame()           { return m_animation.getCurrentFrame(); }
		const v8f&     getFrame( u32 id )          { return m_frames[ id ]; }
		u32            getFrameID() const          { return m_animation.getCurrentFrame(); }
		f32	           getFrameRate() const        { return m_animation.getFrameRate(); }
		gtMaterial *   getMaterial()               { return m_material; }
		gtTexture*	   getTexture()                { return m_material->textureLayer[ 0u ].texture;}
		void           inverseHorizontal( bool v ) { m_inverseHorizontal = v; }
		bool           isInverseHorizontal()       { return m_inverseHorizontal; }
		bool           isLoop() const              { return m_animation.isLoop(); }
		bool           isPlay() const              { return m_animation.isPlay(); }
		void           pauseAnimation()            { m_animation.pause(); }
		void           playAnimation()             { m_animation.play(); }
		void           setFrame( u32 frame )       { m_animation.setCurrentFrame( frame );}
		void           setLoop( bool value = true ){ m_animation.setLoop( value ); }
		void           setLoopSegment( u32 begin, u32 end ){ m_animation.setLoopSegment( begin, end ); }
		void           setReverse( bool v )        { m_animation.setReverse( v ); }
		void           stopAnimation()             { m_animation.stop(); }

		void update(){
			gtMatrix4 translationMatrix;
			math::makeTranslationMatrix( m_position, translationMatrix );

			gtMatrix4 rotationMatrix;
			math::makeRotationMatrix( rotationMatrix, m_orientation );

			gtMatrix4	scaleMatrix;
			scaleMatrix[ 0u ] *= m_scale.x;
			scaleMatrix[ 1u ] *= m_scale.y;
			scaleMatrix[ 2u ] *= m_scale.z;

			m_worldMatrix = translationMatrix * rotationMatrix * scaleMatrix;

			if( m_parent )
				m_worldMatrix = m_parent->getAbsoluteWorldMatrix() * m_worldMatrix;

			m_worldMatrixAbsolute = m_worldMatrix;

			auto * childs = &getChildList();
			{
				auto it = childs->begin();
				for(; it != childs->end(); ++it){
					(*it)->update();
				}
			}

		}


		void render(){
			if( m_isVisible ){
				updateAnimation();
				m_gs->drawModel( m_rModel.data() );
			}
		}

		void addFrame( const v4u& rect ){

			v2f lt, rb;

			f32 mulX = gtConst1F / (f32)m_width;
			f32 mulY = gtConst1F / (f32)m_height;

			lt.x = rect.x * mulX;
			lt.y = rect.y * mulY;
			rb.x = rect.z * mulX;
			rb.y = rect.w * mulY;

			m_frames.push_back(
				v8f(
					lt.x,
					rb.y,
					lt.x,
					lt.y,
					rb.x,
					lt.y,
					rb.x,
					rb.y
				)
			);

			m_animation.addFrame();
			auto * ls = &m_animation.getLoopSegment();
			m_animation.setLoopSegment( ls->x, m_frames.size() - 1u );
		}

		void resetAnimation( bool full = true, const v4u& rect = v4u() ){
			m_animation.clear();
			m_frames.clear();

			if( full ){
				v4u rect2( 0u, 0u, m_texture->getWidth(), m_texture->getHeight() );
				addFrame( rect2 );
			}
			else
				addFrame( rect );
		}


		void updateAnimation(){

			if( m_animation.getFrameCount() > 1u ){
				if( m_animation.isPlay() ){

					if( m_timer > m_timerLimit ){
						m_animation.stepFrame();
						m_timer = 0u;
					}

					m_timer += *m_delta;
				}
			}
		}

		void createAnimation( u32 numOfFrames, const v2u& size, bool directionIsHorizontal = true ){

			m_animation.clear();
			m_frames.clear();

			u32 x1 = 0u, y1 = 0u;
			u32 x2 = size.x, y2 = size.y;
			if( directionIsHorizontal ){
				for( u32 i = 0u; i < numOfFrames; ++i ){
					addFrame( v4u( x1, y1, x2, y2 ) );
					x1 += size.x;
					x2 += size.x;
					if( x2 > m_width ){
						x1 = 0u;
						x2 = size.x;
						y1 += size.y;
						y2 += size.y;
					}
				}
			}
		}


		void	setFrameRate( f32 rate ){
			if( rate == gtConst0F )
				rate = gtConst1F;
			m_animation.setFrameRate( rate );
			m_timerLimit = 1.f / rate;
		}

	};

}

#endif

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
