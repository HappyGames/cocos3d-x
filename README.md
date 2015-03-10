# cocos3d-x
www.cocos3dx.org


the c++ version of cocos3d
Powerful cocos3d IOS game engine and run on Android and many other platforms,

Cocos3dx is easy to use and developers who are familiar with cocos2dx will learn and understand cocos3dx very quickly.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
COCOS2D-X -->> COCOS3D-X  PORT INSTRUCTIONS
1. CCAction: ///////////////////////////////////////////////////////////

.h
add a member(void* m_pUserData) and two inline member accessors
	inline void setUserData( void* userData ) { m_pUserData = userData; }
	inline void* getUserData() { return m_pUserData; }
	
.cpp
add constructor initializer of m_pUserData

2. CCActionManager:  ////////////////////////////////////////////////////
	to override the CCActionManager we need do the flowing modifications
.h 
move struct _hashElement declaration from .cpp to .h
change 'void removeAction(CCAction *pAction)' to 'virtual void removeAction(CCObject *pAction)'
	
.cpp
remove struct _hashElement declaration 
modify the implementation of void CCActionManager::removeAction(CCObject *pAction)
change 'CCObject *pTarget = pAction->getOriginalTarget()' under 'tHashElement *pElement = NULL;' to 'CCObject *pTarget = ((CCAction*)pAction)->getOriginalTarget()' 
	and leave other codes unchanged.
	

3. CCNode: ////////////////////////////////////////////////////////////

.h
make function 'void transform(void)' virtual

3. CCParticleSystem: //////////////////////////////////////////////////

.cpp
	find 'm_pParent->removeChild(this, true) in CCParticleSystem::update( float )' and add parent non-null condition as below
		if ( m_pParent != NULL )
			m_pParent->removeChild(this, true);
			
4. CCParticleSystemQuad: //////////////////////////////////////////////

.h 
add getter member function for m_pQuads and m_uParticleIdx as below
	const ccV3F_C4B_T2F_Quad* getQuads() { return m_pQuads; };
	unsigned int  getParticleIndex() { return m_uParticleIdx; };

5. CCEGEView: /////////////////////////////////////////////////////////


6. CCTexture2D: ///////////////////////////////////////////////////////

.h
make function 'bool initWithData(const void* data, CCTexture2DPixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const CCSize& contentSize)' virtual 

7. CCDirector: ////////////////////////////////////////////////////////

.cpp
modify the implementation of void CCDirector::purgeDirector() as below after 'CHECK_GL_ERROR_DEBUG();' in this function
	#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		// OpenGL view
		// m_pobOpenGLView was allocated on the heap, keep the address here
		CCEGLView* pGLView = m_pobOpenGLView;

		// safe to end the glview now
		m_pobOpenGLView->end();
		m_pobOpenGLView = NULL;

		// delete CCDirector
		release();

		// destroy gl after all gl resources are released
		pGLView->destroyGL();
		CC_SAFE_DELETE( pGLView );
	#else
		// OpenGL view
		m_pobOpenGLView->end();
		m_pobOpenGLView = NULL;

		// delete CCDirector
		release();
	#endif
	
8. base64
	
.cpp
	to fix the compile error on android, include <cstdio>.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////