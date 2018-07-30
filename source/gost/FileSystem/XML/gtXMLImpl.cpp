#include "common.h"

gtPtr<gtXMLDocument> gtFileSystem::XMLRead( const gtString& file ){

	gtPtr_t( gtXMLDocumentImpl, xml, new gtXMLDocumentImpl(file) );
	if( !xml.data() ){
		gtLogWriter::printWarning( u"Can not create XML document." );
		return nullptr;
	}

	if( !xml->init() ){
		return nullptr;
	}

	xml->addRef();
	return gtPtrNew<gtXMLDocument>( xml.data() );
}

void writeText( gtString& outText, const gtString& inText ){
	u32 sz = inText.size();
	for( u32 i = gtConst0U; i < sz; ++i ){
		if( inText[ i ] == u'\'' ){
			outText += u"&apos;";
		}else if( inText[ i ] == u'\"' ){
			outText += u"&quot;";
		}else if( inText[ i ] == u'<' ){
			outText += u"&lt;";
		}else if( inText[ i ] == u'>' ){
			outText += u"&gt;";
		}else if( inText[ i ] == u'&' ){
			outText += u"&amp;";
		}else{
			outText += inText[ i ];
		}
	}
}

void writeName( gtString& outText, const gtString& inText ){
	outText += "<";
	outText += inText;
}

bool writeNodes( gtString& outText, gtXMLNode* node, u32 tabCount ){
	for( u32 i = gtConst0U; i < tabCount; ++i ){
		outText += u"\t";
	}

	++tabCount;

	writeName( outText, node->name );
	
	u32 sz = node->attributeList.size();
	if( sz ){
		for( u32 i = gtConst0U; i < sz; ++i ){
			outText += u" ";
			outText += node->attributeList[ i ]->name;
			outText += u"=";
			outText += u"\"";
			writeText( outText, node->attributeList[ i ]->value );
			outText += u"\"";
		}
	}

	if( !node->nodeList.size() && !node->text.size() ){
		outText += u"/>\r\n";
		return true;
	}else{
		outText += u">\r\n";
		sz = node->nodeList.size();
		for( u32 i = gtConst0U; i < sz; ++i ){
			if( !writeNodes( outText, node->nodeList[ i ], tabCount ) ){
				for( u32 o = gtConst0U; o < tabCount; ++o ){
					outText += u"\t";
				}
				outText += u"</";
				outText += node->nodeList[ i ]->name;
				outText += u">\n";
			}
		}
	}

	if( node->text.size() ){
		for( u32 o = gtConst0U; o < tabCount; ++o ){
			outText += u"\t";
		}
		writeText( outText, node->text );
		outText += u"\n";
	}
	--tabCount;

	return false;
}

void gtFileSystem::XMLWrite( const gtString& file, gtXMLNode* rootNode, bool utf8 ){

	gtString outText( u"<?xml version=\"1.0\"" );
	if( utf8 ) outText += " encoding=\"UTF-8\"";
	outText += " ?>\r\n";

	writeNodes( outText, rootNode, 0 );
	outText += u"</";
	outText += rootNode->name;
	outText += u">\n";

	gtFile_t out = util::createFileForWriteText( file );

	gtTextFileInfo ti;
	ti.m_hasBOM = true;
	if( utf8 ){
		ti.m_format = gtTextFileFormat::UTF_8;
		out->setTextFileInfo( ti );
		
		gtStringA mbstr;
		util::stringUTF16_to_UTF8( outText, mbstr );

		out->write( mbstr );

	}else{
		ti.m_endian = gtTextFileEndian::Little;
		ti.m_format = gtTextFileFormat::UTF_16;
		out->setTextFileInfo( ti );
		out->write( outText );
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