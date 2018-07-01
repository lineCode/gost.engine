#include "common.h"

extern "C"{
	__declspec(dllexport) void	GetPluginInfo( gtPluginInfo& info ){
		info.m_author.assign( u"532235" );
		info.m_description.assign( u"Plugin for using gamepads." );
		info.m_GUID = GT_UID_INPUT_DINPUT;
		info.m_name.assign( u"DirectInput" );
		info.m_type = gtPluginType::Input;
		info.m_version = 1;
		info.m_build = 1;
	}
	__declspec(dllexport) gtInputController * gtLoadInputDriver( gtGraphicsSystemInfo params ){
		gtLogWriter::printInfo( u"Init DirectInput..." );
		gtPtr<gtInputControllerImpl>	 driver = gtPtrNew<gtInputControllerImpl>(new gtInputControllerImpl());
		
		if( driver.data() ){
			
			if( driver->init()){
				driver->addRef();
			}else{
				driver->release();
				gtLogWriter::printError( u"Can not initialize D3D11 driver." );
				return nullptr;
			}
		}
		return driver.data();
	}
}

namespace gost{

	BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
                                   VOID* pContext ){
		LPDIRECTINPUTDEVICE8 g_pJoystick = ( LPDIRECTINPUTDEVICE8 )pContext;

		static int nSliderCount = 0;
		static int nPOVCount = 0;

		if( pdidoi->dwType & DIDFT_AXIS ){
			DIPROPRANGE diprg;
			diprg.diph.dwSize = sizeof( DIPROPRANGE );
			diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
			diprg.diph.dwHow = DIPH_BYID;
			diprg.diph.dwObj = pdidoi->dwType;
			diprg.lMin = -1000;
			diprg.lMax = +1000;

			if( FAILED( g_pJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
				return DIENUM_STOP;
		}
		return DIENUM_CONTINUE;
	}

	gtInputControllerImpl::gtInputControllerImpl():
		m_mainSystem( gtMainSystem::getInstance() ),
		m_directInput( nullptr ),
		m_dll(nullptr)
	{
		m_type = gtInputDeviceType::Gamepad;
		m_context.m_di = this;
	}
	gtInputControllerImpl::~gtInputControllerImpl(){

		auto sz = m_gamepads.size();
		for( u32 i = gtConst0U; i < sz; ++i ){
			if( m_gamepads[ i ].m_gamepad ){
				m_gamepads[ i ].m_gamepad->Unacquire();
				m_gamepads[ i ].m_gamepad->Release();
				m_gamepads[ i ].m_gamepad = nullptr;
			}
		}

		if( m_directInput )
			m_directInput->Release();

		if( m_dll )
			FreeLibrary( m_dll );
	}

	LPDIRECTINPUT8 gtInputControllerImpl::getDI(){
		return m_directInput;
	}

	u32 gtInputControllerImpl::getNumOfActiveDevices(){
		return m_gamepads.size();
	}

	//BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi,
 //                                  VOID* pContext ){
	//	DI_ENUM_CONTEXT* cntx = ( DI_ENUM_CONTEXT* )pContext;

	//	static int nSliderCount = 0;
	//	static int nPOVCount = 0;

	//	if( pdidoi->dwType & DIDFT_AXIS ){
	//		DIPROPRANGE diprg;
	//		diprg.diph.dwSize = sizeof( DIPROPRANGE );
	//		diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	//		diprg.diph.dwHow = DIPH_BYID;
	//		diprg.diph.dwObj = pdidoi->dwType;
	//		diprg.lMin = -1000;
	//		diprg.lMax = +1000;

	//		if( FAILED( cntx->gamepad->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
	//			return DIENUM_STOP;

	//	}

	//	return DIENUM_CONTINUE;
	//}

	BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext ){
		DI_ENUM_CONTEXT* cntx = ( DI_ENUM_CONTEXT* )pContext;
		HRESULT hr;
		
		LPDIRECTINPUTDEVICE8    gamepad = nullptr;

		hr = cntx->m_di->getDI()->CreateDevice( pdidInstance->guidInstance, &gamepad, NULL );

		if( gamepad ){
			cntx->m_di->addGamepad( pdidInstance, gamepad );
		}else
			return DIENUM_STOP;

		return DIENUM_CONTINUE;
	}

	void gtInputControllerImpl::addGamepad( const DIDEVICEINSTANCE* pdidInstance, LPDIRECTINPUTDEVICE8 gamepad ){
		
		HRESULT hr;

		gtGameControllerDeviceImpl device;

		memcpy(&device.guid,&pdidInstance->guidInstance,sizeof(GT_GUID));
		memcpy(&device.guidManufacturer,&pdidInstance->guidProduct,sizeof(GT_GUID));

		u32 sz = m_gamepads.size();
		bool old = false;
		u32 oldID;
		for( u32 i = gtConst0U; i < sz; ++i ){
			if( m_gamepads[ i ].guid == device.guid ){

				if( m_gamepads[ i ].m_active )
					gamepad->Release();
				else{
					oldID = i;
					old = true;
					break;
				}
				return;
			}
		}
		
		device.name = pdidInstance->tszProductName;
		
		HWND hWnd = (HWND)gtMainSystem::getInstance()->getMainVideoDriver()->getParams().m_outWindow->getHandle();

		if( FAILED( hr = gamepad->GetCapabilities( &device.caps ) ) ){
			gtLogWriter::printWarning( u"Can not get gamepad capabilities. Error code: %u", hr );
		}

		if( FAILED( hr = gamepad->SetDataFormat( &c_dfDIJoystick2 ) ) ){
			gtLogWriter::printWarning( u"Can not set set data format for gamepad. Error code: %u", hr );
		}

		if( FAILED( hr = gamepad->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE |
                                                DISCL_FOREGROUND ) ) ){
			gtLogWriter::printWarning( u"Can not set cooperative level for gamepad. Error code: %u", hr );
		}

		 if( FAILED( hr = gamepad->EnumObjects( EnumObjectsCallback,
                                               ( VOID* )gamepad, DIDFT_ALL ) ) ){
			gtLogWriter::printWarning( u"Can not set enum objects. Error code: %u", hr );
		 }

		device.m_active = true;
		device.m_gamepad = gamepad;
		if( !old ){
			device.m_id   = m_gamepads.size() + gtConst1U;
			m_gamepads.push_back( device );
		}else{
			m_gamepads[ oldID ] = device;
			device.m_id  = oldID  + gtConst1U;
			m_gamepads[ oldID ].m_id = device.m_id;
		}
		device.m_gamepad->Acquire();
		
		gtEvent event;
		event.type = gtEventType::Joystick;
		event.joystickEvent.joystick = old ? &m_gamepads[ oldID ] : &m_gamepads[ m_gamepads.size() - gtConst1U ];
		event.joystickEvent.joystickEventID = GT_EVENT_JOYSTICK_ADD;
		event.joystickEvent.joystickID = device.m_id;
		gtMainSystem::getInstance()->addEvent( event );

		return;
	}

	void gtInputControllerImpl::update(){
		HRESULT hr;
		if( FAILED( hr = m_directInput->EnumDevices( DI8DEVCLASS_GAMECTRL,
                                         EnumJoysticksCallback,
                                         &m_context, DIEDFL_ATTACHEDONLY ) ) ){
		}
	}

	gtInputDevice*	gtInputControllerImpl::getInputDevice( u32 id ){
		if( m_gamepads.size() ){
			if( id < m_gamepads.size() ){
				return &m_gamepads[ id ];
			}
			return nullptr;
		}
		return nullptr;
	}

	bool gtInputControllerImpl::init(){

		gtString dinput_path = gtFileSystem::getSystemPath();
		dinput_path += u"dinput8.dll";
		if( !gtFileSystem::existFile( dinput_path ) ){
			gtLogWriter::printError( u"File not exist: %s", dinput_path.data() );
			return false;
		}

		m_dll = LoadLibrary( (wchar_t*)dinput_path.data() );
		if( !m_dll ){
			gtLogWriter::printError( u"Can not load dll: %s", dinput_path.data() );
			return false;
		}

		gt_DirectInput8Create_t gt_DirectInput8Create
			= GT_LOAD_FUNCTION_SAFE_CAST( gt_DirectInput8Create_t,m_dll, "DirectInput8Create");
		if( !gt_DirectInput8Create ){
			gtLogWriter::printError( u"Could not get proc address of DirectInput8Create");
			return false;
		}

		HRESULT hr;
		if( FAILED( hr = gt_DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION,
			IID_IDirectInput8, ( VOID** )&m_directInput, NULL ) ) ){
			gtLogWriter::printWarning( u"Can not initialize DirectInput plugin. Error code: %u", hr );
			return false;
		}

		return true;
	}
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