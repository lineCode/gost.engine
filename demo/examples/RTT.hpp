#ifndef DEMO_EXAMPLE_RTT_H__
#define DEMO_EXAMPLE_RTT_H__

class demo::DemoApplication;
class DemoExample_RTT : public demo::DemoExample{
	gtMainSystem *			m_mainSystem;
	gtInputSystem *			m_input;
	gtGraphicsSystem *				m_gs;
	gtSceneSystem*			m_sceneSystem;
	demo::DemoApplicationEventConsumer * m_eventConsumer;

	demo::DemoApplication*	m_demoApp;
	
	gtCamera *				m_cameraFPS;

	f32 m_delta;
	gtVector2<s16>			m_oldCoord;

	gtPtr<gtTexture>		m_RTT;

public:

	DemoExample_RTT();
	DemoExample_RTT( demo::DemoApplication * );
	~DemoExample_RTT();

	bool Init();
	void Restart();
	void Shutdown();
	void Update();
	void Input( f32 delta );
	void Render();
	void Render2D();
};

DemoExample_RTT::DemoExample_RTT():
	m_demoApp( nullptr ),
	m_gs( nullptr ),
	m_sceneSystem( nullptr )
{}

DemoExample_RTT::DemoExample_RTT( demo::DemoApplication * app ):
	m_demoApp( app ),
	m_gs( nullptr ),
	m_sceneSystem( nullptr )
{
	m_eventConsumer = m_demoApp->GetEventConsumer();
}

DemoExample_RTT::~DemoExample_RTT(){}

bool DemoExample_RTT::Init(){
	m_mainSystem  = gtMainSystem::getInstance();
	m_input		  = m_mainSystem->getInputSystem();
	m_gs          = m_mainSystem->getMainVideoDriver();
	m_sceneSystem = m_mainSystem->getSceneSystem( m_gs );

	if( !m_demoApp->InitDefaultScene() ){
		return false;
	}

	auto window = m_gs->getParams().m_outWindow;


	m_cameraFPS = m_sceneSystem->addCamera( v3f( 0.08f, 1.76f, 7.16f ) );
	m_cameraFPS->setCameraType( gtCameraType::FPS );
	m_cameraFPS->setName( "FPS" );

	m_RTT = m_gs->createRenderTargetTexture( v2u( 256u ), gtImageFormat::R8G8B8A8 );
	if( !m_RTT )
		return false;

	return true;
}

void DemoExample_RTT::Restart(){}
void DemoExample_RTT::Shutdown(){
	m_sceneSystem->clearScene();
}

void DemoExample_RTT::Input( f32 d ){
	m_delta = d;
	if( m_input->isKeyDown( gtKey::K_W )
		|| m_input->isKeyDown( gtKey::K_UP )
		|| m_demoApp->inputGamepadUpHold() ){
		m_cameraFPS->setPosition( m_cameraFPS->getPosition() + v3f( 0.f, 0.f, 10.f * m_delta ) );
	}

	if( m_input->isKeyDown( gtKey::K_S )
		|| m_input->isKeyDown( gtKey::K_DOWN )
		|| m_demoApp->inputGamepadDownHold() ){
		m_cameraFPS->setPosition( m_cameraFPS->getPosition() - v3f( 0.f, 0.f, 10.f * m_delta ) );
	}

	if( m_input->isKeyDown( gtKey::K_A )
		|| m_input->isKeyDown( gtKey::K_LEFT )
		|| m_demoApp->inputGamepadLeftHold() ){
		m_cameraFPS->setPosition( m_cameraFPS->getPosition() + v3f( 10.f * m_delta, 0.f, 0.f ) );
	}

	if( m_input->isKeyDown( gtKey::K_D )
		|| m_input->isKeyDown( gtKey::K_RIGHT )
		|| m_demoApp->inputGamepadRightHold() ){
		m_cameraFPS->setPosition( m_cameraFPS->getPosition() - v3f( 10.f * m_delta, 0.f, 0.f ) );
	}

	if( m_input->isKeyDown( gtKey::K_Q )
		|| m_input->isKeyDown( gtKey::K_PGDOWN )
		|| m_demoApp->inputGamepadL1Hold() ){
		m_cameraFPS->setPosition( m_cameraFPS->getPosition() + v3f( 0.f, 10.f * m_delta, 0.f ) );
	}

	if( m_input->isKeyDown( gtKey::K_E )
		|| m_input->isKeyDown( gtKey::K_PGUP )
		|| m_demoApp->inputGamepadR1Hold() ){
		m_cameraFPS->setPosition( m_cameraFPS->getPosition() - v3f( 0.f, 10.f * m_delta, 0.f ) );
	}

	if( m_eventConsumer->keyDown( gtKey::K_ESCAPE )
		|| m_demoApp->inputGamepadMainMenuStart() ){
		m_demoApp->Pause();
	}

	auto coords = m_input->getCursorPosition();
	auto new_coords = coords - m_oldCoord;
	auto rot = m_cameraFPS->getRotation();
	if( m_input->isLMBDown() ){
		m_cameraFPS->setRotation( m_cameraFPS->getRotation() + 
			v3f(
				-math::degToRad((f32)new_coords.y)*0.05f,
				-math::degToRad((f32)new_coords.x)*0.05f,
				0.000f
			)
		);
	}

	if( m_input->isRMBDown() ){
		m_cameraFPS->setRotation( m_cameraFPS->getRotation() + 
			v3f(
				0.f,
				0.f,
				math::degToRad((f32)new_coords.x)*0.05f
			)
		);
	}

	m_oldCoord = coords;
}

void DemoExample_RTT::Render(){
	m_gs->setRenderTarget( m_RTT.data() );
	m_demoApp->RenderDefaultScene();      //render to texture
	m_gs->setRenderTarget( nullptr, true, true, m_demoApp->GetDefaultClearColor() );

	m_demoApp->RenderDefaultScene();  //render to screen
}

void DemoExample_RTT::Render2D(){
	m_gs->draw2DImage( v4i(0, 0, 250, 250), m_RTT.data() );
}

void DemoExample_RTT::Update(){

}


#endif

/*
Copyright (c) 2018 532235

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