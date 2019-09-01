#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
#include "lua_module_register.h"
#include "BattleScene.h"
#include "DebugHelper.h"
#include "luaBinding.h"
//#include "lua_extensions.h"

#include "CCMyShader.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_LINUX)
#include "ide-support/CodeIDESupport.h"
#endif

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
#include "runtime/Runtime.h"
#include "ide-support/RuntimeLuaImpl.h"
#endif

#if (TEST_BATTLE)
#include "TestBattle.h"
#endif

using namespace CocosDenshion;

USING_NS_CC;
using namespace std;

#define TEST_HTTP (false)
#if(TEST_HTTP)
#include "TestHttpClient.h"
#endif

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
    // NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
    RuntimeEngine::getInstance()->end();
#endif
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    CCMyShader::initSharder();

#if(TEST_HTTP)
    (new TestHttpClient())->startTestAgent();
#endif

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }
    
    
    //director->getOpenGLView()->setDesignResolutionSize(480, 320, ResolutionPolicy::SHOW_ALL);
    glview->setDesignResolutionSize(1136, 640, ResolutionPolicy::FIXED_WIDTH);
    // turn on display FPS
#if (SHOW_STATS)
    director->setDisplayStats(true);
#else
    director->setDisplayStats(false);
#endif
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60.0);
    
    director->setDepthTest(false);
    
    FileUtils::getInstance()->addSearchPath(cocos2d::FileUtils::getInstance()->getWritablePath());
    FileUtils::getInstance()->addSearchPath("res");
    FileUtils::getInstance()->addSearchPath("res/ui");
    FileUtils::getInstance()->addSearchPath("src/app/views");
    FileUtils::getInstance()->addSearchPath("src/pb");
    FileUtils::getInstance()->addSearchPath("res/items/");

    
#if (USE_LUA_ENGINE)  
    //register custom function
    //LuaStack* stack = engine->getLuaStack();
    //register_custom_function(stack->getLuaState());
    //register custom function
    auto engine = LuaEngine::getInstance();

    //binding game lua
    lua_State* L = engine->getLuaStack()->getLuaState();
//    luaopen_lua_extensions(L);
    bindingAll(L);

    #if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
        // NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
        RuntimeEngine::getInstance()->addRuntime(RuntimeLuaImpl::create(), kRuntimeEngineLua);
        RuntimeEngine::getInstance()->start();
        cocos2d::log("iShow!");
    #else
        //auto engine = LuaEngine::getInstance();
        ScriptEngineManager::getInstance()->setScriptEngine(engine);
        lua_module_register(L);
        engine->getLuaStack()->setXXTEAKeyAndSign("2dxLua", strlen("2dxLua"), "XXTEA", strlen("XXTEA"));
        if (engine->executeScriptFile("src/main.lua"))
        {
            return false;
        }
    #endif

#elif (TEST_BATTLE)
    director->runWithScene(TestBattle::getInstance()->createScene());
    
#else
    // create a scene. it's an autorelease object
    director->runWithScene(BattleSG::createScene());

#endif
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
