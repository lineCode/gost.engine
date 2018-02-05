/*!	GOST
	\file gtSprite.h
	\brief 2D sprite

	\attention Думаю что можно сделать возможность создавать billboard. Быть может в конструкторе можно добавить \b bool \b asBillboard, и в методе обновления делать магию с ориентацией спрайта.
*/

#pragma once
#ifndef __GT_SPRITE_H__
#define __GT_SPRITE_H__ ///< include guard

namespace gost{

	class gtDriver;

		///	2D картинка с возможностью создания анимации
	class gtSprite : public gtGameObject{

		gtObjectType m_type;			///< тип объекта
		 
		gtQuaternion	m_quaternion;	///< ориентация

		v3f			m_scale;			///< масштаб
		v3f			m_rotation;			///< углы поворота

		gtAnimation m_animation;		///< объект для работы анимации
		gtArray<v8f> m_frames;			///< кадры анимации

		gtMaterial	 m_material;		///< материал
		gtTexture *  m_texture;			///< текстура

		gtRenderModel* m_rModel;		///< модель

		gtMainSystem * m_system;		///< указатель на главную систему
		gtDriver * m_driver;			///< указатель на видео драйвер, который владеет hardware буферами данного спрайта

		bool m_firstFrame;				///< для того чтобы правильно создавать анимацию
		bool m_inverseHorizontal;		///< изменять UV координаты по горизонтали

		u32 width, height;				///< ширина и высота указанной текстуры

		f32 m_timer;					///< таймер
		f32 m_timerLimit;				///< лимит таймера, после которого таймер сбрасывается до нуля

		gtSprite( void ){};

	public:

			///	создаёт спрайт
			/// \param t: текстура
			/// \param size: ширина и высота
			/// \param d: видео драйвер который будет создавать спрайт
		gtSprite( gtTexture * t, const v2f& size, gtDriver * d ):
			m_type( gtObjectType::SPRITE ),
			m_scale( 1.f ),
			m_rModel( nullptr ),
			m_driver( d ),
			m_firstFrame( true ),
			m_inverseHorizontal( false ),
			m_timer( 0.f ),
			m_timerLimit( 1.f )
		{
#ifdef GT_DEBUG
			m_debugName.assign(u"gtSprite");
#endif
			m_system = gtMainSystem::getInstance();
			gtModel* model = m_system->getModelSystem()->createPlane( size[ 1u ], size[ 0u ], gtSide::FRONT );

			model->getSubModel( 0u )->m_material.textureLayer[ 0u ].texture = t;
			model->getSubModel( 0u )->m_material.type = gtMaterialType::Sprite;
			model->getSubModel( 0u )->m_material.owner = this;
			model->getSubModel( 0u )->m_material.flags = gtMaterialFlag::MF_BLEND;

			m_material = model->getSubModel( 0u )->m_material;

			m_texture = t;

			m_rModel = d->createModel( model );


			width = 1u, height = 1u;
			if( m_material.textureLayer[ 0u ].texture ){
				width = m_material.textureLayer[ 0u ].texture->getWidth();
				height = m_material.textureLayer[ 0u ].texture->getHeight();
			}

		//	m_quaternion.set(v3f_t(0.f,0.f,-PI/2.f));
		//	m_quaternion = m_quaternion * gtQuaternion(v3f_t(0.f,0.f,-PI/2.f));

			m_position.z_ = -1.f;

			resetAnimation( true );

			if( model )	model->release();
			
		}

			/// d-tor
		virtual ~gtSprite( void ){
			
			if( m_rModel ){
				m_rModel->release();
				m_rModel = nullptr;
			}

			
		}

		gtAabb*				getAabb( void ){
			return m_rModel->getAabb();
		}

			/// Получить тип объекта
			/// \return Вернёт тип объекта
		gtObjectType getType( void ){
			return m_type;
		}

			///	Получить масштаб
			///	\return Возвратит масштаб
		const v3f& getScale( void ){
			return m_scale;
		}

			///	Получить вращение
			///	\return Вернёт вращение
		const v3f& getRotation( void ){
			return m_rotation;
		}

			///	Установить масштаб
			/// \param s: масштаб
		void setScale( const v3f& s ){
			m_scale = s;
		}

			///	Установить вращение
			/// \param r: вращение (углы Эйлера в радианах)
		void setRotation( const v3f& r ){
			m_rotation = r;
			m_quaternion.set( r );
		}

			/// Получить текстуру
			/// \return Вернёт текстуру
		gtTexture*	getTexture( void ){
			return m_material.textureLayer[ 0u ].texture;
		}

			/// Обновит матрицы
		void update( void ){
			gtMatrix4 translationMatrix;
			math::makeTranslationMatrix( translationMatrix, m_position );

			gtMatrix4 rotationMatrix;
			math::makeRotationMatrix( rotationMatrix, m_quaternion );

			gtMatrix4	scaleMatrix;
			scaleMatrix[ 0u ] *= m_scale[ 0u ];
			scaleMatrix[ 1u ] *= m_scale[ 1u ];
			scaleMatrix[ 2u ] *= m_scale[ 2u ];

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

			/// Инвертирует по горизонтали
			/// \param v: \b true если нужно инвертировать
		void inverseHorizontal( bool v ){
			m_inverseHorizontal = v;
		}

			/// Проиграть анимацию задом наперёд
			/// \param v: \b true если нужно проиграть анимацию задом наперёд
		void setReverse( bool v ){
			m_animation.setReverse( v );
		}

			/// Инвертировано ли по горизонтали
			/// \return \b true если инвертировано ли по горизонтали
		bool isInverseHorizontal( void ){
			return m_inverseHorizontal;
		}

			/// Нарисует спрайт.
		void render( void ){
			if( m_isVisible ){
				updateAnimation();
				m_driver->drawModel( m_rModel );
			}
		}

			/// Добавит кадр.
			/// \param rect: координаты левого верхнего и правого нижнего углов
		void addFrame( const v4u& rect ){

			v2f lt, rb;

			f32 mulX = 1.f / (f32)width;
			f32 mulY = 1.f / (f32)height;

			lt.x_ = rect.x_ * mulX;
			lt.y_ = rect.y_ * mulY;
			rb.x_ = rect.z_ * mulX;
			rb.y_ = rect.w_ * mulY;
			
			m_frames.push_back(
				v8f_t(
					lt.x_,
					rb.y_,
					lt.x_,
					lt.y_,
					rb.x_,
					lt.y_,
					rb.x_,
					rb.y_
				)
			);

			m_animation.addFrame();
			auto * ls = &m_animation.getLoopSegment();
			m_animation.setLoopSegment( ls->x_, m_frames.size() - 1u );
		}

			/// Удалит анимацию и установит текущий кадр
			/// \param full: если \b true то будет показана вся картинка
			/// \param rect: если \b full == \b false то картинка будет указано по этим координатам. Левый верхний и правый нижний угол.
		void resetAnimation( bool full = true, const v4u& rect = v4u() ){
			m_animation.clear();
			m_frames.clear();
			
			if( full ){
				v4u rect2({ 0u, 0u, m_texture->getWidth(), m_texture->getHeight() });
				addFrame( rect2 );
			}
			else
				addFrame( rect );
		}

			/// Получить текущий кадр
			/// \return Вернёт текущий кадр
		u32 getCurrentFrame( void ){
			return m_animation.getCurrentFrame();
		}

			/// Получить координаты текущего кадра
			/// \return Координаты текущего кадра
		const v8f& getFrame( u32 id ){
			return m_frames[ id ];
		}

			/// Включено ли зацикливание анимации
			/// \return Вернёт \b true если зацикливание включёно
		bool isLoop( void ) const {
			return m_animation.isLoop();
		}

			/// Воспроизводится ли анимация
			/// \return Вернёт \b true если анимация воспроизводится
		bool isPlay( void ) const {
			return m_animation.isPlay();
		}

			/// Установить зацикливание анимации
			/// \param value: \b true если нужно зациклить анимацию
		void setLoop( bool value = true ){
			m_animation.setLoop( value );
		}

			/// Установить сегмент анимации, начало и конец.
			/// \param begin: начальный кадр
			/// \param end: завершающий кадр
		void setLoopSegment( u32 begin, u32 end ){
			m_animation.setLoopSegment( begin, end );
		}

			/// Получить номер текущего кадра
			/// \return Вернёт номер текущего кадра
		u32 getFrameID( void ) const {
			return m_animation.getCurrentFrame();
		}

			/// Включить воспроизведение анимации
		void playAnimation( void ){
			m_animation.play();
		}
			
			/// Приостановить анимацию
		void pauseAnimation( void ){
			m_animation.pause();
		}

			/// Остановить анимацию
		void stopAnimation( void ){
			m_animation.stop();
		}

			/// Установить текущий кадр
			/// \param frame: номер кадра
		void setFrame( u32 frame ){
			m_animation.setCurrentFrame( frame );
		}

			/// Обновить анимацию
		void updateAnimation( void ){
			u32 t1 = 0u;
			static u32 t2 = 0u;
			if( m_animation.getFrameCount() > 1u ){
				if( m_animation.isPlay() ){
					
					t1 = m_system->getTime();
					
					if( m_timer > m_timerLimit ){
						m_animation.stepFrame();
						m_timer = 0u;
					}

					m_timer += t1 - t2;
					
					t2 = t1;
				}
			}
		}

			/// Создать анимацию
			/// \param numOfFrames: количество кадров
			/// \param size: размер кадра
			/// \param directionIsHorizontal: анимация в атласе указана по горизонтали. Так как обычно она и так указана по горизонтали, то встаёт вопрос о нужности этого \b bool по этому не реализована функция прохода по вертикали
		void createAnimation( u32 numOfFrames, const v2u& size, bool directionIsHorizontal = true ){

			m_animation.clear();
			m_frames.clear();

			u32 x1 = 0u, y1 = 0u;
			u32 x2 = size.x_, y2 = size.y_;
			if( directionIsHorizontal ){
				for( u32 i = 0u; i < numOfFrames; ++i ){
					addFrame( v4u_t( x1, y1, x2, y2 ) );
					x1 += size.x_;
					x2 += size.x_;
					if( x2 > width ){
						x1 = 0u;
						x2 = size.x_;
						y1 += size.y_;
						y2 += size.y_;
					}
				}
			}
		}

			/// Получить количество кадров в секунду
			/// \return Вернёт количество кадров в секунду
		f32	getFrameRate( void ) const {
			return m_animation.getFrameRate();
		}

			/// Установить кадры в секунду
			/// \param rate: кадры в секунду
		void	setFrameRate( f32 rate ){
			if( rate == 0 )
				rate = 1;
			m_animation.setFrameRate( rate );
			m_timerLimit = 1000 / rate;
		}

		gtMaterial * getMaterial( void ){
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