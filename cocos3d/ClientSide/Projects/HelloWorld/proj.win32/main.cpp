#include "main.h"
#include "../Classes/AppDelegate.h"
#include "CCEGLView.h"


USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

//#if _DEBUG
//	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
//#endif

	// create the application instance
	AppDelegate app;
	CCEGLView* eglView = CCEGLView::sharedOpenGLView();
	eglView->setViewName( "Hello 3D World" );
	eglView->setFrameSize( SCREEN_WIDTH, SCREEN_HEIGHT );
	CCApplication::sharedApplication()->run();

//#if _DEBUG
//	_CrtDumpMemoryLeaks();
//#endif
}
