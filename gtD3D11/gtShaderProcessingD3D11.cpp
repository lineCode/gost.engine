#include "common.h"

gtShaderProcessingD3D11::gtShaderProcessingD3D11( ID3D11DeviceContext*	d3d11DevCon ):
	m_shaderD3D11( nullptr ),
	m_material( nullptr ),
	m_d3d11DevCon( d3d11DevCon ),
	m_standartTexture( nullptr )
{
}

gtShaderProcessingD3D11::~gtShaderProcessingD3D11(){
}

void gtShaderProcessingD3D11::setStandartTexture( gtTexture * t ){
	m_standartTexture = t;
}

			// ������� � ������ �������� float
void	gtShaderProcessingD3D11::setFloat( const gtStringA&, f32 ){
}

			// ������� � ������ �������� int
void	gtShaderProcessingD3D11::setInt( const gtStringA&, s32 ){
}

			// ������� � ������ �������
void	gtShaderProcessingD3D11::setMatrix4x4( const gtStringA&, gtMatrix4& ){
}

			// ������� � ������ ������3
void	gtShaderProcessingD3D11::setVec3f( const gtStringA&, v3f& ){
}

			// ������� � ������ ������4
void	gtShaderProcessingD3D11::setVec4f( const gtStringA&, v3f& ){
}

			// ������� � ������ ��������
			// id - ����� �������� � ���������
void	gtShaderProcessingD3D11::setTexture( const gtStringA& /*uniformName*/, s32 id ){

	auto * texture = m_material->textureLayer[ id ].texture;

	if( !texture )
		texture = m_standartTexture;

	if( !texture )
		return;

	if( texture ){
		gtTextureD3D11* textured3d11 = (gtTextureD3D11*)texture;
		m_d3d11DevCon->PSSetShaderResources( id, 1, textured3d11->getResourceView() );
		m_d3d11DevCon->PSSetSamplers( id, 1, textured3d11->getSamplerState() );
	}
}

void gtShaderProcessingD3D11::mapData( void* data, s32 id ){
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		m_d3d11DevCon->Map( m_shaderD3D11->m_constantBuffers[ id ], 0, D3D11_MAP_WRITE_DISCARD, 0, 
			&mappedResource );
	
		D3D11_BUFFER_DESC d;
		m_shaderD3D11->m_constantBuffers[ id ]->GetDesc( &d );
		memcpy( mappedResource.pData, data, d.ByteWidth );

		m_d3d11DevCon->Unmap( m_shaderD3D11->m_constantBuffers[ id ], 0 );
}
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
void	gtShaderProcessingD3D11::sendDataVS( void* data, s32 slot, u32 id ){
	mapData( data, id );
	m_d3d11DevCon->VSSetConstantBuffers( slot, 1, &m_shaderD3D11->m_constantBuffers[ id ] );
}

			//	��. �������� sendDataVS
void	gtShaderProcessingD3D11::sendDataPS( void* data, s32 slot, u32 id ){
	mapData( data, id );
	m_d3d11DevCon->PSSetConstantBuffers( slot, 1, &m_shaderD3D11->m_constantBuffers[ id ] );
}



void gtShaderProcessingD3D11::setShader( gtShader * s ){
	m_shaderD3D11 = (gtShaderImpl*)s;
}

void gtShaderProcessingD3D11::setMaterial( gtMaterial * m ){
	m_material = m;
}
