
•
#AccountGetUidFromSNUidRequest.protoApiRequest.proto"l
AccountGetUidFromSNUidRequest
api (2.ApiRequest
loginKey (	
snUid (	
serverId (	
î
$AccountGetUidFromSNUidResponse.protoApiResponse.proto"Y
AccountGetUidFromSNUidResponse
api (2.ApiResponse
uid (	
userKey (	
Ç
AccountLoginRequest.protoApiRequest.proto"S
AccountLoginRequest
api (2.ApiRequest
userName (	
passWord (	
q
AccountLoginResponse.protoApiResponse.proto"@
AccountLoginResponse
api (2.ApiResponse
snUid (	
˚
 AccountNetworkLoginRequest.protoApiRequest.proto"ƒ
AccountNetworkLoginRequest
api (2.ApiRequest
userKey (	
token (	
tokenSecret (	
snUid (	

snUserName (	
	timestamp (	
password (	
account	 (	
Ω
!AccountNetworkLoginResponse.protoApiResponse.proto"Ñ
AccountNetworkLoginResponse
api (2.ApiResponse
snUid (	

snUserName (	
accessToken (	

loginToken (	
ó
AccountRegisterRequest.protoApiRequest.proto"e
AccountRegisterRequest
api (2.ApiRequest
userName (	
passWord (	
email (	
w
AccountRegisterResponse.protoApiResponse.proto"C
AccountRegisterResponse
api (2.ApiResponse
snUid (	
z
AdventureListAreaRequest.protoApiRequest.proto"F
AdventureListAreaRequest
api (2.ApiRequest
islandId (
¨
AdventureListAreaResponse.protoApiResponse.proto AdventureListStageResponse.proto"˙
AreasInfoPB
islandId (
areaId (
name (	
stars (
status (
stageTotalStar (
eliteStageTotalStar (0
stageStarTreasures (2.StarTreasuresListPB5
eliteStageStarTreasures	 (2.StarTreasuresListPB"W
AdventureListAreaResponse
api (2.ApiResponse
	areasInfo (2.AreasInfoPB
l
 AdventureListIslandRequest.protoApiRequest.proto"6
AdventureListIslandRequest
api (2.ApiRequest
÷
!AdventureListIslandResponse.protoApiResponse.proto"?
IslandsInfoPB
islandId (
name (	
status ("]
AdventureListIslandResponse
api (2.ApiResponse#
islandsInfo (2.IslandsInfoPB
ü
AdventureListStageRequest.protoApiRequest.proto"j
AdventureListStageRequest
api (2.ApiRequest
islandId (
areaId (
	stageType (
ã
 AdventureListStageResponse.protoApiResponse.protoStarInfoPB.protoPrizePB.proto"Ø
StagesInfoPB

id (
name (	
description (	
icon (
isMaster (
status (
starInfo (2.StarInfoPB
count (

battleMode	 ("`
StarTreasuresListPB
boxId (
star (
isOpen (

rewardList (2.PrizePB"û
AdventureListStageResponse
api (2.ApiResponse!

stagesInfo (2.StagesInfoPB/
starTreasuresList (2.StarTreasuresListPB
	totalStar (
∆
&AdventureOpenStarTreasureRequest.protoApiRequest.proto"â
 AdventureOpenStarTreasureRequest
api (2.ApiRequest
islandId (
areaId (
	stageType (
starTreasureId (
≤
'AdventureOpenStarTreasureResponse.protoApiResponse.protoPrizePB.proto"e
!AdventureOpenStarTreasureResponse
api (2.ApiResponse%
starTreasureBoxInfo (2.PrizePB
∂
"AdventureStageConfirmRequest.protoApiRequest.proto"~
AdventureStageConfirmRequest
api (2.ApiRequest
islandId (
areaId (
stageId (
	stageType (
Ù
#AdventureStageConfirmResponse.protoApiResponse.protoStarInfoPB.protoPrizePB.protoMonsterPB.protoBattleTeamPB.protoDramaPB.proto"=
	NPCInfoPB
	monsterId (
isBoss (
NPCId ("∑
UserStagePB
starInfo (2.StarInfoPB
exp (
coin (
energyBefore (
energyAfter (
battleTimes (
moppingTicket (
dailyResetTimes ("Î
AdventureStageConfirmResponse
api (2.ApiResponse
	treasures (2.PrizePB
NPC (2
.MonsterPB
NPCInfo (2
.NPCInfoPB
	userStage (2.UserStagePB!

battleTeam (2.BattleTeamPB
dramas (2.DramaPB
Æ
ApiRequest.proto"ô

ApiRequest
version (	
	timestamp (	
sign (	
isTest (
lang (	

deviceName (	
clientId (
transactionId (
À
ApiResponse.protoEventPB.proto"¶
ApiResponse
version (	
	timestamp (	
errorNo (
errorMessage (	
sign (	
isTest (
event (2.EventPB
transactionId (
≠
BattleBattleResultRequest.protoApiRequest.protoStarInfoPB.proto"Â
BattleBattleResultRequest
api (2.ApiRequest
islandId (
areaId (
stageId (
	stageType (	

battleStar (2.StarInfoPB
	wonBattle (
progress (

monsterIds	 (
NPCIds
 (
ø
 BattleBattleResultResponse.protoApiResponse.protoMonsterPB.protoPrizePB.protoStarInfoPB.proto"$
UnlockPB
type (	

id ("Ø
BattleBattleResultResponse
api (2.ApiResponse
monsters (2
.MonsterPB
	treasures (2.PrizePB
unlocks (2	.UnlockPB
refresh (2.RefreshListPB"Ä
RefreshListPB
island (2
.RefreshPB
area (2
.RefreshPB
stage (2
.RefreshPB

eliteStage (2
.RefreshPB"{
	RefreshPB

id (
status (
stageTotalStar (
eliteStageTotalStar (
starInfo (2.StarInfoPB
ï
BattleBuyTimesRequest.protoApiRequest.proto"d
BattleBuyTimesRequest
api (2.ApiRequest
islandId (
areaId (
stageId (
x
BattleBuyTimesResponse.protoApiResponse.proto"E
BattleBuyTimesResponse
api (2.ApiResponse
newTimes (
â
BattleExecBattleRequest.protoApiRequest.proto"’
BattleExecBattleRequest
api (2.ApiRequest
islandId (
areaId (
stageId (
	stageType (	
battleId (
progress (

stratagems (

monsterIds	 (
NPCIds
 (
¬
BattleExecBattleResponse.protoApiResponse.protoMonsterPB.proto"|
BattleExecBattleResponse
api (2.ApiResponse 
monstersInfo (2
.MonsterPB#
NPCMonstersInfo (2
.MonsterPB
∂
BattleMoppingRequest.protoApiRequest.proto"Ö
BattleMoppingRequest
api (2.ApiRequest
islandId (
areaId (
stageId (
	stageType (
times (
÷
BattleMoppingResponse.protoApiResponse.protoPrizePB.proto"ô
BattleMoppingResponse
api (2.ApiResponse3
moppingTreasureList (2.MoppingTreasureListPB
exp (
coin (
moppingTicket ("F
MoppingTreasureListPB-
moppingTreasures (2.MoppingTreasuresPB"1
MoppingTreasuresPB
	treasures (2.PrizePB
™
"BattleSetBattleConfigRequest.protoApiRequest.proto"r
BattleSetBattleConfigRequest
api (2.ApiRequest
	monsterId (
formationId (
position (	
¢
#BattleSetBattleConfigResponse.protoApiResponse.protoMonsterPB.proto"W
BattleSetBattleConfigResponse
api (2.ApiResponse
monster (2
.MonsterPB
S
BattleTeamPB.proto"=
BattleTeamPB
pos (
	monsterId (
isUse (
Å
DepositGetListRequest.protoApiRequest.proto"P
DepositGetListRequest
api (2.ApiRequest
type (	
channel (	
ı
DepositGetListResponse.protoApiResponse.protoPrizePB.proto"Q
DepositGetListResponse
api (2.ApiResponse
deposits (2
.DepositPB"û
	DepositPB
key (	
link (	
depositedAt (	 
reason (2.DepositReasonPB
	prizeInfo (2.PrizePB
status (

actionType ("?
DepositReasonPB
title (	
content (	
from (	
}
 DepositReceiveBatchRequest.protoApiRequest.proto"G
DepositReceiveBatchRequest
api (2.ApiRequest
channel (	
ô
!DepositReceiveBatchResponse.protoApiResponse.protoPrizePB.proto"R
DepositReceiveBatchResponse
api (2.ApiResponse
prizes (2.PrizePB
Ä
DepositReceiveRequest.protoApiRequest.proto"O
DepositReceiveRequest
api (2.ApiRequest
key (	
channel (	
è
DepositReceiveResponse.protoApiResponse.protoPrizePB.proto"M
DepositReceiveResponse
api (2.ApiResponse
prizes (2.PrizePB
d
DramaPB.proto"S
DramaPB
asset (	
name (	
side (
content (	
flag (
Œ
EventPB.proto"ó
UserPB
level (
exp (
VipLevel (
energy (

energy_max (
vitality (
vitality_max (
coin (
cash	 (
prestige
 (
gacha_token (
time_to_energy_max (
time_to_next_energy (
time_to_vitality_max (
time_to_next_vitality (
exp_for_current_level (&
deposit_count (2.DepositCountPB
skill_up_point (#
time_to_next_skill_up_point (
energy_buy_times (
vitality_buy_times ( 
skill_up_point_buy_times ("-
DepositCountPB
type (	
count ("1
EventInfoPB
eventId (
	eventInfo (	"A
EventPB
user (2.UserPB
	eventInfo (2.EventInfoPB
j
FormationPB.proto"U
FormationPB

id (
level (
skillIds (
learnCompleteTime (
§
GachaInfoPB.proto"é
GachaInfoPB

id (
	gachaType (	
	freeTimes (
times (
lastFreeTime (	
resetFreeTime (	
	resetTime (	
s
GachaRollGachaRequest.protoApiRequest.proto"B
GachaRollGachaRequest
api (2.ApiRequest
gachaId (
È
GachaRollGachaResponse.protoApiResponse.protoPrizePB.protoItemPB.protoGachaInfoPB.proto"Ö
GachaRollGachaResponse
api (2.ApiResponse
prizes (2.PrizePB
item (2.ItemPB
	gachaInfo (2.GachaInfoPB
L
GiftPackagePB.proto"5
GiftPackagePB
giftPackageId (
count (
—
#HighladderBattleResultRequest.protoApiRequest.proto"ó
HighladderBattleResultRequest
api (2.ApiRequest
rank (
progress (
	monsterId (
enemyMonsterId (
	wonBattle (
¢
$HighladderBattleResultResponse.protoApiResponse.protoPrizePB.proto"◊
HighladderBattleResultResponse
api (2.ApiResponse
fightPrizes (2.PrizePB 
highRankPrizes (2.PrizePB
prize (2.PrizePB
displayPrizes (2.PrizePB
rank (
	rankDelta (
u
HighladderBuyItemRequest.protoApiRequest.proto"A
HighladderBuyItemRequest
api (2.ApiRequest
pos (
Õ
HighladderBuyItemResponse.protoApiResponse.protoPrizePB.protoHighladderShopPB.proto"p
HighladderBuyItemResponse
api (2.ApiResponse
prize (2.PrizePB
shop (2.HighladderShopPB
b
HighladderDrawRequest.protoApiRequest.proto"1
HighladderDrawRequest
api (2.ApiRequest
Ü
HighladderDrawResponse.protoApiResponse.proto"S
HighladderDrawResponse
api (2.ApiResponse
cash (
prestige (
∫
!HighladderExecBattleRequest.protoApiRequest.proto"Ç
HighladderExecBattleRequest
api (2.ApiRequest
rank (
progress (
	monsterId (
enemyMonsterId (
À
"HighladderExecBattleResponse.protoApiResponse.protoMonsterPB.proto"Ä
HighladderExecBattleResponse
api (2.ApiResponse
monsterInfo (2
.MonsterPB$
EnemyMonsterInfo (2
.MonsterPB
ò
HighladderFightRequest.protoApiRequest.proto"f
HighladderFightRequest
api (2.ApiRequest
rank (

monsterIds (
isWinner (
Û
HighladderFightResponse.protoApiResponse.protoPrizePB.proto"Ø
HighladderFightResponse
api (2.ApiResponse
fightPrizes (2.PrizePB 
highRankPrizes (2.PrizePB
prize (2.PrizePB
displayPrizes (2.PrizePB
x
HighladderGetEnemyRequest.protoApiRequest.proto"C
HighladderGetEnemyRequest
api (2.ApiRequest
rank (
‘
 HighladderGetEnemyResponse.protoApiResponse.protoMonsterPB.protoBattleTeamPB.proto"x
HighladderGetEnemyResponse
api (2.ApiResponse
monsters (2
.MonsterPB!

battleTeam (2.BattleTeamPB
h
HighladderGetListRequest.protoApiRequest.proto"4
HighladderGetListRequest
api (2.ApiRequest
Ê
HighladderGetListResponse.protoApiResponse.protoMonsterPB.proto"Å
HighladderGetListResponse
api (2.ApiResponse$
ranks (2.HighladderRankItemPB#
self (2.HighladderSelfItemPB"è
HighladderRankItemPB
rank (
name (	
monsters (2
.MonsterPB
showMonster (2
.MonsterPB
power (
level ("à
HighladderSelfItemPB
rank (
prestige (
rankCash (
rankPrestige (
	cash2Draw (
prestige2Draw (
ä
(HighladderGetOthersMonstersRequest.protoApiRequest.proto"L
"HighladderGetOthersMonstersRequest
api (2.ApiRequest
rank (
Ø
)HighladderGetOthersMonstersResponse.protoApiResponse.protoMonsterPB.proto"^
#HighladderGetOthersMonstersResponse
api (2.ApiResponse
monsters (2
.MonsterPB
n
!HighladderGetRankingRequest.protoApiRequest.proto"7
HighladderGetRankingRequest
api (2.ApiRequest
˙
"HighladderGetRankingResponse.protoApiResponse.protoMonsterPB.proto"ç
HighladderGetRankingResponse
api (2.ApiResponse&
self (2.HighladderRankingItemPB*
rankings (2.HighladderRankingItemPB"ü
HighladderRankingItemPB
rank (
name (	
monsters (2
.MonsterPB
showMonster (2
.MonsterPB
power (
level (
uid (
z
'HighladderGetRankingRewardRequest.protoApiRequest.proto"=
!HighladderGetRankingRewardRequest
api (2.ApiRequest
ã
(HighladderGetRankingRewardResponse.protoApiResponse.protoPrizePB.proto"l
"HighladderGetRankingRewardResponse
api (2.ApiResponse+
rewards (2.HighladderRankingRewardPB"O
HighladderRankingRewardPB
from (

to (
prizes (2.PrizePB
h
HighladderGetShopRequest.protoApiRequest.proto"4
HighladderGetShopRequest
api (2.ApiRequest
•
HighladderGetShopResponse.protoApiResponse.protoHighladderShopPB.proto"W
HighladderGetShopResponse
api (2.ApiResponse
shop (2.HighladderShopPB
ê
"HighladderQuickBattleRequest.protoApiRequest.proto"X
HighladderQuickBattleRequest
api (2.ApiRequest
rank (
progress (
∂
#HighladderQuickBattleResponse.protoApiResponse.protoPrizePB.proto"Ï
HighladderQuickBattleResponse
api (2.ApiResponse
fightPrizes (2.PrizePB 
highRankPrizes (2.PrizePB
prize (2.PrizePB
displayPrizes (2.PrizePB
rank (
	rankDelta (
battleResult (
p
"HighladderRefreshShopRequest.protoApiRequest.proto"8
HighladderRefreshShopRequest
api (2.ApiRequest
≠
#HighladderRefreshShopResponse.protoApiResponse.protoHighladderShopPB.proto"[
HighladderRefreshShopResponse
api (2.ApiResponse
shop (2.HighladderShopPB
ß
HighladderShopPB.protoPrizePB.proto"å
HighladderShopPB
prestige ($
items (2.HighladderShopItemPB
refreshTimes (
refreshPrestige (
	resetTime ("o
HighladderShopItemPB
pos (
prize (2.PrizePB
prestige (
discount (
buyed (
b
InitBattleTeamRequest.protoApiRequest.proto"1
InitBattleTeamRequest
api (2.ApiRequest
ù
InitBattleTeamResponse.protoApiResponse.protoBattleTeamPB.proto"V
InitBattleTeamResponse
api (2.ApiResponse!

battleTeam (2.BattleTeamPB
b
InitFormationsRequest.protoApiRequest.proto"1
InitFormationsRequest
api (2.ApiRequest
¡
InitFormationsResponse.protoApiResponse.protoFormationPB.proto"{
InitFormationsResponse
api (2.ApiResponse
	formation (2.FormationPB%
unlockFormation (2.FormationPB
X
InitGachaRequest.protoApiRequest.proto",
InitGachaRequest
api (2.ApiRequest
ê
InitGachaResponse.protoApiResponse.protoGachaInfoPB.proto"O
InitGachaResponse
api (2.ApiResponse
	gachaInfo (2.GachaInfoPB
d
InitGiftPackageRequest.protoApiRequest.proto"2
InitGiftPackageRequest
api (2.ApiRequest
£
InitGiftPackageResponse.protoApiResponse.protoGiftPackagePB.proto"Z
InitGiftPackageResponse
api (2.ApiResponse$
giftPackages (2.GiftPackagePB
X
InitItemsRequest.protoApiRequest.proto",
InitItemsRequest
api (2.ApiRequest
Ç
InitItemsResponse.protoApiResponse.protoItemPB.proto"F
InitItemsResponse
api (2.ApiResponse
items (2.ItemPB
^
InitMaterialRequest.protoApiRequest.proto"/
InitMaterialRequest
api (2.ApiRequest
î
InitMaterialResponse.protoApiResponse.protoMaterialPB.proto"Q
InitMaterialResponse
api (2.ApiResponse
	materials (2.MaterialPB
p
"InitMaterialSynthesisRequest.protoApiRequest.proto"8
InitMaterialSynthesisRequest
api (2.ApiRequest
∞
#InitMaterialSynthesisResponse.protoApiResponse.proto"c
InitMaterialSynthesisResponse
api (2.ApiResponse'
materialSynthesis (2.SynthesisPB"I
SynthesisPB

materialId (&
material (2.MaterialSynthesisPB"F
MaterialSynthesisPB
rawMaterialId (
rawMaterialCount (
f
InitMonsterPartsRequest.protoApiRequest.proto"3
InitMonsterPartsRequest
api (2.ApiRequest
•
InitMonsterPartsResponse.protoApiResponse.protoMonsterPartPB.proto"[
InitMonsterPartsResponse
api (2.ApiResponse$
monsterParts (2.MonsterPartPB
^
InitMonstersRequest.protoApiRequest.proto"/
InitMonstersRequest
api (2.ApiRequest
∂
InitMonstersResponse.protoApiResponse.protoMonsterPB.proto"t
InitMonstersResponse
api (2.ApiResponse
monsters (2
.MonsterPB#
notHaveMonsters (2
.MonsterPB
h
InitTreasureBoxesRequest.protoApiRequest.proto"4
InitTreasureBoxesRequest
api (2.ApiRequest
¨
InitTreasureBoxesResponse.protoApiResponse.protoTreasureBoxesPB.proto"_
InitTreasureBoxesResponse
api (2.ApiResponse'
treasureBoxes (2.TreasureBoxesPB
p
"InitTreasureItemPartsRequest.protoApiRequest.proto"8
InitTreasureItemPartsRequest
api (2.ApiRequest
æ
#InitTreasureItemPartsResponse.protoApiResponse.protoTreasureItemPartPB.proto"j
InitTreasureItemPartsResponse
api (2.ApiResponse.
treasureItemParts (2.TreasureItemPartPB
h
InitTreasureItemsRequest.protoApiRequest.proto"4
InitTreasureItemsRequest
api (2.ApiRequest
™
InitTreasureItemsResponse.protoApiResponse.protoTreasureItemPB.proto"^
InitTreasureItemsResponse
api (2.ApiResponse&
treasureItems (2.TreasureItemPB
V
InitUserRequest.protoApiRequest.proto"+
InitUserRequest
api (2.ApiRequest
“
InitUserResponse.protoApiResponse.protoAdventureListAreaResponse.proto!AdventureListIslandResponse.proto AdventureListStageResponse.proto"æ
InitUserResponse
api (2.ApiResponse#
userIslands (2.IslandsInfoPB
	userAreas (2.AreasInfoPB!

userStages (2.StagesInfoPB&
userEliteStages (2.StagesInfoPB
E
ItemPB.proto"5
ItemPB
itemId (
type (	
count (
;
MaterialPB.proto"'

MaterialPB

id (
count (
p
MonsterInfoRequest.protoApiRequest.proto"B
MonstersInfoRequest
api (2.ApiRequest
	monsterId (
è
MonsterInfoResponse.protoApiResponse.protoMonsterPB.proto"N
MonstersInfoResponse
api (2.ApiResponse
monster (2
.MonsterPB
˝
MonsterPB.proto"Å
MonsterTreasureItemsPB
slotId (
inventoryTreasureItemId (
treasureItemId (
level (
quality ("Ä
MonsterRelationshipP2PPB

id (
flag (

activation (
relationshipLevel (
relationshipLevelId ("I
MonsterFormationsPB
slotId (
formationId (
isUse ("ó
EquipmentPB

id (
level (
quality (
nextLevelCoin (
nextQualityCoin (,
nextQualityMaterial (2.MaterialTreePB"C
MaterialTreePB
needMaterialId (
needMaterialCount ("Ê
SkillsPB

id (
level (
maxLevel (
mark (	
type (
MPCost (

energyCost (
energyStart (
useLimit	 (
sourceAttribute
 (
soldier (2
.SoldierPB
isUnlock ("È
	SoldierPB

id (
num (

arrowSpeed (

orderSpeed (

HP (

MP (
attack (
power (
speed	 (
strikeCD
 (

atkHitRate (
abilityHitRate (
atkDodgeRate (
abilityDodgeRate (

DR (

physicalDR (
spellDR (
atkCSR (

abilityCSR (
ArPR (
SRPR (
healingBonus (
atkVamp (
abilityVamp (
atkVampRate (
abilityVampRate (
physicalResilience (
spellResilience (

holyDamage (

atkAntiCSR (
abilityAntiCSR (
physicalCSDR  (
	spellCSDR! (
mark" (	
type# (

damageType$ (
asset% (	
name& (	
description' (	
icon( (	"„	
	MonsterPB

id (
level (
exp (
quality (
star (

maxSoldier (

arrowSpeed (

HP (

MP	 (
position
 (	
attack (
power (
speed (
strikeCD (

atkHitRate (
abilityHitRate (
atkDodgeRate (
abilityDodgeRate (

DR (

physicalDR (
spellDR (
atkCSR (

abilityCSR (
ArPR (
SRPR (
	abilityCD (
healingBonus (
atkVamp (
atkVampRate (
abilityVamp (
abilityVampRate (
physicalResilience  (
spellResilience! (
ERHPBase" (

ERKillBase# (

ERLoseBase$ (
spellCostReduction% (

holyDamage& (

atkAntiCSR' (
abilityAntiCSR( (
physicalCSDR) (
	spellCSDR* ((

formations+ (2.MonsterFormationsPB
skills, (2	.SkillsPB
soldier- (2
.SoldierPB
	equipment. (2.EquipmentPB
mark/ (	

damageType0 (.
treasureItems1 (2.MonsterTreasureItemsPB2
relationshipP2P2 (2.MonsterRelationshipP2PPB
asset3 (	
name4 (	
description5 (	

AI6 (	
breakthroughLevel7 (
destinyLevel8 (

destinyExp9 (
NPCId: (

CE; (
destinyLevelNext< (
_
MonsterPartPB.proto"H
MonsterPartPB
monsterPartId (
	monsterId (
count (
Å
!MonstersBreakthroughRequest.protoApiRequest.proto"J
MonstersBreakthroughRequest
api (2.ApiRequest
	monsterId (
∆
"MonstersBreakthroughResponse.protoApiResponse.protoMonsterPB.protoItemPB.proto"n
MonstersBreakthroughResponse
api (2.ApiResponse
monster (2
.MonsterPB
items (2.ItemPB
¨
$MonstersChangeFormationRequest.protoApiRequest.proto"r
MonstersChangeFormationRequest
api (2.ApiRequest
	monsterId (
slotId (
formationId (
¶
%MonstersChangeFormationResponse.protoApiResponse.protoMonsterPB.proto"Y
MonstersChangeFormationResponse
api (2.ApiResponse
monster (2
.MonsterPB
ø
'MonstersChangeTreasureItemRequest.protoApiRequest.proto"Å
!MonstersChangeTreasureItemRequest
api (2.ApiRequest
	monsterId (
slotId (
inventoryTreasureItemId (
Î
(MonstersChangeTreasureItemResponse.protoApiResponse.protoMonsterPB.protoTreasureItemPB.proto"Ñ
"MonstersChangeTreasureItemResponse
api (2.ApiResponse
monster (2
.MonsterPB&
treasureItems (2.TreasureItemPB
í
#MonstersDestinyLevelUpRequest.protoApiRequest.proto"Y
MonstersDestinyLevelUpRequest
api (2.ApiRequest
	monsterId (
num (
…
$MonstersDestinyLevelUpResponse.protoApiResponse.protoMonsterPB.protoItemPB.proto"o
MonstersDestinyLevelUpResponse
api (2.ApiResponse
monster (2
.MonsterPB
item (2.ItemPB
≠
%MonstersEquipmentLevelUpRequest.protoApiRequest.proto"r
MonstersEquipmentLevelUpRequest
api (2.ApiRequest
	monsterId (
equipmentId (
times (
≥
&MonstersEquipmentLevelUpResponse.protoApiResponse.protoMonsterPB.proto"û
 MonstersEquipmentLevelUpResponse
api (2.ApiResponse
level (
coin (%
turns (2.EquipmentLevelUpTurns
monster (2
.MonsterPB"D
EquipmentLevelUpTurns
level (
isCrit (
coin (
ñ
+MonstersEquipmentMaterialMergeRequest.protoApiRequest.proto"U
%MonstersEquipmentMaterialMergeRequest
api (2.ApiRequest

materialId (
∏
,MonstersEquipmentMaterialMergeResponse.protoApiResponse.protoMaterialPB.proto"c
&MonstersEquipmentMaterialMergeResponse
api (2.ApiResponse
	materials (2.MaterialPB
¢
'MonstersEquipmentQualityUpRequest.protoApiRequest.proto"e
!MonstersEquipmentQualityUpRequest
api (2.ApiRequest
	monsterId (
equipmentId (
Ó
(MonstersEquipmentQualityUpResponse.protoApiResponse.protoMonsterPB.protoMaterialPB.proto"ã
"MonstersEquipmentQualityUpResponse
api (2.ApiResponse
level (
monster (2
.MonsterPB
	materials (2.MaterialPB
^
MonstersListRequest.protoApiRequest.proto"/
MonstersListRequest
api (2.ApiRequest
å
MonstersListResponse.protoApiResponse.protoMonsterPB.proto"4
MonsterCanCompose
	monsterId (
name (	"\
MonsterCannotCompose
	monsterId (
name (	
partNum (

composeNum ("µ
MonstersListResponse
api (2.ApiResponse
monsters (2
.MonsterPB.
canComposeMonsters (2.MonsterCanCompose4
cannotComposeMonsters (2.MonsterCannotCompose
s
MonstersMergeRequest.protoApiRequest.proto"C
MonstersMergeRequest
api (2.ApiRequest
	monsterId (
Ã
MonstersMergeResponse.protoApiResponse.protoMonsterPB.protoMonsterPartPB.proto"t
MonstersMergeResponse
api (2.ApiResponse
monster (2
.MonsterPB#
monsterPart (2.MonsterPartPB
{
MonstersQualityUpRequest.protoApiRequest.proto"G
MonstersQualityUpRequest
api (2.ApiRequest
	monsterId (
´
MonstersQualityUpResponse.protoMonsterPB.protoApiResponse.proto"d
MonstersQualityUpResponse
api (2.ApiResponse
quality (
monster (2
.MonsterPB
à
MonstersSkillUpRequest.protoApiRequest.proto"V
MonstersSkillUpRequest
api (2.ApiRequest
	monsterId (
skillId (
ñ
MonstersSkillUpResponse.protoApiResponse.protoMonsterPB.proto"Q
MonstersSkillUpResponse
api (2.ApiResponse
monster (2
.MonsterPB
u
MonstersStarUpRequest.protoApiRequest.proto"D
MonstersStarUpRequest
api (2.ApiRequest
	monsterId (
º
MonstersStarUpResponse.protoApiResponse.protoMonsterPB.proto"x
MonstersStarUpResponse
api (2.ApiResponse
star (
monsterPartCount (
monster (2
.MonsterPB
º
(MonstersTreasureItemLevelUpRequest.protoApiRequest.proto"~
"MonstersTreasureItemLevelUpRequest
api (2.ApiRequest
inventoryTreasureItemId (
itemId (
count (
‚
)MonstersTreasureItemLevelUpResponse.protoApiResponse.protoTreasureItemPB.protoItemPB.proto"~
#MonstersTreasureItemLevelUpResponse
api (2.ApiResponse%
treasureItem (2.TreasureItemPB
item (2.ItemPB
î
&MonstersTreasureItemMergeRequest.protoApiRequest.proto"X
 MonstersTreasureItemMergeRequest
api (2.ApiRequest
treasureItemPartId (
É
'MonstersTreasureItemMergeResponse.protoApiResponse.protoTreasureItemPB.protoTreasureItemPartPB.proto"î
!MonstersTreasureItemMergeResponse
api (2.ApiResponse%
treasureItem (2.TreasureItemPB-
treasureItemPart (2.TreasureItemPartPB
°
*MonstersTreasureItemQualityUpRequest.protoApiRequest.proto"a
$MonstersTreasureItemQualityUpRequest
api (2.ApiRequest
inventoryTreasureItemId (
è
+MonstersTreasureItemQualityUpResponse.protoApiResponse.protoTreasureItemPB.protoItemPB.proto"®
%MonstersTreasureItemQualityUpResponse
api (2.ApiResponse%
treasureItem (2.TreasureItemPB&
removeTreasureItemInventoryIds (
item (2.ItemPB
Ö
#MonstersUseDestinyItemRequest.protoApiRequest.proto"L
MonstersUseDestinyItemRequest
api (2.ApiRequest
	monsterId (
„
$MonstersUseDestinyItemResponse.protoApiResponse.protoMonsterPB.protoItemPB.proto"à
MonstersUseDestinyItemResponse
api (2.ApiResponse
monster (2
.MonsterPB
item (2.ItemPB
directlyLevelUp (
ú
MonstersUseExpItemRequest.protoApiRequest.proto"g
MonstersUseExpItemRequest
api (2.ApiRequest
	monsterId (
itemId (
count (
¡
 MonstersUseExpItemResponse.protoApiResponse.protoMonsterPB.protoItemPB.proto"k
MonstersUseExpItemResponse
api (2.ApiResponse
monster (2
.MonsterPB
item (2.ItemPB
d
PlayerBuyEnergyRequest.protoApiRequest.proto"2
PlayerBuyEnergyRequest
api (2.ApiRequest
{
PlayerBuyEnergyResponse.protoApiResponse.proto"G
PlayerBuyEnergyResponse
api (2.ApiResponse
	newEnergy (
p
"PlayerBuySkillUpPointRequest.protoApiRequest.proto"8
PlayerBuySkillUpPointRequest
api (2.ApiRequest
ç
#PlayerBuySkillUpPointResponse.protoApiResponse.proto"S
PlayerBuySkillUpPointResponse
api (2.ApiResponse
newSkillUpPoint (
h
PlayerBuyVitalityRequest.protoApiRequest.proto"4
PlayerBuyVitalityRequest
api (2.ApiRequest
Å
PlayerBuyVitalityResponse.protoApiResponse.proto"K
PlayerBuyVitalityResponse
api (2.ApiResponse
newVitality (
©
#PlayerChangeBattleTeamRequest.protoApiRequest.protoBattleTeamPB.proto"\
PlayerChangeBattleTeamRequest
api (2.ApiRequest!

battleTeam (2.BattleTeamPB
≠
$PlayerChangeBattleTeamResponse.protoApiResponse.protoBattleTeamPB.proto"^
PlayerChangeBattleTeamResponse
api (2.ApiResponse!

battleTeam (2.BattleTeamPB
ö
'PlayerGetOtherUsersMonsterRequest.protoApiRequest.proto"]
!PlayerGetOtherUsersMonsterRequest
api (2.ApiRequest
uid (
	monsterId (
¨
(PlayerGetOtherUsersMonsterResponse.protoApiResponse.protoMonsterPB.proto"\
"PlayerGetOtherUsersMonsterResponse
api (2.ApiResponse
monster (2
.MonsterPB
â
(PlayerGetOtherUsersMonstersRequest.protoApiRequest.proto"K
"PlayerGetOtherUsersMonstersRequest
api (2.ApiRequest
uid (
Ø
)PlayerGetOtherUsersMonstersResponse.protoApiResponse.protoMonsterPB.proto"^
#PlayerGetOtherUsersMonstersResponse
api (2.ApiResponse
monsters (2
.MonsterPB
É
!PlayerLearnFormationRequest.protoApiRequest.proto"L
PlayerLearnFormationRequest
api (2.ApiRequest
formationId (
¶
"PlayerLearnFormationResponse.protoApiResponse.protoFormationPB.proto"Z
PlayerLearnFormationResponse
api (2.ApiResponse
	formation (2.FormationPB
î
&PlayerModifyBattleMonsterRequest.protoApiRequest.proto"X
 PlayerModifyBattleMonsterRequest
api (2.ApiRequest
battleTeamMonsters (
≥
'PlayerModifyBattleMonsterResponse.protoApiResponse.protoBattleTeamPB.proto"a
!PlayerModifyBattleMonsterResponse
api (2.ApiResponse!

battleTeam (2.BattleTeamPB
ô
,PlayerSetLearnFormationCompleteRequest.protoApiRequest.proto"W
&PlayerSetLearnFormationCompleteRequest
api (2.ApiRequest
formationId (
º
-PlayerSetLearnFormationCompleteResponse.protoApiResponse.protoFormationPB.proto"e
'PlayerSetLearnFormationCompleteResponse
api (2.ApiResponse
	formation (2.FormationPB
î
!PlayerUseGiftPackageRequest.protoApiRequest.proto"]
PlayerUseGiftPackageRequest
api (2.ApiRequest
giftPackageId (
count (
’
"PlayerUseGiftPackageResponse.protoApiResponse.protoPrizePB.protoGiftPackagePB.proto"x
PlayerUseGiftPackageResponse
api (2.ApiResponse
prizes (2.PrizePB#
giftPackage (2.GiftPackagePB

PlayerUseItemRequest.protoApiRequest.proto"O
PlayerUseItemRequest
api (2.ApiRequest
itemId (
count (
â
PlayerUseItemResponse.protoApiResponse.protoItemPB.proto"I
PlayerUseItemResponse
api (2.ApiResponse
item (2.ItemPB
å
!PlayerUseTreasureBoxRequest.protoApiRequest.proto"U
PlayerUseTreasureBoxRequest
api (2.ApiRequest
boxId (
count (
€
"PlayerUseTreasureBoxResponse.protoApiResponse.protoPrizePB.protoTreasureBoxesPB.proto"|
PlayerUseTreasureBoxResponse
api (2.ApiResponse
prizes (2.PrizePB'
treasureBoxes (2.TreasureBoxesPB
ﬂ
PrizePB.protoMonsterPB.protoItemPB.protoMonsterPartPB.protoTreasureItemPartPB.protoTreasureItemPB.protoFormationPB.protoMaterialPB.protoGiftPackagePB.proto"Ö
PrizePB

id (
type (	
value (
content (
name (	
des (	
quality (
monster (2
.MonsterPB
toParts	 (2
.ToPartsPB
dropItem_Item
 (2.ItemPB$
dropItem_Monster (2
.MonsterPB,
dropItem_MonsterPart (2.MonsterPartPB6
dropItem_TreasureItemPart (2.TreasureItemPartPB.
dropItem_TreasureItem (2.TreasureItemPB(
dropItem_Formation (2.FormationPB&
dropItem_Material (2.MaterialPB,
dropItem_GiftPackage (2.GiftPackagePB"(
	ToPartsPB
partId (
num (
è
ShopBuyItemRequest.protoApiRequest.proto"a
ShopBuyItemRequest
api (2.ApiRequest
shopType (
prizeId (
buyNum (
∂
ShopBuyItemResponse.protoApiResponse.protoShopItemPB.protoPrizePB.proto"e
ShopBuyItemResponse
api (2.ApiResponse
goods (2.ShopItemPB
prize (2.PrizePB
Ó
ShopItemPB.protoPrizePB.proto" 

ShopItemPB

goodsPrize (2.PrizePB
price (
	priceType (	
mark (
originalPrice (
vipLevel (

buyLimited (
quota (
	startTime	 (
endTime
 (
h
ShopListRequest.protoApiRequest.proto"=
ShopListRequest
api (2.ApiRequest
shopType (
å
ShopListResponse.protoApiResponse.protoShopItemPB.proto"M
ShopListResponse
api (2.ApiResponse
	goodsList (2.ShopItemPB
≤
StarInfoPB.proto"2
StarConditionPB
flag (
	condition (	"j

StarInfoPB
flag (
conditionDisplay (	
status ($

conditions (2.StarConditionPB
a
TreasureBoxesPB.proto"H
TreasureBoxesPB
treasureBoxId (
groupId (
count (
∏
TreasureItemPB.proto"ü
TreasureItemPB
inventoryTreasureItemId (
treasureItemId (
level (
exp (
quality (
quippedType (

useMonster (
s
TreasureItemPartPB.proto"W
TreasureItemPartPB
treasureItemPartId (
treasureItemId (
count (