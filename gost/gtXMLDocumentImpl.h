//	GOST

#pragma once
#ifndef __GT_XML_DOCUMENT_IMPL_H__
#define __GT_XML_DOCUMENT_IMPL_H__

namespace gost{


	class gtXMLDocumentImpl GT_FINAL : public gtXMLDocument{
		bool		m_isInit;
		gtXMLNode	m_root;
		gtString	m_fileName;
		gtString	m_text;

		gtString m_expect_apos;
		gtString m_expect_quot;
		gtString m_expect_eq;
		gtString m_expect_slash;
		gtString m_expect_lt;
		gtString m_expect_gt;
		gtString m_expect_sub;
		gtString m_expect_ex;

		u32 m_cursor, m_sz;

		enum _toke_type{
			tt_default,
			tt_string
		};
		struct _token{
			_token( gtString N, u32 R, u32 C, _toke_type t = _toke_type::tt_default ):
				name( N ), line( R ), col( C ), type( t )
			{}
			gtString name;
			u32 line;
			u32 col;
			_toke_type type;
		};

		gtArray<_token> m_tokens;

		void getTokens( void );
		void decodeEnts( gtString& outText );
		
		char16_t * getName( char16_t * ptr, gtString& outText, u32& line, u32& col );
		char16_t * getString( char16_t * ptr, gtString& outText, u32& line, u32& col );
		char16_t * skipSpace( char16_t * ptr, u32& line, u32& col );

		bool charForName( char16_t * ptr );
		bool charForString( char16_t * ptr );
		bool charIsSymbol( char16_t * ptr );

		bool analyzeTokens( void );
		bool buildXMLDocument( void );
		bool getSubNode( gtXMLNode * node );
		bool getAttributes( gtXMLNode * node );
		bool tokenIsName( void );
		bool nextToken( void );
		bool unexpectedToken( const _token& token, gtString expected );

		void skipPrologAndDTD( void );

		void printNode( gtXMLNode* node, u32 indent );

		bool tokenIsString( void );

		bool XPath_getTokens( gtArray<gtXPathToken> * arr, const gtString& XPath_expression );
		bool XPath_isName( char16_t * ptr );

		char16_t* XPath_getName( char16_t*ptr, gtString * name );
		void XPathGetNodes( u32 level, u32 maxLevel, gtArray<gtString*> elements, gtXMLNode* node, gtArray<gtXMLNode*>* outArr );
	public:

		gtXMLDocumentImpl( const gtString& fileName );
		virtual ~gtXMLDocumentImpl( void );

		bool init( void );

		gtXMLNode* getRootNode( void );
		void print( void );
		const gtString& getText( void );
		gtArray<gtXMLNode*> selectNodes( const gtString& XPath_expression );
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