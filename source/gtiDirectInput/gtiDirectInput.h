#ifndef __GT_DIRECT_INPUT_H__
#define __GT_DIRECT_INPUT_H__

namespace gost{

	class gtInputControllerImpl;
	struct DI_ENUM_CONTEXT{
		gtInputControllerImpl * m_di;
		LPDIRECTINPUTDEVICE8 gamepad;
	};

	struct gtGameControllerDeviceImpl : public gtInputDevice{
		gtGameControllerDeviceImpl():
			m_gamepad( nullptr )
		{
			caps.dwSize = sizeof(caps);
			mainSystem = gtMainSystem::getInstance();
		}

		virtual ~gtGameControllerDeviceImpl(){
		}

		gtMainSystem * mainSystem;
		LPDIRECTINPUTDEVICE8 m_gamepad;
		u32 m_id;
		DIDEVCAPS caps;
		
		u32  id(){
			return m_id;
		}
		void poll(){
			HRESULT hr;
			DIJOYSTATE2 js;

			if( m_gamepad ){
				hr = m_gamepad->Poll();
				if( FAILED( hr ) ){
					hr = m_gamepad->Acquire();
					while( hr == DIERR_INPUTLOST ){
						gtLogWriter::printInfo( u"Lost gamepad device" );
						hr = m_gamepad->Acquire();
					}
					return;
				}

				if( FAILED( hr = m_gamepad->GetDeviceState( sizeof( DIJOYSTATE2 ), &js ) ) ){
					m_active = false;
					m_gamepad->Unacquire();
					m_gamepad->Release();
					m_gamepad = nullptr;
					gtEvent event;
					event.type = gtEventType::Joystick;
					event.joystickEvent.joystick = this;
					event.joystickEvent.joystickEventID = GT_EVENT_JOYSTICK_REMOVE;
					event.joystickEvent.joystickID = m_id;
					mainSystem->addEvent( event );
					return;
				}
				for( s32 i = 0; i < 128; i++ ){
					m_buttons[ i ] = false;
					if( js.rgbButtons[i] & 0x80 ){
						m_buttons[ i ] = true;
					}

				}
				m_POV1 = js.rgdwPOV[0];
				m_POV2 = js.rgdwPOV[1];
				m_POV3 = js.rgdwPOV[2];
				m_POV4 = js.rgdwPOV[3];
				m_lX   = js.lX;
				m_lY   = js.lY;
				m_lZ   = js.lZ;
				m_lRx  = js.lRx;
				m_lRy  = js.lRy;
				m_lRz  = js.lRz;

			

			}else{
				m_active = false;
			}
		}
	};


	class gtInputControllerImpl : public gtInputController{

		gtMainSystem * m_mainSystem;

		LPDIRECTINPUT8          m_directInput;
		gtArray<gtGameControllerDeviceImpl> m_gamepads;

		HMODULE m_dll;
		DI_ENUM_CONTEXT m_context;
	public:

		gtInputControllerImpl();
		~gtInputControllerImpl();
	

		bool init();

		LPDIRECTINPUT8 getDI();
		void addGamepad( const DIDEVICEINSTANCE* pdidInstance, LPDIRECTINPUTDEVICE8 );


		u32 getNumOfActiveDevices();
		gtInputDevice*	getInputDevice( u32 id );
		void update();
	};
	

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