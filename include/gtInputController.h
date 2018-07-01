#pragma once
#ifndef __GT_GAME_CONTROLLER_H__
#define __GT_GAME_CONTROLLER_H__

namespace gost{

	enum class gtInputDeviceType : u32 {
		Keyboard,
		Mouse,
		Gamepad,
		SteeringWheel,
		Joystick
	};

	struct gtInputDevice{
		gtInputDevice():
			m_POV1(gtConst_1),
			m_POV2(gtConst_1),
			m_POV3(gtConst_1),
			m_POV4(gtConst_1),
			m_lX(gtConst0),
			m_lY(gtConst0),
			m_lZ(gtConst0),
			m_lRx(gtConst0),
			m_lRy(gtConst0),
			m_lRz(gtConst0),
			m_active( false )
		{
			memset(m_buttons,gtConst0,gtConst256U);
		}

		virtual ~gtInputDevice(){}

		GT_GUID guid;
		GT_GUID guidManufacturer;
		gtString name;
		u8 m_buttons[ gtConst256U ];
		s32 m_POV1;
		s32 m_POV2;
		s32 m_POV3;
		s32 m_POV4;
		s32 m_lX;
		s32 m_lY;
		s32 m_lZ;
		s32 m_lRx;
		s32 m_lRy;
		s32 m_lRz;
		bool m_active;

		virtual void poll() = 0;
		virtual u32  id() = 0;
	};

	class gtInputController : public gtRefObject{
	protected:
		gtInputDeviceType m_type;
	public:
		virtual ~gtInputController(){}
		gtInputDeviceType getType() const { return m_type; }

		virtual u32 getNumOfActiveDevices() = 0;
		virtual gtInputDevice*	getInputDevice( u32 id ) = 0;
		virtual void update() = 0;

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