//
//  SGNode.h
//  SGNode
//
//  Created by fuchenhao on 1/4/15.
//
//

/*

#ifndef __SG_NODE_H__
#define __SG_NODE_H__

#include "cocos2d.h"
#include "SGCamera.h"

class CameraTwins;

class SGNode : public cocos2d::Node
{
public:
    static SGNode* create();
    SGCamera* getRoleCamera(CameraTwins &inTwin);
    SGCamera* getBGCamera(CameraTwins &inTwin);

private:
    cocos2d::CustomCommand _afterDrawCommand;
    
    bool removeDone;
    float clippingPos;
    void resetClippingPos();
    
    void roleClipping(GLint x, GLint y, GLsizei width, GLsizei height,
                      cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags);
    void cocosClipping(float x, float y, float width, float height);
    
    virtual bool init() override;
    
    void removeCamera2(float dt);
    void visitTwoCameras(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags);
    void visitTwoCamerasWithAni(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags);
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags) override;
};

#endif *//* defined(__SG_NODE_H__) */
