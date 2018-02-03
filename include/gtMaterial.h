/*!	GOST
	\file gtMaterial.h
	\brief working with materials
*/
#pragma once
#ifndef __GT_MATERIAL_H__
#define __GT_MATERIAL_H__ ///< include guard

namespace gost{


		///	Тип операции смешевания.
	enum gtMaterialBlendOp : u32 {

			///	Result = Source + Destination
		BOP_ADD,
			
			///	Result = Source - Destination
		BOP_SUBSTRACT,

			///	Result = Destination - Source
		BOP_SUBSTRACT_REVERSE,

			///	минимальное из Destination Source
		BOP_MIN,

			///	максимальное из Destination Source
		BOP_MAX
	};

		///	флаги
	enum gtMaterialFlag : u32 {

		///	рисовать только сетку
		MF_WIREFRAME = 1u,

		///	рисовать заднюю грань
		MF_BACKFACE = 2u,
		
		///	рисовать ли объект с прозрачностью
		MF_BLEND = 4u,

		///	не отбрасывать тень
		MF_NOCASTSHADOW = 8u,

		///	не получать тень
		MF_NORECEIVESHADOW = 16u,

		///	не получать тень от самого себя
		MF_NOSELFSHADOW = 32u

	};

		///	параметры для каждой текстуры
	struct gtMaterialTextureLayer{

			/// c-tor
		gtMaterialTextureLayer( void ):
			diffuseColor( 1.f ),
			blendOperation( gtMaterialBlendOp::BOP_SUBSTRACT ),
			level( 1.f ),
			texture( nullptr )
		{}

			///	основной цвет. Зависит от шейдера. 
			///	В стандартных шейдерах будет применяться к текстуре.
			///	Например если текстура белая, то diffuseColor({1.f,0.f,0.f,1.f}) покрасит её в красный.
			///	Вычисление такое. 1 * 1 = 1 красный компонент. 1 * 0 = 0 синий. 1 * 0 = 0 зелёный.
		gtColor	diffuseColor;

			///	Тип операции смешивания.
		gtMaterialBlendOp	blendOperation;

			///	Значение от 1.0 до 0
			///	Работает так: тексель *= level;
		f32		level;

			///	текстуры
		gtTexture * texture;
	};
		
		/// shader type, if used standart shader
	enum class gtMaterialType : u32 {
		Sprite,
		Standart,
		GUI
	};

		/// material
	class gtMaterial{

	public:
			/// c-tor
		gtMaterial( void ):
			shader( nullptr ),
			specularColor( 1.f ),
			specularLevel( 1.f ),
			glossiness( 0.f ),
			opacity( 1.f ),
			fresnel( 1.f ),
			roughness( 0.f ),
			flags( 0u ),
			type( gtMaterialType::Standart ),
			owner( nullptr )
		{}

			/// d-tor
		~gtMaterial( void ){}

			///	шейдер
		gtShader * shader;

			///	цвет затенения. Чёрный по умолчанию.
		gtColor ambientColor;

			///	цвет отражённого света
		gtColor specularColor;

			///	интенсивность отражения света.
		f32		specularLevel;
			
			///	блеск
		f32		glossiness;

			///	шероховатость
		f32		roughness;

			///	непрозрачность. Значение от 1 до 0
			///	Для прозрачности должен быть флаг MF_BLEND.
		f32		opacity;

			///	пригодится там где нужно
		f32		fresnel; // /freɪˈnɛl/, fray-NEL;
			
			///	флаги
		u32		flags;

			///	Тут хранятся указатели на текстуры и параметры для них
		gtMaterialTextureLayer	textureLayer[ 16u ];

			///	Если не указан shader то будет работать стандартный
		gtMaterialType type;

			/// Вспомогателный указатель. Хранит адрес объекта который использует данный материал
		gtGameObject * owner;
	};


}


#endif

/*
Copyright (c) 2017-2018 532235

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