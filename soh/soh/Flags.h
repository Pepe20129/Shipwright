#pragma once

#include <libultraship/libultra/types.h>
#include "soh/Enhancements/randomizer/randomizerTypes.h"
#include "z64scene.h"

typedef enum FlagType {
    FLAG_TYPE_NONE,
    FLAG_TYPE_EVENT_CHECK_INF,
    FLAG_TYPE_ITEM_GET_INF,
    FLAG_TYPE_INF_TABLE,
    FLAG_TYPE_EVENT_INF,
    FLAG_TYPE_RANDOMIZER_INF,
    FLAG_TYPE_GS_TOKEN,
    FLAG_TYPE_SCENE_SWITCH,
    FLAG_TYPE_SCENE_TREASURE,
    FLAG_TYPE_SCENE_CLEAR,
    FLAG_TYPE_SCENE_COLLECTIBLE,
    FLAG_TYPE_MAX,
} FlagType;

typedef struct Flag {
    FlagType type;
    s32 id;

    /*
     * Only used for the following types:
     *  - FLAG_TYPE_SCENE_SWITCH
     *  - FLAG_TYPE_SCENE_TREASURE
     *  - FLAG_TYPE_SCENE_CLEAR
     *  - FLAG_TYPE_SCENE_COLLECTIBLE
     */
    SceneID scene;

#ifdef __cplusplus
    static Flag FromEventCheckInf(s32 eventCheckInf);
    static Flag FromItemGetInf(s32 itemGetInf);
    static Flag FromInfTable(s32 infTable);
    static Flag FromEventInf(s32 eventInf);
    static Flag FromRandomizerInf(RandomizerInf randInf);
    static Flag FromGSToken(s32 gsToken);
    static Flag FromRawParts(FlagType flagType, s32 flagID, SceneID scene);

    bool Get();
    void Set();
    void Unset();
#endif
} Flag;

#ifndef __cplusplus
Flag Flag_None();
Flag Flag_FromEventCheckInf(s32 eventCheckInf);
Flag Flag_FromItemGetInf(s32 itemGetInf);
Flag Flag_FromInfTable(s32 infTable);
Flag Flag_FromEventInf(s32 eventInf);
Flag Flag_FromRandomizerInf(RandomizerInf randInf);
Flag Flag_FromGSToken(s32 gsToken);

Flag Flag_FromRawParts(FlagType flagType, s32 flagID, SceneID scene);

bool Flag_Get(Flag flag);
void Flag_Set(Flag flag);
void Flag_Unset(Flag flag);
#endif