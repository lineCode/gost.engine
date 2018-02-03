#pragma once
#ifndef __GT_MODEL_IMPL_H__
#define __GT_MODEL_IMPL_H__

namespace gost{

	class gtModelImpl : public gtModel{
	
		gtArray< gtSubModel* > m_submodels;

		gtVertexType*	m_typeArray;

		u32 m_stride;

		gtAabb m_box;

		gtSubModel*	createSubModel( u32 v_count, u32 i_count, u32 stride );

	public:

		gtModelImpl( u32, gtVertexType* );
		virtual ~gtModelImpl();

			//	возвратит суб модель, с типом, в зависимости от типа вершины
		gtSubModel*	addSubModel( u32 v_count, u32 i_count, u32 stride );

			//	добавит новую суб модель (создаст, с выделением памяти)
		gtSubModel*	addSubModel( gtSubModel* subModel );

			// нужно учитывать количество суб моделей
		gtSubModel*	getSubModel( u32 id );

			//	Вернёт количество мешбуфферов/субмоделей
		u32		getSubmodelsCount( void );

			//	Возврат количество байт на вершину
		u32		getStride( void );

			//	Возврат на массив gtVertexType, который должен заканчиваться gtVertexType::end
		gtVertexType*	getTypeArray( void );

			//	Вернёт габаритную коробку
		const gtAabb&	getAabb( void );

			//	Установит габаритную коробку
		void			setAabb( const gtAabb& );

			//	Вычислит Aabb всех субмоделей и на их основе
			//	построит Aabb целой модели
		void			updateAabb( void );
	};

}

#endif