
local model_user = _REQUIRE("model/model_user")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_monsterPartStorage = _REQUIRE("model/model_monsterPartStorage.lua")
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
local model_treasureItemStorage = _REQUIRE("model/model_treasureItemStorage.lua")
_REQUIRE("pb/pbCommon.lua")

local helper = {};

function helper:getDrop(drop)
    if(drop.type == pbCommon.ObjectType.coin) then
        return;
    end

    if(drop.type == pbCommon.ObjectType.cash) then
        return;
    end

    if(drop.type == pbCommon.ObjectType.prestige) then
        return;
    end

    if(drop.type == pbCommon.ObjectType.Item) then
        model_itemStorage:setNode(drop.dropItem_Item);
        return;
    end

    if(drop.type == pbCommon.ObjectType.TreasureItem) then
        model_treasureItemStorage:updateItem(drop.dropItem_TreasureItem);-- itemStorage:setNode(drop.dropItem_Item);
        return;
    end

    if(drop.type == pbCommon.ObjectType.TreasureItemPart) then
        model_treasureItemStorage:updateItem(drop.dropItem_TreasureItemPart);-- itemStorage:setNode(drop.dropItem_Item);
        return;
    end

    if(drop.type == pbCommon.ObjectType.Monster) then
        if(model_monsterStorage:haveMonster(drop.value)) then
            if(model_monsterStorage:isMonsterActive(drop.value)) then
                model_monsterPartStorage:sumCount(drop.toParts.partId, drop.toParts.num);
            else
                model_monsterStorage:activeMonster(drop.value);
            end
        else
            model_monsterStorage:setNode(drop.dropItem_Monster, 1);
        end
        return;
    end

    if(drop.type == pbCommon.ObjectType.MonsterPart) then
        model_monsterPartStorage:setNode(drop.dropItem_MonsterPart);-- :sumCount(drop.value, drop.content)
        return;
    end

    if(drop.type == pbCommon.ObjectType.TreasureBox) then
        return;
    end

    if(drop.type == pbCommon.ObjectType.Material) then
        model_materialStorage:setNode(drop.dropItem_Material);-- :sumCount(drop.value, drop.content);
        return;
    end

    release_print("ERROR, unsupport drop type = " .. drop.type);

end


return helper;
