#pragma once
#ifndef __GT_XML_DOCUMENT_H__
#define __GT_XML_DOCUMENT_H__ //< include guard

namespace gost{

	struct gtXMLAttribute{
		gtXMLAttribute( const gtString& Name,
			const gtString& Value ):
			name( Name ),
			value( Value )
		{}
		gtString name;
		gtString value;
	};

	struct gtXMLNode{
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

		gtString name;
		gtString text;
		gtArray<gtXMLAttribute> attributeList;
		gtArray<gtXMLNode> nodeList;

		void addAttribute( const gtString& Name,
			const gtString& Value ){
			attributeList.push_back( gtXMLAttribute( Name, Value ) );
		}

		void addNode( const gtXMLNode& node ){
			nodeList.push_back( node );
		}

		gtXMLNode& operator=( const gtXMLNode& node ){
			name = node.name;
			text = node.text;
			attributeList = node.attributeList;
			nodeList = node.nodeList;

			return *this;
		}

		void clear( void ){
			name.clear();
			text.clear();
			attributeList.clear();
			nodeList.clear();
		}
	};

	class gtXMLDocument : public gtRefObject{
	public:

		virtual gtXMLNode* getRootNode( void ) = 0;
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