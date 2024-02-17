#include "src/overlays/misc/ovl_kaleido_scope/z_kaleido_scope.h"
#include "z64item.h"

u8 gCyanSlotAgeReqs[24] = {
    AGE_REQ_NONE, // CYAN_SLOT_EMPTY_1
    AGE_REQ_NONE, // CYAN_SLOT_EMPTY_2
    AGE_REQ_NONE, // CYAN_SLOT_EMPTY_3
    AGE_REQ_NONE, // CYAN_SLOT_EMPTY_4
    AGE_REQ_NONE, // CYAN_SLOT_EMPTY_5
    AGE_REQ_NONE, // CYAN_SLOT_EMPTY_6
    AGE_REQ_NONE, // CYAN_SLOT_EMPTY_7
    AGE_REQ_NONE, // CYAN_SLOT_NUT
    AGE_REQ_NONE, // CYAN_SLOT_BOMB
    AGE_REQ_NONE, // CYAN_SLOT_BOW
    AGE_REQ_NONE, // CYAN_SLOT_OCARINA
    AGE_REQ_NONE, // CYAN_SLOT_EMPTY_8
    AGE_REQ_NONE, // CYAN_SLOT_EMPTY_9
    AGE_REQ_NONE, // CYAN_SLOT_BOMBCHU
    AGE_REQ_NONE, // CYAN_SLOT_HOOKSHOT
    AGE_REQ_NONE, // CYAN_SLOT_BOOMERANG
    AGE_REQ_NONE, // CYAN_SLOT_HAMMER
    AGE_REQ_NONE, // CYAN_SLOT_EMPTY_10
    AGE_REQ_NONE, // CYAN_SLOT_EMPTY_11
    AGE_REQ_NONE, // CYAN_SLOT_EMPTY_12
    AGE_REQ_NONE, // CYAN_SLOT_BOTTLE
    AGE_REQ_NONE, // CYAN_SLOT_MISC
    AGE_REQ_NONE, // CYAN_SLOT_EMPTY_13
    AGE_REQ_NONE, // CYAN_SLOT_EMPTY_14
};

u8 gCyanItemAgeReqs[86] = {
    AGE_REQ_CHILD, // ITEM_DEKU_STICK
    AGE_REQ_NONE,  // ITEM_DEKU_NUT
    AGE_REQ_NONE,  // ITEM_BOMB
    AGE_REQ_ADULT, // ITEM_BOW
    AGE_REQ_ADULT, // ITEM_ARROW_FIRE
    AGE_REQ_NONE,  // ITEM_DINS_FIRE
    AGE_REQ_CHILD, // ITEM_SLINGSHOT
    AGE_REQ_NONE,  // ITEM_OCARINA_FAIRY
    AGE_REQ_NONE,  // ITEM_OCARINA_OF_TIME
    AGE_REQ_NONE,  // ITEM_BOMBCHU
    AGE_REQ_ADULT, // ITEM_HOOKSHOT
    AGE_REQ_ADULT, // ITEM_LONGSHOT
    AGE_REQ_ADULT, // ITEM_ARROW_ICE
    AGE_REQ_NONE,  // ITEM_FARORES_WIND
    AGE_REQ_NONE,  // ITEM_BOOMERANG
    AGE_REQ_NONE,  // ITEM_LENS_OF_TRUTH
    AGE_REQ_CHILD, // ITEM_MAGIC_BEAN
    AGE_REQ_ADULT, // ITEM_HAMMER
    AGE_REQ_ADULT, // ITEM_ARROW_LIGHT
    AGE_REQ_NONE,  // ITEM_NAYRUS_LOVE
    AGE_REQ_NONE,  // ITEM_BOTTLE_EMPTY
    AGE_REQ_NONE,  // ITEM_BOTTLE_POTION_RED
    AGE_REQ_NONE,  // ITEM_BOTTLE_POTION_GREEN
    AGE_REQ_NONE,  // ITEM_BOTTLE_POTION_BLUE
    AGE_REQ_NONE,  // ITEM_BOTTLE_FAIRY
    AGE_REQ_NONE,  // ITEM_BOTTLE_FISH
    AGE_REQ_NONE,  // ITEM_BOTTLE_MILK_FULL
    AGE_REQ_NONE,  // ITEM_BOTTLE_RUTOS_LETTER
    AGE_REQ_NONE,  // ITEM_BOTTLE_BLUE_FIRE
    AGE_REQ_NONE,  // ITEM_BOTTLE_BUG
    AGE_REQ_NONE,  // ITEM_BOTTLE_BIG_POE
    AGE_REQ_NONE,  // ITEM_BOTTLE_MILK_HALF
    AGE_REQ_NONE,  // ITEM_BOTTLE_POE
    AGE_REQ_CHILD, // ITEM_WEIRD_EGG
    AGE_REQ_CHILD, // ITEM_CHICKEN
    AGE_REQ_CHILD, // ITEM_ZELDAS_LETTER
    AGE_REQ_CHILD, // ITEM_MASK_KEATON
    AGE_REQ_CHILD, // ITEM_MASK_SKULL
    AGE_REQ_CHILD, // ITEM_MASK_SPOOKY
    AGE_REQ_NONE,  // ITEM_MASK_BUNNY_HOOD
    AGE_REQ_CHILD, // ITEM_MASK_GORON
    AGE_REQ_CHILD, // ITEM_MASK_ZORA
    AGE_REQ_CHILD, // ITEM_MASK_GERUDO
    AGE_REQ_NONE,  // ITEM_MASK_TRUTH
    AGE_REQ_CHILD, // ITEM_SOLD_OUT
    AGE_REQ_ADULT, // ITEM_POCKET_EGG
    AGE_REQ_ADULT, // ITEM_POCKET_CUCCO
    AGE_REQ_ADULT, // ITEM_COJIRO
    AGE_REQ_ADULT, // ITEM_ODD_MUSHROOM
    AGE_REQ_ADULT, // ITEM_ODD_POTION
    AGE_REQ_ADULT, // ITEM_POACHERS_SAW
    AGE_REQ_ADULT, // ITEM_BROKEN_GORONS_SWORD
    AGE_REQ_ADULT, // ITEM_PRESCRIPTION
    AGE_REQ_NONE,  // ITEM_EYEBALL_FROG
    AGE_REQ_ADULT, // ITEM_EYE_DROPS
    AGE_REQ_ADULT, // ITEM_CLAIM_CHECK
    AGE_REQ_ADULT, // ITEM_BOW_FIRE
    AGE_REQ_ADULT, // ITEM_BOW_ICE
    AGE_REQ_ADULT, // ITEM_BOW_LIGHT
    AGE_REQ_CHILD, // ITEM_SWORD_KOKIRI
    AGE_REQ_ADULT, // ITEM_SWORD_MASTER
    AGE_REQ_ADULT, // ITEM_SWORD_BIGGORON
    AGE_REQ_CHILD, // ITEM_SHIELD_DEKU
    AGE_REQ_NONE,  // ITEM_SHIELD_HYLIAN
    AGE_REQ_ADULT, // ITEM_SHIELD_MIRROR
    AGE_REQ_NONE,  // ITEM_TUNIC_KOKIRI
    AGE_REQ_ADULT, // ITEM_TUNIC_GORON
    AGE_REQ_ADULT, // ITEM_TUNIC_ZORA
    AGE_REQ_NONE,  // ITEM_BOOTS_KOKIRI
    AGE_REQ_ADULT, // ITEM_BOOTS_IRON
    AGE_REQ_ADULT, // ITEM_BOOTS_HOVER
    AGE_REQ_CHILD, // ITEM_BULLET_BAG_30
    AGE_REQ_CHILD, // ITEM_BULLET_BAG_40
    AGE_REQ_CHILD, // ITEM_BULLET_BAG_50
    AGE_REQ_ADULT, // ITEM_QUIVER_30
    AGE_REQ_ADULT, // ITEM_QUIVER_40
    AGE_REQ_ADULT, // ITEM_QUIVER_50
    AGE_REQ_NONE,  // ITEM_BOMB_BAG_20
    AGE_REQ_NONE,  // ITEM_BOMB_BAG_30
    AGE_REQ_NONE,  // ITEM_BOMB_BAG_40
    AGE_REQ_CHILD, // ITEM_STRENGTH_GORONS_BRACELET
    AGE_REQ_ADULT, // ITEM_STRENGTH_SILVER_GAUNTLETS
    AGE_REQ_ADULT, // ITEM_STRENGTH_GOLD_GAUNTLETS
    AGE_REQ_NONE,  // ITEM_SCALE_SILVER
    AGE_REQ_NONE,  // ITEM_SCALE_GOLDEN
    AGE_REQ_ADULT, // ITEM_GIANTS_KNIFE
};

u8 gCyanItemSlots[86] = {
    CYAN_SLOT_NONE,      // ITEM_DEKU_STICK
    CYAN_SLOT_NUT,       // ITEM_DEKU_NUT
    CYAN_SLOT_BOMB,      // ITEM_BOMB
    CYAN_SLOT_BOW,       // ITEM_BOW
    CYAN_SLOT_BOW,       // ITEM_ARROW_FIRE
    CYAN_SLOT_NONE,      // ITEM_DINS_FIRE
    CYAN_SLOT_NONE,      // ITEM_SLINGSHOT
    CYAN_SLOT_NONE,      // ITEM_OCARINA_FAIRY
    CYAN_SLOT_OCARINA,   // ITEM_OCARINA_OF_TIME
    CYAN_SLOT_BOMBCHU,   // ITEM_BOMBCHU
    CYAN_SLOT_NONE,      // ITEM_HOOKSHOT
    CYAN_SLOT_HOOKSHOT,  // ITEM_LONGSHOT
    CYAN_SLOT_BOW,       // ITEM_ARROW_ICE
    CYAN_SLOT_NONE,      // ITEM_FARORES_WIND
    CYAN_SLOT_BOOMERANG, // ITEM_BOOMERANG
    CYAN_SLOT_NONE,      // ITEM_LENS_OF_TRUTH
    CYAN_SLOT_NONE,      // ITEM_MAGIC_BEAN
    CYAN_SLOT_HAMMER,    // ITEM_HAMMER
    CYAN_SLOT_BOW,       // ITEM_ARROW_LIGHT
    CYAN_SLOT_NONE,      // ITEM_NAYRUS_LOVE
    CYAN_SLOT_BOTTLE,    // ITEM_BOTTLE_EMPTY
    CYAN_SLOT_BOTTLE,    // ITEM_BOTTLE_POTION_RED
    CYAN_SLOT_BOTTLE,    // ITEM_BOTTLE_POTION_GREEN
    CYAN_SLOT_BOTTLE,    // ITEM_BOTTLE_POTION_BLUE
    CYAN_SLOT_BOTTLE,    // ITEM_BOTTLE_FAIRY
    CYAN_SLOT_BOTTLE,    // ITEM_BOTTLE_FISH
    CYAN_SLOT_BOTTLE,    // ITEM_BOTTLE_MILK_FULL
    CYAN_SLOT_BOTTLE,    // ITEM_BOTTLE_RUTOS_LETTER
    CYAN_SLOT_BOTTLE,    // ITEM_BOTTLE_BLUE_FIRE
    CYAN_SLOT_BOTTLE,    // ITEM_BOTTLE_BUG
    CYAN_SLOT_BOTTLE,    // ITEM_BOTTLE_BIG_POE
    CYAN_SLOT_BOTTLE,    // ITEM_BOTTLE_MILK_HALF
    CYAN_SLOT_BOTTLE,    // ITEM_BOTTLE_POE
    CYAN_SLOT_NONE,      // ITEM_WEIRD_EGG
    CYAN_SLOT_NONE,      // ITEM_CHICKEN
    CYAN_SLOT_NONE,      // ITEM_ZELDAS_LETTER
    CYAN_SLOT_NONE,      // ITEM_MASK_KEATON
    CYAN_SLOT_NONE,      // ITEM_MASK_SKULL
    CYAN_SLOT_NONE,      // ITEM_MASK_SPOOKY
    CYAN_SLOT_MISC,      // ITEM_MASK_BUNNY_HOOD
    CYAN_SLOT_NONE,      // ITEM_MASK_GORON
    CYAN_SLOT_NONE,      // ITEM_MASK_ZORA
    CYAN_SLOT_NONE,      // ITEM_MASK_GERUDO
    CYAN_SLOT_MISC,      // ITEM_MASK_TRUTH
    CYAN_SLOT_NONE,      // ITEM_SOLD_OUT
    CYAN_SLOT_NONE,      // ITEM_POCKET_EGG
    CYAN_SLOT_NONE,      // ITEM_POCKET_CUCCO
    CYAN_SLOT_NONE,      // ITEM_COJIRO
    CYAN_SLOT_NONE,      // ITEM_ODD_MUSHROOM
    CYAN_SLOT_NONE,      // ITEM_ODD_POTION
    CYAN_SLOT_NONE,      // ITEM_POACHERS_SAW
    CYAN_SLOT_NONE,      // ITEM_BROKEN_GORONS_SWORD
    CYAN_SLOT_NONE,      // ITEM_PRESCRIPTION
    CYAN_SLOT_MISC,      // ITEM_EYEBALL_FROG
    CYAN_SLOT_NONE,      // ITEM_EYE_DROPS
    CYAN_SLOT_NONE,      // ITEM_CLAIM_CHECK
    CYAN_SLOT_BOW,       // ITEM_BOW_FIRE
    CYAN_SLOT_BOW,       // ITEM_BOW_ICE
    CYAN_SLOT_BOW,       // ITEM_BOW_LIGHT
    CYAN_SLOT_NONE,      // ITEM_SWORD_KOKIRI
    CYAN_SLOT_NONE,      // ITEM_SWORD_MASTER
    CYAN_SLOT_NONE,      // ITEM_SWORD_BIGGORON
    CYAN_SLOT_NONE,      // ITEM_SHIELD_DEKU
    CYAN_SLOT_NONE,      // ITEM_SHIELD_HYLIAN
    CYAN_SLOT_NONE,      // ITEM_SHIELD_MIRROR
    CYAN_SLOT_NONE,      // ITEM_TUNIC_KOKIRI
    CYAN_SLOT_NONE,      // ITEM_TUNIC_GORON
    CYAN_SLOT_NONE,      // ITEM_TUNIC_ZORA
    CYAN_SLOT_NONE,      // ITEM_BOOTS_KOKIRI
    CYAN_SLOT_NONE,      // ITEM_BOOTS_IRON
    CYAN_SLOT_NONE,      // ITEM_BOOTS_HOVER
    CYAN_SLOT_NONE,      // ITEM_BULLET_BAG_30
    CYAN_SLOT_NONE,      // ITEM_BULLET_BAG_40
    CYAN_SLOT_NONE,      // ITEM_BULLET_BAG_50
    CYAN_SLOT_NONE,      // ITEM_QUIVER_30
    CYAN_SLOT_NONE,      // ITEM_QUIVER_40
    CYAN_SLOT_NONE,      // ITEM_QUIVER_50
    CYAN_SLOT_NONE,      // ITEM_BOMB_BAG_20
    CYAN_SLOT_NONE,      // ITEM_BOMB_BAG_30
    CYAN_SLOT_NONE,      // ITEM_BOMB_BAG_40
    CYAN_SLOT_NONE,      // ITEM_STRENGTH_GORONS_BRACELET
    CYAN_SLOT_NONE,      // ITEM_STRENGTH_SILVER_GAUNTLETS
    CYAN_SLOT_NONE,      // ITEM_STRENGTH_GOLD_GAUNTLETS
    CYAN_SLOT_NONE,      // ITEM_SCALE_SILVER
    CYAN_SLOT_NONE,      // ITEM_SCALE_GOLDEN
    CYAN_SLOT_NONE,      // ITEM_GIANTS_KNIFE
};

//used for the save editor
u8 gCyanAmmoItems[16] = {
    ITEM_NONE, ITEM_NONE,    ITEM_NONE, ITEM_NONE, ITEM_NONE, ITEM_NONE,
    ITEM_NONE, ITEM_NUT,     ITEM_BOMB, ITEM_BOW,  ITEM_NONE, ITEM_NONE,
    ITEM_NONE, ITEM_BOMBCHU, ITEM_NONE, ITEM_NONE,
};

u8 gCyanBowSlotItems[BOW_SLOT_ITEM_COUNT] = {
    ITEM_BOW,
    ITEM_ARROW_FIRE,
    ITEM_ARROW_ICE,
    ITEM_ARROW_LIGHT,
};

u8 Cyan_FindIndex(u8 array[], int size, u8 target) {
    for (int i = 0; i < size; i++) {
        if (array[i] == target) {
            return i;
        }
    }

    return -1;
}

#define INDEX_OF(array, item) Cyan_FindIndex(array, ARRAY_COUNT(array), item)

u8 Cyan_GetPrevBowSlotItem() {
    u8 currentBowItemIndex = INDEX_OF(gCyanBowSlotItems, INV_CONTENT(ITEM_BOW));
    if (currentBowItemIndex == -1) {
        return ITEM_NONE;
    }
    for (int i = 0; i < ARRAY_COUNT(gCyanBowSlotItems); i++) {
        u8 newBowItemIndex = (currentBowItemIndex - i - 1 + ARRAY_COUNT(gCyanBowSlotItems)) % ARRAY_COUNT(gCyanBowSlotItems);
        if (gSaveContext.cyan.bowItems & (1 << newBowItemIndex)) {
            return gCyanBowSlotItems[newBowItemIndex];
        }
    }
    return ITEM_NONE;
}

u8 Cyan_GetNextBowSlotItem() {
    u8 currentBowItemIndex = INDEX_OF(gCyanBowSlotItems, INV_CONTENT(ITEM_BOW));
    if (currentBowItemIndex == -1) {
        return ITEM_NONE;
    }
    for (int i = 0; i < ARRAY_COUNT(gCyanBowSlotItems); i++) {
        u8 newBowItemIndex = (currentBowItemIndex + i + 1) % ARRAY_COUNT(gCyanBowSlotItems);
        if (gSaveContext.cyan.bowItems & (1 << newBowItemIndex)) {
            return gCyanBowSlotItems[newBowItemIndex];
        }
    }
    return ITEM_NONE;
}

void Cyan_HandleItemCycles(PlayState* play) {
    KaleidoScope_HandleItemCycleExtras(
        play,
        CYAN_SLOT_BOW,
        true,
        Cyan_GetPrevBowSlotItem(),
        Cyan_GetNextBowSlotItem(),
        false
    );
}

void Cyan_DrawItemCycles(PlayState* play) {
    KaleidoScope_DrawItemCycleExtras(
        play,
        CYAN_SLOT_BOW,
        true,
        Cyan_GetPrevBowSlotItem(),
        Cyan_GetNextBowSlotItem()
    );
}