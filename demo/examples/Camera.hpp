#ifndef DEMO_EXAMPLE_CAMERA_H__
#define DEMO_EXAMPLE_CAMERA_H__

/*
	Show cameras
*/

class demo::DemoApplication;
class DemoExample_Camera : public demo::DemoExample{
	gtMainSystem *			m_mainSystem;
	gtDriver *				m_driver;
	gtSceneSystem*			m_sceneSystem;
	demo::DemoApplicationEventConsumer * m_eventConsumer;

	demo::DemoApplication*	m_demoApp;
	
	gtCamera *				m_cameraLookAt;
	gtCamera *				m_cameraFPS;
	gtCamera *				m_cameraFree;
	gtCamera *				m_camera2D;
	gtCamera *				m_cameraActive;

	f32 m_delta;

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
	m_driver( nullptr ),
	m_sceneSystem( nullptr )
{}

DemoExample_Camera::DemoExample_Camera( demo::DemoApplication * app ):
	m_demoApp( app ),
	m_driver( nullptr ),
	m_sceneSystem( nullptr )
{
	m_eventConsumer = m_demoApp->GetEventConsumer();
}

DemoExample_Camera::~DemoExample_Camera(){}

bool DemoExample_Camera::Init(){
	m_mainSystem  = gtMainSystem::getInstance();
	m_driver      = m_mainSystem->getMainVideoDriver();
	m_sceneSystem = m_mainSystem->getSceneSystem( m_driver );

	if( !m_demoApp->InitDefaultScene() ){
		return false;
	}

	m_cameraLookAt = m_sceneSystem->addCamera( v3f( -10.f, 7.f, 10.f ) );

	m_cameraActive = m_cameraLookAt;


	return true;
}

void DemoExample_Camera::Restart(){}
void DemoExample_Camera::Shutdown(){
	m_sceneSystem->clearScene();
}

void DemoExample_Camera::Input( f32 d ){
	m_delta = d;
	if( m_mainSystem->isKeyPressed( gtKey::K_W )
		|| m_mainSystem->isKeyPressed( gtKey::K_UP )
		|| m_demoApp->inputGamepadMainMenuUpHold() ){
		m_cameraActive->setPosition( m_cameraActive->getPosition() + v3f( 0.f, 0.f, 10.f * m_delta ) );
	}

	if( m_mainSystem->isKeyPressed( gtKey::K_S )
		|| m_mainSystem->isKeyPressed( gtKey::K_DOWN )
		|| m_demoApp->inputGamepadMainMenuDownHold() ){
		m_cameraActive->setPosition( m_cameraActive->getPosition() - v3f( 0.f, 0.f, 10.f * m_delta ) );
	}

	if( m_mainSystem->isKeyPressed( gtKey::K_A )
		|| m_mainSystem->isKeyPressed( gtKey::K_LEFT )
		|| m_demoApp->inputGamepadMainMenuLeftHold() ){
		m_cameraActive->setPosition( m_cameraActive->getPosition() + v3f( 10.f * m_delta, 0.f, 0.f ) );
	}

	if( m_mainSystem->isKeyPressed( gtKey::K_D )
		|| m_mainSystem->isKeyPressed( gtKey::K_RIGHT )
		|| m_demoApp->inputGamepadMainMenuRightHold() ){
		m_cameraActive->setPosition( m_cameraActive->getPosition() - v3f( 10.f * m_delta, 0.f, 0.f ) );
	}

	if( m_mainSystem->isKeyPressed( gtKey::K_Q )
		|| m_mainSystem->isKeyPressed( gtKey::K_PGDOWN )){
		m_cameraActive->setPosition( m_cameraActive->getPosition() + v3f( 0.f, 10.f * m_delta, 0.f ) );
	}

	if( m_mainSystem->isKeyPressed( gtKey::K_E )
		|| m_mainSystem->isKeyPressed( gtKey::K_PGUP )){
		m_cameraActive->setPosition( m_cameraActive->getPosition() - v3f( 0.f, 10.f * m_delta, 0.f ) );
	}

	if( m_eventConsumer->keyDown( gtKey::K_ESCAPE )
		|| m_demoApp->inputGamepadMainMenuStart() ){
		m_demoApp->Pause();
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