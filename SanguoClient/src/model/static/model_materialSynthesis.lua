
_REQUIRE("cocos/cocos2d/json.lua")


local model = {};
model.map = {};

function model:setNode(node)
    if(self.map[node.materialId] == nil) then
        self.map[node.materialId] = {};
    end
    
    local group = self.map[node.materialId];
    group[#(group) + 1] = node;
end

function model:getSynthesis(id)
    return self.map[id];
end

function model:getRequirementMaterialId(synthesisNode)
    return synthesisNode.id;
end

function model:getRequirementMaterialCount(synthesisNode)
    return synthesisNode.count;
end

function model:initFromPb(pb)
    for key, value in pairs(pb.materialSynthesis) do
        local group = {};
        self.map[value.materialId] = group;
        for rawKey, rawValue in pairs(value.material) do
            group.id = rawValue.rawMaterialId;
            group.count = rawValue.rawMaterialCount;
        end
    end
end


local function init()
    local path = "config/static/MaterialSynthesis.Json";
    local content = cc.FileUtils:getInstance():getStringFromFile(path);
    content = _REQUIRE("SanGuoLib").convertJsonContent(content, path);
    local doc = json.decode(content);
--    print_r(doc);

    for key, value in pairs(doc) do
        model:setNode(value);
    end

    release_print("init model success");

end

--init();


return model;
