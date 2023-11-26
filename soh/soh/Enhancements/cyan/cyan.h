#ifndef CYAN_H
#define CYAN_H

typedef enum {
    /* 0x00 */ CYAN_SLOT_EMPTY_1,
    /* 0x01 */ CYAN_SLOT_EMPTY_2,
    /* 0x02 */ CYAN_SLOT_EMPTY_3,
    /* 0x03 */ CYAN_SLOT_EMPTY_4,
    /* 0x04 */ CYAN_SLOT_EMPTY_5,
    /* 0x05 */ CYAN_SLOT_EMPTY_6,

    /* 0x06 */ CYAN_SLOT_EMPTY_7,
    /* 0x07 */ CYAN_SLOT_NUT,
    /* 0x08 */ CYAN_SLOT_BOMB,
    /* 0x09 */ CYAN_SLOT_BOW,
    /* 0x0A */ CYAN_SLOT_OCARINA,
    /* 0x0B */ CYAN_SLOT_EMPTY_8,

    /* 0x0C */ CYAN_SLOT_EMPTY_9,
    /* 0x0D */ CYAN_SLOT_BOMBCHU,
    /* 0x0E */ CYAN_SLOT_HOOKSHOT,
    /* 0x0F */ CYAN_SLOT_BOOMERANG,
    /* 0x10 */ CYAN_SLOT_HAMMER,
    /* 0x11 */ CYAN_SLOT_EMPTY_10,

    /* 0x12 */ CYAN_SLOT_EMPTY_11,
    /* 0x13 */ CYAN_SLOT_EMPTY_12,
    /* 0x14 */ CYAN_SLOT_BOTTLE,
    /* 0x15 */ CYAN_SLOT_MISC,
    /* 0x16 */ CYAN_SLOT_EMPTY_13,
    /* 0x17 */ CYAN_SLOT_EMPTY_14,

    /* 0x18 */ CYAN_SLOT_TUNIC_KOKIRI,
    /* 0x19 */ CYAN_SLOT_TUNIC_GORON,
    /* 0x1A */ CYAN_SLOT_TUNIC_ZORA,
    /* 0x1B */ CYAN_SLOT_BOOTS_KOKIRI,
    /* 0x1C */ CYAN_SLOT_BOOTS_IRON,
    /* 0x1D */ CYAN_SLOT_BOOTS_HOVER,
    /* 0xFF */ CYAN_SLOT_NONE = 0xFF
} CyanInventorySlot;

extern u8 gCyanSlotAgeReqs[24];

extern u8 gCyanItemAgeReqs[86];

extern u8 gCyanItemSlots[86];

extern u8 gCyanAmmoItems[16];

extern s16 sCyanAmmoVtxOffset[15];

extern s16 cyan_D_8082B11C[4];

#endif // CYAN_H