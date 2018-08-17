#pragma once
#ifndef __GT_COLOR_H__
#define __GT_COLOR_H__ //< include guard

namespace gost{
		
namespace math{
		//	char to float
		//	255 / 255 = 1.F // ok
		//	0 / 255 - bad
		//	0 * some = 0
		//	255	* 0.00392156862745 = 0.99999999999975 // good
	constexpr f32 gtColorDivider = 0.00392156862745f;
}

	class gtColor{

		f32	m_data[ 4u ];


	public:
		gtColor(){
			m_data[ 0u ] = m_data[ 1u ] = m_data[ 2u ] = 0.f;
			m_data[ 3u ] = 1.f;
		}

		gtColor( f32 v ){
			m_data[ 0u ] = m_data[ 1u ] = m_data[ 2u ] = v;
			m_data[ 3u ] = 1.f;
		}

		gtColor( f32 r, f32 g, f32 b, f32 a = 1.f ){
			m_data[ 0u ] = r;
			m_data[ 1u ] = g;
			m_data[ 2u ] = b;
			m_data[ 3u ] = a;
		}

		gtColor( s32 r, s32 g, s32 b, s32 a = 255 ){
			this->setAsByteAlpha( a );
			this->setAsByteRed( r );
			this->setAsByteGreen( g );
			this->setAsByteBlue( b );
		}

		gtColor( u32 uint_data ){
			setAsInteger( uint_data );
		}

		const f32 * getData() const { return &m_data[ 0u ]; }

		const f32 getRed() const   { return m_data[ 0u ]; }
		const f32 getGreen() const { return m_data[ 1u ]; }
		const f32 getBlue() const  { return m_data[ 2u ]; }
		const f32 getAlpha() const { return m_data[ 3u ]; }

		const u8 getAsByteRed() const   { return static_cast<u8>( m_data[ 0u ] * 255.f ); }
		const u8 getAsByteGreen() const { return static_cast<u8>( m_data[ 1u ] * 255.f ); }
		const u8 getAsByteBlue() const  { return static_cast<u8>( m_data[ 2u ] * 255.f ); }
		const u8 getAsByteAlpha() const { return static_cast<u8>( m_data[ 3u ] * 255.f ); }

		u32 getAsInteger(){
			return GT_MAKEFOURCC(
				this->getAsByteBlue(),
				this->getAsByteGreen(),
				this->getAsByteRed(),
				this->getAsByteAlpha()
			);
		}

		void setAlpha( f32 v ){ m_data[ 3u ] = v;	}
		void setRed( f32 v )  { m_data[ 0u ] = v;	}
		void setGreen( f32 v ){ m_data[ 1u ] = v;	}
		void setBlue( f32 v ) { m_data[ 2u ] = v;	}

		void setAsByteAlpha( s32 v ){ m_data[ 3u ] = static_cast<f32>(v) * math::gtColorDivider; }
		void setAsByteRed( s32 v )  { m_data[ 0u ] = static_cast<f32>(v) * math::gtColorDivider; }
		void setAsByteGreen( s32 v ){ m_data[ 1u ] = static_cast<f32>(v) * math::gtColorDivider; }
		void setAsByteBlue( s32 v ) { m_data[ 2u ] = static_cast<f32>(v) * math::gtColorDivider; }

			//	argb
			//	\param v: u32 color
		void setAsInteger( u32 v ){
			this->setAsByteRed( static_cast<u8>( v >> 16u ) );
			this->setAsByteGreen( static_cast<u8>( v >> 8u ) );
			this->setAsByteBlue( static_cast<u8>( v ) );
			this->setAsByteAlpha( static_cast<u8>( v >> 24u ) );
		}

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

			// \param v: rgb components
			// \param a: alpha component
		void set( f32 v, f32 a = 1.f ){
			setAlpha( a );
			setRed( v );
			setGreen( v );
			setBlue( v );
		}
	};

		//	HTML colors
	const gtColor gtColorWhite                = 0xffffffff;
	const gtColor gtColorAliceBlue            = 0xffF0F8FF;
	const gtColor gtColorAntiqueWhite         = 0xffFAEBD7;
	const gtColor gtColorAqua                 = 0xff00FFFF;
	const gtColor gtColorAquamarine           = 0xff7FFFD4;
	const gtColor gtColorAzure                = 0xffF0FFFF;
	const gtColor gtColorBeige                = 0xffF5F5DC;
	const gtColor gtColorBisque               = 0xffFFE4C4;
	const gtColor gtColorBlack                = 0xff000000;
	const gtColor gtColorBlanchedAlmond       = 0xffFFEBCD;
	const gtColor gtColorBlue                 = 0xff0000FF;
	const gtColor gtColorBlueViolet           = 0xff8A2BE2;
	const gtColor gtColorBrown                = 0xffA52A2A;
	const gtColor gtColorBurlyWood            = 0xffDEB887;
	const gtColor gtColorCadetBlue            = 0xff5F9EA0;
	const gtColor gtColorChartreuse           = 0xff7FFF00;
	const gtColor gtColorChocolate            = 0xffD2691E;
	const gtColor gtColorCoral                = 0xffFF7F50;
	const gtColor gtColorCornflowerBlue       = 0xff6495ED;
	const gtColor gtColorCornsilk             = 0xffFFF8DC;
	const gtColor gtColorCrimson              = 0xffDC143C;
	const gtColor gtColorCyan                 = 0xff00FFFF;
	const gtColor gtColorDarkBlue             = 0xff00008B;
	const gtColor gtColorDarkCyan             = 0xff008B8B;
	const gtColor gtColorDarkGoldenRod        = 0xffB8860B;
	const gtColor gtColorDarkGray             = 0xffA9A9A9;
	const gtColor gtColorDarkGrey             = 0xffA9A9A9;
	const gtColor gtColorDarkGreen            = 0xff006400;
	const gtColor gtColorDarkKhaki            = 0xffBDB76B;
	const gtColor gtColorDarkMagenta          = 0xff8B008B;
	const gtColor gtColorDarkOliveGreen       = 0xff556B2F;
	const gtColor gtColorDarkOrange           = 0xffFF8C00;
	const gtColor gtColorDarkOrchid           = 0xff9932CC;
	const gtColor gtColorDarkRed              = 0xff8B0000;
	const gtColor gtColorDarkSalmon           = 0xffE9967A;
	const gtColor gtColorDarkSeaGreen         = 0xff8FBC8F;
	const gtColor gtColorDarkSlateBlue        = 0xff483D8B;
	const gtColor gtColorDarkSlateGray        = 0xff2F4F4F;
	const gtColor gtColorDarkSlateGrey        = 0xff2F4F4F;
	const gtColor gtColorDarkTurquoise        = 0xff00CED1;
	const gtColor gtColorDarkViolet           = 0xff9400D3;
	const gtColor gtColorDeepPink             = 0xffFF1493;
	const gtColor gtColorDeepSkyBlue          = 0xff00BFFF;
	const gtColor gtColorDimGray              = 0xff696969;
	const gtColor gtColorDimGrey              = 0xff696969;
	const gtColor gtColorDodgerBlue           = 0xff1E90FF;
	const gtColor gtColorFireBrick            = 0xffB22222;
	const gtColor gtColorFloralWhite          = 0xffFFFAF0;
	const gtColor gtColorForestGreen          = 0xff228B22;
	const gtColor gtColorFuchsia              = 0xffFF00FF;
	const gtColor gtColorGainsboro            = 0xffDCDCDC;
	const gtColor gtColorGhostWhite           = 0xffF8F8FF;
	const gtColor gtColorGold                 = 0xffFFD700;
	const gtColor gtColorGoldenRod            = 0xffDAA520;
	const gtColor gtColorGray                 = 0xff808080;
	const gtColor gtColorGrey                 = 0xff808080;
	const gtColor gtColorGreen                = 0xff008000;
	const gtColor gtColorGreenYellow          = 0xffADFF2F;
	const gtColor gtColorHoneyDew             = 0xffF0FFF0;
	const gtColor gtColorHotPink              = 0xffFF69B4;
	const gtColor gtColorIndianRed            = 0xffCD5C5C;
	const gtColor gtColorIndigo               = 0xff4B0082;
	const gtColor gtColorIvory                = 0xffFFFFF0;
	const gtColor gtColorKhaki                = 0xffF0E68C;
	const gtColor gtColorLavender             = 0xffE6E6FA;
	const gtColor gtColorLavenderBlush        = 0xffFFF0F5;
	const gtColor gtColorLawnGreen            = 0xff7CFC00;
	const gtColor gtColorLemonChiffon         = 0xffFFFACD;
	const gtColor gtColorLightBlue            = 0xffADD8E6;
	const gtColor gtColorLightCoral           = 0xffF08080;
	const gtColor gtColorLightCyan            = 0xffE0FFFF;
	const gtColor gtColorLightGoldenRodYellow = 0xffFAFAD2;
	const gtColor gtColorLightGray            = 0xffD3D3D3;
	const gtColor gtColorLightGrey            = 0xffD3D3D3;
	const gtColor gtColorLightGreen           = 0xff90EE90;
	const gtColor gtColorLightPink            = 0xffFFB6C1;
	const gtColor gtColorLightSalmon          = 0xffFFA07A;
	const gtColor gtColorLightSeaGreen        = 0xff20B2AA;
	const gtColor gtColorLightSkyBlue         = 0xff87CEFA;
	const gtColor gtColorLightSlateGray       = 0xff778899;
	const gtColor gtColorLightSlateGrey       = 0xff778899;
	const gtColor gtColorLightSteelBlue       = 0xffB0C4DE;
	const gtColor gtColorLightYellow          = 0xffFFFFE0;
	const gtColor gtColorLime                 = 0xff00FF00;
	const gtColor gtColorLimeGreen            = 0xff32CD32;
	const gtColor gtColorLinen                = 0xffFAF0E6;
	const gtColor gtColorMagenta              = 0xffFF00FF;
	const gtColor gtColorMaroon               = 0xff800000;
	const gtColor gtColorMediumAquaMarine     = 0xff66CDAA;
	const gtColor gtColorMediumBlue           = 0xff0000CD;
	const gtColor gtColorMediumOrchid         = 0xffBA55D3;
	const gtColor gtColorMediumPurple         = 0xff9370DB;
	const gtColor gtColorMediumSeaGreen       = 0xff3CB371;
	const gtColor gtColorMediumSlateBlue      = 0xff7B68EE;
	const gtColor gtColorMediumSpringGreen    = 0xff00FA9A;
	const gtColor gtColorMediumTurquoise      = 0xff48D1CC;
	const gtColor gtColorMediumVioletRed      = 0xffC71585;
	const gtColor gtColorMidnightBlue         = 0xff191970;
	const gtColor gtColorMintCream            = 0xffF5FFFA;
	const gtColor gtColorMistyRose            = 0xffFFE4E1;
	const gtColor gtColorMoccasin             = 0xffFFE4B5;
	const gtColor gtColorNavajoWhite          = 0xffFFDEAD;
	const gtColor gtColorNavy                 = 0xff000080;
	const gtColor gtColorOldLace              = 0xffFDF5E6;
	const gtColor gtColorOlive                = 0xff808000;
	const gtColor gtColorOliveDrab            = 0xff6B8E23;
	const gtColor gtColorOrange               = 0xffFFA500;
	const gtColor gtColorOrangeRed            = 0xffFF4500;
	const gtColor gtColorOrchid               = 0xffDA70D6;
	const gtColor gtColorPaleGoldenRod        = 0xffEEE8AA;
	const gtColor gtColorPaleGreen            = 0xff98FB98;
	const gtColor gtColorPaleTurquoise        = 0xffAFEEEE;
	const gtColor gtColorPaleVioletRed        = 0xffDB7093;
	const gtColor gtColorPapayaWhip           = 0xffFFEFD5;
	const gtColor gtColorPeachPuff            = 0xffFFDAB9;
	const gtColor gtColorPeru                 = 0xffCD853F;
	const gtColor gtColorPink                 = 0xffFFC0CB;
	const gtColor gtColorPlum                 = 0xffDDA0DD;
	const gtColor gtColorPowderBlue           = 0xffB0E0E6;
	const gtColor gtColorPurple               = 0xff800080;
	const gtColor gtColorRebeccaPurple        = 0xff663399;
	const gtColor gtColorRed                  = 0xffFF0000;
	const gtColor gtColorRosyBrown            = 0xffBC8F8F;
	const gtColor gtColorRoyalBlue            = 0xff4169E1;
	const gtColor gtColorSaddleBrown          = 0xff8B4513;
	const gtColor gtColorSalmon               = 0xffFA8072;
	const gtColor gtColorSandyBrown           = 0xffF4A460;
	const gtColor gtColorSeaGreen             = 0xff2E8B57;
	const gtColor gtColorSeaShell             = 0xffFFF5EE;
	const gtColor gtColorSienna               = 0xffA0522D;
	const gtColor gtColorSilver               = 0xffC0C0C0;
	const gtColor gtColorSkyBlue              = 0xff87CEEB;
	const gtColor gtColorSlateBlue            = 0xff6A5ACD;
	const gtColor gtColorSlateGray            = 0xff708090;
	const gtColor gtColorSlateGrey            = 0xff708090;
	const gtColor gtColorSnow                 = 0xffFFFAFA;
	const gtColor gtColorSpringGreen          = 0xff00FF7F;
	const gtColor gtColorSteelBlue            = 0xff4682B4;
	const gtColor gtColorTan                  = 0xffD2B48C;
	const gtColor gtColorTeal                 = 0xff008080;
	const gtColor gtColorThistle              = 0xffD8BFD8;
	const gtColor gtColorTomato               = 0xffFF6347;
	const gtColor gtColorTurquoise            = 0xff40E0D0;
	const gtColor gtColorViolet               = 0xffEE82EE;
	const gtColor gtColorWheat                = 0xffF5DEB3;
	const gtColor gtColorWhiteSmoke           = 0xffF5F5F5;
	const gtColor gtColorYellow               = 0xffFFFF00;
	const gtColor gtColorYellowGreen          = 0xff9ACD32;

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