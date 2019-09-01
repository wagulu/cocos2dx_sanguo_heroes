local model_user = _REQUIRE("model/model_user")


module("pbCommon", package.seeall)


StageType = {}
StageType.NORMAL = 1
StageType.MASTER = 2


AreaStatus = {}
AreaStatus.DISABLE = 0
AreaStatus.LOCK = 1
AreaStatus.DICLEAR = 2
AreaStatus.UNCOMPLETE = 3
AreaStatus.COMPLETE = 4



MonsterPosition = {}
MonsterPosition.FRONT1 = "f1"
MonsterPosition.BEHIND1 = "b1"


ObjectType = {}
ObjectType.coin = "Coin";
ObjectType.cash = "Cash";
ObjectType.prestige = "HighladderPrestige";
ObjectType.Monster = "Monster"
ObjectType.MonsterPart = "MonsterPart"
ObjectType.FavorabilityItem = "FavorabilityItem"
ObjectType.Material = "Material"
ObjectType.TreasureItem = "TreasureItem"
ObjectType.TreasureItemPart = "TreasureItemPart"
ObjectType.TreasureBox = "TreasureBox"
ObjectType.Item = "Item"
ObjectType.Item_CoinItem = "CoinItem"
ObjectType.Item_CashItem = "CashItem"
ObjectType.Item_ExpItem = "ExpItem"
ObjectType.Item_EnergyItem = "EnergyItem"
ObjectType.Item_VitalityItem = "VitalityItem"
ObjectType.Item_MoppingTicketItem = "MoppingTicketItem"
ObjectType.Item_TreasureItemExpItem = "TreasureItemExpItem"
ObjectType.Item_TreasureItemQualityItem = "TreasureItemQualityItem"
ObjectType.Item_NormalGachaTicket = "NormalGachaTicket"
ObjectType.BreakthroughItem = "BreakthroughItem"


FightResult = {}
FightResult.LOSE = 0
FightResult.WIN = 1
FightResult.DRAW = 2

BattleMode = {}
BattleMode.NORMAL = 1
BattleMode.DUEL = 2

function isTypeItem(type)
    return(type == ObjectType.Item
        or type == ObjectType.Item_CoinItem
        or type == ObjectType.Item_CashItem
        or type == ObjectType.Item_ExpItem
        or type == ObjectType.Item_EnergyItem
        or type == ObjectType.Item_VitalityItem
        or type == ObjectType.Item_MoppingTicketItem
        or type == ObjectType.Item_TreasureItemExpItem
        or type == ObjectType.Item_TreasureItemQualityItem
        or type == ObjectType.BreakthroughItem
        );
end

ColorType={"1","2","2_1","3","3_1","3_2","4","4_1","4_2","4_3"}
Colors = {cc.c3b(255, 255, 255),cc.c3b(0, 255, 0),cc.c3b(0, 186, 255),cc.c3b(204, 0, 255)}--白绿蓝紫


TREASURE_ITEM_QUALITY_UP_REQUIRE_MATERIAL_ID = 14;
NORMALGACHATICKET_ID = 20

TREASURE_ITEM_QUALITY_UP_REQUIRE_MATERIAL_TYPE = ObjectType.Item_TreasureItemQualityItem;


ShopListType = {}
ShopListType.ITEM = 1;

PriceType = {}
PriceType.COIN = 1;
PriceType.CASH = 2;


UseableItemType = {}
UseableItemType.EXP_ITEM = ObjectType.ExpItem;
UseableItemType.COIN_ITEM = ObjectType.CoinItem;
UseableItemType.CASH_ITEM = ObjectType.CashItem;


dropWayType = {
    chanpter = {name = "" , sceneName = "battle/ChapterScene"},
    shop = {name = "商城" ,sceneName = "arena/StoreScene"}
}

--interface


function struct_apiRequest()
    local api = {
        --            version = 1,
        --            timestamp = 2,
        sign = model_user.userKey, --"a156eb6f261f4cce82466acadcc53c76",
--        sign = "a156eb6f261f4cce82466acadcc53c76",
        --            isTest = 4,
        --            lang = 5,
        --            deviceName = 6,
        --            clientId = 7,
        transactionId = 15,
        lang = "zh_CN",
    }
    return api;
end

