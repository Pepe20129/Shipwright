#pragma once

#include "src/overlays/actors/ovl_En_GirlA/z_en_girla.h"
#include "src/overlays/actors/ovl_En_Box/z_en_box.h"
#include "variables.h"

#ifdef __cplusplus
extern "C" {
#endif
void Pit_InitSave();
void Pit_SetEquipment(uint8_t linkAge);
void Pit_InitCurrentFloor();
void Pit_Update();
void Pit_GoToNextFloor();
void Pit_SetEntrace();
void Pit_HandleBlueWarp(PlayState*);
void Pit_OnEnemyDefeat(Actor*);
void Pit_OnActorKill(Actor*);
s32 Pit_Shop_ChangeItem(EnGirlA*);
s32 Pit_Shop_CanBuy(PlayState*, EnGirlA*);
void Pit_Shop_ItemGive(PlayState*, EnGirlA*);
void Pit_Shop_BuyEvent(PlayState*, EnGirlA*);
void Pit_Shop_InitItem(EnGirlA*);
void Pit_Shop_InitializeItemAction(EnGirlA*);
void Pit_Shop_ItemDraw(EnGirlA*);
GetItemEntry Pit_OpenChest(EnBox*);
void Pit_SetUpChest(EnBox*);
#ifdef __cplusplus
};
#endif

#ifdef __cplusplus
#include "../custom-message/CustomMessageManager.h"
CustomMessage Pit_Shop_GetItemMessage(u16);
CustomMessage Pit_Shop_GetItemConfirmMessage(u16);
#endif