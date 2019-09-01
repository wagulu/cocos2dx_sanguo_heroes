
local USE_CJSON = IS_RELEASE_VERSION and false;--or false

local cjson;
if(USE_CJSON) then
    cjson = require "cjson"
end



_REQUIRE("cocos/cocos2d/json.lua")

local model = class("model_static", {})

local totalTimeConsume = 0;
local totalConvertTimeConsume = 0;
local totalDecodeTimeConsume = 0;


function model:setNode(node)
    self.map[node.id] = node;
end

function model:getNode(id)
    return self.map[id];
end

function model:load(path)
    self.map = {};

    local t1 = os.clock();
    local content = cc.FileUtils:getInstance():getStringFromFile(path);
    local t2 = os.clock();
    if not(USE_CJSON) then
        content = _REQUIRE("SanGuoLib").convertJsonContent(content, path);
    end
    local t3 = os.clock();
    local doc;
    if not(USE_CJSON) then
        doc = json.decode(content);
    else
        doc = cjson.decode(content);
    end
    local t4 = os.clock();
    --    print_r(doc);

    totalTimeConsume = totalTimeConsume + (t4 - t1);
    totalConvertTimeConsume = totalConvertTimeConsume + (t4 - t2);
    totalDecodeTimeConsume = totalDecodeTimeConsume + (t4 - t3);
    release_print("time consume" ..
        ", total = " .. tostring(math.floor(totalTimeConsume * 1000) / 1000) ..
        ", convert = " .. tostring(math.floor(totalConvertTimeConsume * 1000) / 1000) ..
        ", decode = " .. tostring(math.floor(totalDecodeTimeConsume * 1000) / 1000) ..
        ""
        );

    for key, value in pairs(doc) do
        self:setNode(value);
    end

    release_print("init model success, path = " .. path);

end

return model;
