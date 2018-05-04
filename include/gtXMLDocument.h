#pragma once
#ifndef __GT_XML_DOCUMENT_H__
#define __GT_XML_DOCUMENT_H__

namespace gost{

	enum class gtXPathTokenType{
		Slash,
		Double_slash,
		Name,
		Equal,			// price=9.80
		Not_equal,		// price!=9.80
		More,			// price>9.80
		Less,			// price<9.80
		More_eq,		// price>=9.80
		Less_eq,		// price<=9.80
		Apos,
		Number,
		Comma, //,
		Function,
		Function_open,  //(
		Function_close, //)
		Attribute,
		Bit_or,			// //book | //cd
		Sq_open,		// [
		Sq_close,		// ]
		Div,			// 8 div 4
		Mod,			// 5 mod 2
		Add,			// 6 + 4
		Sub,			// 6 - 4
		Mul,			// 6 * 4
		And,			// price>9.00 and price<9.90
		Or,				// price=9.80 or price=9.70,
		Axis_namespace,	//::
		Axis,
		NONE = 0xFFFFFFF
	};

	enum class gtXPathAxis{
		Ancestor,			// parent, grandparent, etc.
		Ancestor_or_self,	// parent, grandparent, etc. + current node
		Attribute,
		Child,
		Descendant,			// children, grandchildren, etc.
		Descendant_or_self,	// children, grandchildren, etc. + current node
		Following,
		Following_sibling,
		Namespace,
		Parent,
		Preceding,
		Preceding_sibling,
		Self,
		NONE = 0xFFFFFFF
	};

	struct gtXPathToken{
		gtXPathToken( void ):
		    m_type( gtXPathTokenType::NONE ),
		    m_axis(gtXPathAxis::NONE),
		    m_number( 0.f )
		    {}

		gtXPathToken( gtXPathTokenType type,
			gtString string,
			f32 number )
		: m_type( type ),
		m_axis(gtXPathAxis::NONE),
		m_string( string ),
		m_number( number )
		{}

		gtXPathTokenType    m_type;
		gtXPathAxis         m_axis;
		gtString            m_string;
		f32                 m_number;
	};

	struct gtXMLAttribute : public gtRefObject {
		gtXMLAttribute(){}
		gtXMLAttribute( const gtString& Name,
			const gtString& Value ):
			name( Name ),
			value( Value )
		{}
		gtString name;
		gtString value;
	};

	struct gtXMLNode : public gtRefObject {
		gtXMLNode( void ){}
		gtXMLNode( const gtString& Name ):
			name( Name )
		{}
		gtXMLNode( const gtXMLNode& node ){
			name = node.name;
			text = node.text;
			attributeList = node.attributeList;
			nodeList = node.nodeList;
		}

		~gtXMLNode( void ){
			clear();
		}

		gtString name;
		gtString text;
		gtArray<gtXMLAttribute*> attributeList;
		gtArray<gtXMLNode*> nodeList;

		void addAttribute( const gtString& Name,
			const gtString& Value ){
			attributeList.push_back( new gtXMLAttribute( Name, Value ) );
		}

		void addAttribute( gtXMLAttribute* a ){
			attributeList.push_back( a );
		}

		void addNode( gtXMLNode* node ){
			nodeList.push_back( node );
		}

		gtXMLNode& operator=( const gtXMLNode& node ){
			name = node.name;
			text = node.text;
			attributeList = node.attributeList;
			nodeList = node.nodeList;

			return *this;
		}

		gtXMLAttribute*	getAttribute( const gtString& Name ){
			u32 sz = attributeList.size();
			for( u32 i = 0u; i < sz; ++i ){
				if( attributeList[ i ]->name == Name ){
					return attributeList[ i ];
				}
			}
			return nullptr;
		}

		void clear( void ){
			name.clear();
			text.clear();
			u32 sz = attributeList.size();
			for( u32 i = 0u; i < sz; ++i ){
				delete attributeList[ i ];
			}
			sz = nodeList.size();
			for( u32 i = 0u; i < sz; ++i ){
				delete nodeList[ i ];
			}
			attributeList.clear();
			nodeList.clear();
		}
	};

	class gtXMLDocument : public gtRefObject{
	public:

		virtual gtXMLNode* getRootNode( void ) = 0;

		virtual void print( void ) = 0;

		virtual const gtString& getText( void ) = 0;

		virtual gtArray<gtXMLNode*> selectNodes( const gtString& XPath_expression ) = 0;
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
