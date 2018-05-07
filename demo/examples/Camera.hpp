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

	demo::DemoApplication*	m_demoApp;
	
public:

	DemoExample_Camera();
	DemoExample_Camera( demo::DemoApplication * );
	~DemoExample_Camera();

	bool Init( void );
	void Restart( void );
	void Shutdown( void );
	void Run( void );
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
{}

DemoExample_Camera::~DemoExample_Camera(){}

bool DemoExample_Camera::Init( void ){
	m_mainSystem  = gtMainSystem::getInstance();
	m_driver      = m_mainSystem->getMainVideoDriver();
	m_sceneSystem = m_mainSystem->getSceneSystem( m_driver );

	if( !m_demoApp->InitDefaultScene() ){
		return false;
	}

	auto camera = m_sceneSystem->addCamera( v3f( -10.f, 7.f, 10.f ) );
	camera->setFar( 1000.f );


	return true;
}

void DemoExample_Camera::Restart( void ){}
void DemoExample_Camera::Shutdown( void ){}

void DemoExample_Camera::Run( void ){
	while( m_mainSystem->update() ){

		m_driver->beginRender( true, gtColorDarkGray );

		m_demoApp->RenderDefaultScene();

		m_driver->endRender();
	}
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