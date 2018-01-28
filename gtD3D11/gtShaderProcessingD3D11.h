#pragma once
#ifndef __GT_SHADER_PROCESSING_D3D11_H__
#define __GT_SHADER_PROCESSING_D3D11_H__

namespace gost{


	class gtShaderProcessingD3D11 : public gtShaderProcessing{

		gtShaderImpl * m_shaderD3D11;
		gtMaterial * m_material;

		ID3D11DeviceContext*	m_d3d11DevCon;
		gtTexture* m_standartTexture;

	public:

		gtShaderProcessingD3D11( ID3D11DeviceContext* );
		virtual ~gtShaderProcessingD3D11();

			// ѕослать в шейдер значение float
		void	setFloat( const gtStringA& uniformName, f32 a );

			// ѕослать в шейдер значение int
		void	setInt( const gtStringA& uniformName, s32 a );

			// ѕослать в шейдер матрицу
		void	setMatrix4x4( const gtStringA& uniformName, gtMatrix4& matrix );

			// ѕослать в шейдер вектор3
		void	setVec3f( const gtStringA& uniformName, v3f& vec3 );

			// ѕослать в шейдер вектор4
		void	setVec4f( const gtStringA& uniformName, v4f& vec4 );

			// ѕослать в шейдер текстуру
			// id - номер текстуры в материале
		void	setTexture( const gtStringA& uniformName, s32 id ) override;

			//	дл€ константных буферов
			//	slot - номер очереди дл€ вершинного шейдера. Ќачинаетс€ с нул€.
			//	если посылаетс€ в пиксельный, то там нужно тоже начинать с нул€.
			//	id - это номер буфера (созданного через вызов createShaderObject)
			//	буфер можно послать как в VS так и в PS, по этому slot и id могут различатьс€
			//	напр.	
			//	sendDataVS(data, 0, 0);
			//	sendDataPS(data, 0, 1);
			//	или
			//	sendDataVS(data, 0, 0);		//	sendDataVS(data, 0, 0); <-\
			//	sendDataVS(data, 1, 1);		//	sendDataVS(data, 1, 1);		|- 1 буфер и туда и туда
			//	sendDataPS(data, 0, 2);		//	sendDataPS(data, 0, 0); <-/
			//	sendDataPS(data, 1, 3);		//	sendDataPS(data, 1, 2);
		void	sendDataVS( void* data, s32 slot, u32 id ) override;

			//	см. описание sendDataVS
		void	sendDataPS( void* data, s32 slot, u32 id ) override;

		// ==============================================================
		void setShader( gtShader * );
		void setMaterial( gtMaterial * );

		/* дл€ передачи данных в константный шейдер */
		void mapData( void* data, s32 id );
	};

}


#endif
