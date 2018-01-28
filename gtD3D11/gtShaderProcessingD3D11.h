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

			// ������� � ������ �������� float
		void	setFloat( const gtStringA& uniformName, f32 a );

			// ������� � ������ �������� int
		void	setInt( const gtStringA& uniformName, s32 a );

			// ������� � ������ �������
		void	setMatrix4x4( const gtStringA& uniformName, gtMatrix4& matrix );

			// ������� � ������ ������3
		void	setVec3f( const gtStringA& uniformName, v3f& vec3 );

			// ������� � ������ ������4
		void	setVec4f( const gtStringA& uniformName, v4f& vec4 );

			// ������� � ������ ��������
			// id - ����� �������� � ���������
		void	setTexture( const gtStringA& uniformName, s32 id ) override;

			//	��� ����������� �������
			//	slot - ����� ������� ��� ���������� �������. ���������� � ����.
			//	���� ���������� � ����������, �� ��� ����� ���� �������� � ����.
			//	id - ��� ����� ������ (���������� ����� ����� createShaderObject)
			//	����� ����� ������� ��� � VS ��� � � PS, �� ����� slot � id ����� �����������
			//	����.	
			//	sendDataVS(data, 0, 0);
			//	sendDataPS(data, 0, 1);
			//	���
			//	sendDataVS(data, 0, 0);		//	sendDataVS(data, 0, 0); <-\
			//	sendDataVS(data, 1, 1);		//	sendDataVS(data, 1, 1);		|- 1 ����� � ���� � ����
			//	sendDataPS(data, 0, 2);		//	sendDataPS(data, 0, 0); <-/
			//	sendDataPS(data, 1, 3);		//	sendDataPS(data, 1, 2);
		void	sendDataVS( void* data, s32 slot, u32 id ) override;

			//	��. �������� sendDataVS
		void	sendDataPS( void* data, s32 slot, u32 id ) override;

		// ==============================================================
		void setShader( gtShader * );
		void setMaterial( gtMaterial * );

		/* ��� �������� ������ � ����������� ������ */
		void mapData( void* data, s32 id );
	};

}


#endif
