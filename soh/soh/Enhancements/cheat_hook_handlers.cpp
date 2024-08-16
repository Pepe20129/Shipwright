#include <libultraship/bridge.h>
#include "soh/OTRGlobals.h"
#include "soh/Enhancements/game-interactor/GameInteractor.h"
#include "soh/Enhancements/game-interactor/GameInteractor_Hooks.h"
#include "soh/Enhancements/enhancementTypes.h"

extern "C" {
#include "macros.h"
#include "variables.h"
s32 Flags_GetRandomizerInf(RandomizerInf flag);

extern SaveContext gSaveContext;
extern PlayState* gPlayState;
}

void CheatsOnVanillaBehaviorHandler(GIVanillaBehavior id, bool* should, void* opt) {
    switch (id) {
        case VB_DEKU_STICK_BREAK: {
            if (CVarGetInteger(CVAR_CHEAT("DekuStickCheat"), DEKU_STICK_NORMAL) != DEKU_STICK_NORMAL) {
                *should = false;
            }
            break;
        }
        case VB_DEKU_STICK_BE_ON_FIRE: {
            if (CVarGetInteger(CVAR_CHEAT("DekuStickCheat"), DEKU_STICK_NORMAL) == DEKU_STICK_UNBREAKABLE_AND_ALWAYS_ON_FIRE) {
                Player* player = GET_PLAYER(gPlayState);
                player->unk_860 = 200;    // Keeps the stick's flame lit
                player->unk_85C = 1.0f;   // Ensures the stick is the proper length
                *should = true;
            }
            break;
        }
        case VB_DEKU_STICK_BURN_OUT: {
            if (CVarGetInteger(CVAR_CHEAT("DekuStickCheat"), DEKU_STICK_NORMAL) != DEKU_STICK_NORMAL) {
                *should = false;
            }
            break;
        }
        case VB_DEKU_STICK_BURN_DOWN: {
            if (CVarGetInteger(CVAR_CHEAT("DekuStickCheat"), DEKU_STICK_NORMAL) != DEKU_STICK_NORMAL) {
                *should = false;
            }
            break;
        }
    }
}

void RegisterInfiniteCheats() {
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnGameFrameUpdate>([]() {
        if (!GameInteractor::IsSaveLoaded(true)) {
            return;
        }

        if (CVarGetInteger(CVAR_CHEAT("InfiniteMoney"), 0) != 0 && (!IS_RANDO || Flags_GetRandomizerInf(RAND_INF_HAS_WALLET))) {
            if (gSaveContext.rupees < CUR_CAPACITY(UPG_WALLET)) {
                gSaveContext.rupees = CUR_CAPACITY(UPG_WALLET);
            }
        }

        if (CVarGetInteger(CVAR_CHEAT("InfiniteHealth"), 0) != 0) {
            if (gSaveContext.health < gSaveContext.healthCapacity) {
                gSaveContext.health = gSaveContext.healthCapacity;
            }
        }

        if (CVarGetInteger(CVAR_CHEAT("InfiniteAmmo"), 0) != 0) {
            // Deku Sticks
            if (AMMO(ITEM_STICK) < CUR_CAPACITY(UPG_STICKS)) {
                AMMO(ITEM_STICK) = CUR_CAPACITY(UPG_STICKS);
            }

            // Deku Nuts
            if (AMMO(ITEM_NUT) < CUR_CAPACITY(UPG_NUTS)) {
                AMMO(ITEM_NUT) = CUR_CAPACITY(UPG_NUTS);
            }

            // Bombs
            if (AMMO(ITEM_BOMB) < CUR_CAPACITY(UPG_BOMB_BAG)) {
                AMMO(ITEM_BOMB) = CUR_CAPACITY(UPG_BOMB_BAG);
            }

            // Fairy Bow (Ammo)
            if (AMMO(ITEM_BOW) < CUR_CAPACITY(UPG_QUIVER)) {
                AMMO(ITEM_BOW) = CUR_CAPACITY(UPG_QUIVER);
            }

            // Fairy Slingshot (Ammo)
            if (AMMO(ITEM_SLINGSHOT) < CUR_CAPACITY(UPG_BULLET_BAG)) {
                AMMO(ITEM_SLINGSHOT) = CUR_CAPACITY(UPG_BULLET_BAG);
            }

            // Bombchus (max: 50, no upgrades)
            if (INV_CONTENT(ITEM_BOMBCHU) == ITEM_BOMBCHU && AMMO(ITEM_BOMBCHU) < 50) {
                AMMO(ITEM_BOMBCHU) = 50;
            }
        }

        if (CVarGetInteger(CVAR_CHEAT("InfiniteMagic"), 0) != 0) {
            if (gSaveContext.isMagicAcquired && gSaveContext.magic != (gSaveContext.isDoubleMagicAcquired + 1) * 0x30) {
                gSaveContext.magic = (gSaveContext.isDoubleMagicAcquired + 1) * 0x30;
            }
        }

        if (CVarGetInteger(CVAR_CHEAT("InfiniteNayru"), 0) != 0) {
            gSaveContext.nayrusLoveTimer = 0x44B;
        }
    });
}

void RegisterMoonJumpOnL() {
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnGameFrameUpdate>([]() {
        if (!GameInteractor::IsSaveLoaded(true)) return;

        if (CVarGetInteger(CVAR_CHEAT("MoonJumpOnL"), 0) != 0) {
            Player* player = GET_PLAYER(gPlayState);

            if (CHECK_BTN_ANY(gPlayState->state.input[0].cur.button, BTN_L)) {
                player->actor.velocity.y = 6.34375f;
            }
        }
    });
}

void RegisterEasyISG() {
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnGameFrameUpdate>([]() {
        if (!GameInteractor::IsSaveLoaded(true)) return;

        if (CVarGetInteger(CVAR_CHEAT("EasyISG"), 0) != 0) {
            Player* player = GET_PLAYER(gPlayState);
            player->meleeWeaponState = 1;
        }
    });
}

//Permanent quick put away (QPA) glitched damage value
void RegisterEasyQPA() {
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnGameFrameUpdate>([]() {
        if (!GameInteractor::IsSaveLoaded(true)) return;

        if (CVarGetInteger(CVAR_CHEAT("EasyQPA"), 0) != 0) {
            Player* player = GET_PLAYER(gPlayState);
            player->meleeWeaponQuads[0].info.toucher.dmgFlags = 0x16171617;
            player->meleeWeaponQuads[1].info.toucher.dmgFlags = 0x16171617;
        }
    });
}

void RegisterUnrestrictedItems() {
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnGameFrameUpdate>([]() {
        if (!GameInteractor::IsSaveLoaded(true)) return;

        if (CVarGetInteger(CVAR_CHEAT("NoRestrictItems"), 0) != 0) {
            u8 sunsBackup = gPlayState->interfaceCtx.restrictions.sunsSong;
            memset(&gPlayState->interfaceCtx.restrictions, 0, sizeof(gPlayState->interfaceCtx.restrictions));
            gPlayState->interfaceCtx.restrictions.sunsSong = sunsBackup;
        }
    });
}

void RegisterFreezeTime() {
    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnGameFrameUpdate>([]() {
        if (CVarGetInteger(CVAR_CHEAT("FreezeTime"), 0) != 0) {
            if (CVarGetInteger(CVAR_GENERAL("PrevTime"), -1) == -1) {
                CVarSetInteger(CVAR_GENERAL("PrevTime"), gSaveContext.dayTime);
            }

            int32_t prevTime = CVarGetInteger(CVAR_GENERAL("PrevTime"), gSaveContext.dayTime);
            gSaveContext.dayTime = prevTime;
        } else {
            CVarClear(CVAR_GENERAL("PrevTime"));
        }
    });
}

void Cheats_Register() {
    static uint32_t onVanillaBehaviorHook = 0;

    RegisterInfiniteCheats();
    RegisterMoonJumpOnL();
    RegisterEasyISG();
    RegisterEasyQPA();
    RegisterUnrestrictedItems();
    RegisterFreezeTime();

    GameInteractor::Instance->RegisterGameHook<GameInteractor::OnLoadGame>([](int32_t fileNum) mutable {
        GameInteractor::Instance->UnregisterGameHook<GameInteractor::OnVanillaBehavior>(onVanillaBehaviorHook);
        onVanillaBehaviorHook = 0;
        onVanillaBehaviorHook = GameInteractor::Instance->RegisterGameHook<GameInteractor::OnVanillaBehavior>(CheatsOnVanillaBehaviorHandler);
    });
}
