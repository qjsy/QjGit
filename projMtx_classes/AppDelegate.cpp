
#include "AppDelegate.h"
#include "LoginScene.h"
#include "GameState.h"
#include "HeroManager.h"
#include "CopyManager.h"
#include "ProcessData.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    // turn off display FPS
    pDirector->setDisplayStats(true);
//    pDirector->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

//    登录场景
    CCScene *pScene = LoginScene::create();

    // run
    pDirector->runWithScene(pScene);

    HeroManager::getInstance();
    CopyManager::getInstance();
    
    /*启动加载本地数据的线程*/
    ProcessData::getInstance()->startLoadLocalDataThread();
    /*启动计时线程*/
    GameState::getInstance()->startTimerThread();
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
