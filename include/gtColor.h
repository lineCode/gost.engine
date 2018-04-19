/*!	GOST
	\file gtColor.h
	\brief color
*/

#pragma once
#ifndef __GT_COLOR_H__
#define __GT_COLOR_H__ //< include guard

namespace gost{
		
		//	char to float
		//	255 / 255 = 1.F // ok
		//	0 / 255 - bad
		//	0 * some = 0
		//	255	* 0.00392156862745 = 0.99999999999975 // good
	constexpr f32 gtColorDivider = 0.00392156862745f;

		//	HTML colors
	constexpr u32 gtColorWhite = 0xffffffff;
	constexpr u32 gtColorAliceBlue = 0xffF0F8FF;
	constexpr u32 gtColorAntiqueWhite = 0xffFAEBD7;
	constexpr u32 gtColorAqua = 0xff00FFFF;
	constexpr u32 gtColorAquamarine = 0xff7FFFD4;
	constexpr u32 gtColorAzure = 0xffF0FFFF;
	constexpr u32 gtColorBeige = 0xffF5F5DC;
	constexpr u32 gtColorBisque = 0xffFFE4C4;
	constexpr u32 gtColorBlack = 0xff000000;
	constexpr u32 gtColorBlanchedAlmond  = 0xffFFEBCD;
	constexpr u32 gtColorBlue = 0xff0000FF;
	constexpr u32 gtColorBlueViolet = 0xff8A2BE2;
	constexpr u32 gtColorBrown = 0xffA52A2A;
	constexpr u32 gtColorBurlyWood = 0xffDEB887;
	constexpr u32 gtColorCadetBlue = 0xff5F9EA0;
	constexpr u32 gtColorChartreuse = 0xff7FFF00;
	constexpr u32 gtColorChocolate = 0xffD2691E;
	constexpr u32 gtColorCoral = 0xffFF7F50;
	constexpr u32 gtColorCornflowerBlue = 0xff6495ED;
	constexpr u32 gtColorCornsilk = 0xffFFF8DC;
	constexpr u32 gtColorCrimson = 0xffDC143C;
	constexpr u32 gtColorCyan = 0xff00FFFF;
	constexpr u32 gtColorDarkBlue = 0xff00008B;
	constexpr u32 gtColorDarkCyan = 0xff008B8B;
	constexpr u32 gtColorDarkGoldenRod = 0xffB8860B;
	constexpr u32 gtColorDarkGray = 0xffA9A9A9;
	constexpr u32 gtColorDarkGrey = 0xffA9A9A9;
	constexpr u32 gtColorDarkGreen = 0xff006400;
	constexpr u32 gtColorDarkKhaki = 0xffBDB76B;
	constexpr u32 gtColorDarkMagenta = 0xff8B008B;
	constexpr u32 gtColorDarkOliveGreen = 0xff556B2F;
	constexpr u32 gtColorDarkOrange = 0xffFF8C00;
	constexpr u32 gtColorDarkOrchid = 0xff9932CC;
	constexpr u32 gtColorDarkRed = 0xff8B0000;
	constexpr u32 gtColorDarkSalmon = 0xffE9967A;
	constexpr u32 gtColorDarkSeaGreen = 0xff8FBC8F;
	constexpr u32 gtColorDarkSlateBlue = 0xff483D8B;
	constexpr u32 gtColorDarkSlateGray = 0xff2F4F4F;
	constexpr u32 gtColorDarkSlateGrey = 0xff2F4F4F;
	constexpr u32 gtColorDarkTurquoise = 0xff00CED1;
	constexpr u32 gtColorDarkViolet = 0xff9400D3;
	constexpr u32 gtColorDeepPink = 0xffFF1493;
	constexpr u32 gtColorDeepSkyBlue = 0xff00BFFF;
	constexpr u32 gtColorDimGray = 0xff696969;
	constexpr u32 gtColorDimGrey = 0xff696969;
	constexpr u32 gtColorDodgerBlue = 0xff1E90FF;
	constexpr u32 gtColorFireBrick = 0xffB22222;
	constexpr u32 gtColorFloralWhite = 0xffFFFAF0;
	constexpr u32 gtColorForestGreen = 0xff228B22;
	constexpr u32 gtColorFuchsia = 0xffFF00FF;
	constexpr u32 gtColorGainsboro = 0xffDCDCDC;
	constexpr u32 gtColorGhostWhite = 0xffF8F8FF;
	constexpr u32 gtColorGold = 0xffFFD700;
	constexpr u32 gtColorGoldenRod = 0xffDAA520;
	constexpr u32 gtColorGray = 0xff808080;
	constexpr u32 gtColorGrey = 0xff808080;
	constexpr u32 gtColorGreen = 0xff008000;
	constexpr u32 gtColorGreenYellow = 0xffADFF2F;
	constexpr u32 gtColorHoneyDew = 0xffF0FFF0;
	constexpr u32 gtColorHotPink = 0xffFF69B4;
	constexpr u32 gtColorIndianRed = 0xffCD5C5C;
	constexpr u32 gtColorIndigo = 0xff4B0082;
	constexpr u32 gtColorIvory = 0xffFFFFF0;
	constexpr u32 gtColorKhaki = 0xffF0E68C;
	constexpr u32 gtColorLavender = 0xffE6E6FA;
	constexpr u32 gtColorLavenderBlush = 0xffFFF0F5;
	constexpr u32 gtColorLawnGreen = 0xff7CFC00;
	constexpr u32 gtColorLemonChiffon = 0xffFFFACD;
	constexpr u32 gtColorLightBlue = 0xffADD8E6;
	constexpr u32 gtColorLightCoral = 0xffF08080;
	constexpr u32 gtColorLightCyan = 0xffE0FFFF;
	constexpr u32 gtColorLightGoldenRodYellow = 0xffFAFAD2;
	constexpr u32 gtColorLightGray = 0xffD3D3D3;
	constexpr u32 gtColorLightGrey = 0xffD3D3D3;
	constexpr u32 gtColorLightGreen = 0xff90EE90;
	constexpr u32 gtColorLightPink = 0xffFFB6C1;
	constexpr u32 gtColorLightSalmon = 0xffFFA07A;
	constexpr u32 gtColorLightSeaGreen = 0xff20B2AA;
	constexpr u32 gtColorLightSkyBlue = 0xff87CEFA;
	constexpr u32 gtColorLightSlateGray = 0xff778899;
	constexpr u32 gtColorLightSlateGrey = 0xff778899;
	constexpr u32 gtColorLightSteelBlue = 0xffB0C4DE;
	constexpr u32 gtColorLightYellow = 0xffFFFFE0;
	constexpr u32 gtColorLime = 0xff00FF00;
	constexpr u32 gtColorLimeGreen = 0xff32CD32;
	constexpr u32 gtColorLinen = 0xffFAF0E6;
	constexpr u32 gtColorMagenta = 0xffFF00FF;
	constexpr u32 gtColorMaroon = 0xff800000;
	constexpr u32 gtColorMediumAquaMarine = 0xff66CDAA;
	constexpr u32 gtColorMediumBlue = 0xff0000CD;
	constexpr u32 gtColorMediumOrchid = 0xffBA55D3;
	constexpr u32 gtColorMediumPurple = 0xff9370DB;
	constexpr u32 gtColorMediumSeaGreen = 0xff3CB371;
	constexpr u32 gtColorMediumSlateBlue = 0xff7B68EE;
	constexpr u32 gtColorMediumSpringGreen = 0xff00FA9A;
	constexpr u32 gtColorMediumTurquoise = 0xff48D1CC;
	constexpr u32 gtColorMediumVioletRed = 0xffC71585;
	constexpr u32 gtColorMidnightBlue = 0xff191970;
	constexpr u32 gtColorMintCream = 0xffF5FFFA;
	constexpr u32 gtColorMistyRose = 0xffFFE4E1;
	constexpr u32 gtColorMoccasin = 0xffFFE4B5;
	constexpr u32 gtColorNavajoWhite = 0xffFFDEAD;
	constexpr u32 gtColorNavy = 0xff000080;
	constexpr u32 gtColorOldLace = 0xffFDF5E6;
	constexpr u32 gtColorOlive = 0xff808000;
	constexpr u32 gtColorOliveDrab = 0xff6B8E23;
	constexpr u32 gtColorOrange = 0xffFFA500;
	constexpr u32 gtColorOrangeRed = 0xffFF4500;
	constexpr u32 gtColorOrchid = 0xffDA70D6;
	constexpr u32 gtColorPaleGoldenRod = 0xffEEE8AA;
	constexpr u32 gtColorPaleGreen = 0xff98FB98;
	constexpr u32 gtColorPaleTurquoise = 0xffAFEEEE;
	constexpr u32 gtColorPaleVioletRed = 0xffDB7093;
	constexpr u32 gtColorPapayaWhip = 0xffFFEFD5;
	constexpr u32 gtColorPeachPuff = 0xffFFDAB9;
	constexpr u32 gtColorPeru = 0xffCD853F;
	constexpr u32 gtColorPink = 0xffFFC0CB;
	constexpr u32 gtColorPlum = 0xffDDA0DD;
	constexpr u32 gtColorPowderBlue = 0xffB0E0E6;
	constexpr u32 gtColorPurple = 0xff800080;
	constexpr u32 gtColorRebeccaPurple = 0xff663399;
	constexpr u32 gtColorRed = 0xffFF0000;
	constexpr u32 gtColorRosyBrown = 0xffBC8F8F;
	constexpr u32 gtColorRoyalBlue = 0xff4169E1;
	constexpr u32 gtColorSaddleBrown = 0xff8B4513;
	constexpr u32 gtColorSalmon = 0xffFA8072;
	constexpr u32 gtColorSandyBrown = 0xffF4A460;
	constexpr u32 gtColorSeaGreen = 0xff2E8B57;
	constexpr u32 gtColorSeaShell = 0xffFFF5EE;
	constexpr u32 gtColorSienna = 0xffA0522D;
	constexpr u32 gtColorSilver = 0xffC0C0C0;
	constexpr u32 gtColorSkyBlue = 0xff87CEEB;
	constexpr u32 gtColorSlateBlue = 0xff6A5ACD;
	constexpr u32 gtColorSlateGray = 0xff708090;
	constexpr u32 gtColorSlateGrey = 0xff708090;
	constexpr u32 gtColorSnow = 0xffFFFAFA;
	constexpr u32 gtColorSpringGreen = 0xff00FF7F;
	constexpr u32 gtColorSteelBlue = 0xff4682B4;
	constexpr u32 gtColorTan = 0xffD2B48C;
	constexpr u32 gtColorTeal = 0xff008080;
	constexpr u32 gtColorThistle = 0xffD8BFD8;
	constexpr u32 gtColorTomato = 0xffFF6347;
	constexpr u32 gtColorTurquoise = 0xff40E0D0;
	constexpr u32 gtColorViolet = 0xffEE82EE;
	constexpr u32 gtColorWheat = 0xffF5DEB3;
	constexpr u32 gtColorWhiteSmoke = 0xffF5F5F5;
	constexpr u32 gtColorYellow = 0xffFFFF00;
	constexpr u32 gtColorYellowGreen = 0xff9ACD32;

		//	цвет
	class gtColor{

			//	данные
		f32	m_data[ 4u ];


	public:
			//	конструктор по умолчанию
		gtColor( void ){
			//	r g b a - для лучшей совместимости с d3d11
			m_data[ 0u ] = m_data[ 1u ] = m_data[ 2u ] = 0.f;
			m_data[ 3u ] = 1.f;
		}

			//	устаналивает цвет одним значением
			// \param v: value
		gtColor( f32 v ){
			m_data[ 0u ] = m_data[ 1u ] = m_data[ 2u ] = v;
			m_data[ 3u ] = 1.f;
		}

			//	устанавливает цвет указанными значениями
			// \param r: red component
			// \param g: green component
			// \param b: blue component
			// \param a: alpha component
		gtColor( f32 r, f32 g, f32 b, f32 a = 1.f ){
			m_data[ 0u ] = r;
			m_data[ 1u ] = g;
			m_data[ 2u ] = b;
			m_data[ 3u ] = a;
		}

			//	устанавливает цвет указанными значениями
			//	для избежания неоднозначности можно сделать так
			//	gtColor c( u8(255), 255,255,255);
			// \param r: red component
			// \param g: green component
			// \param b: blue component
			// \param a: alpha component
		gtColor( s32 r, s32 g, s32 b, s32 a = 255 ){
			this->setAsByteAlpha( a );
			this->setAsByteRed( r );
			this->setAsByteGreen( g );
			this->setAsByteBlue( b );
		}

			//	set color from u32
			// \param uint_data: color (0xFFFFFfFF)
			// \attention нужно всё перепроверить, не критично, по этому пока за это не брался.
		gtColor( u32 uint_data ){
			setAsInteger( uint_data );
		}

			//	возвратит красный цвет
			//	\return red component
		const f32 getRed( void ) const {
			return m_data[ 0u ];
		}

			//	возвратит зелёный цвет
			//	\return green component
		const f32 getGreen( void ) const {
			return m_data[ 1u ];
		}

			//	возвратит синий цвет
			//	\return blue component
		const f32 getBlue( void ) const {
			return m_data[ 2u ];
		}

			//	возвратит значение альфа-канала
			//	\return alpha component
		const f32 getAlpha( void ) const {
			return m_data[ 3u ];
		}

			//	указатель на массив
			//	\return pointer
		const f32 * getData( void ) const {
			return &m_data[ 0u ];
		}

			//	возвратит красный цвет
			//	\return red component
		const u8 getAsByteRed( void ) const {
			return static_cast<u8>( m_data[ 0u ] * 255.f );
		}

			//	возвратит зелёный цвет
			//	\return green component
		const u8 getAsByteGreen( void ) const {
			return static_cast<u8>( m_data[ 1u ] * 255.f );
		}

			//	возвратит синий цвет
			//	\return blue component
		const u8 getAsByteBlue( void ) const {
			return static_cast<u8>( m_data[ 2u ] * 255.f );
		}

			//	возвратит значение альфа-канала
			//	\return alpha component
		const u8 getAsByteAlpha( void ) const {
			return static_cast<u8>( m_data[ 3u ] * 255.f );
		}

			//	получить в виде целого типа (0xffffffff)
			//	не уверен что правильная последовательность
			//	придёт время протестирую
			//	\return u32 color
		u32 getAsInteger( void ){
			return GT_MAKEFOURCC(
				this->getAsByteBlue(),
				this->getAsByteGreen(),
				this->getAsByteRed(),
				this->getAsByteAlpha()
			);
		}

			//	установить значение альфа-канала
			//	\param v: alpha component
		void setAlpha( f32 v ){	m_data[ 3u ] = v;	}

			//	установить значение красного цвета
			//	\param v: red component
		void setRed( f32 v ){	m_data[ 0u ] = v;	}

			//	установить знаечние зелёного цвета
			//	\param v: green component
		void setGreen( f32 v ){	m_data[ 1u ] = v;	}

			//	установить значение синего цвета
			//	\param v: blue component
		void setBlue( f32 v ){	m_data[ 2u ] = v;	}

			//	установить значение альфа-канала
			//	\param v: alpha component
		void setAsByteAlpha( s32 v ){
			m_data[ 3u ] = static_cast<f32>(v) * gtColorDivider;
		}

			//	установить значение красного цвета
			//	\param v: red component
		void setAsByteRed( s32 v ){
			m_data[ 0u ] = static_cast<f32>(v) * gtColorDivider;
		}

			//	установить знаечние зелёного цвета
			//	\param v: green component
		void setAsByteGreen( s32 v ){
			m_data[ 1u ] = static_cast<f32>(v) * gtColorDivider;
		}

			//	установить значение синего цвета
			//	\param v: blue component
		void setAsByteBlue( s32 v ){
			m_data[ 2u ] = static_cast<f32>(v) * gtColorDivider;
		}

			//	argb
			//	\param v: u32 color
		void setAsInteger( u32 v ){
			this->setAsByteRed( static_cast<u8>( v >> 16 ) );
			this->setAsByteGreen( static_cast<u8>( v >> 8 ) );
			this->setAsByteBlue( static_cast<u8>( v ) );
			this->setAsByteAlpha( static_cast<u8>( v >> 24 ) );
		}

			//	установить значением float
			// \param r: red component
			// \param g: green component
			// \param b: blue component
			// \param z: alpha component
		void set( f32 r, f32 g, f32 b, f32 a = 1.f ){
			setAlpha( a );
			setRed( r );
			setGreen( g );
			setBlue( b );
		}

			//	установить значением float.
			//	одно значение на каждый цвет
			// \param v: rgb components
			// \param a: alpha component
		void set( f32 v, f32 a = 1.f ){
			setAlpha( a );
			setRed( v );
			setGreen( v );
			setBlue( v );
		}
	};

	namespace util{

			//	print color
		GT_FORCE_INLINE void printColor( const gtColor& c ){
			gtLogWriter::printInfo( u"Color:" );
			const f32 * data = c.getData();
			for( u32 i{ 0u }; i < 4u; ++i )
				gtLogWriter::printInfo( u"\t\t%f", data[ i ] );
		}

	}

}

#endif

/*
Copyright (c) 2017-2018 532235

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