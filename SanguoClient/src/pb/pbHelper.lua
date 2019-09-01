
_REQUIRE("common.print_r")

-------------------
-- chance pbHelper

local base = _G

-- -----------------------------------------------------------------------------
-- Module declaration
-----------------------------------------------------------------------------
module("pbHelper")

local pb = base._REQUIRE("protobuf")

local protoBufPath = "ProtoBuf/pb/"
local protoBufType = ".pb"

function getEncodePB(pbName, pbData)

    local resultBuffer = pb.encode(pbName, pbData)
    
--    for test
--    local result = pb.decode(pbName, resultBuffer)
-- --    cclog("===== result:" + result)

    return resultBuffer
end

function getDecodePB(pbName, stringPb)

--    local outputURI = decodeURI(stringPb)

    local resultTable = pb.decode(pbName, stringPb)
    if(base.type(resultTable) == "table") then
        pb.expandWholeTable(resultTable);
        return resultTable
    else
        return nil;
    end
--    local test1 = resultTable["NPC"][1]["ArPR"];
--    local test2 = resultTable["NPC"][1]["ArPR"];
end

-- Only one pb file : all.pb now
local function initPb()

    local buffer = base.cc.FileUtils:getInstance():getStringFromFile(protoBufPath .. "all" .. protoBufType)
    pb.register(buffer, protoBufPath)
end

function printPB(pb)
    if(pb ~= nil) then
        base.release_print("====    print PB");
        base.print_r(pb);
        base.release_print("====    print PB end");
    end
end

initPb()
