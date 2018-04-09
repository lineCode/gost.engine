/*!	GOST
	\file gtModel.h
	\brief software model
*/
#pragma once
#ifndef __GT_MODEL_H__
#define __GT_MODEL_H__ //< include guard

namespace gost{
	
		//	software модель
	class gtModel : public gtRefObject{
	public:

			//	создаст возвратит суб модель для редактирования
			//	\param v_count: количество вершин
			//	\param i_count: количество индексов
			//	\param stride: размер вершины в байтах
		virtual gtSubModel*	addSubModel( u32 v_count, u32 i_count, u32 stride ) = 0;

			//	добавит новую суб модель (создаст, с выделением памяти)
			// \param subModel: sub model
		virtual gtSubModel*	addSubModel( gtSubModel* subModel ) = 0;

			// Получить суб модель. Нужно учитывать количество суб моделей
			// \param id: sub model id
			// \return sub model
		virtual gtSubModel*	getSubModel( u32 id ) = 0;

			//	Вернёт количество мешбуфферов/субмоделей
			// \return sub model count
		virtual u32		getSubmodelsCount( void ) = 0;

			//	Возврат количество байт на вершину
			// \return vertex size in bytes
		virtual u32		getStride( void ) = 0;


			/**
				При создании модели, зная, какие данные находятся в вершине, нужно создать
				массив gtVertexType, и указать последний елемент gtVertexType::end (чтобы не посылать ещё 1 аргумент для обозначения его размера)
				
				\return Возврат на массив gtVertexType, который должен заканчиваться gtVertexType::end
			*/
		virtual gtVertexType*	getTypeArray( void ) = 0;

			//	Вернёт габаритную коробку
			// \return aabb
		virtual const gtAabb&	getAabb( void ) = 0;

			//	Установит габаритную коробку
			// \param aabb: aabb
		virtual void			setAabb( const gtAabb& aabb ) = 0;

		virtual const gtObb&	getObb( void ) = 0;
		virtual void			setObb( const gtObb& obb ) = 0;


			//	Вычислит obb/aabb всех субмоделей и на их основе построит obb/aabb целой модели
		virtual void			updateBoundingVolume( void ) = 0;
	};

}

#endif

/*
Copyright (c) 2017, 2018 532235

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