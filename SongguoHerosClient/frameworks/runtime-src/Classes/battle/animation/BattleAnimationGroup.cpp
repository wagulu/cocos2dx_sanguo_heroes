
#include "BattleAnimationGroup.h"

BattleAnimationGroup::BattleAnimationGroup(std::vector<BattleAnimation*> *animations) : animations(animations) {};

BattleAnimationGroup::~BattleAnimationGroup()
{
    for(BattleAnimation *animation : *animations)
    {
        delete animation;
    }
    animations->clear();
}

void BattleAnimationGroup::playCurrent(bool loop, bool randomStart)
{
    for(BattleAnimation *animation : *animations)
    {
        animation->getAnimation()->setVisible(true);
        animation->playCurrent(loop, randomStart);
    }
}


void BattleAnimationGroup::hideEffect()
{
    for(BattleAnimation *animation : *animations)
    {
        animation->getAnimation()->setVisible(false);
    }
}

void BattleAnimationGroup::setPositionX(float x)
{
    for(BattleAnimation *animation : *animations)
    {
        animation->setPositionX(x);
    }
}

void BattleAnimationGroup::setPosition(float x, float y, float z)
{
    for(BattleAnimation *animation : *animations)
    {
        animation->setPosition(x, y, z);
    }
}

void BattleAnimationGroup::setScale(float scale)
{
    for(BattleAnimation *animation : *animations)
    {
        animation->setScale(scale);
    }
}

void BattleAnimationGroup::setVisible(bool visible)
{
    for(BattleAnimation *animation : *animations)
    {
        animation->setVisible(visible);
    }
}

bool BattleAnimationGroup::isComplete()
{
    for(BattleAnimation *animation : *animations)
    {
        if(!animation->isComplete())
        {
            return false;
        }
    }
    return true;
}

void BattleAnimationGroup::update(float dt)
{
    for(BattleAnimation *animation : *animations)
    {
        animation->update(dt);
    }
}

