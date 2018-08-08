#pragma once
#ifndef __GT_GUI_SYSTEM_IMPL_H__
#define __GT_GUI_SYSTEM_IMPL_H__

namespace gost{

	class gtGUISystemImpl : public gtGUISystem{
		gtMainSystem *		  m_mainSystem;
		gtInputSystem *		  m_inputSystem;
		gtGraphicsSystem *    m_gs;
		gtArray<gtPair<gtGUIObject*,u32>> m_userInputObjects;

		gtVector2<s16> m_coords;
		
		// Необходимо понимать, на каком типе элемента была нажата клавиша мыши
		// Когда клавиша отпускается, m_captureState == None
		gtGUIObjectType m_captureState;
		gtGUIObjectType m_captureState_old;

		// Тут хранится состояние о том, на что установлен фокус GUI
		gtGUIObjectType m_focusState;

		bool m_menu_is_activeItem; // Если активный хотябы 1 пункт
		void __updateMenuFirst();
		void __updateMenuSecond();

	public:

		gtGUISystemImpl();
		~gtGUISystemImpl();
		
		void addToUserInput( gtGUIObject *, u32 id );
		void replaceUserInput( gtGUIObject * object_to_replace, gtGUIObject * new_object, u32 id );
		void clearUserInput();

		gtPtr<gtGUIFont>		createFont( const gtString& fontName, gtImage * fromImage = nullptr );
		gtPtr<gtGUIFont>		createBuiltInFont();
		gtPtr<gtGUIStaticText>  createStaticText( const gtString& text, s32 positionX, s32 positionY, gtGUIFont* font );
		gtPtr<gtGUITextField>	createTextField( const v4i& rect, gtGUIFont* font, bool fixedHeight = true, bool fixedWidth = true );

		gtPtr<gtGUIShape>		createShapeRectangle( const v4i& rect, const gtColor& color, bool useGradient = false, 
			const gtColor& first_color = gtColorBlack, const gtColor& second_color = gtColorBlack, bool useVerticalGradient = false );

		gtPtr<gtGUIMenu>        createMenu( const gtGUIMenuParameters& params );

		void					removeFromUserInput( gtGUIObject * );

		void					setCurrentRenderDriver( gtGraphicsSystem * driver );
		
		void					init();
		void					update();
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