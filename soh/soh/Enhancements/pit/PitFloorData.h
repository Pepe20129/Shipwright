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
#include "src/overlays/actors/ovl_En_Tite/z_en_tite.h"
#include "src/overlays/actors/ovl_En_Wf/z_en_wf.h"
#include "src/overlays/actors/ovl_En_Am/z_en_am.h"
#include "src/overlays/actors/ovl_En_Zf/z_en_zf.h"
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

#define TEKTITE(x, z, type) \
{                           \
    ACTOR_EN_TITE,          \
    { x, 0, z },            \
    { 0, 0, 0 },            \
    type                    \
}

#define WOLFOS(x, z, type) \
{                          \
    ACTOR_EN_WF,           \
    { x, 0, z },           \
    { 0, 0, 0 },           \
    type                   \
}

#define REEBA(x, z, big) \
{                        \
    ACTOR_EN_REEBA,      \
    { x, 0, z },         \
    { 0, 0, 0 },         \
    big                  \
}

#define ARMOS(x, z) \
{                   \
    ACTOR_EN_AM,    \
    { x, 0, z },    \
    { 0, 0, 0 },    \
    ARMOS_ENEMY     \
}

#define BABY_DODONGO(x, z) \
{                          \
    ACTOR_EN_DODOJR,       \
    { x, 0, z },           \
    { 0, 0, 0 },           \
    0                      \
}

#define DODONGO(x, z) \
{                     \
    ACTOR_EN_DODONGO, \
    { x, 0, z },      \
    { 0, 0, 0 },      \
    -1                \
}

#define LIZALFOS(x, z)      \
{                           \
    ACTOR_EN_ZF,            \
    { x, 0, z },            \
    { 0, 0, 0 },            \
    ENZF_TYPE_LIZALFOS_LONE \
}

#define DINOLFOS(x, z) \
{                      \
    ACTOR_EN_ZF,       \
    { x, 0, z },       \
    { 0, 0, 0 },       \
    ENZF_TYPE_DINOLFOS \
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
            GROUND_GHOMA_EGG(0, 125),
            MAD_SCRUB(50, -300, 5),
            MAD_SCRUB(-50, -300, 5)
        }
    },

    //floor 10
    {
        FloorType::REST
    },

    //floors 11-19
    {
        FloorType::ENEMY,
        {
            KEESE(100, 70, -200, KEESE_NORMAL_FLY),
            KEESE(-100, 70, -200, KEESE_NORMAL_FLY),
            BABY_DODONGO(0, -200),
            BABY_DODONGO(0, -100),
        }
    },
    {
        FloorType::ENEMY,
        {
            BABY_DODONGO(100, -200),
            BABY_DODONGO(0, -200),
            BABY_DODONGO(-100, -200),
            BABY_DODONGO(0, -100),
        }
    },
    {
        FloorType::ENEMY,
        {
            KEESE(100, 70, -200, KEESE_NORMAL_FLY),
            KEESE(-100, 70, -200, KEESE_NORMAL_FLY),
            KEESE(0, 70, -200, KEESE_NORMAL_FLY),
            KEESE(0, 50, -250, KEESE_NORMAL_FLY),
            BABY_DODONGO(0, -200),
            TEKTITE(100, -100, TEKTITE_RED),
            TEKTITE(0, -100, TEKTITE_RED),
            TEKTITE(-100, -100, TEKTITE_RED),
        }
    },
    {
        FloorType::ENEMY,
        {
            KEESE(100, 70, 200, KEESE_NORMAL_FLY),
            KEESE(-100, 70, 200, KEESE_NORMAL_FLY),
            BABY_DODONGO(0, -300),
            BABY_DODONGO(0, -200),
            BABY_DODONGO(0, 125),
            TEKTITE(100, -100, TEKTITE_RED),
            TEKTITE(-100, -100, TEKTITE_RED),
        }
    },
    {
        FloorType::ENEMY,
        {
            WOLFOS(100, -200, WOLFOS_NORMAL),
            WOLFOS(-100, -200, WOLFOS_NORMAL),
            DODONGO(0, -150),
            TEKTITE(50, -100, TEKTITE_RED),
            TEKTITE(-50, -100, TEKTITE_RED),
        }
    },
    {
        FloorType::ENEMY,
        {
            KEESE(0, 70, 200, KEESE_NORMAL_FLY),
            DODONGO(100, -200),
            DODONGO(0, -150),
            DODONGO(-100, -200),
        }
    },
    {
        FloorType::ENEMY,
        {
            WOLFOS(100, -200, WOLFOS_NORMAL),
            WOLFOS(-100, -200, WOLFOS_NORMAL),
            DODONGO(00, -200),
            LIZALFOS(50, -100),
            LIZALFOS(-50, -100),
        }
    },
    {
        FloorType::ENEMY,
        {
            WOLFOS(100, -300, WOLFOS_NORMAL),
            WOLFOS(-100, -300, WOLFOS_NORMAL),
            KEESE(100, 70, 200, KEESE_NORMAL_FLY),
            KEESE(-100, 70, 200, KEESE_NORMAL_FLY),
            KEESE(0, 70, 200, KEESE_NORMAL_FLY),
            KEESE(0, 50, 250, KEESE_NORMAL_FLY),
            DODONGO(100, -200),
            DODONGO(-100, -200),
            TEKTITE(150, 0, TEKTITE_RED),
            TEKTITE(-150, 0, TEKTITE_RED),
        }
    },
    {
        FloorType::ENEMY,
        {
            WOLFOS(100, -300, WOLFOS_NORMAL),
            WOLFOS(-100, -300, WOLFOS_NORMAL),
            WOLFOS(0, -300, WOLFOS_NORMAL),
            KEESE(100, 70, 200, KEESE_NORMAL_FLY),
            KEESE(-100, 70, 200, KEESE_NORMAL_FLY),
            KEESE(0, 70, 200, KEESE_NORMAL_FLY),
            KEESE(0, 50, 250, KEESE_NORMAL_FLY),
            DODONGO(100, -200),
            DODONGO(-100, -200),
            TEKTITE(150, 0, TEKTITE_RED),
            TEKTITE(-150, 0, TEKTITE_RED),
            TEKTITE(0, 150, TEKTITE_RED),
            LIZALFOS(50, -100),
            LIZALFOS(-50, -100),
        }
    },

    //floor 20
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
#undef TEKTITE
#undef WOLFOS
#undef REEBA
#undef ARMOS
#undef BABY_DODONGO
#undef DODONGO