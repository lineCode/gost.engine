#pragma once
#ifndef __GT_MATERIAL_H__
#define __GT_MATERIAL_H__ 

namespace gost{

	enum class gtMaterialBlendOp : u32 {
			//	Result = Source + Destination
		Add,

			//	Result = Source - Destination
		Sub,

			//	Result = Destination - Source
		Seb_reverse,

		Min,

		Max
	};

	enum class gtMaterialFlag : u32 {

		Wireframe = gtConst1U,

		Backface = gtConst2U,

		Blend = gtConst4U,

		Nocastshadows = gtConst8U,

		Noreceiveshadows = gtConst16U,

		Noselfshadows = gtConst32U,

		Blenddiscard = gtConst64U

	};

	struct gtMaterialTextureLayer{

			// c-tor
		gtMaterialTextureLayer():
			diffuseColor( gtConst1F ),
			blendOperation( gtMaterialBlendOp::Sub ),
			level( gtConst1F ),
			texture( nullptr )
		{}

		gtColor	diffuseColor;

		gtMaterialBlendOp	blendOperation;

		f32		level;

		gtTexture * texture;
	};

		// shader type, if used standart shader
	enum class gtMaterialType : u32 {
		Sprite,
		Standart,
		GUI
	};

		// material
	class gtMaterial{

	public:
			// c-tor
		gtMaterial():
			shader( nullptr ),
			specularColor( gtConst1F ),
			specularLevel( gtConst1F ),
			glossiness( gtConst0F ),
			roughness( gtConst0F ),
			opacity( gtConst1F ),
			fresnel( gtConst1F ),
			flags( gtConst0U ),
			type( gtMaterialType::Standart ),
			owner( nullptr ),
			alphaToCoverage( false )
		{}

			// d-tor
		~gtMaterial(){}

		gtShader * shader;

		gtColor ambientColor;

		gtColor specularColor;

		f32		specularLevel;

		f32		glossiness;

		f32		roughness;

			//	Need MF_BLEND.
		f32		opacity;

		f32		fresnel; // /freɪˈnɛl/, fray-NEL;

		u32		flags;

		gtMaterialTextureLayer	textureLayer[ gtConst16U ];

		gtMaterialType type;

		gtGameObject * owner;

		bool	alphaToCoverage;
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
