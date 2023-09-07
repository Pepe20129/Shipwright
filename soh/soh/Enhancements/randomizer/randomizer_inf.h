#pragma once

typedef enum {
    RAND_INF_DUNGEONS_DONE_DEKU_TREE,
    RAND_INF_DUNGEONS_DONE_DODONGOS_CAVERN,
    RAND_INF_DUNGEONS_DONE_JABU_JABUS_BELLY,
    RAND_INF_DUNGEONS_DONE_FOREST_TEMPLE,
    RAND_INF_DUNGEONS_DONE_FIRE_TEMPLE,
    RAND_INF_DUNGEONS_DONE_WATER_TEMPLE,
    RAND_INF_DUNGEONS_DONE_SPIRIT_TEMPLE,
    RAND_INF_DUNGEONS_DONE_SHADOW_TEMPLE,

    RAND_INF_TRIALS_DONE_LIGHT_TRIAL,
    RAND_INF_TRIALS_DONE_FOREST_TRIAL,
    RAND_INF_TRIALS_DONE_FIRE_TRIAL,
    RAND_INF_TRIALS_DONE_WATER_TRIAL,
    RAND_INF_TRIALS_DONE_SPIRIT_TRIAL,
    RAND_INF_TRIALS_DONE_SHADOW_TRIAL,

    RAND_INF_COWS_MILKED_KF_LINKS_HOUSE_COW,
    RAND_INF_COWS_MILKED_HF_COW_GROTTO_COW,
    RAND_INF_COWS_MILKED_LLR_STABLES_LEFT_COW,
    RAND_INF_COWS_MILKED_LLR_STABLES_RIGHT_COW,
    RAND_INF_COWS_MILKED_LLR_TOWER_LEFT_COW,
    RAND_INF_COWS_MILKED_LLR_TOWER_RIGHT_COW,
    RAND_INF_COWS_MILKED_KAK_IMPAS_HOUSE_COW,
    RAND_INF_COWS_MILKED_DMT_COW_GROTTO_COW,
    RAND_INF_COWS_MILKED_GV_COW,
    RAND_INF_COWS_MILKED_JABU_JABUS_BELLY_MQ_COW,

    RAND_INF_SCRUBS_PURCHASED_DODONGOS_CAVERN_DEKU_SCRUB_NEAR_BOMB_BAG_LEFT,
    RAND_INF_SCRUBS_PURCHASED_DODONGOS_CAVERN_DEKU_SCRUB_SIDE_ROOM_NEAR_DODONGOS,
    RAND_INF_SCRUBS_PURCHASED_DODONGOS_CAVERN_DEKU_SCRUB_NEAR_BOMB_BAG_RIGHT,
    RAND_INF_SCRUBS_PURCHASED_DODONGOS_CAVERN_DEKU_SCRUB_LOBBY,
    RAND_INF_SCRUBS_PURCHASED_JABU_JABUS_BELLY_DEKU_SCRUB,
    RAND_INF_SCRUBS_PURCHASED_GANONS_CASTLE_DEKU_SCRUB_CENTER_LEFT,
    RAND_INF_SCRUBS_PURCHASED_GANONS_CASTLE_DEKU_SCRUB_CENTER_RIGHT,
    RAND_INF_SCRUBS_PURCHASED_GANONS_CASTLE_DEKU_SCRUB_RIGHT,
    RAND_INF_SCRUBS_PURCHASED_GANONS_CASTLE_DEKU_SCRUB_LEFT,
    RAND_INF_SCRUBS_PURCHASED_HF_DEKU_SCRUB_GROTTO,
    RAND_INF_SCRUBS_PURCHASED_ZR_DEKU_SCRUB_GROTTO_REAR,
    RAND_INF_SCRUBS_PURCHASED_ZR_DEKU_SCRUB_GROTTO_FRONT,
    RAND_INF_SCRUBS_PURCHASED_SFM_DEKU_SCRUB_GROTTO_REAR,
    RAND_INF_SCRUBS_PURCHASED_SFM_DEKU_SCRUB_GROTTO_FRONT,
    RAND_INF_SCRUBS_PURCHASED_LH_DEKU_SCRUB_GROTTO_LEFT,
    RAND_INF_SCRUBS_PURCHASED_LH_DEKU_SCRUB_GROTTO_RIGHT,
    RAND_INF_SCRUBS_PURCHASED_LH_DEKU_SCRUB_GROTTO_CENTER,
    RAND_INF_SCRUBS_PURCHASED_GV_DEKU_SCRUB_GROTTO_REAR,
    RAND_INF_SCRUBS_PURCHASED_GV_DEKU_SCRUB_GROTTO_FRONT,
    RAND_INF_SCRUBS_PURCHASED_LW_DEKU_SCRUB_GROTTO_REAR,
    RAND_INF_SCRUBS_PURCHASED_LW_DEKU_SCRUB_GROTTO_FRONT,
    RAND_INF_SCRUBS_PURCHASED_DMC_DEKU_SCRUB_GROTTO_LEFT,
    RAND_INF_SCRUBS_PURCHASED_DMC_DEKU_SCRUB_GROTTO_RIGHT,
    RAND_INF_SCRUBS_PURCHASED_DMC_DEKU_SCRUB_GROTTO_CENTER,
    RAND_INF_SCRUBS_PURCHASED_GC_DEKU_SCRUB_GROTTO_LEFT,
    RAND_INF_SCRUBS_PURCHASED_GC_DEKU_SCRUB_GROTTO_RIGHT,
    RAND_INF_SCRUBS_PURCHASED_GC_DEKU_SCRUB_GROTTO_CENTER,
    RAND_INF_SCRUBS_PURCHASED_LLR_DEKU_SCRUB_GROTTO_LEFT,
    RAND_INF_SCRUBS_PURCHASED_LLR_DEKU_SCRUB_GROTTO_RIGHT,
    RAND_INF_SCRUBS_PURCHASED_LLR_DEKU_SCRUB_GROTTO_CENTER,
    RAND_INF_SCRUBS_PURCHASED_COLOSSUS_DEKU_SCRUB_GROTTO_REAR,
    RAND_INF_SCRUBS_PURCHASED_COLOSSUS_DEKU_SCRUB_GROTTO_FRONT,
    RAND_INF_SCRUBS_PURCHASED_LW_DEKU_SCRUB_NEAR_DEKU_THEATER_RIGHT,
    RAND_INF_SCRUBS_PURCHASED_LW_DEKU_SCRUB_NEAR_DEKU_THEATER_LEFT,
    RAND_INF_SCRUBS_PURCHASED_LW_DEKU_SCRUB_NEAR_BRIDGE,
    RAND_INF_SCRUBS_PURCHASED_DMC_DEKU_SCRUB,
    RAND_INF_SCRUBS_PURCHASED_DEKU_TREE_MQ_DEKU_SCRUB,
    RAND_INF_SCRUBS_PURCHASED_DODONGOS_CAVERN_MQ_DEKU_SCRUB_LOBBY_REAR,
    RAND_INF_SCRUBS_PURCHASED_DODONGOS_CAVERN_MQ_DEKU_SCRUB_LOBBY_FRONT,
    RAND_INF_SCRUBS_PURCHASED_DODONGOS_CAVERN_MQ_DEKU_SCRUB_STAIRCASE,
    RAND_INF_SCRUBS_PURCHASED_DODONGOS_CAVERN_MQ_DEKU_SCRUB_SIDE_ROOM_NEAR_LOWER_LIZALFOS,
    RAND_INF_SCRUBS_PURCHASED_GANONS_CASTLE_MQ_DEKU_SCRUB_RIGHT,
    RAND_INF_SCRUBS_PURCHASED_GANONS_CASTLE_MQ_DEKU_SCRUB_CENTER_LEFT,
    RAND_INF_SCRUBS_PURCHASED_GANONS_CASTLE_MQ_DEKU_SCRUB_CENTER,
    RAND_INF_SCRUBS_PURCHASED_GANONS_CASTLE_MQ_DEKU_SCRUB_CENTER_RIGHT,
    RAND_INF_SCRUBS_PURCHASED_GANONS_CASTLE_MQ_DEKU_SCRUB_LEFT,

    RAND_INF_SHOP_ITEMS_KF_SHOP_ITEM_1,
    RAND_INF_SHOP_ITEMS_KF_SHOP_ITEM_2,
    RAND_INF_SHOP_ITEMS_KF_SHOP_ITEM_3,
    RAND_INF_SHOP_ITEMS_KF_SHOP_ITEM_4,
    RAND_INF_SHOP_ITEMS_KF_SHOP_ITEM_5,
    RAND_INF_SHOP_ITEMS_KF_SHOP_ITEM_6,
    RAND_INF_SHOP_ITEMS_KF_SHOP_ITEM_7,
    RAND_INF_SHOP_ITEMS_KF_SHOP_ITEM_8,
    RAND_INF_SHOP_ITEMS_GC_SHOP_ITEM_1,
    RAND_INF_SHOP_ITEMS_GC_SHOP_ITEM_2,
    RAND_INF_SHOP_ITEMS_GC_SHOP_ITEM_3,
    RAND_INF_SHOP_ITEMS_GC_SHOP_ITEM_4,
    RAND_INF_SHOP_ITEMS_GC_SHOP_ITEM_5,
    RAND_INF_SHOP_ITEMS_GC_SHOP_ITEM_6,
    RAND_INF_SHOP_ITEMS_GC_SHOP_ITEM_7,
    RAND_INF_SHOP_ITEMS_GC_SHOP_ITEM_8,
    RAND_INF_SHOP_ITEMS_ZD_SHOP_ITEM_1,
    RAND_INF_SHOP_ITEMS_ZD_SHOP_ITEM_2,
    RAND_INF_SHOP_ITEMS_ZD_SHOP_ITEM_3,
    RAND_INF_SHOP_ITEMS_ZD_SHOP_ITEM_4,
    RAND_INF_SHOP_ITEMS_ZD_SHOP_ITEM_5,
    RAND_INF_SHOP_ITEMS_ZD_SHOP_ITEM_6,
    RAND_INF_SHOP_ITEMS_ZD_SHOP_ITEM_7,
    RAND_INF_SHOP_ITEMS_ZD_SHOP_ITEM_8,
    RAND_INF_SHOP_ITEMS_KAK_BAZAAR_ITEM_1,
    RAND_INF_SHOP_ITEMS_KAK_BAZAAR_ITEM_2,
    RAND_INF_SHOP_ITEMS_KAK_BAZAAR_ITEM_3,
    RAND_INF_SHOP_ITEMS_KAK_BAZAAR_ITEM_4,
    RAND_INF_SHOP_ITEMS_KAK_BAZAAR_ITEM_5,
    RAND_INF_SHOP_ITEMS_KAK_BAZAAR_ITEM_6,
    RAND_INF_SHOP_ITEMS_KAK_BAZAAR_ITEM_7,
    RAND_INF_SHOP_ITEMS_KAK_BAZAAR_ITEM_8,
    RAND_INF_SHOP_ITEMS_KAK_POTION_SHOP_ITEM_1,
    RAND_INF_SHOP_ITEMS_KAK_POTION_SHOP_ITEM_2,
    RAND_INF_SHOP_ITEMS_KAK_POTION_SHOP_ITEM_3,
    RAND_INF_SHOP_ITEMS_KAK_POTION_SHOP_ITEM_4,
    RAND_INF_SHOP_ITEMS_KAK_POTION_SHOP_ITEM_5,
    RAND_INF_SHOP_ITEMS_KAK_POTION_SHOP_ITEM_6,
    RAND_INF_SHOP_ITEMS_KAK_POTION_SHOP_ITEM_7,
    RAND_INF_SHOP_ITEMS_KAK_POTION_SHOP_ITEM_8,
    RAND_INF_SHOP_ITEMS_MARKET_BAZAAR_ITEM_1,
    RAND_INF_SHOP_ITEMS_MARKET_BAZAAR_ITEM_2,
    RAND_INF_SHOP_ITEMS_MARKET_BAZAAR_ITEM_3,
    RAND_INF_SHOP_ITEMS_MARKET_BAZAAR_ITEM_4,
    RAND_INF_SHOP_ITEMS_MARKET_BAZAAR_ITEM_5,
    RAND_INF_SHOP_ITEMS_MARKET_BAZAAR_ITEM_6,
    RAND_INF_SHOP_ITEMS_MARKET_BAZAAR_ITEM_7,
    RAND_INF_SHOP_ITEMS_MARKET_BAZAAR_ITEM_8,
    RAND_INF_SHOP_ITEMS_MARKET_POTION_SHOP_ITEM_1,
    RAND_INF_SHOP_ITEMS_MARKET_POTION_SHOP_ITEM_2,
    RAND_INF_SHOP_ITEMS_MARKET_POTION_SHOP_ITEM_3,
    RAND_INF_SHOP_ITEMS_MARKET_POTION_SHOP_ITEM_4,
    RAND_INF_SHOP_ITEMS_MARKET_POTION_SHOP_ITEM_5,
    RAND_INF_SHOP_ITEMS_MARKET_POTION_SHOP_ITEM_6,
    RAND_INF_SHOP_ITEMS_MARKET_POTION_SHOP_ITEM_7,
    RAND_INF_SHOP_ITEMS_MARKET_POTION_SHOP_ITEM_8,
    RAND_INF_SHOP_ITEMS_MARKET_BOMBCHU_SHOP_ITEM_1,
    RAND_INF_SHOP_ITEMS_MARKET_BOMBCHU_SHOP_ITEM_2,
    RAND_INF_SHOP_ITEMS_MARKET_BOMBCHU_SHOP_ITEM_3,
    RAND_INF_SHOP_ITEMS_MARKET_BOMBCHU_SHOP_ITEM_4,
    RAND_INF_SHOP_ITEMS_MARKET_BOMBCHU_SHOP_ITEM_5,
    RAND_INF_SHOP_ITEMS_MARKET_BOMBCHU_SHOP_ITEM_6,
    RAND_INF_SHOP_ITEMS_MARKET_BOMBCHU_SHOP_ITEM_7,
    RAND_INF_SHOP_ITEMS_MARKET_BOMBCHU_SHOP_ITEM_8,

    RAND_INF_MERCHANTS_CARPET_SALESMAN,
    RAND_INF_MERCHANTS_MEDIGORON,
    RAND_INF_MERCHANTS_GRANNYS_SHOP,

    RAND_INF_ADULT_TRADES_LW_TRADE_COJIRO,
    RAND_INF_ADULT_TRADES_GV_TRADE_SAW,
    RAND_INF_ADULT_TRADES_DMT_TRADE_BROKEN_SWORD,
    RAND_INF_ADULT_TRADES_LH_TRADE_FROG,
    RAND_INF_ADULT_TRADES_DMT_TRADE_EYEDROPS,

    RAND_INF_KAK_100_GOLD_SKULLTULA_REWARD,

    RAND_INF_GREG_FOUND,

    RAND_INF_TRIAL_SHUFFLE_LIGHT_TRIAL,
    RAND_INF_TRIAL_SHUFFLE_FOREST_TRIAL,
    RAND_INF_TRIAL_SHUFFLE_FIRE_TRIAL,
    RAND_INF_TRIAL_SHUFFLE_WATER_TRIAL,
    RAND_INF_TRIAL_SHUFFLE_SPIRIT_TRIAL,
    RAND_INF_TRIAL_SHUFFLE_SHADOW_TRIAL,

    // If you add anything to this list, you need to update the size of randomizerInf in z64save.h to be ceil(RAND_INF_MAX / 16)

    RAND_INF_MAX,
} RandomizerInf;
