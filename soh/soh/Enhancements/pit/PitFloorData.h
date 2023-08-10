extern "C" {
#include <z64.h>
#include "macros.h"
#include "functions.h"
#include "variables.h"
#include "functions.h"
#include "src/overlays/actors/ovl_En_Firefly/z_en_firefly.h"
#include "src/overlays/actors/ovl_En_Ossan/z_en_ossan.h"
#include "src/overlays/actors/ovl_En_Dekubaba/z_en_dekubaba.h"
#include "src/overlays/actors/ovl_En_Goma/z_en_goma.h"
}

struct EnemyData {
    s16 actorId;
    Vec3f pos;
    Vec3s rot;
    s16 params;
};

enum FloorType {
    ENEMY,
    REST,
    //END
};

struct FloorData {
    FloorType type;
    std::vector<EnemyData> enemies;
};

#define KEESE(x, y, z, type) \
{                            \
    ACTOR_EN_FIREFLY,        \
    { x, y, z },             \
    { 0, 0, 0 },             \
    type                     \
}

#define NORMAL_DEKUBABA(x, z) \
{                             \
    ACTOR_EN_DEKUBABA,        \
    { x, 0, z },              \
    { 0, 0, 0 },              \
    DEKUBABA_NORMAL           \
}

#define BIG_DEKUBABA(x, z) \
{                          \
    ACTOR_EN_DEKUBABA,     \
    { x, 0, z },           \
    { 0, 0, 0 },           \
    DEKUBABA_BIG           \
}

#define MAD_SCRUB(x, z, shotsPerRound) \
{                                      \
    ACTOR_EN_DEKUNUTS,                 \
    { x, 0, z },                       \
    { 0, 0, 0 },                       \
    shotsPerRound << 8                 \
}

#define GROUND_GHOMA_EGG(x, z) \
{                              \
    ACTOR_EN_GOMA,             \
    { x, 0, z },               \
    { 0, 0, 0 },               \
    7                          \
}

#define CEILING_GHOMA_EGG(x, z) \
{                               \
    ACTOR_EN_GOMA,              \
    { x, 0, z },                \
    { 0, 0, 0 },                \
    8                           \
}

//the max coordinates in the room are 466 horizontally
//-Z is forward
std::vector<FloorData> floorData = {
    //floors 1-9
    {
        FloorType::ENEMY,
        {
            KEESE(100, 70, -200, KEESE_NORMAL_FLY),
            KEESE(-100, 70, -200, KEESE_NORMAL_FLY),
        }
    },
    {
        FloorType::ENEMY,
        {
            NORMAL_DEKUBABA(100, -200),
            NORMAL_DEKUBABA(-100, -200)
        }
    },
    {
        FloorType::ENEMY,
        {
            KEESE(100, 70, -200, KEESE_NORMAL_FLY),
            KEESE(-100, 70, -200, KEESE_NORMAL_FLY),
            KEESE(0, 70, -200, KEESE_NORMAL_FLY),
            KEESE(0, 50, -250, KEESE_NORMAL_FLY)
        }
    },
    {
        FloorType::ENEMY,
        {
            KEESE(100, 70, -200, KEESE_NORMAL_FLY),
            KEESE(-100, 70, -200, KEESE_NORMAL_FLY),
            NORMAL_DEKUBABA(0, -200)
        }
    },
    {
        FloorType::ENEMY,
        {
            KEESE(100, 70, -200, KEESE_NORMAL_FLY),
            KEESE(-100, 70, -200, KEESE_NORMAL_FLY),
            KEESE(0, 70, -200, KEESE_NORMAL_FLY),
            NORMAL_DEKUBABA(0, -200),
            NORMAL_DEKUBABA(0, -300)
        }
    },
    {
        FloorType::ENEMY,
        {
            KEESE(100, 70, -200, KEESE_NORMAL_FLY),
            KEESE(-100, 70, -200, KEESE_NORMAL_FLY),
            NORMAL_DEKUBABA(0, -200),
            GROUND_GHOMA_EGG(100, -200),
            GROUND_GHOMA_EGG(-100, -200)
        }
    },
    {
        FloorType::ENEMY,
        {
            NORMAL_DEKUBABA(0, -200),
            MAD_SCRUB(100, -200, 2),
            MAD_SCRUB(-100, -200, 2)
        }
    },
    {
        FloorType::ENEMY,
        {
            GROUND_GHOMA_EGG(100, -200),
            GROUND_GHOMA_EGG(-100, -200),
            NORMAL_DEKUBABA(0, -200),
            MAD_SCRUB(50, -300, 3),
            MAD_SCRUB(-50, -300, 3)
        }
    },
    {
        FloorType::ENEMY,
        {
            KEESE(100, 70, -200, KEESE_NORMAL_FLY),
            KEESE(-100, 70, -200, KEESE_NORMAL_FLY),
            KEESE(0, 70, -200, KEESE_NORMAL_FLY),
            BIG_DEKUBABA(0, -200),
            GROUND_GHOMA_EGG(100, -200),
            GROUND_GHOMA_EGG(-100, -200),
            GROUND_GHOMA_EGG(0, 200),
            MAD_SCRUB(50, -300, 5),
            MAD_SCRUB(-50, -300, 5)
        }
    },

    //floor 10
    {
        FloorType::REST
    }
};

#undef KEESE
#undef NORMAL_DEKUBABA
#undef BIG_DEKUBABA
#undef MAD_SCRUB
#undef GROUND_GHOMA_EGG
#undef CEILING_GHOMA_EGG