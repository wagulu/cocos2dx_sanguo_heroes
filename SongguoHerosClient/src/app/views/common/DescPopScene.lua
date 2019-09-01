local DescPopScene = class("DescPopScene", cc.load("mvc").ViewBase)
_REQUIRE "cocos.cocos2d.json"
DescPopScene.RESOURCE_FILENAME = "Scene/TextPopup.csb"

local model_user = _REQUIRE("model/model_user")

function DescPopScene:onCreate()
    --cclog("精力体力恢复时间戳 %s %s %s %s", model_user.time_to_next_energy, model_user.time_to_next_vitality,model_user.time_to_energy_max ,model_user.time_to_vitality_max)
    local data = self.args[1]

    local pos = data.pos
    self.type_ = data.t_type
    local TextPopup = self:getResourceNode():getChildByName("TextPopup")
    local Panel_1 = TextPopup:getChildByName("Panel_1")
    
    self:refershUI(Panel_1,data)
 
    local Panel_Bg = TextPopup:getChildByName("Panel_Bg")  
    SCREEN_SCALE_BG(Panel_Bg)
    Panel_Bg:setTouchEnabled(true)
    self:addBtnTouchEventListener(Panel_Bg, function(sender,eventType)
       if eventType==2 then
          Timer:kill(self.timerId)
          self.timerId = nil
          self:removeDlgNoAction()
       end
    end)   
    Panel_1:setPosition(cc.p(pos.x,pos.y - Panel_1:getChildByName("Image_popup_bg_1"):getContentSize().height * 0.5))
end

function DescPopScene:refershUI(Panel_1, data)
    
    local label_Energy_L = data.label_Energy_L or nil
    local label_Energy_R = data.label_Energy_R or nil
    
    local label_Vitality_L_ = data.label_Vitality_L or nil            
    local label_Vitality_R_ = data.label_Vitality_R or nil
   
    --  判定是否达到满点 
    if (model_user.time_to_energy_max == 0 and self.type_ == 1) then
         self:changeMaxUI(Panel_1)
         return
    elseif(model_user.time_to_vitality_max == 0 and self.type_ == 2) then
         self:changeMaxUI(Panel_1)
         return
    end   
    local text = 
        {
            data.text1,
            data.text2,
            data.text3,
            data.text4
        }
    local leftTime1 = 0
    local leftTime2 = 0
    local strTime1 = 0
    local strTime2 = 0
    local bAdd = false
 
    if(self.type_ == 1) then
        leftTime1,leftTime2,bAdd = self:getEnergyTime()     
    elseif (self.type_ == 2) then
        leftTime1,leftTime2,bAdd = self:getVitalityTime() 
    end
    local strTime1 = self:getFormationSec(leftTime1)
    local strTime2 = self:getFormationSec(leftTime2)
    
    for i = 1,4  do
        local label_Text = Panel_1:getChildByName(LANG("Text_%d",i))
        if i == 2 then
            label_Text:setString(LANG("%s%s",text[i],strTime1))
        elseif i == 3 then
            label_Text:setString(LANG("%s%s",text[i],strTime2))
        else 
            label_Text:setString(LANG(text[i]))
        end
    end 
    
    self.timerId = Timer:start(function()
        if(self.type_ == 1) then
            leftTime1,leftTime2,bAdd = self:getEnergyTime()     
        elseif (self.type_ == 2) then
            leftTime1,leftTime2,bAdd = self:getVitalityTime() 
        end
            strTime1 = self:getFormationSec(leftTime1)
            strTime2 = self:getFormationSec(leftTime2)
        for i = 1,4  do
            local label_Text = Panel_1:getChildByName(LANG("Text_%d",i))
            if i == 2 then
                label_Text:setString(LANG("%s%s",text[i],strTime1))
            elseif i == 3 then
                label_Text:setString(LANG("%s%s",text[i],strTime2))
            else 
                label_Text:setString(LANG(text[i]))
            end
        end  

        if label_Energy_L then
            local tmp = model_user.energy
            if bAdd then
                tmp = model_user.energy + 1
            end
            label_Energy_L:setString(tmp)
            label_Energy_R:setString(model_user.energyMax)
        end
        
        if label_Vitality_L_ then
            local tmp = model_user.vitality
            if bAdd then
                tmp = model_user.vitality + 1
            end
            label_Vitality_L_:setString(tmp)
            label_Vitality_R_:setString(model_user.vitalityMax)

        end

    end,1) -- 1秒刷新一次
end

function DescPopScene:changeMaxUI(Panel_1)
   for i = 1,4  do
        local label_Text = Panel_1:getChildByName(LANG("Text_%d",i))
        label_Text:hide()
        if i == 1 then
            label_Text:show()
            if self.type_ == 1 then  
                label_Text:setString(LANG("当前体力已达到最大值"))  
            elseif self.type_ == 2 then 
                label_Text:setString(LANG("当前精力已达到最大值"))
            end
        end
   end 
end

function DescPopScene:getEnergyTime()
    local ENERGY_RECOVER_INTERVAL_SEC = 360
    local time = checknumber(os.time())
    local next = checknumber(model_user.time_to_next_energy)
    local max =  checknumber(model_user.time_to_energy_max)
    local time1 = 0
    local time2 = 0
    local bAdd = false
    if(next > 0) then
        if(time >= next and not(model_user:isEnergyFull())) then
            next = next + ENERGY_RECOVER_INTERVAL_SEC
            bAdd = true
        end
        if not(model_user:isEnergyFull()) then
           time1 = (next - time)
           time2 = (max -time)
        end
    end
    return time1,time2,bAdd
end

function DescPopScene:getVitalityTime()
    local VITALITY_INTERVAL_SEC = 600  
    local time = checknumber(os.time())
    local next = checknumber(model_user.time_to_next_vitality)
    local max =  checknumber(model_user.time_to_vitality_max)
    local time1 = 0
    local time2 = 0
    local bAdd = false
    if(next > 0) then
        if(time >= next and not(model_user:isVitalityFull())) then
            next = next + VITALITY_INTERVAL_SEC
            bAdd = true
        end
        if not(model_user:isVitalityFull()) then
            time1 = (next - time)
            time2 = (max -time)
        end
    end
    return time1,time2,bAdd
end

function DescPopScene:getFormationSec(lTime)
    local leftTime = checknumber(lTime)
    if(leftTime > 0) then
	   local second = math.floor(leftTime % 60)
       local minute = math.floor((leftTime - second) % 3600 / 60)
       local hour = math.floor((leftTime - second - minute) / 3600)
        return LANG("%02d:%02d:%02d", hour, minute, second)
   end
   return LANG("00:00:00")
end

return DescPopScene