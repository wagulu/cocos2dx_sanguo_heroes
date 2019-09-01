local PlayerInfoPopPanel = class("PlayerInfoPopPanel", cc.load("mvc").ViewBase)
local initRequestHelper = _REQUIRE("controller/helper/initRequestHelper.lua")

_REQUIRE "cocos.cocos2d.json"
PlayerInfoPopPanel.RESOURCE_FILENAME = "Scene/PlayerInfoScene2.csb"
local model_user = _REQUIRE("model/model_user")

function PlayerInfoPopPanel:onCreate()
    local panel_MainScene = self:getResourceNode():getChildByName("PlayerInfo2")
    panel_MainScene:setTouchEnabled(true)
    local panel_Center = panel_MainScene:getChildByName("Panel_Center")
    local panel_R = panel_Center:getChildByName("Panel_R")
    local Panel_Bg = panel_MainScene:getChildByName("Panel_Bg")
    panel_R:setTouchEnabled(true)
    -- 更换形象
    local btn_4 = panel_R:getChildByName("Button_4")
    btn_4:hide()
    -- 修改
    local btn_5 = panel_R:getChildByName("Button_5")
    btn_5:hide()
    -- 更换
    local btn_6 = panel_R:getChildByName("Button_6")
    btn_6:hide()
    --  点击空白关闭
    SCREEN_SCALE_BG(Panel_Bg)
    Panel_Bg:setTouchEnabled(true)
    self:addBtnTouchEventListener(Panel_Bg,function(sender,eventType)
        if eventType == 2 then
            self:removeDlgNoAction()
        end
    end)
    -- 头像显示
    local headNode = self:createNode("Node/Node_RisingStar_Head.csb")
    local ProjectNode_1 = panel_R:getChildByName("ProjectNode_1")
    ProjectNode_1:addChild(headNode)
    -- 账号id
    local Text_4 = panel_R:getChildByName("Text_4")
    Text_4:setString(LANG(model_user.userName))
    -- 返回登陆
    local Button_7 = panel_R:getChildByName("Button_7") 
    self:addBtnTouchEventListener(Button_7,function(sender,eventType)
        if eventType==2 then
            self:showDlg("common/CommonPopScene",{confirmStr=LANG("是"),cancelStr=LANG("否"),text=LANG("是否要返回登录页?"),
                fun=function(data)
                    if data.cmd=="confirm" then -- 返回登陆页面请求
                        SWITSCENE("login/LoginScene")
                    elseif data.cmd =="cancel" then 

                    end
                end})
        end 
    end)
    -- 系统设置
    local Button_8 = panel_R:getChildByName("Button_8")    
    self:addBtnTouchEventListener(Button_8,function(sender,eventType)
        if eventType==2 then
            self:showDlg("home/SettingScene")
        end
    end)
end

return PlayerInfoPopPanel