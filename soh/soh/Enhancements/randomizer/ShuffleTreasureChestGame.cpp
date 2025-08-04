#include <soh/OTRGlobals.h>
#include "soh_assets.h"
#include "static_data.h"
#include <libultraship/libultra.h>
#include "global.h"
#include "soh/ResourceManagerHelpers.h"
#include "soh/ObjectExtension/ObjectExtension.h"

extern "C" {
#include "src/overlays/actors/ovl_Door_Shutter/z_door_shutter.h"
#include "src/overlays/actors/ovl_En_Changer/z_en_changer.h"
extern PlayState* gPlayState;
}

void DoorShutter_RandomizerInit(void* actorRef) {
    if (gPlayState->sceneNum != SCENE_TREASURE_BOX_SHOP) {
        return;
    }

    DoorShutter* shutterDoor = static_cast<DoorShutter*>(actorRef);

    // turn the temp switches into permanent switches
    shutterDoor->dyna.actor.params &= ~0x20;

    // do the unlocked check again
    if (Flags_GetSwitch(gPlayState, shutterDoor->dyna.actor.params & 0x1F)) {
        shutterDoor->unk_16E = 0;
    }
}

void ItemEtcetera_RandomizerInit(void* actorRef) {
    if (gPlayState->sceneNum != SCENE_TREASURE_BOX_SHOP) {
        return;
    }

    Actor* actor = static_cast<Actor*>(actorRef);

    // remove the items visible with lens inside the chests
    Actor_Kill(actor);
}

extern "C" void EnChanger_RandomizerWait(EnChanger* thisx, PlayState* play) {}

void EnChanger_RandomizerInit(void* actorRef) {
    if (gPlayState->sceneNum != SCENE_TREASURE_BOX_SHOP) {
        return;
    }

    EnChanger* enChanger = static_cast<EnChanger*>(actorRef);

    // prevent opening one chest from also opening the other & the vanilla "item hover" animation
    enChanger->actionFunc = EnChanger_RandomizerWait;
}

void EnBox_RandomizerInit(void* actorRef) {
    if (gPlayState->sceneNum != SCENE_TREASURE_BOX_SHOP || gPlayState->roomCtx.curRoom.num == 6) {
        return;
    }

    Actor* chest = static_cast<Actor*>(actorRef);

    // make the chests not react to lens as the items visible with lens inside have been removed
    chest->flags &= ~ACTOR_FLAG_REACT_TO_LENS;
}

void RegisterShuffleTreasureChestGame() {
    bool shouldRegister = IS_RANDO && RAND_GET_OPTION(RSK_SHUFFLE_CHEST_MINIGAME);

    // prevents the chest and keys from being reset in the chest minigame upon entry
    COND_VB_SHOULD(VB_TAKARA_MAN_RESET_CHESTS_AND_KEYS, shouldRegister, {
        *should = false;
    });

    // replace the vanilla shopkeeper item with the randomized item
    COND_VB_SHOULD(VB_TAKARA_MAN_OFFER_GET_ITEM, shouldRegister, {
        Flags_SetRandomizerInf(RAND_INF_TREASURE_CHEST_GAME_SHOPKEEPER_ITEM);
        *should = false;
    });

    // make the chests never swap so they're consistent
    COND_VB_SHOULD(VB_EN_CHANGER_SWAP_CHESTS, shouldRegister, {
        *should = false;
    });

    COND_VB_SHOULD(VB_CHEST_SET_TREASURE_FLAG, shouldRegister, {
        if (gPlayState->sceneNum != SCENE_TREASURE_BOX_SHOP || gPlayState->roomCtx.curRoom.num == 6) {
            return;
        }

        Actor* chest = va_arg(args, Actor*);

        bool rightChest = chest->world.pos.x > 0;

        u32 randInf = RAND_INF_MARKET_TREASURE_CHEST_GAME_LEFT_1 + gPlayState->roomCtx.curRoom.num - 1 + (rightChest ? 5 : 0);

        LUSLOG_WARN("gPlayState->roomCtx.curRoom.num = %d", gPlayState->roomCtx.curRoom.num);
        LUSLOG_WARN("rightChest = %d", rightChest);
        LUSLOG_WARN("randInf = %d", randInf);

        Flags_SetRandomizerInf((RandomizerInf)randInf);

        *should = false;
    });

    COND_VB_SHOULD(VB_CHEST_CONSIDER_CHEST_OPEN, shouldRegister, {
        if (gPlayState->sceneNum != SCENE_TREASURE_BOX_SHOP || gPlayState->roomCtx.curRoom.num == 6) {
            return;
        }

        Actor* chest = va_arg(args, Actor*);

        bool rightChest = chest->world.pos.x > 0;

        RandomizerInf randInf = (RandomizerInf)(RAND_INF_MARKET_TREASURE_CHEST_GAME_LEFT_1 + gPlayState->roomCtx.curRoom.num - 1 + (rightChest ? 5 : 0));

        LUSLOG_WARN("gPlayState->roomCtx.curRoom.num = %d", gPlayState->roomCtx.curRoom.num);
        LUSLOG_WARN("rightChest = %d", rightChest);
        LUSLOG_WARN("randInf = %d", randInf);

        *should = Flags_GetRandomizerInf(randInf);
    });

    COND_ID_HOOK(OnActorInit, ACTOR_DOOR_SHUTTER, shouldRegister, DoorShutter_RandomizerInit);

    COND_ID_HOOK(OnActorInit, ACTOR_ITEM_ETCETERA, shouldRegister, ItemEtcetera_RandomizerInit);

    COND_ID_HOOK(OnActorInit, ACTOR_EN_CHANGER, shouldRegister, EnChanger_RandomizerInit);

    COND_ID_HOOK(OnActorInit, ACTOR_EN_BOX, shouldRegister, EnBox_RandomizerInit);
}

#define TREASURE_CHEST_GAME_CHECK(rc, params, name, rht, rg, rand_inf)                                              \
    locationTable[rc] = Location::Chest(rc, RCQUEST_BOTH, RCTYPE_CHEST_GAME, ACTOR_EN_BOX, SCENE_TREASURE_BOX_SHOP, \
                                        params, name, rht, rg, SpoilerCollectionCheck::RandomizerInf(rand_inf), false)

void Rando::StaticData::RegisterTreasureChestGameLocations() {
    static bool registered = false;
    if (registered)
        return;
    registered = true;
    // clang-format off
    locationTable[RC_MARKET_TREASURE_CHEST_GAME_SHOPKEEPER_ITEM] = Location::Base(RC_MARKET_TREASURE_CHEST_GAME_SHOPKEEPER_ITEM, RCQUEST_BOTH, RCTYPE_CHEST_GAME, ACTOR_EN_TAKARA_MAN, SCENE_TREASURE_BOX_SHOP, 0, "Treasure Chest Game Shopkeeper Item", RHT_NONE, RG_TREASURE_GAME_SMALL_KEY, SpoilerCollectionCheck::RandomizerInf(RAND_INF_TREASURE_CHEST_GAME_SHOPKEEPER_ITEM));

    TREASURE_CHEST_GAME_CHECK(RC_MARKET_TREASURE_CHEST_GAME_LEFT_1,  20023, "Treasure Chest Game First Room Left Chest",   RHT_MARKET_TREASURE_CHEST_GAME_LEFT_1,  RG_GREEN_RUPEE,             RAND_INF_MARKET_TREASURE_CHEST_GAME_LEFT_1);
    TREASURE_CHEST_GAME_CHECK(RC_MARKET_TREASURE_CHEST_GAME_LEFT_2,  20034, "Treasure Chest Game Second Room Left Chest",  RHT_MARKET_TREASURE_CHEST_GAME_LEFT_2,  RG_GREEN_RUPEE,             RAND_INF_MARKET_TREASURE_CHEST_GAME_LEFT_2);
    TREASURE_CHEST_GAME_CHECK(RC_MARKET_TREASURE_CHEST_GAME_LEFT_3,  20068, "Treasure Chest Game Third Room Left Chest",   RHT_MARKET_TREASURE_CHEST_GAME_LEFT_3,  RG_BLUE_RUPEE,              RAND_INF_MARKET_TREASURE_CHEST_GAME_LEFT_3);
    TREASURE_CHEST_GAME_CHECK(RC_MARKET_TREASURE_CHEST_GAME_LEFT_4,  20070, "Treasure Chest Game Fourth Room Left Chest",  RHT_MARKET_TREASURE_CHEST_GAME_LEFT_4,  RG_BLUE_RUPEE,              RAND_INF_MARKET_TREASURE_CHEST_GAME_LEFT_4);
    TREASURE_CHEST_GAME_CHECK(RC_MARKET_TREASURE_CHEST_GAME_LEFT_5,  20104, "Treasure Chest Game Fifth Room Left Chest",   RHT_MARKET_TREASURE_CHEST_GAME_LEFT_5,  RG_RED_RUPEE,               RAND_INF_MARKET_TREASURE_CHEST_GAME_LEFT_5);
    TREASURE_CHEST_GAME_CHECK(RC_MARKET_TREASURE_CHEST_GAME_RIGHT_1, 20001, "Treasure Chest Game First Room Right Chest",  RHT_MARKET_TREASURE_CHEST_GAME_RIGHT_1, RG_TREASURE_GAME_SMALL_KEY, RAND_INF_MARKET_TREASURE_CHEST_GAME_RIGHT_1);
    TREASURE_CHEST_GAME_CHECK(RC_MARKET_TREASURE_CHEST_GAME_RIGHT_2, 20003, "Treasure Chest Game Second Room Right Chest", RHT_MARKET_TREASURE_CHEST_GAME_RIGHT_2, RG_TREASURE_GAME_SMALL_KEY, RAND_INF_MARKET_TREASURE_CHEST_GAME_RIGHT_2);
    TREASURE_CHEST_GAME_CHECK(RC_MARKET_TREASURE_CHEST_GAME_RIGHT_3, 20005, "Treasure Chest Game Third Room Right Chest",  RHT_MARKET_TREASURE_CHEST_GAME_RIGHT_3, RG_TREASURE_GAME_SMALL_KEY, RAND_INF_MARKET_TREASURE_CHEST_GAME_RIGHT_3);
    TREASURE_CHEST_GAME_CHECK(RC_MARKET_TREASURE_CHEST_GAME_RIGHT_4, 20007, "Treasure Chest Game Fourth Room Right Chest", RHT_MARKET_TREASURE_CHEST_GAME_RIGHT_4, RG_TREASURE_GAME_SMALL_KEY, RAND_INF_MARKET_TREASURE_CHEST_GAME_RIGHT_4);
    TREASURE_CHEST_GAME_CHECK(RC_MARKET_TREASURE_CHEST_GAME_RIGHT_5, 20009, "Treasure Chest Game Fifth Room Right Chest",  RHT_MARKET_TREASURE_CHEST_GAME_RIGHT_5, RG_TREASURE_GAME_SMALL_KEY, RAND_INF_MARKET_TREASURE_CHEST_GAME_RIGHT_5);
    // clang-format-on
}

static RegisterShipInitFunc initFunc(RegisterShuffleTreasureChestGame, { "IS_RANDO" });
static RegisterShipInitFunc initTreasureChestGameLocations(Rando::StaticData::RegisterTreasureChestGameLocations);
