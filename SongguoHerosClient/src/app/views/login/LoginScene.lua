_REQUIRE("common/UpdateManager.lua")

local initRequestHelper = _REQUIRE("controller/helper/initRequestHelper.lua")

local LoginScene = class("LoginScene", cc.load("mvc").ViewBase)
LoginScene.RESOURCE_FILENAME = "Scene/LoginScene.csb"
local ctlBase = _REQUIRE("controller.request.ctlBase")

function LoginScene:onCreate()
    initRequestHelper:resetLoadingProgressPercentage() -- 重置登陆请求
    ctlBase:setShowLoading(false)  -- 登陆页面不显示网络加载页
    local panel_login = self:getResourceNode():getChildByName("panel_login")
    
    local Panel_Bg = panel_login:getChildByName("Panel_Bg"):getChildByName("Panel_Bg_Anim"):getChildByName("bg")
    SCREEN_SCALE_BG(Panel_Bg)
    
    local Panel_Center = panel_login:getChildByName("Panel_Center")    
    local Panel_Function = Panel_Center:getChildByName("Panel_Function") 
    
    local txt_userName = Panel_Function:getChildByName("txt_userName")
    local txt_userPwd = Panel_Function:getChildByName("txt_userPwd")
    local btn_login = Panel_Function:getChildByName("btn_login")
    local username = cc.UserDefault:getInstance():getStringForKey("username");
    local password = cc.UserDefault:getInstance():getStringForKey("password");
    txt_userName:setString(username)
    txt_userPwd:setString(password)
    local usermail = "mail@mail.com";
    -- 切换为登陆状态
    self:changeState(1)
    
    -- 显示版本号
    self:showGameVersion()
    
    -- 进入游戏 
    self:addBtnTouchEventListener(btn_login,function(sender,eventType)
        if eventType==2 then
--            if true then
--            	SWITSCENE("mail/MailListScene.lua")
--            	return;
--            end
            --self:showDlg("battle/MopResultScene",{})
            username = txt_userName:getString()
            password = txt_userPwd:getString()
            release_print("password = " .. tostring(password));
            PopMsg.getInstance():flashShow(username)
            if username ~="" and password ~="" then
                --model_user.username = username
                --model_user.password = password
                cc.UserDefault:getInstance():setStringForKey("username",username)
                cc.UserDefault:getInstance():setStringForKey("password",password)
                
                local function onInitComplete()
                    Timer:kill(self.timerId_)
                    ctlBase:setShowLoading(true)
                    SWITSCENE("home/MainCityScene")
                end
                self:changeState(2) -- 切换为loading状态
                self:updateProgressBar()
                initRequestHelper:init(username, password, username.."@mail.com", false, onInitComplete)
            end
        end
    end
    )  
    self.lastPercent = -1
    --SanGuoSoundLib.playMusic("music1")
end

function LoginScene:showGameVersion()
    local versionConfig = UpdateManager.currentConfig
    if versionConfig then
        local panel_login = self:getResourceNode():getChildByName("panel_login")
        local textField = cc.Label:createWithSystemFont("版本号：" .. versionConfig.fullVersion, FONT, 20)
        textField:setAnchorPoint(cc.p(1, 0))
        textField:setPositionX(CC_DESIGN_RESOLUTION.width+(CC_SCREEN_SIZE.width-CC_DESIGN_RESOLUTION.width)/2)
        panel_login:addChild(textField)
    end
end

function LoginScene:updateProgressBar()
  local panel_login = self:getResourceNode():getChildByName("panel_login")
  local Panel_Bottom = panel_login:getChildByName("Panel_Bottom")
  local LoadingBar = Panel_Bottom:getChildByName("LoadingBar")
  local Text_Progress = Panel_Bottom:getChildByName("Text_Progress")
  local Particle_bar = Panel_Bottom:getChildByName("Particle_bar")
  local Bar_Bg = Panel_Bottom:getChildByName("Bar_Bg")  
  
   LoadingBar:setPercent(0)
  self.timerId_ = Timer:start((function()  
        local percent = initRequestHelper:getLoadingProgressPercentage()
        LoadingBar:setPercent(percent)
        Text_Progress:setString((percent).."%")
        
        -- local width = 620 * (CC_SCREEN_SIZE.width / CC_DESIGN_RESOLUTION.width)  
        -- if(self.lastPercent ~= percent/100) then            
        --     local offset = percent/100
        --     if(self.lastPercent ~= -1) then 
        --        offset = percent/100 - self.lastPercent
        --     end
        --     self.lastPercent = percent/100
        --     Particle_bar:setPositionX(Particle_bar:getPositionX() + offset*width) 
        -- end         
 
  end), 0.02)    
end


function LoginScene:changeState(state)
    local panel_login = self:getResourceNode():getChildByName("panel_login")
    local Panel_Center = panel_login:getChildByName("Panel_Center")    
    local Panel_Function = Panel_Center:getChildByName("Panel_Function") 
    local Panel_Bottom = panel_login:getChildByName("Panel_Bottom") 
    local Text_loading = Panel_Center:getChildByName("Text_loading") 
    local Image_1 = Panel_Center:getChildByName("Image_1")
    local Text_Progress = Panel_Bottom:getChildByName("Text_Progress")
    local Particle_bar = Panel_Bottom:getChildByName("Particle_bar")
	if state == 1 then
	   Panel_Function:show()
	   Image_1:show()
	   Panel_Bottom:hide()
	   Text_loading:hide()
       Text_Progress:hide()
       Particle_bar:hide()
	elseif state == 2 then
        Panel_Function:hide()
        Image_1:hide()
        Panel_Bottom:show()
        Text_loading:show()
        Text_Progress:show()
        Particle_bar:hide() --  暂时隐藏
	end
end


return LoginScene
