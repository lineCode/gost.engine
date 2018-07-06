#ifndef DEMO_EXAMPLE_CAMERA_H__
#define DEMO_EXAMPLE_CAMERA_H__

/*
	Show cameras
*/

class demo::DemoApplication;
class DemoExample_Camera : public demo::DemoExample{
	gtMainSystem *			m_mainSystem;
	gtInputSystem *			m_input;
	gtGraphicsSystem *				m_gs;
	gtSceneSystem*			m_sceneSystem;
	demo::DemoApplicationEventConsumer * m_eventConsumer;

	demo::DemoApplication*	m_demoApp;
	
	gtCamera *				m_cameraLookAt;
	gtCamera *				m_cameraFPS;
	gtCamera *				m_cameraFree;
	gtCamera *				m_camera2D;
	gtCamera *				m_cameraActive;

	f32 m_delta;

	u8 m_activeCameraType;

public:

	DemoExample_Camera();
	DemoExample_Camera( demo::DemoApplication * );
	~DemoExample_Camera();

	bool Init();
	void Restart();
	void Shutdown();
	void Update();
	void Input( f32 delta );
	void Render();
	void Render2D();
};

DemoExample_Camera::DemoExample_Camera():
	m_demoApp( nullptr ),
	m_gs( nullptr ),
	m_sceneSystem( nullptr ),
	m_activeCameraType( 0 )
{}

DemoExample_Camera::DemoExample_Camera( demo::DemoApplication * app ):
	m_demoApp( app ),
	m_gs( nullptr ),
	m_sceneSystem( nullptr ),
	m_activeCameraType( 0 )
{
	m_eventConsumer = m_demoApp->GetEventConsumer();
}

DemoExample_Camera::~DemoExample_Camera(){}

bool DemoExample_Camera::Init(){
	m_mainSystem  = gtMainSystem::getInstance();
	m_input		  = m_mainSystem->getInputSystem();
	m_gs          = m_mainSystem->getMainVideoDriver();
	m_sceneSystem = m_mainSystem->getSceneSystem( m_gs );

	if( !m_demoApp->InitDefaultScene() ){
		return false;
	}

	auto window = m_gs->getParams().m_outWindow;
	m_camera2D = m_sceneSystem->addCamera2D( v4f( window->getRect() ) );
	m_camera2D->setName( "2D" );
	m_camera2D->setPosition( v4f( 0.f, 1.f, -5.f ) );
	m_camera2D->setFOV( 0.2f );

	m_cameraLookAt = m_sceneSystem->addCamera( v3f( 1.47f, 2.66f, 5.75f ) );
	m_cameraLookAt->setName( "LookAt" );

	m_cameraFPS = m_sceneSystem->addCamera( v3f( 0.08f, 1.76f, 7.16f ) );
	m_cameraFPS->setName( "FPS" );

	m_cameraFree = m_sceneSystem->addCamera( v3f( -10.f, 7.f, 10.f ) );
	m_cameraFree->setName( "Free" );

	m_cameraFPS->setCameraType( gtCameraType::FPS );
	m_cameraFree->setCameraType( gtCameraType::Free );

	m_cameraActive = m_cameraLookAt;
	m_sceneSystem->setActiveCamera( m_cameraActive );

	return true;
}

void DemoExample_Camera::Restart(){}
void DemoExample_Camera::Shutdown(){
	m_sceneSystem->clearScene();
}

void DemoExample_Camera::Input( f32 d ){
	m_delta = d;
	if( m_input->isKeyDown( gtKey::K_W )
		|| m_input->isKeyDown( gtKey::K_UP )
		|| m_demoApp->inputGamepadUpHold() ){
		m_cameraActive->setPosition( m_cameraActive->getPosition() + v3f( 0.f, 0.f, 10.f * m_delta ) );
	}

	if( m_input->isKeyDown( gtKey::K_S )
		|| m_input->isKeyDown( gtKey::K_DOWN )
		|| m_demoApp->inputGamepadDownHold() ){
		m_cameraActive->setPosition( m_cameraActive->getPosition() - v3f( 0.f, 0.f, 10.f * m_delta ) );
	}

	if( m_input->isKeyDown( gtKey::K_A )
		|| m_input->isKeyDown( gtKey::K_LEFT )
		|| m_demoApp->inputGamepadLeftHold() ){
		m_cameraActive->setPosition( m_cameraActive->getPosition() + v3f( 10.f * m_delta, 0.f, 0.f ) );
	}

	if( m_input->isKeyDown( gtKey::K_D )
		|| m_input->isKeyDown( gtKey::K_RIGHT )
		|| m_demoApp->inputGamepadRightHold() ){
		m_cameraActive->setPosition( m_cameraActive->getPosition() - v3f( 10.f * m_delta, 0.f, 0.f ) );
	}

	if( m_input->isKeyDown( gtKey::K_Q )
		|| m_input->isKeyDown( gtKey::K_PGDOWN )
		|| m_demoApp->inputGamepadL1Hold() ){
		m_cameraActive->setPosition( m_cameraActive->getPosition() + v3f( 0.f, 10.f * m_delta, 0.f ) );
	}

	if( m_input->isKeyDown( gtKey::K_E )
		|| m_input->isKeyDown( gtKey::K_PGUP )
		|| m_demoApp->inputGamepadR1Hold() ){
		m_cameraActive->setPosition( m_cameraActive->getPosition() - v3f( 0.f, 10.f * m_delta, 0.f ) );
	}

	if( m_eventConsumer->keyDown( gtKey::K_ESCAPE )
		|| m_demoApp->inputGamepadMainMenuStart() ){
		m_demoApp->Pause();
	}

	if( m_eventConsumer->keyDown( gtKey::K_1 )
		|| m_demoApp->inputGamepadMainMenuEscape() ){
		if( ++m_activeCameraType > 3 )
			m_activeCameraType = 0;

		switch( m_activeCameraType ){
		case gtConst0U:
			m_cameraActive = m_cameraLookAt;
			break;
		case gtConst1U:
			m_cameraActive = m_cameraFPS;
			break;
		case gtConst2U:
			m_cameraActive = m_cameraFree;
			break;
		case gtConst3U:
			m_cameraActive = m_camera2D;
			break;
		}

		m_sceneSystem->setActiveCamera( m_cameraActive );
	}
}

void DemoExample_Camera::Render(){
}

void DemoExample_Camera::Render2D(){
}

void DemoExample_Camera::Update(){

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