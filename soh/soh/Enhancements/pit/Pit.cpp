#include "Pit.h"
#include "PitFloorData.h"
#include "soh/OTRGlobals.h"
#include "../custom-message/CustomMessageTypes.h"
#include "../item-tables/ItemTableManager.h"
#include "../randomizer/randomizerTypes.h"

extern "C" {
#include <z64.h>
#include "macros.h"
#include "functions.h"
#include "variables.h"
#include "functions.h"
extern SaveContext gSaveContext;
extern PlayState* gPlayState;
extern s32 Object_Spawn(ObjectContext* objectCtx, s16 objectId);
}

#include <array>
#include <string>
#include <vector>

std::vector<Actor*> currentFloorEnemies;

//debug room
#define PIT_ENTRANCE 0x520

//dinner room
//#define PIT_ENTRANCE 0x1C

void Pit_SetEntrace() {
    gSaveContext.respawnFlag = 0;
    if (gPlayState != NULL) {
        gPlayState->nextEntranceIndex = PIT_ENTRANCE;
    }
    gSaveContext.entranceIndex = PIT_ENTRANCE;
    gSaveContext.cutsceneIndex = 0x8000;
    currentFloorEnemies.clear();
}

#undef PIT_ENTRANCE

void Pit_InitSave() {
    // Set player name to Lonk for the few textboxes that show up. Player can't input their own name.
    std::array<char, 8> brPlayerName = { 21, 50, 49, 46, 62, 62, 62, 62 };
    for (int i = 0; i < ARRAY_COUNT(gSaveContext.playerName); i++) {
        gSaveContext.playerName[i] = brPlayerName[i];
    }

    Pit_SetEntrace();

    // Skip boss cutscenes
    gSaveContext.eventChkInf[7] |= 1;    // gohma
    gSaveContext.eventChkInf[7] |= 2;    // dodongo
    gSaveContext.eventChkInf[7] |= 4;    // phantom ganon
    gSaveContext.eventChkInf[7] |= 8;    // volvagia
    gSaveContext.eventChkInf[7] |= 0x10; // morpha
    gSaveContext.eventChkInf[7] |= 0x20; // twinrova
    gSaveContext.eventChkInf[7] |= 0x40; // barinade
    gSaveContext.eventChkInf[7] |= 0x80; // bongo bongo

    // Sets all rando flags to false
    for (s32 i = 0; i < ARRAY_COUNT(gSaveContext.randomizerInf); i++) {
        gSaveContext.randomizerInf[i] = 0;
    }

    /*
    // Items
    std::array<u8, 24> brItems = {
        ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE,
        ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE,
        ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE,
        ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE,
    };

    for (int item = 0; item < ARRAY_COUNT(gSaveContext.inventory.items); item++) {
        gSaveContext.inventory.items[item] = brItems[item];
    }

    // Ammo
    std::array<s8, 16> brAmmo = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    for (int ammo = 0; ammo < ARRAY_COUNT(gSaveContext.inventory.ammo); ammo++) {
        gSaveContext.inventory.ammo[ammo] = brAmmo[ammo];
    }
     */

    // Upgrades
    Inventory_ChangeUpgrade(UPG_WALLET, 3);
    Inventory_ChangeUpgrade(UPG_STICKS, 1);
    Inventory_ChangeUpgrade(UPG_NUTS, 1);

    gSaveContext.linkAge = LINK_AGE_CHILD;
    Pit_SetEquipment(LINK_AGE_CHILD);
}

void Pit_SpawnBlueWarp() {
    if (floorData[gSaveContext.currentPitFloor].type == FloorType::REST) {
        Actor_Spawn(&gPlayState->actorCtx, gPlayState, ACTOR_DOOR_WARP1, 0, 0, 250, 0, 0, 0, -1, false);
    } else {
        Actor_Spawn(&gPlayState->actorCtx, gPlayState, ACTOR_DOOR_WARP1, 0, 0, 0, 0, 0, 0, -1, false);
    }
}

void Pit_Update() {
    gSaveContext.naviTimer = 0;
    gSaveContext.dayTime = 32768;
}

void Pit_HandleBlueWarp(PlayState* play) {
    Pit_GoToNextFloor();
}

void Pit_GoToNextFloor() {
    gSaveContext.currentPitFloor += 1;
    Pit_SetEntrace();
}

void Pit_SetEquipment(uint8_t linkAge) {
    if (linkAge == LINK_AGE_CHILD) {
        gSaveContext.equips.buttonItems[0] = ITEM_SWORD_KOKIRI;
        gSaveContext.inventory.equipment |= 1 << 0; // Kokiri Sword
        Inventory_ChangeEquipment(EQUIP_SWORD, PLAYER_SWORD_KOKIRI);
    } else {
        gSaveContext.equips.buttonItems[0] = ITEM_SWORD_MASTER;
        gSaveContext.inventory.equipment |= 1 << 1; // Master Sword
        Inventory_ChangeEquipment(EQUIP_SWORD, PLAYER_SWORD_MASTER);
    }
}

void Pit_SpawnEnemiesForCurrentFloor() {
    if (
        floorData[gSaveContext.currentPitFloor].type != FloorType::ENEMY ||
        gSaveContext.currentPitFloor < 0 ||
        gSaveContext.currentPitFloor >= floorData.size()
    ) {
        return;
    }
    std::vector<EnemyData> enemiesToSpawn = floorData[gSaveContext.currentPitFloor].enemies;
    for (auto enemyData : enemiesToSpawn) {
        Actor* spawnedActor = Actor_Spawn(
            &gPlayState->actorCtx,
            gPlayState,
            enemyData.actorId,
            enemyData.pos.x, enemyData.pos.y, enemyData.pos.z,
            enemyData.rot.x, enemyData.rot.y, enemyData.rot.z,
            enemyData.params,
            false
        );
        if (spawnedActor != NULL) {
            currentFloorEnemies.push_back(spawnedActor);
        }
    }
}

//this will return the number of rest floors before your current floor
//it will return 0 on the first rest floor, 1 on the second one and so on
u8 Pit_GetRestFloorNumber() {
    u8 restFloorNumber = 0;
    for (u16 i = 0; i < (floorData.size() < gSaveContext.currentPitFloor ? floorData.size() : gSaveContext.currentPitFloor); i += 1) {
        if (floorData[i].type == FloorType::REST) {
            restFloorNumber += 1;
        }
    }
    return restFloorNumber;
}

#pragma region Shop

typedef struct {
    GetItemEntry giEntry;
    s16 price;
} PitShopItem;

std::vector<PitShopItem> shopItems;

void Pit_Shop_CreateShopItems() {
    if (!shopItems.empty()) {
        osSyncPrintf("Error: Pit_Shop_CreateShopItems was called when shopItems is not empty, this should never happen");
        return;
    }
    shopItems = {
        {
            ItemTableManager::Instance->RetrieveItemEntry(MOD_NONE, GI_STICKS_1),
            1
        },
        {
            ItemTableManager::Instance->RetrieveItemEntry(MOD_NONE, GI_NUTS_5),
            10
        },
        {
            ItemTableManager::Instance->RetrieveItemEntry(MOD_NONE, GI_SHIELD_HYLIAN),
            10
        },
        {
            ItemTableManager::Instance->RetrieveItemEntry(MOD_NONE, GI_NAYRUS_LOVE),
            50
        },
        {
            ItemTableManager::Instance->RetrieveItemEntry(MOD_NONE, GI_BOOMERANG),
            25
        },
        {
            ItemTableManager::Instance->RetrieveItemEntry(MOD_NONE, GI_MASK_BUNNY),
            15
        },
        {
            ItemTableManager::Instance->RetrieveItemEntry(MOD_NONE, GI_HEART_CONTAINER),
            5
        },
        {
            ItemTableManager::Instance->RetrieveItemEntry(MOD_RANDOMIZER, RG_BOTTLE_WITH_RED_POTION),
            15
        }
    };
}

PitShopItem Pit_GetShopItemData(EnGirlA* shopItem) {
    if (shopItems.empty()) {
        osSyncPrintf("Error: Pit_GetShopItemData was called with an empty shopItems");
        return {
            ItemTableManager::Instance->RetrieveItemEntry(MOD_NONE, ITEM_SOLD_OUT),
            0
        };
    }
    if (shopItem->randoSlotIndex < 0 || shopItem->randoSlotIndex >= shopItems.size()) {
        osSyncPrintf("Error: Pit_GetShopItemData was called with a shopItem->randoSlotIndex outside of the bounds of shopItems");
        return shopItems[0];
    }
    return shopItems[shopItem->randoSlotIndex];
}

void Pit_Shop_ItemDraw(EnGirlA* shopItem) {
    GetItemEntry giEntry = Pit_GetShopItemData(shopItem).giEntry;

    EnItem00_CustomItemsParticles(&shopItem->actor, gPlayState, giEntry);
    GetItemEntry_Draw(gPlayState, giEntry);
}

void Pit_Shop_InitializeItemAction(EnGirlA* shopItem) {
    PitShopItem pitShopItem = Pit_GetShopItemData(shopItem);
    shopItem->actor.textId = TEXT_SHOP_ITEM_PIT + shopItem->randoSlotIndex;
    shopItem->itemBuyPromptTextId = TEXT_SHOP_ITEM_PIT_CONFIRM + shopItem->randoSlotIndex;
    shopItem->getItemId = pitShopItem.giEntry.getItemId;
    shopItem->basePrice = pitShopItem.price;
    shopItem->giDrawId = pitShopItem.giEntry.gid;
}

void Pit_Shop_InitItem(EnGirlA* shopItem) {
    s16 objectId = Pit_GetShopItemData(shopItem).giEntry.objectId;
    
    shopItem->objBankIndex = Object_GetIndex(&gPlayState->objectCtx, objectId);

    // If the object isn't already spawned, spawn it now
    if (shopItem->objBankIndex < 0) {
        shopItem->objBankIndex = Object_Spawn(&gPlayState->objectCtx, objectId);
    }
}

//currently this will always return 0 as the shop items will always be SI_PIT_ITEM
//in the future maybe this could be changed to do what rando does and use this
//when changing to SI_SOLD_OUT
s32 Pit_Shop_ChangeItem(EnGirlA* shopItem) {
    return 0;
}

s32 Pit_Shop_CanBuy(PlayState* play, EnGirlA* shopItem) {
    return CANBUY_RESULT_CANT_GET_NOW;
    //CANBUY_RESULT_SUCCESS
}

// This is called when Pit_Shop_CanBuy returns CANBUY_RESULT_SUCCESS
// The giving of the item is handled here, and no fanfare is played
void Pit_Shop_ItemGive(PlayState* play, EnGirlA* shopItem) {
    //PitShopItem pitShopItem = Pit_GetShopItemData(shopItem);
    //give pitShopItem
}

// This is called when Pit_Shop_CanBuy returns CANBUY_RESULT_SUCCESS_FANFARE
// The giving of the item is handled in ossan.c, and a fanfare is played
void Pit_Shop_BuyEvent(PlayState* play, EnGirlA* shopItem) {
    osSyncPrintf("Error: Pit_Shop_BuyEvent was called, this should never happen");
}

typedef struct {
    s16 xOffset;
    s16 yOffset;
    s16 zOffset;
    s16 yRot;
} ShopItemOffset;

//data taken from z_en_ossan.c
ShopItemOffset shopItemOffsets[8] = {
    {  50, 52, -20, 0xEAAC },
    {  50, 76, -20, 0xEAAC },
    {  80, 52,  -3, 0xEAAC },
    {  80, 76,  -3, 0xEAAC },
    { -50, 52, -20, 0x1554 },
    { -50, 76, -20, 0x1554 },
    { -80, 52,  -3, 0x1554 },
    { -80, 76,  -3, 0x1554 }
};

CustomMessage Pit_Shop_GetItemMessage(u16 textId) {
    return CustomMessage("test", "test", "test");
}

CustomMessage Pit_Shop_GetItemConfirmMessage(u16 textId) {
    return CustomMessage("test", "test", "test");
}

#pragma endregion

GetItemEntry Pit_GetItemInChest() {
    //in case of failure, return greg
    GetItemEntry giEntry = ItemTableManager::Instance->RetrieveItemEntry(MOD_RANDOMIZER, RG_GREG_RUPEE);

    switch (Pit_GetRestFloorNumber()) {
        case 0:
            giEntry = ItemTableManager::Instance->RetrieveItemEntry(MOD_NONE, GI_SHIELD_DEKU);
            break;

        case 1:
            giEntry = ItemTableManager::Instance->RetrieveItemEntry(MOD_NONE, GI_SLINGSHOT);
            break;
    }

    return giEntry;
}

GetItemEntry Pit_OpenChest(EnBox* chest) {
    GetItemEntry giEntry = Pit_GetItemInChest();
    giEntry.getItemId = 0 - giEntry.getItemId;
    giEntry.getItemFrom = ITEM_FROM_CHEST;
    GiveItemEntryFromActorWithFixedRange(&chest->dyna.actor, gPlayState, giEntry);
    return giEntry;
}

void Pit_SetUpChest(EnBox* chest) {
    chest->getItemEntry = Pit_GetItemInChest();
}

void Pit_InitRestFloor() {
    osSyncPrintf("Initializing a rest floor");

    //unset the flag that the chest uses
    Flags_UnsetTreasure(gPlayState, 0);

    //chest
    Actor_Spawn(
        &gPlayState->actorCtx,
        gPlayState,
        ACTOR_EN_BOX,
        200, 0, 0,
        0, 16384, 0,
        0,
        false
    );

    shopItems.clear();
    Pit_Shop_CreateShopItems();

    Object_Spawn(&gPlayState->objectCtx, OBJECT_OSSAN);
    EnTana* shelves = (EnTana*)Actor_Spawn(
        &gPlayState->actorCtx,
        gPlayState,
        ACTOR_EN_TANA,
        0, 0, -260,
        0, 0, 0,
        0,
        false
    );
    EnOssan* shopkeeper = (EnOssan*)Actor_Spawn(
        &gPlayState->actorCtx,
        gPlayState,
        ACTOR_EN_OSSAN,
        0, 0, -300,
        0, 0, 0,
        OSSAN_TYPE_BAZAAR,
        false
    );

    for (s32 i = 0; i < 8; i += 1) {
        shopkeeper->shelfSlots[i] = (EnGirlA*)Actor_Spawn(
            &gPlayState->actorCtx,
            gPlayState,
            ACTOR_EN_GIRLA,
            shelves->actor.world.pos.x + shopItemOffsets[i].xOffset,
            shelves->actor.world.pos.y + shopItemOffsets[i].yOffset,
            shelves->actor.world.pos.z + shopItemOffsets[i].zOffset,
            shelves->actor.shape.rot.x,
            shelves->actor.shape.rot.y + shopItemOffsets[i].yRot,
            shelves->actor.shape.rot.z,
            SI_PIT_ITEM,
            false
        );

        shopkeeper->shelfSlots[i]->randoSlotIndex = i;
    }

    Pit_SpawnBlueWarp();

    //fill health and magic
    gSaveContext.health = gSaveContext.healthCapacity;
    gSaveContext.magic = gSaveContext.magicCapacity;

    osSyncPrintf("Finished initializing a rest floor");
}

void Pit_OnEnemyDefeat(Actor* actor) {
    if (std::find(currentFloorEnemies.begin(), currentFloorEnemies.end(), actor) != currentFloorEnemies.end()) {
        currentFloorEnemies.erase(std::remove(currentFloorEnemies.begin(), currentFloorEnemies.end(), actor), currentFloorEnemies.end());
    }

    if (Actor_Find(&gPlayState->actorCtx, ACTOR_DOOR_WARP1, ACTORCAT_ITEMACTION) == NULL && currentFloorEnemies.empty()) {
        Pit_SpawnBlueWarp();
    }
}

//in case we get out with debug warp menu
void Pit_OnActorKill(Actor* actor) {
    Pit_OnEnemyDefeat(actor);
}

void Pit_InitCurrentFloor() {
    if (gSaveContext.currentPitFloor < 0) {
        osSyncPrintf("Error: Pit_InitCurrentFloor was called with a gSaveContext.currentPitFloor lower than 0");
        gSaveContext.currentPitFloor = 0;
    }

    if (gSaveContext.currentPitFloor >= floorData.size()) {
        osSyncPrintf("Error: Pit_InitCurrentFloor was called with a gSaveContext.currentPitFloor higher than the number of floors");
        gSaveContext.currentPitFloor = floorData.size() - 1;
    }

    switch (floorData[gSaveContext.currentPitFloor].type) {
        case FloorType::ENEMY:
            Pit_SpawnEnemiesForCurrentFloor();
            break;

        case FloorType::REST:
            Pit_InitRestFloor();
            break;

        default:
            break;
    }
}