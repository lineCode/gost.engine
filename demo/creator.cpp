#include "creator.h"


#if defined( GT_PLATFORM_WIN32 )
int WINAPI WinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/ ){
#endif

	GoSTPreInit();

	demo::DemoApplication	*	app = new demo::DemoApplication;
	
	if( !app ){
#if defined( GT_PLATFORM_WIN32 )
		MessageBox( 0, L"Can not allocate memory for application", 0, 0 );
#endif
		return 1;
	}

	if( !app->Init() ){
#if defined( GT_PLATFORM_WIN32 )
		MessageBox( 0, L"Can not init application", 0, 0 );
#endif
		delete app;
		return 2;	
	}

	char * str = (char*)gtMemAlloc(4);

	str[ 0 ] = 'A';
	str[ 1 ] = 'B';
	str[ 2 ] = 'C';
	str[ 3 ] = 'D';

	str = (char*)gtMemReAlloc(str,8);

	str[ 4 ] = 'E';
	str[ 5 ] = 'F';
	str[ 6 ] = 'G';
	str[ 7 ] = 'H';

	gtMemFree(str);

	app->Run();

//	MessageBox( 0, L"Start", 0, 0 );
	delete app;
//	MessageBox( 0, L"End", 0, 0 );

	GoSTClear();

	return 0;
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