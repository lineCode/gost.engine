﻿/*!	GOST
	\file gtGameObject.h
	\brief base object for all scene objects
*/
#pragma once
#ifndef __GT_GAME_OBJECT_H__
#define __GT_GAME_OBJECT_H__ ///< include guard

namespace gost{

		///	тип объекта
	enum class gtObjectType{
		CAMERA,
		STATIC,
		SPRITE,
		DUMMY
	};

		/// Общий класс для всех объектов виртуальной сцены
	class gtGameObject : public gtRefObject{
	protected:

			///	имя
		gtStringA		m_name;

			/// id
		s32				m_id;

			/// родитель
		gtGameObject*	m_parent;

		gtSceneSystem*	m_scene;

			/// список потомков
		gtList<gtGameObject*> m_childs;

			/// матрицы трансформаций
		gtMatrix4		m_worldMatrix, m_worldMatrixAbsolute;

			/// позиция
		v3f				m_position;

			/// виден ли объект
		bool			m_isVisible;


	public:

			/// c-tor
		gtGameObject( void ):
			m_id( -1 ),
			m_parent( nullptr ),
			m_scene( nullptr ),
			m_isVisible( true )
		{
			m_scene = gtMainSystem::getInstance()->getSceneSystem();
		}

			/// d-tor
		virtual ~gtGameObject( void ){
		}

			///	Получить тип объекта
			///	\return Возвратит тип объекта
		virtual gtObjectType		getType( void ) = 0;

			///	Обновит информацию о позиции/вращении/масштабе
		virtual void				update( void ) = 0;

			///	Нарисует объект (если он рисуемый (например не 3D аудио))
		virtual void				render( void ) = 0;

		virtual gtAabb*				getAabb( void ) = 0;

			///	Получить позицию
			///	\return Возвратит позицию
		virtual const v3f&			getPosition( void ){
			return m_position;
		}

		virtual v3f			getPositionInSpace( void ){
			return m_worldMatrixAbsolute[ 3 ].getXYZ();
		}

			///	Установит позицию
			/// \param p: позиция
		virtual void				setPosition( const v3f& p ){
			m_position = p;
		}

			///	Получить матрицу с трансформациями относительно мира
			///	\return Матрица с трансформациями относительно мира
		virtual const gtMatrix4&	getAbsoluteWorldMatrix( void ){
			return m_worldMatrixAbsolute;
		}

			///	Получить матрицу с трансформациями относительно мира либо родителя если он указан
			///	\return Матрица с трансформациями относительно мира либо родителя если он указан
		virtual const gtMatrix4&	getWorldMatrix( void ){
			return m_worldMatrix;
		}

			/// Установит абсолютную матрицу
			/// \param m: матрица
		virtual void		setAbsoluteWorldMatrix( const gtMatrix4& m ){
			m_worldMatrixAbsolute = m;
		}

			/// Установит матрицу
			/// \param m: матрица
		virtual void		setWorldMatrix( const gtMatrix4& m ){
			m_worldMatrix = m;
		}

			///	Получить имя
			///	\return Возвратит имя
		virtual const gtStringA&	getName( void ){
			return m_name;
		}

			///	Установить имя
			/// \param n: имя
		virtual void				setName( const gtStringA& n ){
			m_name = n;
		}

			///	Получить ID
			///	\return Возвратит ID
		virtual s32					getID( void ){
			return m_id;
		}

			///	Установить ID
			/// \param i: id
		virtual void				setID( s32 i ){
			m_id = i;
		}

			///	Установит родителя. nullptr чтобы убрать родителя
			/// \param parent: родитель
		virtual void	setParent( gtGameObject * parent = nullptr ){
			if( m_parent )
				m_parent->removeChild( this );

			if( parent ){
				parent->addChild( this );
				m_parent = parent;
			}
			else{
				m_scene->getRootObject()->addChild( this );
				m_parent = m_scene->getRootObject();
			}
		}

			///	Получить указатель на родителя
			///	\return Вернёт указатель на родителя
		virtual gtGameObject * getParent( void ) const {
			return m_parent;
		}

			///	Получить список потомков
			///	\return Вернёт список потомков
		virtual gtList<gtGameObject*>&	getChildList( void ){
			return m_childs;
		}

			///	Добавить потомка
			/// \param child: потомок
		virtual void	addChild( gtGameObject * child ){
			if( child && (child->m_parent != this) ){

				m_childs.push_back( child );

				child->m_parent = this;
			}
		}

			/// Убрать потомка
			/// \param child: потомок
		virtual void	removeChild( gtGameObject * child ){
			auto it = m_childs.begin();
			auto it_end = m_childs.end();
			for(; it != it_end; ++it ){
				if( (*it) == child ){
					(*it)->m_parent = nullptr;
				//	(*it)->addRef();
					m_childs.erase( it );
					return;
				}
			}
		}

			///	Получить состояние видимости объекта
			///	\return Вернёт состояние видимости объекта
		virtual bool isVisible( void ){
			return m_isVisible;
		}

			///	Установить видимость объекта
			/// \param v: \b false если сделать не видимым
		virtual void setVisible( bool v ){
			m_isVisible = v;
		}

	};

}

#endif

#include "gtCamera.h"
#include "gtStaticObject.h"
#include "gtSprite.h"
#include "gtDummyObject.h"

/*
Copyright (c) 2017

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