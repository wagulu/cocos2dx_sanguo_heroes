#ifndef __BATTLESG_SCENE_H__
#define __BATTLESG_SCENE_H__

#include <vector>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "BattleEvent.h"
#include "BattleLoadingUI.h"

using namespace cocos2d;
using namespace ui;

class BattleSG : public cocos2d::Layer, public entityx::Receiver<BattleSG>
{
public:
    static cocos2d::Scene* createScene();

    BattleSG();
    virtual ~BattleSG();
    
    void receive(const BattleEvent::StartBattleProgress& e);
    void receive(const BattleEvent::StartBattleComplete& e);

    CREATE_FUNC(BattleSG);
    
private:
    void createBattleWorld(float dt);
    
    BattleLoadingUI* m_battleLoadingUI = nullptr;
};

extern "C" int createBattle(lua_State *L);

#endif // __BATTLESG_SCENE_H__
