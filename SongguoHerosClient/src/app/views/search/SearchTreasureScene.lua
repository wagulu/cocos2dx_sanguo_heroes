--寻宝界面
local SearchTreasureScene = class("SearchTreasureScene", cc.load("mvc").ViewBase)
SearchTreasureScene.NEXTSCENE = "battle/ChapterScene"
--_REQUIRE "src/packages/packet/Packet_Regester"

SearchTreasureScene.RESOURCE_FILENAME = "Scene/SearchScene.csb"
--local sceneLogic=_REQUIRE "battle.WinPopupSceneLogic"
function SearchTreasureScene:onCreate()
    self:initUi()
end

function SearchTreasureScene:initUi()
    local Panel_TreasureHunt = self:getResourceNode():getChildByName("Panel_TreasureHunt")
    local Panel_Center = Panel_TreasureHunt:getChildByName("Panel_Center")
    local Panel_Top = Panel_TreasureHunt:getChildByName("Panel_Top")
    local Panel_Bg = Panel_TreasureHunt:getChildByName("Panel_Bg")
    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    --返回按钮
    local  Button_Back=Panel_Top:getChildByName("Button_Back")
    Button_Back:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            SWITSCENE("home/HomeScene")
        end
    end
    )
    --合成按钮
    local  Button_Synthesis=Panel_Center:getChildByName("Button_Synthesis")
    Button_Synthesis:addTouchEventListener(function(sender,eventType)
        self:onCompoundHandle()
--        SWITSCENE("search/SearchResultScene")-- 测试代码
    end
    )

    --珍宝列表
    local scrollView = Panel_Center:getChildByName("ScrollView_1")
    local contentSize =cc.size(69,68)
    local node=self:createNode("Node/Node_Treasure_Bag.csb")
    local item = node:getChildByName("Panel_Treasure_Icon")
    self.space = 10
    local num = 20
    local sSize=cc.size(math.max(scrollView:getInnerContainerSize().width,(contentSize.width+self.space)*num),scrollView:getContentSize().height);
    scrollView:setInnerContainerSize(sSize)
    for i=1,num do
        local newItem = item:clone()
        --newItem:setAnchorPoint(cc.p(0,0))
        newItem:setTouchEnabled(true);
        newItem:addTouchEventListener(function(sender,eventType)
            if eventType ==2 then
                
            end
        end
        )
        local index = i-1
        newItem:setTag(index);
        scrollView:addChild(newItem)
        local x=self.space+contentSize.width/2+((self.space+contentSize.width)*index)
        --local t = math.floor(index/2)
        local y=sSize.height/2
        newItem:setPosition(cc.p(x,y))
    end


    --    self.searchButtons = {}
    --四个搜寻按钮
    for i=1,3 do
        local Button_Hunt = Panel_Center:getChildByName("Node_Hunt_"..i):getChildByName("Panel_TreasureScene"):getChildByName("Button_Hunt")
        local Image_Bg = Panel_Center:getChildByName("Node_Hunt_"..i):getChildByName("Panel_TreasureScene"):getChildByName("Image_Bg") -- 珍宝图片
        Image_Bg:setTouchEnabled(true);
--        self.buttons[i] = btn
        Button_Hunt:setTag(i)
        Image_Bg:setTag(i)
        Button_Hunt:addTouchEventListener(function(sender)
            self:onBtnHandle(sender:getTag())
        end)
        Image_Bg:addTouchEventListener(function(sender)
            self:onImgHandle(sender:getTag())
        end)
    end
end

function SearchTreasureScene:onBtnHandle(index)
--    SWITSCENE("search/SearchScene")
end
--点击图片弹出材料信息弹窗--------------问下材料信息弹窗是哪个
function SearchTreasureScene:onImgHandle(index)
--    SWITSCENE("/")
end

function SearchTreasureScene:onCompoundHandle()
--如果碎片数量足够，则扣除该珍宝所需的合成材料，并合成出珍宝物

--如果碎片数量不足，浮动文字提示玩家：凑齐所有合成材料，方可合成珍宝
end

return SearchTreasureScene

