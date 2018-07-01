#pragma once
#ifndef __GT_SPRITE_H__
#define __GT_SPRITE_H__

namespace gost{

	class gtGraphicsSystem;

		class gtSprite : public gtGameObject{

		gtObjectType m_type;
		 
		gtAnimation m_animation;
		gtArray<v8f> m_frames;	

		gtMaterial	 m_material;
		gtTexture *  m_texture;	

		gtPtr<gtRenderModel> m_rModel;

		gtMainSystem * m_system;		
		gtGraphicsSystem * m_gs;			

		bool m_firstFrame;				
		bool m_inverseHorizontal;		

		u32 width, height;				

		f32 m_timer;					
		f32 m_timerLimit;				

		gtAabb m_aabb;
		gtObb  m_obb;

		gtSprite(){};

	public:

			gtSprite( gtTexture * t, const v2f& size, gtGraphicsSystem * d ):
			m_type( gtObjectType::Sprite ),
			m_rModel( nullptr ),
			m_gs( d ),
			m_firstFrame( true ),
			m_inverseHorizontal( false ),
			m_timer( gtConst0F ),
			m_timerLimit( gtConst1F )
		{
			m_system = gtMainSystem::getInstance();
			auto model = m_system->getModelSystem()->createPlane( size.y, size.x, gtSide::Front );

			model->getSubModel( gtConst0U )->m_material.textureLayer[ gtConst0U ].texture = t;
			model->getSubModel( gtConst0U )->m_material.type = gtMaterialType::Sprite;
			model->getSubModel( gtConst0U )->m_material.owner = this;
			model->getSubModel( gtConst0U )->m_material.flags = (u32)gtMaterialFlag::Blend;

			m_material = model->getSubModel( gtConst0U )->m_material;

			m_texture = t;

			m_rModel = d->createModel( model.data() ).data();

			m_aabb = *m_rModel->getAabb();
			m_obb = *m_rModel->getObb();

			width = gtConst1U, height = gtConst1U;
			if( m_material.textureLayer[ gtConst0U ].texture ){
				width = m_material.textureLayer[ gtConst0U ].texture->getWidth();
				height = m_material.textureLayer[ gtConst0U ].texture->getHeight();
			}

		//	m_quaternion.set(v3f_t(0.f,0.f,-PI/2.f));
		//	m_quaternion = m_quaternion * gtQuaternion(v3f_t(0.f,0.f,-PI/2.f));

			m_position.z = -1.f;

			resetAnimation( true );

		}

			// d-tor
		virtual ~gtSprite(){
		}

		gtAabb*				getAabb(){
			return &m_aabb;
		}

		gtObb*				getObb(){
			return &m_obb;
		}

		gtObjectType getType(){
			return m_type;
		}

		
		gtTexture*	getTexture(){
			return m_material.textureLayer[ gtConst0U ].texture;
		}

		void update(){
			gtMatrix4 translationMatrix;
			math::makeTranslationMatrix( translationMatrix, m_position );

			gtMatrix4 rotationMatrix;
			math::makeRotationMatrix( rotationMatrix, m_orientation );

			gtMatrix4	scaleMatrix;
			scaleMatrix[ gtConst0U ] *= m_scale.x;
			scaleMatrix[ gtConst1U ] *= m_scale.y;
			scaleMatrix[ gtConst2U ] *= m_scale.z;

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

		void inverseHorizontal( bool v ){
			m_inverseHorizontal = v;
		}

		void setReverse( bool v ){
			m_animation.setReverse( v );
		}

		bool isInverseHorizontal(){
			return m_inverseHorizontal;
		}

		void render(){
			if( m_isVisible ){
				updateAnimation();
				m_gs->drawModel( m_rModel.data() );
			}
		}

		void addFrame( const v4u& rect ){

			v2f lt, rb;

			f32 mulX = gtConst1F / (f32)width;
			f32 mulY = gtConst1F / (f32)height;

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
			m_animation.setLoopSegment( ls->x, m_frames.size() - gtConst1U );
		}

		void resetAnimation( bool full = true, const v4u& rect = v4u() ){
			m_animation.clear();
			m_frames.clear();
			
			if( full ){
				v4u rect2( gtConst0U, gtConst0U, m_texture->getWidth(), m_texture->getHeight() );
				addFrame( rect2 );
			}
			else
				addFrame( rect );
		}

		u32 getCurrentFrame(){
			return m_animation.getCurrentFrame();
		}

		const v8f& getFrame( u32 id ){
			return m_frames[ id ];
		}

		bool isLoop() const {
			return m_animation.isLoop();
		}

		bool isPlay() const {
			return m_animation.isPlay();
		}

		void setLoop( bool value = true ){
			m_animation.setLoop( value );
		}

		void setLoopSegment( u32 begin, u32 end ){
			m_animation.setLoopSegment( begin, end );
		}

		u32 getFrameID() const {
			return m_animation.getCurrentFrame();
		}

		void playAnimation(){
			m_animation.play();
		}
			
		void pauseAnimation(){
			m_animation.pause();
		}

		void stopAnimation(){
			m_animation.stop();
		}

		void setFrame( u32 frame ){
			m_animation.setCurrentFrame( frame );
		}

		void updateAnimation(){
			u32 t1 = gtConst0U;
			static u32 t2 = gtConst0U;
			if( m_animation.getFrameCount() > gtConst1U ){
				if( m_animation.isPlay() ){
					
					t1 = m_system->getTime();
					
					if( m_timer > m_timerLimit ){
						m_animation.stepFrame();
						m_timer = gtConst0U;
					}

					m_timer += t1 - t2;
					
					t2 = t1;
				}
			}
		}

		void createAnimation( u32 numOfFrames, const v2u& size, bool directionIsHorizontal = true ){

			m_animation.clear();
			m_frames.clear();

			u32 x1 = gtConst0U, y1 = gtConst0U;
			u32 x2 = size.x, y2 = size.y;
			if( directionIsHorizontal ){
				for( u32 i = gtConst0U; i < numOfFrames; ++i ){
					addFrame( v4u( x1, y1, x2, y2 ) );
					x1 += size.x;
					x2 += size.x;
					if( x2 > width ){
						x1 = gtConst0U;
						x2 = size.x;
						y1 += size.y;
						y2 += size.y;
					}
				}
			}
		}

		f32	getFrameRate() const {
			return m_animation.getFrameRate();
		}

		void	setFrameRate( f32 rate ){
			if( rate == gtConst0F )
				rate = gtConst1F;
			m_animation.setFrameRate( rate );
			m_timerLimit = 1000 / rate;
		}

		gtMaterial * getMaterial(){
			return &m_material;
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