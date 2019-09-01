
local function WARNING(msg)
    if(msg == nil) then
        msg = "unknown reason";
    end

    release_print("===============================================");
    release_print("ERROR, integration test fail: " .. msg);
    release_print("===============================================");
end

local function INFO(msg)
    release_print("===============================================");
    release_print("integration test info: " .. msg);
    release_print("===============================================");
end


local framework = {}

function framework:init(path)
    local content = cc.FileUtils:getInstance():getStringFromFile(path);
    content = _REQUIRE("SanGuoLib").convertJsonContent(content, path);
    local doc = json.decode(content);
    --    print_r(doc);

    self.map = doc;

    release_print("init auto test framework, path = " .. path);

end


function framework:start()
    INFO("auto test start, total step = " .. tostring(#(self.map)));

    self.step = 1;
    self.numSuccess = 0;
    self.numFail = 0;

    self.runNextStep();
end

function framework:runNextStep()
    if(self.step < #(self.map)) then
        self:doStep();
    else
        local msg = "auto test complete\n";
        msg = msg .. "total step = " .. tostring(#(self.map)) .. "\n";
        msg = msg .. "total success step = " .. tostring(self.numSuccess) .. "\n";
        msg = msg .. "total fail step = " .. tostring(self.numFail) .. "\n";
        INFO(msg);
    end    
end

function framework:doStep()
    local msg = "#" .. tostring(self.step) .. " start\n";
    msg = msg .. "total step = " .. tostring(#(self.map)) .. "\n";
    msg = msg .. "total success step = " .. tostring(self.numSuccess) .. "\n";
    msg = msg .. "total fail step = " .. tostring(self.numFail) .. "\n";
    INFO(msg);
    
    local function onStepComplete(success, data, code, msg)
        if(success) then
            self.numSuccess = self.numSuccess + 1;
        else
            self.numFail = self.numFail + 1;
            WARNING(msg);
        end
        
        self.step = self.step + 1;
        self:runNextStep();
    end

    local item = self.map[self.step];
    local controllerName = item[1];
    local controller = _G[controllerName];
    
    if(controller == nil) then
        WARNING("unknown controller = " .. item[1]);
        return;
    end

    controller(onStepComplete, item[2], item[3], item[4], item[5], item[6], item[7], item[8], item[9]);    
end

return framework;
