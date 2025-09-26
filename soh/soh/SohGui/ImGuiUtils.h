#ifndef IM_GUI_UTILS_H
#define IM_GUI_UTILS_H

#pragma once

#include <imgui.h>
#include <array>
#include <map>
#include <string>
#include "soh/Enhancements/randomizer/randomizerTypes.h"
#include "variables.h" //only for gItemIcons

extern "C" {
#include "textures/icon_item_static/icon_item_static.h"
#include "textures/icon_item_24_static/icon_item_24_static.h"
#include "textures/parameter_static/parameter_static.h"
}

const char* GetTextureForItemId(uint32_t itemId);
void RegisterImGuiItemIcons();

typedef struct {
    uint32_t id;
    std::string name;
    std::string nameFaded;
    std::string texturePath;
} ItemMapEntry;

#define ITEM_MAP_ENTRY(id)                                            \
    {                                                                 \
        id, {                                                         \
            id, #id, #id "_Faded", static_cast<char*>(gItemIcons[id]) \
        }                                                             \
    }

// Maps items ids to info for use in ImGui
extern std::map<uint32_t, ItemMapEntry> itemMapping;

extern std::map<uint32_t, ItemMapEntry> gregMapping;

typedef struct {
    uint32_t id;
    std::string name;
    std::string nameFaded;
    std::string texturePath;
} QuestMapEntry;

#define QUEST_MAP_ENTRY(id, tex)       \
    {                                  \
        id, {                          \
            id, #id, #id "_Faded", tex \
        }                              \
    }

// Maps quest items ids to info for use in ImGui
extern std::map<uint32_t, QuestMapEntry> questMapping;

typedef struct {
    uint32_t id;
    std::string name;
    std::string nameFaded;
    ImVec4 color;
} SongMapEntry;

#define SONG_MAP_ENTRY(id, r, g, b)                                                 \
    {                                                                               \
        id, {                                                                       \
            id, #id, #id "_Faded", ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f) \
        }                                                                           \
    }

// Maps song ids to info for use in ImGui
extern std::map<QuestItem, SongMapEntry> songMapping;

#define VANILLA_SONG_MAP_ENTRY(id, r, g, b) \
    { id, #id "_Vanilla", #id "_Vanilla_Faded", ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f) }

// Maps song ids to info for use in ImGui
extern std::array<SongMapEntry, 12> vanillaSongMapping;

namespace ImGuiUtils {
    // Renders text with colors embeded in the text itself.
    // Colors are signified by the '§' character followed by
    // a character 0-9 or a-f to get the following colors:
    // - 0: black
    // - 1: dark_blue
    // - 2: dark_green
    // - 3: dark_aqua
    // - 4: dark_red
    // - 5: dark_purple
    // - 6: gold
    // - 7: gray
    // - 8: dark_gray
    // - 9: blue
    // - a: green
    // - b: aqua
    // - c: red
    // - d: light_purple
    // - e: yellow
    // - f: white
    // 2 '§'s in a row can be used for an unescaped '§'
    void TextColored(std::string str);
}

#endif // IM_GUI_UTILS_H