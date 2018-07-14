#ifndef DEMO_EXAMPLE_PHY_RAYTEST_H__
#define DEMO_EXAMPLE_PHY_RAYTEST_H__

class demo::DemoApplication;
class DemoExample_phy_raytest : public demo::DemoExample{
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

	gtRayf32                m_ray;
	v4f sc1;
	v4f sc2;
	v4f sc3;
	v4f sc4;

	v4f r1, r2, r3;

public:

	DemoExample_phy_raytest();
	DemoExample_phy_raytest( demo::DemoApplication * );
	~DemoExample_phy_raytest();

	bool Init();
	void Restart();
	void Shutdown();
	void Update();
	void Input( f32 delta );
	void Render();
	void Render2D();
};

class DemoExampleProxy_phy_raytest : public demo::DemoExampleProxy{
public:
	demo::DemoExample * allocate( demo::DemoApplication * app ){ return new DemoExample_phy_raytest( app ); }
};

DemoExample_phy_raytest::DemoExample_phy_raytest():
	m_demoApp( nullptr ),
	m_gs( nullptr ),
	m_sceneSystem( nullptr )
{}

DemoExample_phy_raytest::DemoExample_phy_raytest( demo::DemoApplication * app ):
	m_demoApp( app ),
	m_gs( nullptr ),
	m_sceneSystem( nullptr )
{
	m_eventConsumer = m_demoApp->GetEventConsumer();
}

DemoExample_phy_raytest::~DemoExample_phy_raytest(){}

bool DemoExample_phy_raytest::Init(){
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

	m_ray.m_begin.set( 0.f, 1.1f, 10.f );
	m_ray.m_end.set( 0.f, 1.1f, -10.f );

	
	
	m_cameraFPS = m_sceneSystem->addCamera( v3f( 0.08f, 2.781f, 3.0367f ) );

	if( !m_cameraFPS )
		return false;
	m_cameraFPS->setCameraType( gtCameraType::FPS );
	m_cameraFPS->setName( "FPS" );
	m_cameraFPS->setRotation( v3f( math::degToRad( -35.f ), 0.f, 0.f ) );
	m_cameraFPS->setNear( 0.1f );

	return true;
}

void DemoExample_phy_raytest::Restart(){}

void DemoExample_phy_raytest::Shutdown(){
	m_sceneSystem->clearScene();
}

void DemoExample_phy_raytest::Input( f32 d ){
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

	if( m_input->isKeyDown( gtKey::K_1 ) ){
		m_ray.m_begin -= v4f(1.f*m_delta,0.f,0.f);
		m_ray.m_end   -= v4f(1.f*m_delta,0.f,0.f);
	}

	if( m_input->isKeyDown( gtKey::K_2 ) ){
		m_ray.m_begin += v4f(1.f*m_delta,0.f,0.f);
		m_ray.m_end   += v4f(1.f*m_delta,0.f,0.f);
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

	if( m_eventConsumer->keyDownOnce( gtKey::K_F1 ) ){
		sc1 = m_mainSystem->screenToWorld( m_input->getCursorPosition() );
		r1  = m_cameraFPS->getPosition();
	}

	if( m_eventConsumer->keyDownOnce( gtKey::K_F2 ) ){
		sc2 = m_mainSystem->screenToWorld( m_input->getCursorPosition() );
	}

	if( m_eventConsumer->keyDownOnce( gtKey::K_F3 ) ){
		sc3 = m_mainSystem->screenToWorld( m_input->getCursorPosition() );
	}

	if( m_eventConsumer->keyDownOnce( gtKey::K_F4 ) ){
		sc4 = m_mainSystem->screenToWorld( m_input->getCursorPosition() );
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

void DemoExample_phy_raytest::Render(){
	m_demoApp->RenderDefaultScene();

	drawRigidBody( m_groundRigidBody.data(), m_gs );
	
	v4f hitPoint;
	v4f normal;
	auto body = m_ps->rayTest( m_ray, hitPoint, normal );

	// One hit
	if( body ){
		drawRigidBody( body, m_gs, gtColorRed );
		m_gs->drawLineSphere( hitPoint, 0.05f, 3u, gtColorRed, gtColorRed, gtColorRed );

		normal *= 0.5f;

		m_gs->drawLine( hitPoint, hitPoint + normal, gtColorLawnGreen  );
	}

	// Multiple hit
	auto arr = m_ps->rayTestMultiple(m_ray);
	for( u32 i = 0u; i < arr.size(); ++i ){
		body = arr[i].m_body;

		if( body ){
			drawRigidBody( body, m_gs, gtColorFuchsia );
			m_gs->drawLineSphere( arr[i].m_hitPoint, 0.05f, 3u, gtColorYellow, gtColorYellow, gtColorYellow );

			arr[ i ].m_normal *= 0.5f;

			m_gs->drawLine( arr[i].m_hitPoint, arr[i].m_hitPoint + arr[i].m_normal, gtColorMediumSlateBlue  );
		}
	}

	m_gs->drawLine( m_ray.m_begin, m_ray.m_end, gtColorRed );

	
	m_gs->drawLineSphere( sc1, 0.02f, 10.f, gtColorRed, gtColorGreen, gtColorBlue );
	m_gs->drawLineSphere( sc2, 0.02f, 10.f, gtColorRed, gtColorGreen, gtColorBlue );
	m_gs->drawLineSphere( sc3, 0.02f, 10.f, gtColorRed, gtColorGreen, gtColorBlue );
	m_gs->drawLineSphere( sc4, 0.02f, 10.f, gtColorRed, gtColorGreen, gtColorBlue );

	m_gs->drawLine( sc1, sc2, gtColorRed );
	m_gs->drawLine( sc3, sc4, gtColorRed );
	m_gs->drawLine( sc1, sc3, gtColorRed );
	m_gs->drawLine( sc2, sc4, gtColorRed );
	
	m_gs->drawLineSphere( r1, 0.02f, 10.f, gtColorRed, gtColorRed, gtColorRed );

//	r1 = v3f( 0.f, 0.f, 0.f );
//	r2 = v3f( 1.f, 2.f, 10.f );

//	m_gs->drawLine( r1, r2, gtColorRed );
//	m_gs->drawLine( r2, r3, gtColorLimeGreen );

}

void DemoExample_phy_raytest::Render2D(){
}

void DemoExample_phy_raytest::Update(){

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