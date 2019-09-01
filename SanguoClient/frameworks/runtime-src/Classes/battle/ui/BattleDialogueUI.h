//
//  BattleDialogueUI.h
//  sanguoClient
//
//  Created by fuchenhao on 6/23/15.
//
//

#ifndef __sanguoClient__BattleDialogueUI__
#define __sanguoClient__BattleDialogueUI__

#include <vector>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "battle/BattleConfig.h"
#include "BattleEvent.h"
#include "BattleConfig.h"

using namespace cocos2d;
using namespace ui;

class BattleDialogueUI : public cocos2d::Layer, public entityx::Receiver<BattleDialogueUI>
{
    
public:
    
    BattleDialogueUI();
    virtual ~BattleDialogueUI();
    void update(float dt) override;
    
    void receive(const BattleEvent::ShowStartDialogue& e);
    void receive(const BattleEvent::ShowWinDialogue& e);
    void receive(const BattleEvent::ShowLoseDialogue& e);
    
    CREATE_FUNC(BattleDialogueUI);
    
private:
    
    enum Step
    {
        APPEARING,
        PAUSING,
        DISAPPEARING
    };
    
    enum DialogueType
    {
        START,
        WIN,
        LOSE
    };
    
    void complete();
    void showCurrent();
    void next(cocos2d::Ref* ref);
    
    std::vector<BattleConfig::Dialogue*> *m_dialogues;
    Node* m_battleDialogueUI;
    cocostudio::timeline::ActionTimeline* m_dialogueTimeline;
    int m_currentDialogue;
    Step m_crrentStep;
    bool m_canShowNext = false;
    DialogueType m_type;
};

#endif /* defined(__sanguoClient__BattleDialogueUI__) */
