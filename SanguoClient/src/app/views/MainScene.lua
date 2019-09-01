
local ctlAdventureListAreaRequest = _REQUIRE("controller.request.ctlAdventureListAreaRequest");
local ctlAdventureListStageRequest = _REQUIRE("controller.request.ctlAdventureListStageRequest");
local delayCallback = _REQUIRE("common/delayCallback.lua")

local MainScene = class("MainScene", cc.load("mvc").ViewBase)
--_REQUIRE "src/packages/packet/Packet_Regester"

MainScene.RESOURCE_FILENAME = "";
--MainScene.RESOURCE_FILENAME = "Scence/MainScene.csb"

MainScene.animationType = 1;
MainScene.handle = 0;

--Sprite* Utils::maskedSpriteWithSprite(Sprite* textureSprite, Sprite* maskSprite)
--{
--    if (textureSprite->getContentSize().width<maskSprite->getContentSize().width && textureSprite->getContentSize().height<maskSprite->getContentSize().height) {
--        return textureSprite;
--    }
--    // 1
--    RenderTexture * rt = RenderTexture::create( maskSprite->getContentSize().width,
--                                               maskSprite->getContentSize().height );
--    
--    // 2
--    maskSprite->setPosition(maskSprite->getContentSize().width/2,
--                            maskSprite->getContentSize().height/2);
--    textureSprite->setPosition(textureSprite->getContentSize().width/2,
--                               textureSprite->getContentSize().height/2);
--    
--    // 3
--    maskSprite->setBlendFunc( BlendFunc{GL_ONE, GL_ZERO} );
--    textureSprite->setBlendFunc( BlendFunc{GL_DST_ALPHA, GL_ZERO} );
--    
--    // 4
--    rt->begin();
--    maskSprite->visit();
--    textureSprite->visit();
--    rt->end();
--    
--    // 5
--    Sprite *retval = Sprite::createWithTexture(rt->getSprite()->getTexture());
--    retval->setFlippedY(true);
--    return retval;
--}

function MainScene:getCircleImg(textureSprite,maskSprite)
    if (textureSprite:getContentSize().width < maskSprite:getContentSize().width and textureSprite:getContentSize().height < maskSprite:getContentSize().height) then
        return textureSprite
    end
    cclog("maskSprite:"..maskSprite:getContentSize().width)
    local rt = cc.RenderTexture:create(maskSprite:getContentSize().width,maskSprite:getContentSize().width)
    maskSprite:setPosition(maskSprite:getContentSize().width/2,maskSprite:getContentSize().height/2)
    textureSprite:setPosition(textureSprite:getContentSize().width/2,textureSprite:getContentSize().height/2)
    
    maskSprite:setBlendFunc( cc.blendFunc(gl.DST_COLOR, gl.ZERO) );
    textureSprite:setBlendFunc( cc.blendFunc(gl.DST_ALPHA, gl.ZERO) );
    

    
    rt:begin();
    maskSprite:visit();
    textureSprite:visit();
    rt:endToLua()
    
    local retval = cc.Sprite:createWithTexture(rt:getSprite():getTexture());
    
    retval:setFlippedY(true);
    return retval;
end

function MainScene:testClipingNode()
    local layerColor = cc.LayerColor:create(ccc4(0, 128, 0, 150 ), 2000,2000)
    self:addChild(layerColor)
    local content = display.newSprite("Images/Monster_2.png")   
    local sp1 = display.newSprite("Images/hero_circle_4.png")

    local clipNode =  cc.ClippingNode:create() 
    clipNode:setStencil(sp1)
    clipNode:setInverted(false)
    clipNode:setAlphaThreshold(0)
    self:addChild(clipNode)
    clipNode:addChild(content)
    clipNode:setPosition(cc.p(500,300))

end

function MainScene:onEnter()
--    local image3 = self:getCircleImg(self.img1,self.img2)
--    self:addChild(image3)
--    image3:setPosition(200,300)
end




function MainScene:onCreate()

    local imgButton = ccui.ImageView:create("MoppingTicketItem/MoppingTicketItem_1.png")
    imgButton:setPosition(display.cx,display.cy)
    self:addChild(imgButton)
    imgButton:setTouchEnabled(true)
    imgButton:addTouchEventListener(function(sender,eventType)
        if eventType == 2 then
            local img2 = cc.Sprite:create("Monster/hero_circle_4.png")
            img2:setPosition(200,300)
            --self:addChild(img2)
            local image3 = self:getCircleImg(self.img1,img2)
            self:addChild(image3)
            
            image3:setPosition(display.cx,display.cy)
        end
    end)

    
    local img1 = cc.Sprite:create("MoppingTicketItem/MoppingTicketItem_1.png")
    self.img1 = img1
    self:addChild(img1)
    img1:setPosition(100,200)
    
    
--    local function onTimer()
--        if(self.animationType == 1) then
--            self.animationType = 2;
--            SanGuoAnimationLib.setCurrentAnimation(self.handle, "animation_death");
--        else
--            self.animationType = 1;
--            SanGuoAnimationLib.setCurrentAnimation(self.handle, "animation_idle");
--        end
--        
--        delayCallback:create(onTimer, 5);    
--    end

    --self:testAdvanceAnimation(self);
    
--    local function onTimer()
--        SanGuoAnimationLib.freeAllAdvancedAnimation();
--        SanGuoAnimationLib.freeAllAdvancedAnimationAsset();
--        SWITSCENE("login/LoginScene")
--    end
--    delayCallback:create(onTimer, 1);    
    self._richText = ccui.RichText:create()
    self._richText:ignoreContentAdaptWithSize(false)
    self._richText:setContentSize(cc.size(100, 100))

    local re1 = ccui.RichElementText:create(1, cc.c3b(255, 255, 255), 255, "This color is white. ", "Helvetica", 10)
    local re2 = ccui.RichElementText:create(2, cc.c3b(255, 255,   0), 255, "And this is yellow. ", "Helvetica", 10)
    local re3 = ccui.RichElementText:create(3, cc.c3b(0,   0, 255), 255, "This one is blue. ", "Helvetica", 10)
    local re4 = ccui.RichElementText:create(4, cc.c3b(0, 255,   0), 255, "And green. ", "Helvetica", 10)
    local re5 = ccui.RichElementText:create(5, cc.c3b(255,  0,   0), 255, "Last one is red ", "Helvetica", 10)

--    local reimg = ccui.RichElementImage:create(6, cc.c3b(255, 255, 255), 255, "cocosui/sliderballnormal.png")
--
--    ccs.ArmatureDataManager:getInstance():addArmatureFileInfo("cocosui/100/100.ExportJson")
--    local arr = ccs.Armature:create("100")
--    arr:getAnimation():play("Animation1")
--
--    local recustom = ccui.RichElementCustomNode:create(1, cc.c3b(255, 255, 255), 255, arr)
--    local re6 = ccui.RichElementText:create(7, cc.c3b(255, 127,   0), 255, "Have fun!! ", "Helvetica", 10)
    self._richText:pushBackElement(re1)
    self._richText:insertElement(re2, 1)
    self._richText:pushBackElement(re3)
    self._richText:pushBackElement(re4)
    self._richText:pushBackElement(re5)
--    self._richText:insertElement(reimg, 2)
--    self._richText:pushBackElement(recustom)
--    self._richText:pushBackElement(re6)

    self._richText:setPosition(cc.p(300, 400))
    self._richText:setLocalZOrder(10)


    self:addChild(self._richText)

end



function MainScene:btnBackCallback(sender, eventType)
    if eventType == 2 then
    end
end


function MainScene:testAdvanceAnimation(root)
    SanGuoAnimationLib.freeAllAdvancedAnimation();
    SanGuoAnimationLib.freeAllAdvancedAnimationAsset();
    for i = 1,10 do 
        local node = SanGuoAnimationLib.createAdvancedAnimation("hero_11", "animation_idle");
        --    release_print(node:getScaleX());
        --    release_print(node:getScaleY());
        --    release_print(node:getScaleZ());
        --    release_print(node:getPositionX());
        --    release_print(node:getPositionY());
        --    release_print(node:getPositionZ());
        --    release_print(node:isVisible());
        node:setPositionX(100 + i*50);
        node:setPositionY(100+ 50*i);

        self.handle = node:getTag();
        SanGuoAnimationLib.play(self.handle);
        root:addChild(node);
    end
    
end


return MainScene
