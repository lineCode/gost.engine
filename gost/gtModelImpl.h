#pragma once
#ifndef __GT_MODEL_IMPL_H__
#define __GT_MODEL_IMPL_H__

namespace gost{

	class gtModelImpl : public gtModel{
	
		gtArray< gtSubModel* >	m_submodels;
		gtVertexType*			m_typeArray;
		u32						m_stride;
		gtAabb					m_aabb;
		gtObb					m_obb;
		gtSubModel*				createSubModel( u32 v_count, u32 i_count, u32 stride );

	public:

		gtModelImpl( u32, gtVertexType* );
		virtual ~gtModelImpl();

		gtSubModel*		addSubModel( u32 v_count, u32 i_count, u32 stride );
		gtSubModel*		addSubModel( gtSubModel* subModel );
		gtSubModel*		getSubModel( u32 id );
		u32				getSubmodelsCount( void );
		u32				getStride( void );
		gtVertexType*	getTypeArray( void );
		const gtAabb&	getAabb( void );
		void			setAabb( const gtAabb& );
		void			updateBoundingVolume( void );
		const gtObb&	getObb( void );
		void			setObb( const gtObb& obb );
	};

}

#endif