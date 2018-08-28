#pragma once
#ifndef __GT_MATERIAL_H__
#define __GT_MATERIAL_H__ 

namespace gost{
	
		// не реализовано
	enum class gtMaterialBlendOp : u32 {
			//	Result = Source + Destination
		Add,

			//	Result = Source - Destination
		Sub,

			//	Result = Destination - Source
		Sub_reverse,

		Min,

		Max
	};

	
	enum class gtMaterialFlag : u32 {
		Wireframe        = BIT(0),    
		Backface         = BIT(1),
		AlphaBlend       = BIT(2),
		Nocastshadows    = BIT(3),
		Noreceiveshadows = BIT(4),
		Noselfshadows    = BIT(5),
		AlphaDiscard     = BIT(6),
		UseLight         = BIT(7)
	};

	struct gtMaterialTextureLayer{
		gtColor	            diffuseColor = gtColor( 1.f );
		gtMaterialBlendOp	blendOperation = gtMaterialBlendOp::Sub;
		f32		            level = 1.f;
		gtTexture *         texture = nullptr;
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

		gtShader * shader = nullptr;

		gtColor ambientColor = gtColor((u32)0xff303030);
		gtColor specularColor = gtColor( 1.f );
		gtColor diffuseColor = gtColor( 1.f );

		v4f     sunPosition = v4f(0.3f,.6f,-.3f,0.f);

		f32		specularLevel = 1.f;
		f32		shininess = 5.f;
		f32		glossiness = 0.f;
		f32		roughness = 0.f;

		f32		transparent = 0.f;
		f32		fresnel = 1.f;
		u32		flags = (u32)gtMaterialFlag::UseLight;

		gtMaterialTextureLayer	textureLayer[ 16u ];

		gtMaterialType type = gtMaterialType::Standart;

		void*   userData = nullptr;
		u32     userDataSize = 0u;

		bool	alphaToCoverage = false;
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
