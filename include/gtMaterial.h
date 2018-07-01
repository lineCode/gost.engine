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

		Wireframe = 1u,

		Backface = 2u,

		Blend = 4u,

		Nocastshadows = 8u,

		Noreceiveshadows = 16u,

		Noselfshadows = 32u,

		Blenddiscard = 64u

	};

	struct gtMaterialTextureLayer{

			// c-tor
		gtMaterialTextureLayer():
			diffuseColor( 1.f ),
			blendOperation( gtMaterialBlendOp::Sub ),
			level( 1.f ),
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
			specularColor( 1.f ),
			specularLevel( 1.f ),
			glossiness( 0.f ),
			roughness( 0.f ),
			opacity( 1.f ),
			fresnel( 1.f ),
			flags( 0u ),
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

		gtMaterialTextureLayer	textureLayer[ 16u ];

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
