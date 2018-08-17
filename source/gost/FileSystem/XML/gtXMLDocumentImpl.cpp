#include "common.h"
//gtString NAME;
gtXMLDocumentImpl::gtXMLDocumentImpl( const gtString& fileName ):
m_isInit( false ){
	m_fileName = fileName;
	m_expect_apos = u"\'";
	m_expect_quot = u"\"";
	m_expect_eq   = u"=";
	m_expect_slash= u"/";
	m_expect_lt   = u"<";
	m_expect_gt   = u">";
	m_expect_sub  = u"-";
	m_expect_ex   = u"!";
	m_cursor = m_sz = gtConst0U;
	m_tokens.setAddMemoryValue( 4096u );
}

gtXMLDocumentImpl::~gtXMLDocumentImpl(){
}

bool gtXMLDocumentImpl::init(){

	if( gtFileSystem::existFile( m_fileName ) ){
		if( !util::readTextFromFileForUnicode( m_fileName, m_text ) ){
			return false;
		}
	}else{
		m_text = m_fileName;
	}

	getTokens();

	if( !analyzeTokens() ) return false;

	m_tokens.clear();
	m_isInit = true;

	return true;
}

void gtXMLDocumentImpl::skipPrologAndDTD(){
	u32 sz = m_tokens.size();
	while( m_cursor < sz ){
		if( m_tokens[ m_cursor ].name == m_expect_gt ){
			++m_cursor;
			return;
		}else{
			++m_cursor;
		}
	}
}

bool gtXMLDocumentImpl::tokenIsName(){
	if( m_tokens[ m_cursor ].name == m_expect_lt ) return false;
	if( m_tokens[ m_cursor ].name == m_expect_gt ) return false;
	if( m_tokens[ m_cursor ].name == m_expect_sub ) return false;
	if( m_tokens[ m_cursor ].name == m_expect_ex ) return false;
	if( m_tokens[ m_cursor ].name == m_expect_eq ) return false;
	if( m_tokens[ m_cursor ].name == m_expect_apos ) return false;
	if( m_tokens[ m_cursor ].name == m_expect_quot ) return false;
	if( m_tokens[ m_cursor ].name == m_expect_slash ) return false;
	return true;
}

bool gtXMLDocumentImpl::nextToken(){
	++m_cursor;
	if( m_cursor >= m_sz ){
		gtLogWriter::printError( u"End of XML" );
		return true;
	}
//	NAME = m_tokens[ m_cursor ].name;
	return false;
}

bool gtXMLDocumentImpl::unexpectedToken( const _token& token, gtString expected ){
	gtLogWriter::printError( u"XML: Unexpected token: %s Line:%u Col:%u", token.name.data(), token.line, token.col );
	gtLogWriter::printError( u"XML: Expected: %s", expected.data() );
	return false;
}

bool gtXMLDocumentImpl::tokenIsString(){
	return m_tokens[ m_cursor ].type == gtXMLDocumentImpl::_toke_type::tt_string;
}

bool gtXMLDocumentImpl::getAttributes( gtXMLNode * node ){
	for(;;){
		gtPtr<gtXMLAttribute> at = gtPtrNew<gtXMLAttribute>( new gtXMLAttribute );
		if( nextToken() ) return false;
		if( tokenIsName() ){
			at->name = m_tokens[ m_cursor ].name;
		
			if( nextToken() ) return false;
			if( m_tokens[ m_cursor ].name == m_expect_eq ){
				if( nextToken() ) return false;
				if( m_tokens[ m_cursor ].name == m_expect_apos ){
					if( nextToken() ) return false;
					//if( tokenIsName() ){
					if( tokenIsString() ){
						at->value = m_tokens[ m_cursor ].name;
						if( nextToken() ) return false;
						if( m_tokens[ m_cursor ].name == m_expect_apos ){
							at->addRef();
							node->addAttribute( at.data() );
							continue;
						}else{
							return unexpectedToken( m_tokens[ m_cursor ], m_expect_apos );
						}
					}
				}else if( m_tokens[ m_cursor ].name == m_expect_quot ){
					if( nextToken() ) return false;
					//if( tokenIsName() ){ //is string
					if( tokenIsString() ){
						at->value = m_tokens[ m_cursor ].name;
						if( nextToken() ) return false;
						if( m_tokens[ m_cursor ].name == m_expect_quot ){
							at->addRef();
							node->addAttribute( at.data() );
							continue;
						}else{
							return unexpectedToken( m_tokens[ m_cursor ], m_expect_quot );
						}
					}
				}else{
					return unexpectedToken( m_tokens[ m_cursor ], u"\' or \"" );
				}
			}else{
				return unexpectedToken( m_tokens[ m_cursor ], m_expect_eq );
			}
		}else if( m_tokens[ m_cursor ].name == m_expect_gt 
			|| m_tokens[ m_cursor ].name == m_expect_slash ){
			return true;
		}else{
			return unexpectedToken( m_tokens[ m_cursor ], u"attribute or / or >" );
		}
	}
	return false;
}

bool gtXMLDocumentImpl::getSubNode( gtXMLNode * node ){
	
	gtPtr<gtXMLNode> subNode = gtPtrNew<gtXMLNode>( new gtXMLNode );

	gtString name;

	bool next = false;
	while( m_cursor < m_sz ){

		if( m_tokens[ m_cursor ].name == m_expect_lt ){

			if( nextToken() ) return false;

			if( tokenIsName() ){
				name = m_tokens[ m_cursor ].name;
				node->name = name;
				if( nextToken() ) return false;

				// First - attributes
				if( tokenIsName() ){
					--m_cursor;
					if( !getAttributes( node ) ){
						return false;
					}
				}

				if( m_tokens[ m_cursor ].name == m_expect_gt ){
					if( nextToken() ) return false;
					if( tokenIsName() ){
						node->text = m_tokens[ m_cursor ].name;

						if( nextToken() ) return false;

closeNode:
						if( m_tokens[ m_cursor ].name == m_expect_lt ){
							if( nextToken() ) return false;
							if( m_tokens[ m_cursor ].name == m_expect_slash ){
								if( nextToken() ) return false;
								if( m_tokens[ m_cursor ].name == name ){
									if( nextToken() ) return false;
									if( m_tokens[ m_cursor ].name == m_expect_gt ){
										++m_cursor;
										return true;
									}else{
										return unexpectedToken( m_tokens[ m_cursor ], m_expect_gt );
									}
								}else{
									return unexpectedToken( m_tokens[ m_cursor ], name );
								}
							}else if( tokenIsName() ){
								--m_cursor;
								subNode = gtPtrNew<gtXMLNode>( new gtXMLNode );
								goto newNode;
							}else{
								return unexpectedToken( m_tokens[ m_cursor ], m_expect_slash );
							}
						}else{
							return unexpectedToken( m_tokens[ m_cursor ], m_expect_lt );
						}
					}else if( m_tokens[ m_cursor ].name == m_expect_lt ){ // next or </
						if( nextToken() ) return false;
						if( tokenIsName() ){ // next node
							next = true;
							--m_cursor;
						}else if( m_tokens[ m_cursor ].name == m_expect_slash ){ // return true
							if( nextToken() ) return false;
							if( m_tokens[ m_cursor ].name == name ){
								if( nextToken() ) return false;
								if( m_tokens[ m_cursor ].name == m_expect_gt ){
									++m_cursor;
									return true;
								}else{
									return unexpectedToken( m_tokens[ m_cursor ], m_expect_gt );
								}
							}else{
								return unexpectedToken( m_tokens[ m_cursor ], name );
							}
						}else{
							return unexpectedToken( m_tokens[ m_cursor ], u"/ or <entity>" );
						}
					}else{
						return unexpectedToken( m_tokens[ m_cursor ], u"\"text\" or <entity>" );
					}
				}else if( m_tokens[ m_cursor ].name == m_expect_slash ){
					if( nextToken() ) return false;
					if( m_tokens[ m_cursor ].name == m_expect_gt ){
						++m_cursor;
						return true;
					}else{
						return unexpectedToken( m_tokens[ m_cursor ], m_expect_gt );
					}
				}else{
					return unexpectedToken( m_tokens[ m_cursor ], u"> or /" );
				}
			}else{
				return unexpectedToken( m_tokens[ m_cursor ], u"name" );
			}

		}else{
			return unexpectedToken( m_tokens[ m_cursor ], m_expect_lt );
		}

		if( next ){
newNode:
			if( getSubNode( subNode.data() ) ){
				subNode->addRef();
				node->addNode( subNode.data() );

				--m_cursor;
				if( nextToken() ) return false;

				if( m_tokens[ m_cursor ].name == m_expect_lt ){
					if( nextToken() ) return false;
					if( m_tokens[ m_cursor ].name == m_expect_slash ){
						--m_cursor;
						goto closeNode;
					}else if( tokenIsName() ){
						--m_cursor;
						subNode = gtPtrNew<gtXMLNode>( new gtXMLNode );
						goto newNode;
					}else{
						return unexpectedToken( m_tokens[ m_cursor ], u"</close tag> or <new tag>" );
					}
				}else if( tokenIsName() ){
					node->text = m_tokens[ m_cursor ].name;
					if( nextToken() ) return false;
					if( m_tokens[ m_cursor ].name == m_expect_lt ){
						if( nextToken() ) return false;
						if( m_tokens[ m_cursor ].name == m_expect_slash ){
							--m_cursor;
							goto closeNode;
						}else if( tokenIsName() ){
							--m_cursor;
							//subNode.clear();
							subNode = gtPtrNew<gtXMLNode>( new gtXMLNode );
							goto newNode;
						}else{
							return unexpectedToken( m_tokens[ m_cursor ], u"</close tag> or <new tag>" );
						}
					}else{
						return unexpectedToken( m_tokens[ m_cursor ], m_expect_lt );
					}
				}

			}else{
				return false;
			}
		}
	}

	return true;
}

bool gtXMLDocumentImpl::analyzeTokens(){
	u32 sz = m_tokens.size();
	if( !sz ){
		gtLogWriter::printError( u"Empty XML" );
		return false;
	}

	m_cursor = gtConst0U;
	if( m_tokens[ gtConst0U ].name == u"<" )
		if( m_tokens[ gtConst1U ].name == u"?" )
			if( m_tokens[ gtConst2U ].name == u"xml" ){
				m_cursor = gtConst2U;
				skipPrologAndDTD();
			}

	if( m_tokens[ m_cursor ].name == u"<" )
		if( m_tokens[ m_cursor + gtConst1U ].name == u"!" )
			if( m_tokens[ m_cursor + gtConst2U ].name == u"DOCTYPE" )
				skipPrologAndDTD();

	return buildXMLDocument();
}

bool gtXMLDocumentImpl::buildXMLDocument(){
	m_sz = m_tokens.size();
	return getSubNode( &m_root);
}

gtXMLNode* gtXMLDocumentImpl::getRootNode(){
	return &m_root;
}

bool gtXMLDocumentImpl::charForName( char16_t *ptr ){
	char16_t c = *ptr;
	if( c > 0x80 ) return true;
	if( util::isAlpha( *ptr ) 
			|| util::isDigit( *ptr )
			|| (*ptr == u'_')
			|| (*ptr == u'.')){
		return true;
	}
	return false;
}

bool gtXMLDocumentImpl::charForString( char16_t * ptr ){
	char16_t c = *ptr;
	if( c > 0x80 ) return true;
	if( util::isAlpha( *ptr ) 
			|| util::isDigit( *ptr )
			|| (*ptr == u'_')
			|| (*ptr == u'.')){
		return true;
	}
	return false;
}

bool gtXMLDocumentImpl::charIsSymbol( char16_t * ptr ){
	char16_t c = *ptr;
	if( (c == u'<') || (c == u'>')
		|| (c == u'/') || (c == u'\'')
		|| (c == u'\"') || (c == u'=')
		|| (c == u'?') || (c == u'!')
		|| (c == u'-') ){
		return true;
	}
	return false;
}

char16_t * gtXMLDocumentImpl::getName( char16_t * ptr, gtString& outText, u32& /*line*/, u32& col ){
	while( *ptr ){
		if( charForName( ptr ) ){
			outText += *ptr;
		}else{
			return ptr;
		}
		++ptr;
		++col;
	}
	return ptr;
}

char16_t * gtXMLDocumentImpl::getString( char16_t * ptr, gtString& outText, u32& line, u32& col ){
	while( *ptr ){
		if( *ptr == u'\n' ){
			++line;
			col = gtConst1U;
			outText += *ptr;
			++ptr;
		}else if( *ptr == u'<' ){
			break;
		}else{
			outText += *ptr;
			++col;
			++ptr;
		}
	}
	return ptr;
}

char16_t * gtXMLDocumentImpl::skipSpace( char16_t * ptr, u32& line, u32& col ){
	while( *ptr ){
		if( *ptr == u'\n' ){
			++line;
			col = gtConst1U;
			++ptr;
		}else if( (*ptr == u'\r')
			|| (*ptr == u'\t')
			|| (*ptr == u' ')){
			++col;
			++ptr;
		}else break;
	}
	return ptr;
}

void gtXMLDocumentImpl::decodeEnts( gtString& str ){
	util::stringReplaseSubString( str, gtString(u"&apos;"), gtString(u"\'") );
	util::stringReplaseSubString( str, gtString(u"&quot;"), gtString(u"\"") );
	util::stringReplaseSubString( str, gtString(u"&lt;"), gtString(u"<") );
	util::stringReplaseSubString( str, gtString(u"&gt;"), gtString(u">") );
	util::stringReplaseSubString( str, gtString(u"&amp;"), gtString(u"&") );
}

void gtXMLDocumentImpl::getTokens(){

	char16_t * ptr = m_text.data();
	u32 line = gtConst1U;
	u32 col = gtConst1U;

	bool isString = false;

	bool stringType = false; // "

	gtString str;
	
	u32 oldCol = gtConst0U;

	while( *ptr ){

		if( *ptr == u'\n' ){
			col = gtConst0U;
			++line;
		}else{

			if( !isString ){
				if( charIsSymbol( ptr ) ){

					m_tokens.push_back( _token( gtString( *ptr ), line, col ) );

					if( *ptr == u'\'' ){
						oldCol = col;
						str.clear();
						isString = true;
						stringType = true;
					}else if( *ptr == u'\"' ){
						oldCol = col;
						isString = true;
						stringType = false;
						str.clear();
					}else if( *ptr == u'>' ){
						++ptr;
						++col;
						ptr = skipSpace( ptr, line, col );
						oldCol = col;
						ptr = getString( ptr, str, line, col );
						if( str.size() ){
							util::stringTrimSpace( str );
							decodeEnts( str );
							m_tokens.push_back( _token( str, line, oldCol ) );
							str.clear();
						}
						continue;
					}

				}else if( charForName( ptr ) ){
					oldCol = col;
					gtString name;
					ptr = getName( ptr, name, line, col );
					m_tokens.push_back( _token( name, line, oldCol ) );

					continue;
				}
			}else{
				if( stringType ){ // '
					if( *ptr == u'\'' )
					{
						decodeEnts( str );
						m_tokens.push_back( _token( str, line, oldCol+gtConst1U, gtXMLDocumentImpl::_toke_type::tt_string ) );
						m_tokens.push_back( _token( gtString( *ptr ), line, col ) );
						str.clear();
						isString = false;
						goto chponk;
					}
				}else{ // "
					if( *ptr == u'\"' )
					{
						decodeEnts( str );
						m_tokens.push_back( _token( str, line, oldCol+gtConst1U, gtXMLDocumentImpl::_toke_type::tt_string ) );
						m_tokens.push_back( _token( gtString( *ptr ), line, col ) );
						str.clear();
						isString = false;
						goto chponk;
					}
				}
				str += *ptr;
			}
		}
chponk:

		++col;
		++ptr;

	}
}

void gtXMLDocumentImpl::printNode( gtXMLNode* node, u32 indent ){
	if( node->name.size() ){

		gtString line;

		for( u32 i = gtConst0U; i < indent; ++i ){
			line += u" ";
		}
		
		line += u"<";
		line += node->name;
		line += u">";

		if( node->attributeList.size() ){

			line += u" ( ";

			for( u32 i = gtConst0U; i < node->attributeList.size(); ++i ){
				const gtXMLAttribute * at = node->attributeList[ i ];

				if( at->name.size() ){
					line += at->name;
					line += u":";
					if( at->value.size() ){
						line += u"\"";
						line += at->value;
						line += u"\"";
						line += u" ";
					}else{
						line += u"ERROR ";
					}
				}

			}
			
			line += u" )";

		}

		if( node->text.size() ){
			line += u" = ";
			line += node->text;
		}
		gtLogWriter::printInfo( u"%s", line.data() );

		if( node->nodeList.size() ){
			for( u32 i = gtConst0U; i < node->nodeList.size(); ++i ){
				printNode( node->nodeList[ i ], ++indent );
				--indent;
			}
		}

	}
}

void gtXMLDocumentImpl::print(){
	gtLogWriter::printInfo( u"XML:" );
	printNode( &m_root, gtConst0U );
}

const gtString& gtXMLDocumentImpl::getText(){
	return m_text;
}

bool gtXMLDocumentImpl::XPath_isName( char16_t * ptr ){

	if( *ptr == u':' ){
		if( *(ptr + gtConst1U) == u':' ){
			return false;
		}
	}

	switch( *ptr ){
	case u'/':
	case u'*':
	case u'\'':
	case u',':
	case u'=':
	case u'+':
	case u'-':
	case u'@':
	case u'[':
	case u']':
	case u'(':
	case u')':
	case u'|':
	case u'!':
		return false;
	}

	return true;
}

bool gtXMLDocumentImpl::XPath_getTokens( gtArray<gtXPathToken> * arr, const gtString& XPath_expression ){

	//u32 sz = XPath_expression.size();

	gtString name;

	char16_t * ptr = XPath_expression.data();

	char16_t next;
	while( *ptr ){
		
		name.clear();

		next = *(ptr + gtConst1U);

		gtXPathToken token;

		if( *ptr == u'/' ){
			if( next ){
				if( next == u'/' ){
					++ptr;
					token.m_type = gtXPathTokenType::Double_slash;
				}else{
					token.m_type = gtXPathTokenType::Slash;
				}
			}else{
				token.m_type = gtXPathTokenType::Slash;
			}
		}else if( *ptr == u'*' ){
			token.m_type = gtXPathTokenType::Mul;
		}else if( *ptr == u'=' ){
			token.m_type = gtXPathTokenType::Equal;
		}else if( *ptr == u'\'' ){
			token.m_type = gtXPathTokenType::Apos;
		}else if( *ptr == u'@' ){
			token.m_type = gtXPathTokenType::Attribute;
		}else if( *ptr == u'|' ){
			token.m_type = gtXPathTokenType::Bit_or;
		}else if( *ptr == u',' ){
			token.m_type = gtXPathTokenType::Comma;
		}else if( *ptr == u'+' ){
			token.m_type = gtXPathTokenType::Add;
		}else if( *ptr == u'+' ){
			token.m_type = gtXPathTokenType::Sub;
		}else if( *ptr == u'[' ){
			token.m_type = gtXPathTokenType::Sq_open;
		}else if( *ptr == u']' ){
			token.m_type = gtXPathTokenType::Sq_close;
		}else if( *ptr == u'(' ){
			token.m_type = gtXPathTokenType::Function_open;
		}else if( *ptr == u')' ){
			token.m_type = gtXPathTokenType::Function_close;
		}else if( *ptr == u'<' ){
			if( next ){
				if( next == u'=' ){
					++ptr;
					token.m_type = gtXPathTokenType::Less_eq;
				}else{
					token.m_type = gtXPathTokenType::Less;
				}
			}else{
				token.m_type = gtXPathTokenType::Less;
			}
		}else if( *ptr == u'>' ){
			if( next ){
				if( next == u'/' ){
					++ptr;
					token.m_type = gtXPathTokenType::More_eq;
				}else{
					token.m_type = gtXPathTokenType::More;
				}
			}else{
				token.m_type = gtXPathTokenType::More;
			}
		}else if( *ptr == u':' ){
			if( next ){
				if( next == u':' ){
					++ptr;
					token.m_type = gtXPathTokenType::Axis_namespace;
				}else{
					gtLogWriter::printError( u"XPath: Bad token" );
					return false;
				}
			}else{
				gtLogWriter::printError( u"XPath: Bad token" );
				return false;
			}
		}else if( *ptr == u'!' ){
			if( next ){
				if( next == u'=' ){
					++ptr;
					token.m_type = gtXPathTokenType::Not_equal;
				}else{
					gtLogWriter::printError( u"XPath: Bad token" );
					return false;
				}
			}else{
				gtLogWriter::printError( u"XPath: Bad token" );
				return false;
			}
		}else if( XPath_isName( ptr ) ){
			ptr = XPath_getName( ptr, &name );
			token.m_type = gtXPathTokenType::Name;
			token.m_string = name;
		}else{
			gtLogWriter::printError( u"XPath: Bad token" );
			return false;
		}

		arr->push_back( token );

		++ptr;
	}

	return true;
}

char16_t* gtXMLDocumentImpl::XPath_getName( char16_t*ptr, gtString * name ){
	while( *ptr ){
		if( XPath_isName( ptr ) ) *name += *ptr;
		else{
			break;
		}
		++ptr;
	}
	--ptr;
	return ptr;
}

gtArray<gtXMLNode*> gtXMLDocumentImpl::selectNodes( const gtString& XPath_expression ){
	gtArray<gtXMLNode*> a;

	if( !m_isInit ){
		gtLogWriter::printError( u"Bad gtXMLDocument" );
		return a;
	}

	gtArray<gtXPathToken> XPathTokens;

	if( !XPath_getTokens( &XPathTokens, XPath_expression ) ){
		gtLogWriter::printError( u"Bad XPath expression" );
		return a;
	}

	gtArray<gtString*> elements;

	u32 next = gtConst0U;
	u32 sz = XPathTokens.size();
	for( u32 i = gtConst0U; i < sz; ++i ){
		next = i + gtConst1U;
		if( i == gtConst0U ){
			if( XPathTokens[ i ].m_type != gtXPathTokenType::Slash
				&& XPathTokens[ i ].m_type != gtXPathTokenType::Double_slash){
				gtLogWriter::printError( u"Bad XPath expression \"%s\". Expression must begin with `/`", XPath_expression.data() );
				return a;
			}
		}

		switch( XPathTokens[ i ].m_type ){
			case gtXPathTokenType::Slash:
			if( next >= sz ){
				gtLogWriter::printError( u"Bad XPath expression" );
				return a;
			}
			if( XPathTokens[ next ].m_type == gtXPathTokenType::Name ){
				elements.push_back( &XPathTokens[ next ].m_string );
				++i;
			}else{
				gtLogWriter::printError( u"Bad XPath expression \"%s\". Expected XML element name", XPath_expression.data() );
				return a;
			}
			break;

			case gtXPathTokenType::Double_slash:
			break;
			case gtXPathTokenType::Name:
			break;
			case gtXPathTokenType::Equal:
			break;
			case gtXPathTokenType::Not_equal:
			break;
			case gtXPathTokenType::More:
			break;
			case gtXPathTokenType::Less:
			break;
			case gtXPathTokenType::More_eq:
			break;
			case gtXPathTokenType::Less_eq:
			break;
			case gtXPathTokenType::Apos:
			break;
			case gtXPathTokenType::Number:
			break;
			case gtXPathTokenType::Comma:
			break;
			case gtXPathTokenType::Function:
			break;
			case gtXPathTokenType::Function_open:
			break;
			case gtXPathTokenType::Function_close:
			break;
			case gtXPathTokenType::Attribute:
			break;
			case gtXPathTokenType::Bit_or:
			break;
			case gtXPathTokenType::Sq_open:
			break;
			case gtXPathTokenType::Sq_close:
			break;
			case gtXPathTokenType::Div:
			break;
			case gtXPathTokenType::Mod:
			break;
			case gtXPathTokenType::Add:
			break;
			case gtXPathTokenType::Sub:
			break;
			case gtXPathTokenType::Mul:
			break;
			case gtXPathTokenType::And:
			break;
			case gtXPathTokenType::Or:
			break;
			case gtXPathTokenType::Axis_namespace:
			break;
			case gtXPathTokenType::Axis:
			break;
			case gtXPathTokenType::NONE:
			break;
		}

	}

	if( elements.size() ){
		sz = elements.size();
		XPathGetNodes( gtConst0U, sz - gtConst1U, elements, &m_root, &a );

	}

	return a;
}

void gtXMLDocumentImpl::XPathGetNodes( 
	u32 level, 
	u32 maxLevel, 
	gtArray<gtString*> elements, 
	gtXMLNode* node, 
	gtArray<gtXMLNode*>* outArr ){
//_______________________________
	if( node->name == *elements[ level ] ){
	
		if( level == maxLevel ){
			outArr->push_back( node );
			return;
		}else{
			u32 sz = node->nodeList.size();
			for( u32  i = gtConst0U; i < sz; ++i ){
				XPathGetNodes( level + gtConst1U, maxLevel, elements, node->nodeList[ i ], outArr );
			}
		}

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