#include "common.h"

gtD3D11StandartShaderCallback::gtD3D11StandartShaderCallback():
m_system( nullptr ){
	m_system = gtMainSystem::getInstance();
}

gtD3D11StandartShaderCallback::~gtD3D11StandartShaderCallback(){
}

void gtD3D11StandartShaderCallback::onShader( const gtMaterial& /*m*/, gtShaderProcessing* sp ){
	

	/*в будущем в буфер можно добавить ещё поля, по этому сразу всё сунул в структуру*/
	struct cbMatrix_t{
		gtMatrix4 WVP;
	}cbMatrix;

	gtMatrix4 W = m_system->getMatrixWorld();
	gtMatrix4 V = m_system->getMatrixView();
	gtMatrix4 P = m_system->getMatrixProjection();

	cbMatrix.WVP =  P * V * W;

	cbMatrix.WVP.transpose();

	sp->sendDataVS( &cbMatrix, 0, 0u );

	/* в D3D11 главное правильно установить слот, uniformName не нужен */
	sp->setTexture( "", 0 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
gtD3D11SpriteShaderCallback::gtD3D11SpriteShaderCallback():
m_system( nullptr ){
	m_system = gtMainSystem::getInstance();
}

gtD3D11SpriteShaderCallback::~gtD3D11SpriteShaderCallback(){
}

void gtD3D11SpriteShaderCallback::onShader( const gtMaterial& material, gtShaderProcessing* sp ){

	struct cbPerObject_t{
		gtMatrix4 WVP;
		v2f t1;
		v2f t2;
		v2f t3;
		v2f t4;
	}cbPerObject;

	gtMatrix4 W = m_system->getMatrixWorld();
	gtMatrix4 V = m_system->getMatrixView();
	gtMatrix4 P = m_system->getMatrixProjection();

	cbPerObject.WVP =  P * V * W;
	cbPerObject.WVP.transpose();

	gtSprite * sprite = (gtSprite*)material.owner;

	u32 currentFrame = sprite->getCurrentFrame();

	const v8f * coords = &sprite->getFrame( currentFrame );
	
	if( sprite->isInverseHorizontal() ){
		cbPerObject.t1.x = 1-coords->x;
		cbPerObject.t2.x = 1-coords->z;
		cbPerObject.t3.x = 1-coords->a;
		cbPerObject.t4.x = 1-coords->c;
	}else{
		cbPerObject.t1.x = coords->x;
		cbPerObject.t2.x = coords->z;
		cbPerObject.t3.x = coords->a;
		cbPerObject.t4.x = coords->c;
	}

	cbPerObject.t1.y = coords->y;
	cbPerObject.t2.y = coords->w;
	cbPerObject.t3.y = coords->b;
	cbPerObject.t4.y = coords->d;

	sp->sendDataVS( &cbPerObject, 0, 0u );

	/* в D3D11 главное правильно установить слот, uniformName не нужен */
	sp->setTexture( "", 0 );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
gtD3D11LineShaderCallback::gtD3D11LineShaderCallback():
m_system( nullptr ){
	m_system = gtMainSystem::getInstance();
}

gtD3D11LineShaderCallback::~gtD3D11LineShaderCallback(){
}

void gtD3D11LineShaderCallback::onShader( const gtMaterial& material, gtShaderProcessing* sp ){

	struct cbPerObject_t{
		gtMatrix4 WVP;
		v4f p1;
		v4f p2;
		v4f color;
	}cbPerObject;

	gtMatrix4 V = m_system->getMatrixView();
	gtMatrix4 P = m_system->getMatrixProjection();

	cbPerObject.WVP =  P * V * gtMatrix4();
	cbPerObject.WVP.transpose();

	cbPerObject.p1 = s;
	cbPerObject.p2 = e;
	cbPerObject.color = c;

	sp->sendDataVS( &cbPerObject, 0, 0u );

}