#ifndef DEMO_EXAMPLE_PHY_PICKING_H__
#define DEMO_EXAMPLE_PHY_PICKING_H__

class demo::DemoApplication;
class DemoExample_phy_picking : public demo::DemoExample{
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
	gtPtr<gtRigidBody>      m_boxRigidBody[CUB_NUM_X][CUB_NUM_Y][CUB_NUM_Z];
	
	gtPtr<gtRenderModel>    m_cubeModel;
	gtStaticObject*         m_cubeObjects[CUB_NUM_X][CUB_NUM_Y][CUB_NUM_Z];

	gtRayf32                m_rayPick;
	v4f                     m_hitPoint;
	v4f                     m_hitPointSave;
	v4f                     m_hitNormal;
	gtRigidBody         *   m_pickedBody;
	f32                     m_distance;
public:

	DemoExample_phy_picking();
	DemoExample_phy_picking( demo::DemoApplication * );
	~DemoExample_phy_picking();

	bool Init();
	void Restart();
	void Shutdown();
	void Update();
	void Input( f32 delta );
	void Render();
	void Render2D();
};

class DemoExampleProxy_phy_picking : public demo::DemoExampleProxy{
public:
	demo::DemoExample * allocate( demo::DemoApplication * app ){ return new DemoExample_phy_picking( app ); }
};

DemoExample_phy_picking::DemoExample_phy_picking():
	m_demoApp( nullptr ),
	m_gs( nullptr ),
	m_sceneSystem( nullptr ),
	m_pickedBody( nullptr )
{}

DemoExample_phy_picking::DemoExample_phy_picking( demo::DemoApplication * app ):
	m_demoApp( app ),
	m_gs( nullptr ),
	m_sceneSystem( nullptr ),
	m_pickedBody( nullptr )
{
	m_eventConsumer = m_demoApp->GetEventConsumer();
}

DemoExample_phy_picking::~DemoExample_phy_picking(){}

bool DemoExample_phy_picking::Init(){
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

	const f32 cubeSize = 0.1f;

	m_boxShape = m_ps->createCollisionShapeBox( v3f( cubeSize, cubeSize, cubeSize ) );
	if( !m_boxShape )
		return false;

	gtRigidBodyInfo info;
	
	info.m_mass     = 0.f;    //static
	info.m_position = v3f(0.f,0.f,0.f);
	info.m_shape    = m_groundShape.data();
	m_groundRigidBody = m_ps->createRigidBody( info );
	if( !m_groundRigidBody )
		return false;

	auto cube   = m_mainSystem->getModelSystem()->createCube( cubeSize );
	m_cubeModel = m_gs->createModel( cube.data() );

	std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<f32> dist(0.f, 1.f);

	auto defaultTexture = m_gs->getDefaultTexture();

	for( s32 i = 0; i < CUB_NUM_X; ++i ){
		for( s32 o = 0; o < CUB_NUM_Y; ++o ){
			for( s32 k = 0; k < CUB_NUM_Z; ++k ){
				m_cubeObjects[ i ][ o ][ k ] = m_sceneSystem->addStaticObject( m_cubeModel.data(), info.m_position );
				m_cubeObjects[ i ][ o ][ k ]->getMaterial( 0u ).flags = 0u; //disable shading
				m_cubeObjects[ i ][ o ][ k ]->getMaterial( 0u ).textureLayer[ 0 ].texture = defaultTexture;
				m_cubeObjects[ i ][ o ][ k ]->getMaterial( 0u ).textureLayer[ 0 ].diffuseColor.setRed( dist(mt) );
				m_cubeObjects[ i ][ o ][ k ]->getMaterial( 0u ).textureLayer[ 0 ].diffuseColor.setGreen( dist(mt) );
				m_cubeObjects[ i ][ o ][ k ]->getMaterial( 0u ).textureLayer[ 0 ].diffuseColor.setBlue( dist(mt) );
			}
		}
	}

	const f32 Y_pos = 2.f;

	info.m_mass     = 3.f;    //dynamic
	info.m_position.set(0.f, Y_pos, 1.f);
//	info.m_rotation.set( v3f(math::degToRad( 40.f ), math::degToRad( 35.f ), 0.f) );
	info.m_shape    = m_boxShape.data();

	for( s32 i = 0; i < CUB_NUM_X; ++i ){
		for( s32 o = 0; o < CUB_NUM_Y; ++o ){
			for( s32 k = 0; k < CUB_NUM_Z; ++k ){

				info.m_position.set( 0.2f * o, Y_pos+0.2f*i, 0.2f * k );

				m_boxRigidBody[ i ][ o ][ k ] = m_ps->createRigidBody( info );
				if( !m_boxRigidBody[ i ][ o ][ k ] )
					return false;

				m_boxRigidBody[ i ][ o ][ k ]->setUserData( m_cubeObjects[ i ][ o ][ k ] );
			}
		}
	}

	
	m_cameraFPS = m_sceneSystem->addCamera( v3f( 0.08f, 2.781f, 3.0367f ) );

	if( !m_cameraFPS )
		return false;
	m_cameraFPS->setCameraType( gtCameraType::FPS );
	m_cameraFPS->setName( "FPS" );
	m_cameraFPS->setRotation( v3f( math::degToRad( -35.f ), 0.f, 0.f ) );
	m_cameraFPS->setNear( 1.0f );

	return true;
}

void DemoExample_phy_picking::Restart(){}

void DemoExample_phy_picking::Shutdown(){
	m_sceneSystem->clearScene();
}

void DemoExample_phy_picking::Input( f32 d ){
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

	if( m_eventConsumer->keyDownOnce( gtKey::K_ESCAPE )
		|| m_demoApp->inputGamepadMainMenuStart() ){
		m_demoApp->Pause();
	}

	if( m_input->isKeyDown( gtKey::K_3 ) ){
		for( s32 i = 0; i < CUB_NUM_X; ++i ){
			for( s32 o = 0; o < CUB_NUM_Y; ++o ){
				for( s32 k = 0; k < CUB_NUM_Z; ++k ){
					m_boxRigidBody[ i ][ o ][ k ]->setPosition(v3f(0.2f * o, 10.f+0.2f*i, 0.2f * k));
					m_boxRigidBody[ i ][ o ][ k ]->setRotation( gtQuaternion() );
				}
			}
		}
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



void DemoExample_phy_picking::Render(){
	m_demoApp->RenderDefaultScene();

	drawRigidBody( m_groundRigidBody.data(), m_gs );
	
	m_rayPick = m_mainSystem->getRayFromScreen( m_input->getCursorPosition() );

	auto body = m_ps->rayTest( m_rayPick, m_hitPoint, m_hitNormal );
	if( m_input->isKeyDown( gtKey::K_SPACE ) ){
		if( body ){

			if( !m_pickedBody ){

				m_pickedBody   = body;
				m_distance     = (m_hitPoint.distance( m_rayPick.m_begin ) );
				m_hitPointSave = m_hitPoint;
			}
		}

		if( m_pickedBody ){

			m_pickedBody->activate( true );

			m_hitPointSave = m_pickedBody->getPosition();

			v4f newPoint = m_cameraFPS->getPosition() + ( m_rayPick.m_end - m_cameraFPS->getPosition() ).normalize() * m_distance;
			if( newPoint != m_hitPointSave ){
				v4f vectorNormal = newPoint - m_hitPointSave;
				vectorNormal *= 4.f;

				m_pickedBody->setLinearVelocity( vectorNormal );
				
			}
			
			m_gs->drawLineSphere( newPoint, 0.05f, 6u, gtColorYellow, gtColorRed, gtColorRed );

		}

	}else m_pickedBody = nullptr;


	m_gs->drawLineSphere( m_hitPoint, 0.05f, 6u, gtColorRed, gtColorRed, gtColorRed );

}

void DemoExample_phy_picking::Render2D(){
}

void DemoExample_phy_picking::Update(){

	m_ps->update( m_delta );

	for( s32 i = 0; i < CUB_NUM_X; ++i ){
		for( s32 o = 0; o < CUB_NUM_Y; ++o ){
			for( s32 k = 0; k < CUB_NUM_Z; ++k ){
				m_cubeObjects[ i ][ o ][ k ]->setPosition( m_boxRigidBody[ i ][ o ][ k ]->getPosition() );
				auto q = m_boxRigidBody[ i ][ o ][ k ]->getRotation();
				m_cubeObjects[ i ][ o ][ k ]->setOrientation( -q );
			}
		}
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