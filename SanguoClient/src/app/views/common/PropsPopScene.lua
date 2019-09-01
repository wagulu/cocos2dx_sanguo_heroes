local PropsPopScene = class("PropsPopScene", cc.load("mvc").ViewBase)
PropsPopScene.NEXTSCENE = "battle/ChapterScene"

PropsPopScene.RESOURCE_FILENAME = "Scene/RechargePopup.csb"

local model_static_library = _REQUIRE "src/model/static/model_static_library.lua"
local model_monsterStorage = _REQUIRE "src/model/model_monsterStorage.lua"

function PropsPopScene:onCreate()
    self:initUi()
end

function PropsPopScene:initUi()
    self.id = self.args[1].id
    self.type = self.args[1].type

    local RechargePopup = self:getResourceNode():getChildByName("RechargePopup")
    local Panel_Bg = RechargePopup:getChildByName("Panel_Bg")
    self.Panel_Center = RechargePopup:getChildByName("Panel_Center")
    local popup = self.Panel_Center:getChildByName("Popup")
    local Text_1 = popup:getChildByName("Text_1")
    local Text_Name = popup:getChildByName("Text_Name")
    local ProjectNode_1 = popup:getChildByName("ProjectNode_1")
    local icon = ProjectNode_1:getChildByName("Panel_Object"):getChildByName("Image_1")
    local frame = ProjectNode_1:getChildByName("Panel_Object"):getChildByName("Image_2")
    
    icon:loadTexture(model_static_library:getAsset(self.id, self.type))
    --Text_1:setString(LANG("%s", model_static_library:getDesc(self.id, self.type)))
    local str = "sdfsdfasdfajsdkjfaoskdjfoisjadoifjsaoidjfoiasjdoifjasoidjfoiasjdoifjasodijf"
    Text_1:setString(str)
    Text_Name:setString(LANG("%s", model_static_library:getName(self.id, self.type)))
    frame:loadTexture(model_static_library:getFrame(self.id, self.type))
    
    Panel_Bg:setTouchEnabled(true)
    Panel_Bg:addTouchEventListener(function(sender, eventType)
        if eventType == 2 then
            cclog("-----up")
            self:removeDlgNoAction()
        end
    end)
end


return PropsPopScene