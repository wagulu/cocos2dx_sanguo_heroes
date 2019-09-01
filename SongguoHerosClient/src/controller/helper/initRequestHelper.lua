
_REQUIRE("controller/ctlGlobal")
local model_user = _REQUIRE("model.model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")

local helper = {};
helper.progressLogin = 0;
helper.progressInit = 0;

local username = "fuchenhao";
local password = "fuchenhao";
local usermail = "mail@mail.com";
local reset = false;
local callback = nil;



local function onInitMonsterParts(success, data, code, msg)

    helper.progressInit = helper.progressInit + 1;
    release_print("loading percentage = " .. tostring(helper:getLoadingProgressPercentage()));

    if(success) then
        local haveMonsters = model_monsterStorage:getAllActiveMonsters();
        if(#(haveMonsters) == 0) then
            release_print("ERROR, don't have any actived monsters");
        end

        CTL_INFO("init process complete");
        
        if(callback) then
            callback();
        end
    else
        CTL_WARNING(msg);
    end
end
local function initMonsterParts()
    ctlInitMonsterPartsRequest:sendRequest(onInitMonsterParts);
end


local function onInitMaterialSynthesis(success, data, code, msg)

    helper.progressInit = helper.progressInit + 1;

    if(success) then
    --        listMonsters();
    else
        CTL_WARNING(msg);
    end
end
local function initMaterialSynthesis()
    ctlInitMaterialSynthesisRequest:sendRequest(onInitMaterialSynthesis);
end


local function onInitItem(success, data, code, msg)

    helper.progressInit = helper.progressInit + 1;

    if(success) then
    else
        CTL_WARNING(msg);
    end
end
local function initItem()
    ctlInitItemsRequest:sendRequest(onInitItem);
end


local function onInitTreasureItem(success, data, code, msg)

    helper.progressInit = helper.progressInit + 1;

    if(success) then
    else
        CTL_WARNING(msg);
    end
end
local function initTreasureItem()
    ctlInitTreasureItemsRequest:sendRequest(onInitTreasureItem);
end


local function onInitTreasureItemPart(success, data, code, msg)

    helper.progressInit = helper.progressInit + 1;

    if(success) then
    else
        CTL_WARNING(msg);
    end
end
local function initTreasureItemPart()
    ctlInitTreasureItemPartsRequest:sendRequest(onInitTreasureItemPart);
end


local function onInitMonster(success, data, code, msg)

    helper.progressInit = helper.progressInit + 1;

    if(success) then
    else
        CTL_WARNING(msg);
    end
end
local function initMonster()
    ctlInitMonstersRequest:sendRequest(onInitMonster);
end


local function onInitMaterial(success, data, code, msg)

    helper.progressInit = helper.progressInit + 1;

    if(success) then
    else
        CTL_WARNING(msg);
    end
end
local function initMaterial()
    ctlInitMaterialRequest:sendRequest(onInitMaterial);
end


local function onInitTeam(success, data, code, msg)

    helper.progressInit = helper.progressInit + 1;

    if(success) then
    else
        CTL_WARNING(msg);
    end
end
local function initTeam()
    ctlInitBattleTeamRequest:sendRequest(onInitTeam);
end


local function onInitGacha(success, data, code, msg)

    helper.progressInit = helper.progressInit + 1;

    if(success) then
    else
        CTL_WARNING(msg);
    end
end
local function initGacha()
    ctlInitGachaRequest:sendRequest(onInitGacha);
end



local function onInitTreasureBox(success, data, code, msg)

    helper.progressInit = helper.progressInit + 1;

    if(success) then
    else
        CTL_WARNING(msg);
    end
end
local function initTreasureBox()
    ctlInitTreasureBoxRequest:sendRequest(onInitTreasureBox);
end



local function onInitUser(success, data, code, msg)

    helper.progressInit = helper.progressInit + 1;

    if(success) then
    else
        CTL_WARNING(msg);
    end
end
local function initUser()
    ctlInitUserRequest:sendRequest(onInitUser);
end



-----------------------------------------------------------
--test login

local function onGetUidFromSNUidRequest(success, data, code, msg)

    helper.progressLogin = helper.progressLogin + 1;
    
    if(GLOBAL_MY_APP) then
        GLOBAL_MY_APP:showDlg("common/CommonPopScene",{},{confirmStr=LANG("确定"),cancelStr=LANG("取消"),text="userName"..data.uid})
    end
    
    if(success) then
        CTL_INFO("register / login process complete");

        --don't change order
        initUser();
        initTreasureBox();
        initGacha();
        initTeam();
        initMaterial();
        initMaterialSynthesis();
        initTreasureItemPart();
        initTreasureItem();
        initItem();
        initMonster();
        initMonsterParts();
    else
        CTL_WARNING(msg);
    end
end
local function getUidFromSNUidRequest()
    ctlAccountGetUidFromSNUidRequest:sendRequest(onGetUidFromSNUidRequest);
end


local function onNetworkLogin(success, data, code, msg)

    helper.progressLogin = helper.progressLogin + 1;

    if(success) then
        getUidFromSNUidRequest();
    else
        CTL_WARNING(msg);
    end
end
local function networkLogin()
    ctlAccountNetworkLoginRequest:sendRequest(onNetworkLogin);
end


local function onLogin(success, data, code, msg)
    if(success) then
        CTL_INFO("login success");
        networkLogin();
    else
        CTL_WARNING(msg);
    end
end
local function login()
    ctlAccountLoginRequest:sendRequest(onLogin, username, password);
end


local function onRegister(success, data, code, msg)
    if(success) then
        CTL_INFO("register success");
        networkLogin();
    else
        --TODO, need to check code if register fail
        login();
    end
end
local function register()
    ctlAccountRegisterRequest:sendRequest(onRegister, username, password, usermail);
end



local function accountReset()
    local httpData = "";
    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        SERVER_URL .. "help.php?userId=26",
        "ctlAccountResetRequest");
end


--helper

function helper:init(_username, _password, _usermail, _reset, _callback)
    username = _username;
    password = _password;
    usermail = _usermail;
    reset = _reset;
    callback = _callback;

    if(reset) then
        accountReset();
    end

    register();
end

function helper:getLoadingProgressPercentage()
    local TOTAL_LOGIN_STEP = 2;
    local TOTAL_INIT_STEP = 11;
    local rate = self.progressLogin / TOTAL_LOGIN_STEP * 0.1 + self.progressInit / TOTAL_INIT_STEP * 0.9;
    local percentage = math.floor(rate * 100);
    return percentage;
end

function helper:resetLoadingProgressPercentage()
    helper.progressLogin = 0;
    helper.progressInit = 0;
end

return helper;



