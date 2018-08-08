﻿#pragma once
#ifndef __GT_GUI_MENU_H__
#define __GT_GUI_MENU_H__ 

namespace gost{

	struct gtGUIMenuParameters{

		enum{
			flag_gradient  = BIT(0),
			flag_menuHover = BIT(1)
		};

		 gtGUIFont*      m_font    = nullptr;    //шрифт
		 u32             m_flags   = 0u; // flag_gradient - использовать градиентную заливку. m_menuColor должен быть gtColorBlack
		 
		 v2i             m_iconSize  = v2i(16,16); // Размер иконок слева  от пункта в окошке. Значение x по сути является отступом.
		 v2i             m_iconSize2 = v2i(16,16); // Размер иконок справа от пункта в окошке. 
												   // Используется для стрелки если есть ещё одно подменю, а так же как просто отступ, увеличивая окно.

		 s32             m_menuHeight          = 20;  //высота полоски меню (толщина)
		 s32             m_menuTextIndent      = 0;   // Отступ шрифта в полоске (сверху)
		 gtColor         m_menuColor           = gtColorLightGray; //Цвет полоски
		 gtColor         m_menuGradientColor1; //Если градиент включён - цвет полоски вверху
		 gtColor         m_menuGradientColor2; //Если градиент включён - цвет полоски внизу
		 gtTexture*      m_menuBackgroundTexture = nullptr; //Фон полоски. Текстура растягивается.
		 //используется при наведении курсора на область полоски - нужен flag_menuHover
		 gtColor         m_menuColorHover      = gtColorLightGray; //Цвет полоски 
		 gtColor         m_menuGradientColor1Hover; //Если градиент включён - цвет полоски вверху
		 gtColor         m_menuGradientColor2Hover; //Если градиент включён - цвет полоски внизу

		 
		 gtTexture*      m_itemHoverTexture  = nullptr; //Фон пункта при наведении курсора.
		 gtTexture*      m_itemActiveTexture = nullptr; //Фон пункта когда он активен.
		 gtColor         m_itemTextColor;  // Цвет цекста на полоске
		 gtColor         m_itemHoverColor; //Цвет пункта на полоске при наведении курсора
		 gtColor         m_itemHoverTextColor; //Цвет текста пункта на полоске при наведении курсора
		 gtColor         m_itemBackgroundColor = gtColorLightGray; //Фон окна
		 f32             m_itemHoverTransparent = 0.7f; //Прозрачность пункта на полоске при наведении курсора
		 f32             m_itemBackgroundTransparent = 0.f; //Прозрачность окна

		 gtColor         m_subitemTextColor; //Цвет текста в окошке
		 gtColor         m_subitemTextColorHover; //Цвет текста выделенного пункта в окошке

	};

	class gtGUIMenu : public gtGUIObject{
	public:

		virtual void           addElement( gtGUIObject* element, s32 id ) = 0;
		virtual gtGUIMenuItem* addMenuItem( const gtString& text, s32 userInput_id ) = 0;

		virtual void setGradientColor( const gtColor& color1, const gtColor& color2 ) = 0;
		virtual void setMouseHoverColor( const gtColor& color ) = 0;

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