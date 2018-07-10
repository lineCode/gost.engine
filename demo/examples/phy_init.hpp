#ifndef DEMO_EXAMPLE_PHY_INIT_H__
#define DEMO_EXAMPLE_PHY_INIT_H__

class demo::DemoApplication;
class DemoExample_phy_init : public demo::DemoExample{
	gtMainSystem *			m_mainSystem;
	gtInputSystem *			m_input;
	gtGraphicsSystem *		m_gs;
	gtSceneSystem*			m_sceneSystem;
	demo::DemoApplicationEventConsumer * m_eventConsumer;

	demo::DemoApplication*	m_demoApp;
	gtCamera *				m_cameraFPS;

	f32                     m_delta;
	gtVector2<s16>			m_oldCoord;

	gtPtr<gtPhysicsSystem>  m_ps;
	gtPtr<gtCollisionShape> m_groundShape;
	gtPtr<gtRigidBody>      m_groundRigidBody;
	gtPtr<gtCollisionShape> m_boxShape;
	gtPtr<gtRigidBody>      m_boxRigidBody;
	gtStaticObject*         m_gameObject;
	gtPtr<gtRenderModel>    m_cubeModel;

public:

	DemoExample_phy_init();
	DemoExample_phy_init( demo::DemoApplication * );
	~DemoExample_phy_init();

	bool Init();
	void Restart();
	void Shutdown();
	void Update();
	void Input( f32 delta );
	void Render();
	void Render2D();
};

class DemoExampleProxy_phy_init : public demo::DemoExampleProxy{
public:
	demo::DemoExample * allocate( demo::DemoApplication * app ){ return new DemoExample_phy_init( app ); }
};

DemoExample_phy_init::DemoExample_phy_init():
	m_demoApp( nullptr ),
	m_gs( nullptr ),
	m_sceneSystem( nullptr )
{}

DemoExample_phy_init::DemoExample_phy_init( demo::DemoApplication * app ):
	m_demoApp( app ),
	m_gs( nullptr ),
	m_sceneSystem( nullptr )
{
	m_eventConsumer = m_demoApp->GetEventConsumer();
}

DemoExample_phy_init::~DemoExample_phy_init(){}

bool DemoExample_phy_init::Init(){
	m_mainSystem  = gtMainSystem::getInstance();
	m_input		  = m_mainSystem->getInputSystem();
	m_gs          = m_mainSystem->getMainVideoDriver();
	m_sceneSystem = m_mainSystem->getSceneSystem( m_gs );

	if( !m_demoApp->InitDefaultScene() ){
		return false;
	}

	auto window = m_gs->getParams().m_outWindow;

	gtPhysicsSystemInfo psi;
	m_ps = m_mainSystem->createPhysicsSystem( psi, GT_UID_PHYSICS_BULLET_3_2_87 );
	if( m_ps ){
		if( !m_ps->initialize() ){
			return false;
		}
	}

	m_groundShape    = m_ps->createCollisionShapeBox( v3f( 10.f, 1.f, 10.f ) );
	if( !m_groundShape )
		return false;

	m_boxShape = m_ps->createCollisionShapeBox( v3f( .25f, .25f, .25f ) );
	if( !m_boxShape )
		return false;

	gtRigidBodyInfo info;
	
	info.m_mass     = 0.f;    //static
	info.m_position = v3f(0.f,0.f,0.f);
	info.m_shape    = m_groundShape.data();
	m_groundRigidBody = m_ps->createRigidBody( info );
	if( !m_groundRigidBody )
		return false;

	info.m_mass     = 3.f;    //dynamic
	info.m_position.set(0.f, 3.f, 1.f);
	info.m_rotation.set( v3f(math::degToRad( 40.f ), math::degToRad( 35.f ), 0.f) );
	info.m_shape    = m_boxShape.data();
	m_boxRigidBody = m_ps->createRigidBody( info );
	if( !m_boxRigidBody )
		return false;

	auto cube   = m_mainSystem->getModelSystem()->createCube( 0.25f );
	m_cubeModel = m_gs->createModel( cube.data() );
	m_gameObject = m_sceneSystem->addStaticObject( m_cubeModel.data(), info.m_position );
	m_gameObject->getMaterial( 0u ).flags = 0u; //disable shading
	
	m_cameraFPS = m_sceneSystem->addCamera( v3f( 0.08f, 1.76f, 2.9f ) );

	if( !m_cameraFPS )
		return false;
	m_cameraFPS->setCameraType( gtCameraType::FPS );
	m_cameraFPS->setName( "FPS" );
	m_cameraFPS->setRotation( v3f( math::degToRad( -25.f ), 0.f, 0.f ) );

	return true;
}

void DemoExample_phy_init::Restart(){}

void DemoExample_phy_init::Shutdown(){
	m_sceneSystem->clearScene();
}

void DemoExample_phy_init::Input( f32 d ){
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

	if( m_eventConsumer->keyDown( gtKey::K_1 ) ){
		gtLogWriter::printInfo( u"UP" );
		m_boxRigidBody->setPosition( v3f( 0.f, 8.f, 0.f ) );
	}

	auto coords = m_input->getCursorPosition();
	auto new_coords = coords - m_oldCoord;
	auto rot = m_cameraFPS->getRotation();
	if( m_input->isLMBDown() ){
		m_cameraFPS->setRotation( m_cameraFPS->getRotation() + 
			v3f(
				math::degToRad((f32)new_coords.y)*0.05f,
				math::degToRad((f32)new_coords.x)*0.05f,
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

	GT_FORCE_INLINE void drawRigidBody( gtRigidBody * rb, gtGraphicsSystem * gs, const gtColor& color = gtColor(1.f) ){
		auto shape = rb->getInfo().m_shape;
		auto p     = rb->getPosition();
		auto ne    = shape->getNumEdges();
		auto q     = rb->getRotation();
		gtMatrix4 M;

		math::makeRotationMatrix( M, -q );
		for( auto i = 0u; i < ne; ++i ){
		
			v3f v1, v2;
		
			shape->getEdge( i, v1, v2 );


			v1 = math::mul( v1, M );
			v2 = math::mul( v2, M );

			v1 += p;
			v2 += p;

			gs->drawLine( v1, v2, color );
		
		}
	}

void DemoExample_phy_init::Render(){
	m_demoApp->RenderDefaultScene();

	drawRigidBody( m_groundRigidBody.data(), m_gs );
	drawRigidBody( m_boxRigidBody.data(), m_gs, gtColorRed );
}

void DemoExample_phy_init::Render2D(){
}

void DemoExample_phy_init::Update(){
	m_ps->update( m_delta );

	m_gameObject->setPosition( m_boxRigidBody->getPosition() );
	auto o = m_boxRigidBody->getRotation();
	m_gameObject->setOrientation( -o );
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