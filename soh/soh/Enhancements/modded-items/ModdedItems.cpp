#include "ModdedItems.h"
#include "textures/icon_item_static/icon_item_static.h"
#include "textures/icon_item_24_static/icon_item_24_static.h"
#include "textures/parameter_static/parameter_static.h"
#include "textures/item_name_static/item_name_static.h"
#include "functions.h"
#include "z64save.h"
#include "variables.h"

struct ModdedItemData {
	ModdedItemActionFunc itemAction;
	std::string itemIcon;
	std::string itemNameTexture;
	std::string itemName;
	std::function<s32(void)> currentAmmoGetter;
	std::function<s32(void)> maxAmmoGetter;
	s32 itemAgeRequirement;
};

static std::map<ModdedItem, ModdedItemData> moddedItems;

bool ModdedItems_RegisterModdedItem(ModdedItem moddedItem, ModdedItemActionFunc itemAction, std::string itemIcon, std::string itemNameTexture, std::string itemName, std::function<s32(void)> currentAmmoGetter, std::function<s32(void)> maxAmmoGetter, s32 itemAgeRequirement) {
	if (moddedItem.modId == 0) {
		//can't register a new vanilla item
		return false;
	}

	if (moddedItems.contains(moddedItem))  {
		//can't overwrite an existing item
		return false;
	}

	ModdedItemData moddedItemData = { itemAction, itemIcon, itemNameTexture, itemName, currentAmmoGetter, maxAmmoGetter, itemAgeRequirement };

	moddedItems.insert({ moddedItem, moddedItemData });

	return true;
}

std::vector<ModdedItem> ModdedItems_GetRegisteredModdedItems() {
	std::vector<ModdedItem> items;
	for (std::map<ModdedItem, ModdedItemData>::iterator itr = moddedItems.begin(); itr != moddedItems.end(); ++itr) {
		items.push_back(itr->first);
	}

	return items;
}

void ModdedItems_ExecuteModdedItemAction(PlayState* play, Player* player, ModdedItem moddedItem) {
	if (moddedItems.contains(moddedItem)) {
		moddedItems[moddedItem].itemAction(play, player, moddedItem);
	}
}

void* ModdedItems_GetModdedItemIcon(ModdedItem moddedItem) {
	if (moddedItems.contains(moddedItem)) {
		return (void*)moddedItems[moddedItem].itemIcon.c_str();
	}

	//fallback
	return (void*)gItemIconSoldOutTex;
}

const char* ModdedItems_GetModdedItemName(ModdedItem moddedItem) {
	if (moddedItems.contains(moddedItem)) {
		return moddedItems[moddedItem].itemName.c_str();
	}

	return "[Name not found]";
}

const char* ModdedItems_GetModdedItemNameTexture(ModdedItem moddedItem, s32 language) {
	if (moddedItems.contains(moddedItem)) {
		return moddedItems[moddedItem].itemNameTexture.c_str();
	}

	//fallback
	return gDekuStickItemNameENGTex;
}

s32 ModdedItems_GetCurrentAmmo(ModdedItem moddedItem) {
	if (moddedItems.contains(moddedItem)) {
		return moddedItems[moddedItem].currentAmmoGetter();
	}

	//fallback
	return 0;
}

s32 ModdedItems_GetMaxAmmo(ModdedItem moddedItem) {
	if (moddedItems.contains(moddedItem)) {
		return moddedItems[moddedItem].maxAmmoGetter();
	}

	//fallback
	return -1;
}

s32 ModdedItems_GetItemAgeRequirement(ModdedItem moddedItem) {
	if (moddedItems.contains(moddedItem)) {
		return moddedItems[moddedItem].itemAgeRequirement;
	}

	//fallback
	return 9;
}