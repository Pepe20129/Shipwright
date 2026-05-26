#include "soh/Flags.h"
#include "z64.h"
#include <spdlog/spdlog.h>
#include <cassert>

#include "functions.h"
#include "variables.h"
extern "C" {
#include "macros.h"
extern PlayState* gPlayState;
}

#pragma region Factory Functions

const Flag Flag::FromEventCheckInf(s32 eventCheckInf) {
    return Flag::FromRawParts(FLAG_TYPE_EVENT_CHECK_INF, eventCheckInf, SCENE_ID_MAX);
}

const Flag Flag::FromItemGetInf(s32 itemGetInf) {
    return Flag::FromRawParts(FLAG_TYPE_ITEM_GET_INF, itemGetInf, SCENE_ID_MAX);
}

const Flag Flag::FromInfTable(s32 infTable) {
    return Flag::FromRawParts(FLAG_TYPE_INF_TABLE, infTable, SCENE_ID_MAX);
}

const Flag Flag::FromEventInf(s32 eventInf) {
    return Flag::FromRawParts(FLAG_TYPE_EVENT_INF, eventInf, SCENE_ID_MAX);
}

const Flag Flag::FromRandomizerInf(RandomizerInf randInf) {
    return Flag::FromRawParts(FLAG_TYPE_RANDOMIZER_INF, static_cast<s32>(randInf), SCENE_ID_MAX);
}

const Flag Flag::FromGSToken(s32 gsToken) {
    return Flag::FromRawParts(FLAG_TYPE_GS_TOKEN, gsToken, SCENE_ID_MAX);
}

/// @brief Create a Flag from its raw parts
/// @param type The type of the flag
/// @param id The ID of the flag
/// @param scene The scene of the flag
/// @return The created Flag
const Flag Flag::FromRawParts(FlagType type, s32 id, SceneID scene) {
    if (type >= FLAG_TYPE_MAX) {
        SPDLOG_ERROR("[Flag::FromRawParts] Invalid type ({})", static_cast<s32>(type));
        assert(false);
    }

    return Flag { .type = type, .id = id, .scene = scene };
}

#pragma endregion

/// @brief Gets the current value of this Flag
/// @return The current value of this Flag
bool Flag::Get() const {
    switch (this->type) {
        case FLAG_TYPE_NONE:
            return false;

        case FLAG_TYPE_EVENT_CHECK_INF:
            return Flags_GetEventChkInf(this->id) != 0;
        case FLAG_TYPE_ITEM_GET_INF:
            return Flags_GetItemGetInf(this->id) != 0;
        case FLAG_TYPE_INF_TABLE:
            return Flags_GetInfTable(this->id) != 0;
        case FLAG_TYPE_EVENT_INF:
            return Flags_GetEventInf(this->id) != 0;
        case FLAG_TYPE_RANDOMIZER_INF:
            // Randomizer flags are currently accessible from any quest (boss rush as an example)
            /*
            if (!IS_RANDO) {
                SPDLOG_ERROR("[Flag::Get] Tried to get randomizerInf flag ({}) outside of rando", this->id);
                assert(false);
                return false;
            }
            */
            return Flags_GetRandomizerInf(static_cast<RandomizerInf>(this->id)) != 0;

        case FLAG_TYPE_GS_TOKEN:
            return GET_GS_FLAGS((this->id & 0x1F00) >> 8) != 0;

        case FLAG_TYPE_SCENE_SWITCH:
            return (gSaveContext.sceneFlags[this->scene].swch & (1 << this->id)) != 0;
        case FLAG_TYPE_SCENE_TREASURE:
            return (gSaveContext.sceneFlags[this->scene].chest & (1 << this->id)) != 0;
        case FLAG_TYPE_SCENE_CLEAR:
            return (gSaveContext.sceneFlags[this->scene].clear & (1 << this->id)) != 0;
        case FLAG_TYPE_SCENE_COLLECTIBLE:
            return (gSaveContext.sceneFlags[this->scene].collect & (1 << this->id)) != 0;

        case FLAG_TYPE_MAX:
        default:
            SPDLOG_ERROR("[Flag::Get] Invalid this->type ({})", static_cast<s32>(this->type));
            assert(false);
            return false;
    }
}

/// @brief Sets this Flag to `true`
/// If this Flag is a scene specific flag and its scene is the current scene,
/// the actor context will also be updated
void Flag::Set() const {
    switch (this->type) {
        case FLAG_TYPE_NONE:
            return;

        case FLAG_TYPE_EVENT_CHECK_INF:
            Flags_SetEventChkInf(this->id);
            return;
        case FLAG_TYPE_ITEM_GET_INF:
            Flags_SetItemGetInf(this->id);
            return;
        case FLAG_TYPE_INF_TABLE:
            Flags_SetInfTable(this->id);
            return;
        case FLAG_TYPE_EVENT_INF:
            Flags_SetEventInf(this->id);
            return;
        case FLAG_TYPE_RANDOMIZER_INF:
            // Randomizer flags are currently accessible from any quest (boss rush as an example)
            /*
            if (!IS_RANDO) {
                SPDLOG_ERROR("[Flag::Set] Tried to set randomizerInf flag ({}) outside of rando", this->id);
                assert(false);
                return;
            }
            */
            Flags_SetRandomizerInf(static_cast<RandomizerInf>(this->id));
            return;

        case FLAG_TYPE_GS_TOKEN:
            SET_GS_FLAGS((this->id & 0x1F00) >> 8, this->id & 0xFF);
            return;

        case FLAG_TYPE_SCENE_SWITCH:
            if (gPlayState != nullptr && this->scene == gPlayState->sceneNum) {
                if (this->id < 0x20) {
                    gPlayState->actorCtx.flags.swch |= (1 << this->id);
                } else {
                    gPlayState->actorCtx.flags.tempSwch |= (1 << (this->id - 0x20));
                }
            }

            if (this->id < 0x20) {
                gSaveContext.sceneFlags[this->scene].swch |= (1 << this->id);
            }
            return;
        case FLAG_TYPE_SCENE_TREASURE:
            if (gPlayState != nullptr && this->scene == gPlayState->sceneNum) {
                gPlayState->actorCtx.flags.chest |= (1 << this->id);
            }

            gSaveContext.sceneFlags[this->scene].chest |= (1 << this->id);
            return;
        case FLAG_TYPE_SCENE_CLEAR:
            if (gPlayState != nullptr && this->scene == gPlayState->sceneNum) {
                gPlayState->actorCtx.flags.clear |= (1 << this->id);
            }

            gSaveContext.sceneFlags[this->scene].clear |= (1 << this->id);
            return;
        case FLAG_TYPE_SCENE_COLLECTIBLE:
            if (gPlayState != nullptr && this->scene == gPlayState->sceneNum) {
                if (this->id != 0) {
                    if (this->id < 0x20) {
                        gPlayState->actorCtx.flags.collect |= (1 << this->id);
                    } else {
                        gPlayState->actorCtx.flags.tempCollect |= (1 << (this->id - 0x20));
                    }
                }
            }

            if (this->id != 0 && this->id < 0x20) {
                gSaveContext.sceneFlags[this->scene].collect |= (1 << this->id);
            }
            return;

        case FLAG_TYPE_MAX:
        default:
            SPDLOG_ERROR("[Flag::Set] Invalid this->type ({})", static_cast<s32>(this->type));
            assert(false);
            return;
    }
}

/// @brief Sets this Flag to `false`
/// If this Flag is a scene specific flag and its scene is the current scene,
/// the actor context will also be updated
void Flag::Unset() const {
    switch (this->type) {
        case FLAG_TYPE_NONE:
            return;

        case FLAG_TYPE_EVENT_CHECK_INF:
            Flags_UnsetEventChkInf(this->id);
            return;
        case FLAG_TYPE_ITEM_GET_INF:
            Flags_UnsetItemGetInf(this->id);
            return;
        case FLAG_TYPE_INF_TABLE:
            Flags_UnsetInfTable(this->id);
            return;
        case FLAG_TYPE_EVENT_INF:
            Flags_UnsetEventInf(this->id);
            return;
        case FLAG_TYPE_RANDOMIZER_INF:
            // Randomizer flags are currently accessible from any quest (boss rush as an example)
            /*
            if (!IS_RANDO) {
                SPDLOG_ERROR("[Flag::Unset] Tried to unset randomizerInf flag ({}) outside of rando", this->id);
                assert(false);
                return;
            }
            */
            Flags_UnsetRandomizerInf(static_cast<RandomizerInf>(this->id));
            return;

        case FLAG_TYPE_GS_TOKEN:
            gSaveContext.gsFlags[((this->id & 0x1F00) >> 8) >> 2] &= ~((this->id & 0xFF) << gGsFlagsShifts[((this->id & 0x1F00) >> 8) & 3]);
            return;

        case FLAG_TYPE_SCENE_SWITCH:
            if (gPlayState != nullptr && this->scene == gPlayState->sceneNum) {
                if (this->id < 0x20) {
                    gPlayState->actorCtx.flags.swch &= ~(1 << this->id);
                } else {
                    gPlayState->actorCtx.flags.tempSwch &= ~(1 << (this->id - 0x20));
                }
            }

            if (this->id < 0x20) {
                gSaveContext.sceneFlags[this->scene].swch &= ~(1 << this->id);
            }
            return;
        case FLAG_TYPE_SCENE_TREASURE:
            if (gPlayState != nullptr && this->scene == gPlayState->sceneNum) {
                gPlayState->actorCtx.flags.chest &= ~(1 << this->id);
            }

            gSaveContext.sceneFlags[this->scene].chest &= ~(1 << this->id);
            return;
        case FLAG_TYPE_SCENE_CLEAR:
            if (gPlayState != nullptr && this->scene == gPlayState->sceneNum) {
                gPlayState->actorCtx.flags.clear &= ~(1 << this->id);
            }

            gSaveContext.sceneFlags[this->scene].clear &= ~(1 << this->id);
            return;
        case FLAG_TYPE_SCENE_COLLECTIBLE:
            if (gPlayState != nullptr && this->scene == gPlayState->sceneNum) {
                if (this->id != 0) {
                    if (this->id < 0x20) {
                        gPlayState->actorCtx.flags.collect &= ~(1 << this->id);
                    } else {
                        gPlayState->actorCtx.flags.tempCollect &= ~(1 << (this->id - 0x20));
                    }
                }
            }

            if (this->id != 0 && this->id < 0x20) {
                gSaveContext.sceneFlags[this->scene].collect &= ~(1 << this->id);
            }
            return;

        case FLAG_TYPE_MAX:
        default:
            SPDLOG_ERROR("[Flag::Unset] Invalid this->type ({})", static_cast<s32>(this->type));
            assert(false);
            return;
    }
}

bool Flag::IsSceneFlag() const {
    return this->type == FLAG_TYPE_SCENE_SWITCH || this->type == FLAG_TYPE_SCENE_TREASURE || this->type == FLAG_TYPE_SCENE_CLEAR || this->type == FLAG_TYPE_SCENE_COLLECTIBLE;
}

#pragma region C Compat

/// @brief Creates an empty Flag. For use in C code, do not use this from C++ code (instead use `std::optional`)
/// @return An empty Flag
extern "C" Flag Flag_None() {
    return Flag::FromRawParts(FLAG_TYPE_NONE, 0, SCENE_ID_MAX);
}

extern "C" Flag Flag_FromEventCheckInf(s32 eventCheckInf) {
    return Flag::FromEventCheckInf(eventCheckInf);
}

extern "C" Flag Flag_FromItemGetInf(s32 itemGetInf) {
    return Flag::FromItemGetInf(itemGetInf);
}

extern "C" Flag Flag_FromInfTable(s32 infTable) {
    return Flag::FromInfTable(infTable);
}

extern "C" Flag Flag_FromEventInf(s32 eventInf) {
    return Flag::FromEventInf(eventInf);
}

extern "C" Flag Flag_FromRandomizerInf(RandomizerInf randInf) {
    return Flag::FromRandomizerInf(randInf);
}

extern "C" Flag Flag_FromGSToken(s32 gsToken) {
    return Flag::FromGSToken(gsToken);
}

extern "C" Flag Flag_FromRawParts(FlagType flagType, s32 flagID, SceneID scene) {
    return Flag::FromRawParts(flagType, flagID, scene);
}

extern "C" bool Flag_Get(Flag flag) {
    return flag.Get();
}

extern "C" void Flag_Set(Flag flag) {
    flag.Set();
}

extern "C" void Flag_Unset(Flag flag) {
    flag.Unset();
}

#pragma endregion