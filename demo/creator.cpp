#include "creator.h"

class EventReceiver : public gtEventConsumer{
public:

	struct Context{
		Context():text(nullptr){}
		gtGUITextField * text;
	}context;

	void processEvent( const gtEvent& ev ){
		switch( ev.type ){
		case gtEventType::GUI:{
		}break;
		case gtEventType::Joystick:{
		}break;
		case gtEventType::Keyboard:{
		}break;
		case gtEventType::Mouse:{
		}break;
		case gtEventType::None:{
		}break;
		case gtEventType::System:{
			
		}break;
		case gtEventType::Window:{
			switch( ev.windowEvent.eventID ){
			case GT_EVENT_WINDOW_SIZING:
			case GT_EVENT_WINDOW_MAXIMIZE:
			case GT_EVENT_WINDOW_RESTORE:
				if( context.text ){
					gtString str = context.text->getText();
					context.text->setText( str );
				}
				break;
			}
		}break;
		}
	}
};

#if defined( GT_PLATFORM_WIN32 )
int WINAPI WinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/ ){
#endif

	EventReceiver events;

	gtDeviceCreationParameters mainParams;
//	mainParams.m_consumer = &events;
	auto mainSystem = InitializeGoSTEngine( mainParams );

	gtWindowInfo wi;
    wi.m_style |= gtWindowInfo::maximize;
    wi.m_style |= gtWindowInfo::resize;
	wi.m_style |= gtWindowInfo::center;
	
	v2i screenSize = mainSystem->getScreenSize();
	s32 y = screenSize.y;
	if( (f32)((f32)screenSize.x / (f32)y) < 1.7f ){
		while(true){
			y -= 100;
			if( (f32)((f32)screenSize.x / (f32)y) < 1.7f )
				continue;
			else break;
		}
	}

	wi.m_rect.set( 0, 0, screenSize.x, y );
	
	auto window = mainSystem->createSystemWindow( &wi );
	
	gtDriverInfo di;
	di.m_backBufferSize.set( wi.m_rect.getWidth(), wi.m_rect.getHeight() );
	di.m_outWindow = window.data(); 

	auto driver = mainSystem->createVideoDriver( di, GT_UID_RENDER_D3D11 );
	 
	 
	
	auto bg_shape = mainSystem->getGUISystem(driver.data())->createShapeRectangle( wi.m_rect, gtColor(1.f) );
	bg_shape->setTexture(driver->getTexture(u"../media/logo.png"));
	
	auto font = mainSystem->getGUISystem(driver.data())->createBuiltInFont();
	auto tf = mainSystem->getGUISystem(driver.data())->createTextField( v4i( 20, 10, screenSize.x - 20, 0 ), font.data(), false );
	tf->setBackgroundColor( gtColorBlack  );
	tf->setText( u"\tPermission is hereby granted, free of charge, to any person obtaining a copy of this software \
and associated documentation files (the \"Software\"), to deal in the Software without restriction, \
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, \
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, \
subject to the following conditions:\
\nThe above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.\
\nTHE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT \
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. \
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, \
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE \
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE." );
	tf->getBackgroundShape()->setOpacity( 0.f );
	tf->setTextColor( gtColorLightGray );
	events.context.text = tf.data();
	 
	gtEvent event;

	auto joystickSystem = mainSystem->createGameContoller( GT_UID_INPUT_DINPUT ) ;
	gtGameControllerDevice * gamepad = nullptr;
	u32 joy1ID = 0;

	while( mainSystem->update() ){

		joystickSystem->update();

		while( mainSystem->pollEvent( event ) ){
			switch( event.type ){
			case gtEventType::Joystick:{
				switch(event.joystickEvent.joystickEventID){
				case GT_EVENT_JOYSTICK_ADD:
					gamepad = event.joystickEvent.joystick;
					joy1ID = gamepad->id();
					break;
				case GT_EVENT_JOYSTICK_REMOVE:
					if( event.joystickEvent.joystickID == joy1ID ){
						gamepad = nullptr;
					}
					break;
				}
			}break;
			}
		}

		if( mainSystem->isRun() ){

			if( mainSystem->isKeyPressed( gtKey::K_ESCAPE ) ){
				mainSystem->shutdown(); 
			}

			if( gamepad ){
				gamepad->poll();
				if( gamepad->m_buttons[ 0 ] )
					mainSystem->shutdown();
			}

			driver->beginRender( true, gtColor( 1.0f, 0.0f, 0.f, 1.f ) ); // RGBA.

			driver->setDepthState( false );
			bg_shape->render();
			tf->render();
			driver->setDepthState();

			driver->endRender();
		}
	}
	return 0;
}





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