#include "common.h"

gtXMLDocumentImpl::gtXMLDocumentImpl( const gtString& fileName ){
	m_fileName = fileName;
	m_expect_apos = u"\'";
	m_expect_quot = u"\"";
	m_expect_eq   = u"=";
	m_expect_slash= u"/";
	m_expect_lt   = u"<";
	m_expect_gt   = u">";
	m_expect_sub  = u"-";
	m_expect_ex   = u"!";
}

gtXMLDocumentImpl::~gtXMLDocumentImpl( void ){
}

bool gtXMLDocumentImpl::init( void ){
	if( util::readTextFromFileForUnicode( m_fileName, m_text ) ){
		
		getTokens();

		if( !analyzeTokens() ) return false;

		m_tokens.clear();
		return true;
	}

	return false;
}

void gtXMLDocumentImpl::skipPrologAndDTD( u32& cursor ){
	u32 sz = m_tokens.size();
	while( cursor < sz ){
		if( m_tokens[ cursor ].name == m_expect_gt ){
			++cursor;
			return;
		}else{
			++cursor;
		}
	}
}

bool gtXMLDocumentImpl::tokenIsName( u32 cursor ){
	if( m_tokens[ cursor ].name == m_expect_lt ) return false;
	if( m_tokens[ cursor ].name == m_expect_gt ) return false;
	if( m_tokens[ cursor ].name == m_expect_sub ) return false;
	if( m_tokens[ cursor ].name == m_expect_ex ) return false;
	if( m_tokens[ cursor ].name == m_expect_eq ) return false;
	if( m_tokens[ cursor ].name == m_expect_apos ) return false;
	if( m_tokens[ cursor ].name == m_expect_quot ) return false;
	if( m_tokens[ cursor ].name == m_expect_slash ) return false;
	return true;
}

bool gtXMLDocumentImpl::nextToken( u32& cursor, u32 sz ){
	++cursor;
	if( cursor >= sz ){
		gtLogWriter::printError( u"End of XML" );
		return true;
	}
	return false;
}

bool gtXMLDocumentImpl::unexpectedToken( const _token& token, gtString expected ){
	gtLogWriter::printError( u"XML: Unexpected token: %s Line:%u Col:%u", token.name.data(), token.line, token.col );
	gtLogWriter::printError( u"XML: Expected: %s", expected.data() );
	return false;
}

bool gtXMLDocumentImpl::getAttributes( u32& cursor, u32 sz, gtXMLNode * node ){
	for(;;){
		gtXMLAttribute at;
		if( nextToken( cursor, sz ) ) return false;
		if( tokenIsName( cursor ) ){
			at.name = m_tokens[ cursor ].name;
		
			if( nextToken( cursor, sz ) ) return false;
			if( m_tokens[ cursor ].name == m_expect_eq ){
				if( nextToken( cursor, sz ) ) return false;
				if( m_tokens[ cursor ].name == m_expect_apos ){
					if( nextToken( cursor, sz ) ) return false;
					if( tokenIsName( cursor ) ){
						at.value = m_tokens[ cursor ].name;
						if( nextToken( cursor, sz ) ) return false;
						if( m_tokens[ cursor ].name == m_expect_apos ){
							node->addAttribute( at );
							continue;
						}else{
							return unexpectedToken( m_tokens[ cursor ], m_expect_apos );
						}
					}
				}else if( m_tokens[ cursor ].name == m_expect_quot ){
					if( nextToken( cursor, sz ) ) return false;
					if( tokenIsName( cursor ) ){
						at.value = m_tokens[ cursor ].name;
						if( nextToken( cursor, sz ) ) return false;
						if( m_tokens[ cursor ].name == m_expect_quot ){
							node->addAttribute( at );
							continue;
						}else{
							return unexpectedToken( m_tokens[ cursor ], m_expect_quot );
						}
					}
				}else{
					return unexpectedToken( m_tokens[ cursor ], u"\' or \"" );
				}
			}else{
				return unexpectedToken( m_tokens[ cursor ], m_expect_eq );
			}
		}else if( m_tokens[ cursor ].name == m_expect_gt 
			|| m_tokens[ cursor ].name == m_expect_slash ){
			return true;
		}else{
			return unexpectedToken( m_tokens[ cursor ], u"attribute or / or >" );
		}
	}
	return false;
}

bool gtXMLDocumentImpl::getSubNode( u32& cursor, u32 sz, gtXMLNode * node ){
	
	gtXMLNode subNode;

	gtString name;

	bool next = false;
	while( cursor < sz ){

		if( m_tokens[ cursor ].name == m_expect_lt ){

			if( nextToken( cursor, sz ) ) return false;

			if( tokenIsName( cursor ) ){
				name = m_tokens[ cursor ].name;
				node->name = name;
				if( nextToken( cursor, sz ) ) return false;

				// First - attributes
				if( tokenIsName( cursor ) ){
					--cursor;
					if( !getAttributes( cursor, sz, node ) ){
						return false;
					}
				}

				if( m_tokens[ cursor ].name == m_expect_gt ){
					if( nextToken( cursor, sz ) ) return false;
					if( tokenIsName( cursor ) ){
						node->text = m_tokens[ cursor ].name;

						if( nextToken( cursor, sz ) ) return false;

closeNode:
						if( m_tokens[ cursor ].name == m_expect_lt ){
							if( nextToken( cursor, sz ) ) return false;
							if( m_tokens[ cursor ].name == m_expect_slash ){
								if( nextToken( cursor, sz ) ) return false;
								if( m_tokens[ cursor ].name == name ){
									if( nextToken( cursor, sz ) ) return false;
									if( m_tokens[ cursor ].name == m_expect_gt ){
										++cursor;
										return true;
									}else{
										return unexpectedToken( m_tokens[ cursor ], m_expect_gt );
									}
								}else{
									return unexpectedToken( m_tokens[ cursor ], name );
								}
							}else{
								return unexpectedToken( m_tokens[ cursor ], m_expect_slash );
							}
						}else{
							return unexpectedToken( m_tokens[ cursor ], m_expect_lt );
						}
					}else if( m_tokens[ cursor ].name == m_expect_lt ){ // next or </
						if( nextToken( cursor, sz ) ) return false;
						if( tokenIsName( cursor ) ){ // next node
							next = true;
							--cursor;
						}else if( m_tokens[ cursor ].name == m_expect_slash ){ // return true
							if( nextToken( cursor, sz ) ) return false;
							if( m_tokens[ cursor ].name == name ){
								if( nextToken( cursor, sz ) ) return false;
								if( m_tokens[ cursor ].name == m_expect_gt ){
									++cursor;// NAME = m_tokens[ cursor ].name;
									return true;
								}else{
									return unexpectedToken( m_tokens[ cursor ], m_expect_gt );
								}
							}else{
								return unexpectedToken( m_tokens[ cursor ], name );
							}
						}else{
							return unexpectedToken( m_tokens[ cursor ], u"/ or <entity>" );
						}
					}else{
						return unexpectedToken( m_tokens[ cursor ], u"\"text\" or <entity>" );
					}
				}else if( m_tokens[ cursor ].name == m_expect_slash ){
					if( nextToken( cursor, sz ) ) return false;
					if( m_tokens[ cursor ].name == m_expect_gt ){
						++cursor;
						return true;
					}else{
						return unexpectedToken( m_tokens[ cursor ], m_expect_gt );
					}
				}else{
					return unexpectedToken( m_tokens[ cursor ], u"> or /" );
				}
			}else{
				return unexpectedToken( m_tokens[ cursor ], u"name" );
			}

		}else{
			return unexpectedToken( m_tokens[ cursor ], m_expect_lt );
		}

		if( next ){
newNode:
			if( getSubNode( cursor, sz, &subNode ) ){
				node->addNode( subNode );

				if( m_tokens[ cursor ].name == m_expect_lt ){
					if( nextToken( cursor, sz ) ) return false;
					if( m_tokens[ cursor ].name == m_expect_slash ){
						--cursor;
						goto closeNode;
					}else if( tokenIsName( cursor ) ){
						--cursor;
						subNode.clear();
						goto newNode;
					}else{
						return unexpectedToken( m_tokens[ cursor ], u"</close tag> or <new tag>" );
					}
				}else if( tokenIsName( cursor ) ){
					node->text = m_tokens[ cursor ].name;
					if( nextToken( cursor, sz ) ) return false;
					if( m_tokens[ cursor ].name == m_expect_lt ){
						if( nextToken( cursor, sz ) ) return false;
						if( m_tokens[ cursor ].name == m_expect_slash ){
							--cursor;
							goto closeNode;
						}else if( tokenIsName( cursor ) ){
							--cursor;
							subNode.clear();
							goto newNode;
						}else{
							return unexpectedToken( m_tokens[ cursor ], u"</close tag> or <new tag>" );
						}
					}else{
						return unexpectedToken( m_tokens[ cursor ], m_expect_lt );
					}
				}

			}else{
				return false;
			}
		}
	}

	return true;
}

bool gtXMLDocumentImpl::analyzeTokens( void ){
	u32 sz = m_tokens.size();
	if( !sz ){
		gtLogWriter::printError( u"Empty XML" );
		return false;
	}

	u32 cursor = 0u;
	if( m_tokens[ 0u ].name == u"<" )
		if( m_tokens[ 1u ].name == u"?" )
			if( m_tokens[ 2u ].name == u"xml" ){
				cursor = 2u;
				skipPrologAndDTD( cursor );
			}

	if( m_tokens[ cursor ].name == u"<" )
		if( m_tokens[ cursor + 1u ].name == u"!" )
			if( m_tokens[ cursor + 2u ].name == u"DOCTYPE" )
				skipPrologAndDTD( cursor );

	return buildXMLDocument( cursor, sz );
}

bool gtXMLDocumentImpl::buildXMLDocument( u32& cursor, u32 sz ){
	return getSubNode( cursor, sz, &m_root);
}

gtXMLNode* gtXMLDocumentImpl::getRootNode( void ){
	return &m_root;
}

bool gtXMLDocumentImpl::charForName( char16_t *ptr ){
	char16_t c = *ptr;
	if( c > 0x80 ) return true;
	if( std::isalpha( *ptr ) 
			|| std::isdigit( *ptr )
			|| (*ptr == u'_')
			|| (*ptr == u'.')){
		return true;
	}
	return false;
}

bool gtXMLDocumentImpl::charForString( char16_t * ptr ){
	char16_t c = *ptr;
	if( c > 0x80 ) return true;
	if( std::isalpha( *ptr ) 
			|| std::isdigit( *ptr )
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
			col = 1u;
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
			col = 1u;
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

void gtXMLDocumentImpl::getTokens( void ){

	char16_t * ptr = m_text.data();
	u32 line = 1u;
	u32 col = 1u;

	bool isString = false;

	bool stringType = false; // "

	gtString str;
	
	u32 oldCol = 0u;

	while( *ptr ){

		if( *ptr == u'\n' ){
			col = 0u;
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
						m_tokens.push_back( _token( str, line, oldCol+1u ) );
						m_tokens.push_back( _token( gtString( *ptr ), line, col ) );
						str.clear();
						isString = false;
						goto chponk;
					}
				}else{ // "
					if( *ptr == u'\"' )
					{
						decodeEnts( str );
						m_tokens.push_back( _token( str, line, oldCol+1u ) );
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

void gtXMLDocumentImpl::printNode( const gtXMLNode& node, u32 indent ){
	if( node.name.size() ){

		gtString line;

		for( u32 i = 0u; i < indent; ++i ){
			line += u" ";
		}
		
		line += u"<";
		line += node.name;
		line += u">";

		if( node.attributeList.size() ){

			line += u" ( ";

			for( u32 i = 0u; i < node.attributeList.size(); ++i ){
				const gtXMLAttribute * at = &node.attributeList[ i ];

				if( at->name.size() ){
					line += at->name;
					line += u":";
					if( at->value.size() ){
						line += at->value;
						line += u" ";
					}else{
						line += u"ERROR ";
					}
				}

			}
			
			line += u" )";

		}

		if( node.text.size() ){
			line += u" = ";
			line += node.text;
		}
		gtLogWriter::printInfo( u"%s", line.data() );

		if( node.nodeList.size() ){
			for( u32 i = 0u; i < node.nodeList.size(); ++i ){
				printNode( node.nodeList[ i ], ++indent );
				--indent;
			}
		}

	}
}

void gtXMLDocumentImpl::print( void ){
	gtLogWriter::printInfo( u"XML:" );
	printNode( m_root, 0u );
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