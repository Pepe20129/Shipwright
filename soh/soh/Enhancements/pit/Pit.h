#pragma once

#include "src/overlays/actors/ovl_En_GirlA/z_en_girla.h"
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
void Pit_OnActorKill(Actor*);
s32 Pit_Shop_ChangeItem(EnGirlA*);
s32 Pit_Shop_CanBuy(PlayState*, EnGirlA*);
void Pit_Shop_ItemGive(PlayState*, EnGirlA*);
void Pit_Shop_BuyEvent(PlayState*, EnGirlA*);
void Pit_Shop_InitItem(EnGirlA*);
void Pit_Shop_InitializeItemAction(EnGirlA*);
void Pit_Shop_ItemDraw(EnGirlA*);
#ifdef __cplusplus
};
#endif