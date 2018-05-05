#ifndef DEMO_EXAMPLE_GET_SUPPORTED_FORMATS_H__
#define DEMO_EXAMPLE_GET_SUPPORTED_FORMATS_H__

/*
	Print in output window all supported picture and 3d model formats
*/

class DemoExample_GetSupportedImportFormats : public demo::DemoExample{
	gtMainSystem * m_mainSystem;
public:

	DemoExample_GetSupportedImportFormats();
	~DemoExample_GetSupportedImportFormats();

	bool Init( void );
	void Restart( void );
	void Shutdown( void );
	void Run( void );
};

DemoExample_GetSupportedImportFormats::DemoExample_GetSupportedImportFormats(){}
DemoExample_GetSupportedImportFormats::~DemoExample_GetSupportedImportFormats(){}

bool DemoExample_GetSupportedImportFormats::Init( void ){
	m_mainSystem = gtMainSystem::getInstance();
	return true;
}

void DemoExample_GetSupportedImportFormats::Restart( void ){}
void DemoExample_GetSupportedImportFormats::Shutdown( void ){}

void DemoExample_GetSupportedImportFormats::Run( void ){
	gtArray<gtString> arr;
	util::getSupportedImportImageFormats( arr );

	gtLogWriter::printInfo( u"Supported picture formats:" );

	u32 sz = arr.size();
	for( u32 i = 0u; i < sz; ++i ){
		gtLogWriter::printInfo( u"\t%s", arr[ i ].data() );
	}

	arr.clear();

	util::getSupportedImportModelFormats( arr );

	gtLogWriter::printInfo( u"Supported 3d model formats:" );

	sz = arr.size();
	for( u32 i = 0u; i < sz; ++i ){
		gtLogWriter::printInfo( u"\t%s", arr[ i ].data() );
	}
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