#include "AppDelegate.h"
#include "platform/android/jni/JniHelper.h"
//#include "google-breakpad/client/linux/handler/minidump_descriptor.h"
//#include "google-breakpad/client/linux/handler/exception_handler.h"
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

extern "C"
{

void initDumper();

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JniHelper::setJavaVM(vm);
	
	LOGD( "JNI_OnLoad" );

//	initDumper();

    return JNI_VERSION_1_4;
}

void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit(JNIEnv*  env, jobject thiz, jint w, jint h)
{
    if (!CCDirector::sharedDirector()->getOpenGLView())
    {
        CCEGLView *view = CCEGLView::sharedOpenGLView();
        view->setFrameSize(w, h);

		__android_log_print(ANDROID_LOG_INFO, "FrameSize", "[%d, %d]", w, h );

        AppDelegate *pAppDelegate = new AppDelegate();
        CCApplication::sharedApplication()->run();
    }
    else
    {
        ccDrawInit();
        ccGLInvalidateStateCache();
        
        CCShaderCache::sharedShaderCache()->reloadDefaultShaders();
        CCTextureCache::reloadAllTextures();
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_COME_TO_FOREGROUND, NULL);
        CCDirector::sharedDirector()->setGLDefaultValues(); 
		
		__android_log_print(ANDROID_LOG_INFO, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit", "ddd11111" );
    }
}

#if 0
//!	The dump callback function.
static bool DumpCallback( const google_breakpad::MinidumpDescriptor& descriptor, void* context, bool succeeded )
{
	// Get the dump file path
	std::string dump_filepath = descriptor.path( );

	// Output the dump file path
	LOGD( "CRASH ! The dump file path is '%s'", dump_filepath.c_str() );

	return succeeded;
}

void initDumper()
{
	std::string sWritablePath = "/sdcard";
	static google_breakpad::MinidumpDescriptor sDescriptor( sWritablePath.c_str() );
	static google_breakpad::ExceptionHandler sExceptionHandler( sDescriptor, NULL, DumpCallback, NULL, true, -1 );
}
#endif

}
