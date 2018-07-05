#pragma once
#ifndef __GT_GAME_OBJECT_H__
#define __GT_GAME_OBJECT_H__

namespace gost{

	enum class gtObjectType{
		Camera,
		Static,
		Sprite,
		Dummy
	};

	class gtGameObject : public gtRefObject{
	protected:
		gtStringA		m_name;
		s32				m_id;
		gtGameObject*	m_parent;
		gtSceneSystem*	m_scene;

		gtList<gtGameObject*> m_childs;

		gtMatrix4		m_worldMatrix, m_worldMatrixAbsolute;
		gtMatrix4		m_rotationMatrix;

		v4f				m_position;
		v4f				m_rotation, m_old_rotation;
		v4f				m_scale;
		gtQuaternion	m_orientation;
		bool			m_isVisible;
		bool			m_isBV;
		f32				m_sphereRadius;

		gtBoundingVolumeType m_BVType;
	public:

		gtGameObject():
			m_id( gtConst_1 ),
			m_parent( nullptr ),
			m_scene( nullptr ),
			m_scale( gtConst1F ),
			m_isVisible( true ),
			m_isBV( false ),
			m_sphereRadius( gtConst1F ),
			m_BVType( gtBoundingVolumeType::Sphere )
		{
			m_scene = gtMainSystem::getInstance()->getSceneSystem( nullptr );
		}

		virtual ~gtGameObject(){}

		virtual gtObjectType		getType() = 0;
		virtual void				update() = 0;
		virtual void				render() = 0;
		virtual gtAabb*				getAabb() = 0;
		virtual gtObb*				getObb() = 0;

		virtual const v4f&			getPosition(){ return m_position; }
		virtual v4f					getPositionInSpace(){ return m_worldMatrixAbsolute[ gtConst3U ]; }
		virtual void				setPosition( const v4f& p ){ m_position = p; }

		virtual void				setRotation( const v4f& rotation ){
			if( m_old_rotation != rotation ){
				this->m_rotation = rotation; 

				v4f r =  rotation - m_old_rotation;

				gtQuaternion q(r);
				m_orientation = q * m_orientation;
				m_orientation.normalize();

				m_old_rotation = rotation;

				recalculateBV();
			}
		}

		virtual const v4f&			getRotation(){ return m_rotation; }
		virtual const v4f&			getScale(){ return m_scale; }
		virtual void				setScale( const v4f& s ){
			m_scale = s;
			recalculateBV();
		}

		virtual void				setOrientation( const gtQuaternion& q ){
			m_orientation = q;
			m_orientation.normalize();
			recalculateBV();
		}

		virtual const gtQuaternion& getOrientation(){
			return m_orientation;
		}

		virtual void recalculateBV(){
			gtObb * obb = getObb();
			if( obb ){				
				gtMatrix4 R, S;

				math::makeRotationMatrix( R, m_orientation );
				math::makeScaleMatrix( m_scale, S );

				R = R * S;

				obb->v1 = math::mul( obb->v1z, R );
				obb->v2 = math::mul( obb->v2z, R );
				obb->v3 = math::mul( obb->v3z, R );
				obb->v4 = math::mul( obb->v4z, R );
				obb->v5 = math::mul( obb->v5z, R );
				obb->v6 = math::mul( obb->v6z, R );
				obb->v7 = math::mul( obb->v7z, R );
				obb->v8 = math::mul( obb->v8z, R );

				gtAabb * aabb = getAabb();
				if( aabb ){
					aabb->reset();

					aabb->add( obb->v1 );
					aabb->add( obb->v2 );
					aabb->add( obb->v3 );
					aabb->add( obb->v4 );
					aabb->add( obb->v5 );
					aabb->add( obb->v6 );
					aabb->add( obb->v7 );
					aabb->add( obb->v8 );
				}
				obb->m_min = obb->v1;
				obb->m_max = obb->v2;
				//obb->calculateBaseInfo();
				v4f c;
				aabb->center( c );
				m_sphereRadius = aabb->m_max.distance( c );
			}
		}


		virtual const gtMatrix4&	getAbsoluteWorldMatrix(){ return m_worldMatrixAbsolute; }
		virtual const gtMatrix4&	getWorldMatrix(){ return m_worldMatrix; }
		virtual void				setAbsoluteWorldMatrix( const gtMatrix4& m ){ m_worldMatrixAbsolute = m; }
		virtual void				setWorldMatrix( const gtMatrix4& m ){ m_worldMatrix = m; }
		virtual s32					getID(){ return m_id; }
		virtual void				setID( s32 i ){ m_id = i; }
		
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

		virtual gtGameObject * getParent() const { return m_parent; }
		virtual gtList<gtGameObject*>&	getChildList(){ return m_childs; }

		virtual void	addChild( gtGameObject * child ){
			if( child && (child->m_parent != this) ){

				m_childs.push_back( child );

				child->m_parent = this;
			}
		}

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

		virtual bool isVisible(){ return m_isVisible; }
		virtual void setVisible( bool v ){ m_isVisible = v; }

		virtual void showBV( bool v ){ m_isBV = v; }
		virtual bool isShowBV(){ return m_isBV; }
		virtual void setBVType( gtBoundingVolumeType type ){ m_BVType = type; }

		virtual const f32&				getBVSphereRadius() const { return m_sphereRadius; }
		virtual gtBoundingVolumeType	getBVType() const { return m_BVType; }
	};

}

#endif

#include "SceneSystem/gtCamera.h"
#include "SceneSystem/gtStaticObject.h"
#include "SceneSystem/gtSprite.h"
#include "SceneSystem/gtDummyObject.h"

/*
Copyright (c) 2017-2018

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