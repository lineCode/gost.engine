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
		cbPerObject.t1.x_ = 1-coords->x_;
		cbPerObject.t2.x_ = 1-coords->z_;
		cbPerObject.t3.x_ = 1-coords->a_;
		cbPerObject.t4.x_ = 1-coords->c_;
	}else{
		cbPerObject.t1.x_ = coords->x_;
		cbPerObject.t2.x_ = coords->z_;
		cbPerObject.t3.x_ = coords->a_;
		cbPerObject.t4.x_ = coords->c_;
	}

	cbPerObject.t1.y_ = coords->y_;
	cbPerObject.t2.y_ = coords->w_;
	cbPerObject.t3.y_ = coords->b_;
	cbPerObject.t4.y_ = coords->d_;

	sp->sendDataVS( &cbPerObject, 0, 0u );

	/* в D3D11 главное правильно установить слот, uniformName не нужен */
	sp->setTexture( "", 0 );
}