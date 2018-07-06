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

#define BIT(x)0x1<<(x)

	enum class gtMaterialFlag : u32 {
		Wireframe        = BIT(0),
		Backface         = BIT(1),
		Blend            = BIT(2),
		Nocastshadows    = BIT(3),
		Noreceiveshadows = BIT(4),
		Noselfshadows    = BIT(5),
		Blenddiscard     = BIT(6),
		UseLight         = BIT(6)
	};

	struct gtMaterialTextureLayer{

			// c-tor
		gtMaterialTextureLayer():
			diffuseColor( gtConst1F ),
			blendOperation( gtMaterialBlendOp::Sub ),
			level( gtConst1F ),
			texture( nullptr )
		{}

		gtColor	            diffuseColor;
		gtMaterialBlendOp	blendOperation;
		f32		            level;
		gtTexture *         texture;
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
			ambientColor( (u32)0xff303030 ),
			specularColor( gtConst1F ),
			diffuseColor( gtConst1F ),
			sunPosition( v4f(0.3f,.6,-.3f,0.f) ),
			specularLevel( gtConst1F ),
			shininess( 5.f ),
			glossiness( gtConst0F ),
			roughness( gtConst0F ),
			opacity( gtConst1F ),
			fresnel( gtConst1F ),
			flags( (u32)gtMaterialFlag::UseLight ),
			type( gtMaterialType::Standart ),
			owner( nullptr ),
			alphaToCoverage( false )
		{}

			// d-tor
		~gtMaterial(){}

		gtShader * shader;

		gtColor ambientColor;
		gtColor specularColor;
		gtColor diffuseColor;

		v4f     sunPosition;

		f32		specularLevel;
		f32		shininess;
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
