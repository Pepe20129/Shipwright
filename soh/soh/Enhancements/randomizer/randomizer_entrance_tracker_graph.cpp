#include "randomizer_entrance_tracker_graph.h"

#include <libultraship/bridge/consolevariablebridge.h>
#include "soh/SohGui/SohGui.hpp"
#include "entrance.h"
#include "location_access.h"
#include "randomizer_entrance_tracker.h"
#include "randomizerEnumStrings.h"
#include "randomizerTypes.h"

const float initialSize = 2500.0f;

void EntranceTrackerGraphWindow::Draw() {
    if (!IsVisible()) {
        return;
    }
    DrawElement();
    // Sync up the IsVisible flag if it was changed by ImGui
    SyncVisibilityConsoleVariable();
}

const GraphOptions defaultOptions = {
    {
        0.05f,
        5.0f,
        true,
        true,
    },
    {
        1.0f,
        10.0f,
    },
    {
        80.0f,
        5.0f,
        30.0f,
    },
    {
        20.0f,
        {
            { 3.0f, 2.0f },
            3.0f,
            IM_COL32(48, 48, 64, 220),
            true,
            0.75f,
            1.0f,
        },
    },
    {
        2.0f,
        {
            0.0f,
            { 3.0f, 2.0f },
            3.0f,
            IM_COL32(48, 48, 64, 220),
            true,
            2.0f,
            3.0f,
        },
    },
};

void EntranceTrackerGraphWindow::DrawElement() {
    if (!this->graph.has_value()) {
        LUSLOG_ERROR("[EntranceTrackerGraphWindow::DrawElement] this->graph is std::nullopt");
        return;
    }

    Color_RGBA8 bgColor = { 0, 0, 0, 255 };
    if (Trackers::BeginFloatWindows("Entrance Tracker Graph", mIsVisible, bgColor, TRACKER_WINDOW_WINDOW, true, ImGuiWindowFlags_NoScrollbar)) {
        ImVec2 canvasPos = ImGui::GetCursorScreenPos();
        ImVec2 canvasSize = ImGui::GetContentRegionAvail();

        if (!this->sufficientlyStabilized) {
            float displacement = 0;
            displacement += this->graph.value().StabilizeStep(initialSize * 2.0f, initialSize * 2.0f, 10.0f);
            displacement += this->graph.value().StabilizeStep(initialSize * 2.0f, initialSize * 2.0f, 10.0f);

            LUSLOG_INFO("[EntranceTrackerGraphWindow::DrawElement] Displacement = %f", displacement);

            if (displacement < 28000) {
                this->sufficientlyStabilized = true;
            }
        }

        ImGui::SetNextItemAllowOverlap();

        this->graph.value().Draw(canvasSize, canvasPos);

        ImGui::SetCursorScreenPos(canvasPos);

        ImGui::SetNextItemAllowOverlap();

        ImGui::BeginGroup();

        if (menuOpen) {
            if (ImGui::Button(ICON_FA_COG)) {
                menuOpen = false;
            }

            if (ImGui::Button("Reset View")) {
                this->graph.value().ResetView();
            }

            #define TEST_FLOAT(option_path, text, min, max)                                                                   \
            if (                                                                                                              \
                CVarSliderFloat(                                                                                              \
                    text,                                                                                                     \
                    CVAR_TRACKER_ENTRANCE("Graph." #option_path),                                                             \
                    UIWidgets::FloatSliderOptions()                                                                           \
                        .Min(min)                                                                                             \
                        .Max(max)                                                                                             \
                        .DefaultValue(CVarGetFloat(CVAR_TRACKER_ENTRANCE("Graph." #option_path), defaultOptions.option_path)) \
                        .Format("%.1f")                                                                                       \
                        .Size({ 300.0f, 0.0f })                                                                               \
                        .Step(0.1f)                                                                                           \
                        .Color(THEME_COLOR)                                                                                   \
                )                                                                                                             \
            ) {                                                                                                               \
                this->UpdateGraphOptions();                                                                                   \
            }

            TEST_FLOAT(forceMultipliers.repulsion, "Repulsion Force", 1.0f, 50.0f);
            TEST_FLOAT(forceMultipliers.attraction, "Attraction Force", 1.0f, 50.0f);

            TEST_FLOAT(temperature.starting, "Starting Temperature", 10.0f, 1000.0f);
            TEST_FLOAT(temperature.decreasePerIteration, "Temperature Decrease Per Iteration", 0.01f, 10.0f);

            TEST_FLOAT(edges.thickness, "Edge Thickness", 1.0f, 10.0f);

        } else {
            if (ImGui::Button(ICON_FA_COG)) {
                menuOpen = true;
            }
        }

        ImGui::EndGroup();
    }
    Trackers::EndFloatWindows();
}

ImU32 GetColorForArea(RandomizerArea area) {
    switch (area) {
        case RA_NONE:
        case RA_LINKS_POCKET:
            return IM_COL32(0xD8, 0xD2, 0xE8, 0xFF);
        case RA_KOKIRI_FOREST:
            return IM_COL32(0xB8, 0xE3, 0xC2, 0xFF);
        case RA_THE_LOST_WOODS :
            return IM_COL32(0xAF, 0xCF, 0xA8, 0xFF);
        case RA_SACRED_FOREST_MEADOW:
            return IM_COL32(0xCB, 0xE8, 0xB5, 0xFF);
        case RA_HYRULE_FIELD:
            return IM_COL32(0xDD, 0xE8, 0xA8, 0xFF);
        case RA_LAKE_HYLIA :
            return IM_COL32(0xA9, 0xDC, 0xE3, 0xFF);
        case RA_GERUDO_VALLEY:
            return IM_COL32(0xF2, 0xC6, 0xA0, 0xFF);
        case RA_GERUDO_FORTRESS:
            return IM_COL32(0xE8, 0xB3, 0xA7, 0xFF);
        case RA_HAUNTED_WASTELAND:
            return IM_COL32(0xC9, 0xB2, 0xC8, 0xFF);
        case RA_DESERT_COLOSSUS:
            return IM_COL32(0xEB, 0xCB, 0x8B, 0xFF);
        case RA_THE_MARKET :
            return IM_COL32(0xF4, 0xB7, 0xC5, 0xFF);
        case RA_TEMPLE_OF_TIME :
            return IM_COL32(0xED, 0xE2, 0xC6, 0xFF);
        case RA_HYRULE_CASTLE:
            return IM_COL32(0xB9, 0xC1, 0xE8, 0xFF);
        case RA_OUTSIDE_GANONS_CASTLE:
            return IM_COL32(0xA8, 0x9B, 0xBE, 0xFF);
        case RA_CASTLE_GROUNDS :
            return IM_COL32(0xB8, 0xCB, 0xE8, 0xFF);
        case RA_KAKARIKO_VILLAGE:
            return IM_COL32(0xE7, 0xB2, 0x8D, 0xFF);
        case RA_THE_GRAVEYARD:
            return IM_COL32(0xAA, 0xA0, 0xBA, 0xFF);
        case RA_DEATH_MOUNTAIN_TRAIL:
            return IM_COL32(0xE8, 0xA9, 0x8F, 0xFF);
        case RA_GORON_CITY:
            return IM_COL32(0xF0, 0x8F, 0x78, 0xFF);
        case RA_DEATH_MOUNTAIN_CRATER:
            return IM_COL32(0xF4, 0xB0, 0x8A, 0xFF);
        case RA_ZORAS_RIVER:
            return IM_COL32(0x9F, 0xD8, 0xD2, 0xFF);
        case RA_ZORAS_DOMAIN:
            return IM_COL32(0xA8, 0xD7, 0xE8, 0xFF);
        case RA_ZORAS_FOUNTAIN:
            return IM_COL32(0xC1, 0xE8, 0xE5, 0xFF);
        case RA_LON_LON_RANCH:
            return IM_COL32(0xF4, 0xE1, 0xA1, 0xFF);
        case RA_DEKU_TREE:
            return IM_COL32(0xA9, 0xD6, 0xA0, 0xFF);
        case RA_DODONGOS_CAVERN:
            return IM_COL32(0xC9, 0x9F, 0x88, 0xFF);
        case RA_JABU_JABUS_BELLY:
            return IM_COL32(0xEF, 0xAF, 0xC0, 0xFF);
        case RA_FOREST_TEMPLE:
            return IM_COL32(0xA9, 0xC8, 0xB5, 0xFF);
        case RA_FIRE_TEMPLE:
            return IM_COL32(0xE9, 0x96, 0x7A, 0xFF);
        case RA_WATER_TEMPLE:
            return IM_COL32(0x91, 0xC8, 0xD8, 0xFF);
        case RA_SPIRIT_TEMPLE:
            return IM_COL32(0xE8, 0xC2, 0x9A, 0xFF);
        case RA_SHADOW_TEMPLE:
            return IM_COL32(0xAA, 0xA6, 0xB8, 0xFF);
        case RA_BOTTOM_OF_THE_WELL:
            return IM_COL32(0xB5, 0xB0, 0xA8, 0xFF);
        case RA_ICE_CAVERN:
            return IM_COL32(0xB8, 0xDF, 0xF0, 0xFF);
        case RA_GERUDO_TRAINING_GROUND:
            return IM_COL32(0xD7, 0xA0, 0x8D, 0xFF);
        case RA_GANONS_CASTLE:
            return IM_COL32(0x92, 0x7C, 0x9F, 0xFF);
        default:
            LUSLOG_ERROR("[GetColorForArea] Invalid area (%d)", static_cast<int>(area));
            assert(false);
            return IM_COL32(0xD8, 0xD2, 0xE8, 0xFF);
    }
}

ImU32 GetColorForAreas(std::set<RandomizerArea> areas) {
    switch (areas.size()) {
        case 0:
            return IM_COL32(0xD8, 0xD2, 0xE8, 0xFF);
        case 1:
            return GetColorForArea(*areas.begin());
        default:
            // for now we just pick a random one (sets aren't ordered)
            return GetColorForArea(*areas.begin());
    }
}

uint64_t randoState = 0;

void EntranceTrackerGraphWindow::InitElement() {
    ImU32 labelColor = IM_COL32_WHITE;
    ImU32 edgeColor = IM_COL32_WHITE;
    std::vector<Node> nodes = {};

    std::vector<Edge> edges = {};

    RegionTable_Init();

    for (const auto& region : areaTable) {
        if (region.randomizerRegionKey == RR_NONE) {
            continue;
        }

        std::optional<std::string_view> regionString = EnumToString(region.randomizerRegionKey);

        if (!regionString.has_value()) {
            regionString = "???";
        }

        // random starting positions to allow the forces to move them
        nodes.push_back(Node::New({ static_cast<float>(ShipUtils::next32(&randoState)) / INT32_MAX * initialSize - initialSize / 2, static_cast<float>(ShipUtils::next32(&randoState)) / INT32_MAX * initialSize - initialSize / 2 }, std::string(regionString.value()), GetColorForAreas(region.areas), labelColor));
    }

    for (const auto& region : areaTable) {
        for (const auto& exit : region.exits) {
            // -1 due to skipping RR_NONE
            edges.push_back(Edge::New(exit.GetParentRegionKey() - 1, exit.GetConnectedRegionKey() - 1, exit.GetConditionStr(), edgeColor, labelColor));
        }
    }

    this->graph = Graph::New(nodes, edges, defaultOptions);

    this->UpdateGraphOptions();

    this->graph.value().Stabilize(initialSize * 2.0f, initialSize * 2.0f);
}

void EntranceTrackerGraphWindow::UpdateGraphOptions() {
    if (!this->graph.has_value()) {
        return;
    }

    GraphOptions& currentGraphOptions = this->graph.value().GetOptions();

    #define UPDATE_OPTION_PATH_SIMPLE(option_path, type) currentGraphOptions.option_path = CVarGet ## type (CVAR_TRACKER_ENTRANCE("Graph." #option_path), defaultOptions.option_path)
    #define UPDATE_OPTION_PATH_INT(option_path) UPDATE_OPTION_PATH_SIMPLE(option_path, Integer)
    #define UPDATE_OPTION_PATH_FLOAT(option_path) UPDATE_OPTION_PATH_SIMPLE(option_path, Float)
    #define UPDATE_OPTION_PATH_IMVEC2(option_path) currentGraphOptions.option_path = {                 \
        CVarGetFloat(CVAR_TRACKER_ENTRANCE("Graph." #option_path ".x"), defaultOptions.option_path.x), \
        CVarGetFloat(CVAR_TRACKER_ENTRANCE("Graph." #option_path ".y"), defaultOptions.option_path.y), \
    }
    #define UPDATE_OPTION_PATH_IMU32(option_path) currentGraphOptions.option_path = static_cast<ImU32>(CVarGetInteger(CVAR_TRACKER_ENTRANCE("Graph." #option_path), defaultOptions.option_path))

    UPDATE_OPTION_PATH_FLOAT(zoom.min);
    UPDATE_OPTION_PATH_FLOAT(zoom.max);
    UPDATE_OPTION_PATH_INT(zoom.nodesScaleWithZoom);
    UPDATE_OPTION_PATH_INT(zoom.edgesScaleWithZoom);

    UPDATE_OPTION_PATH_FLOAT(forceMultipliers.repulsion);
    UPDATE_OPTION_PATH_FLOAT(forceMultipliers.attraction);

    UPDATE_OPTION_PATH_FLOAT(temperature.starting);
    UPDATE_OPTION_PATH_FLOAT(temperature.decreasePerIteration);
    UPDATE_OPTION_PATH_FLOAT(temperature.ending);

    UPDATE_OPTION_PATH_FLOAT(nodes.baseSize);
    UPDATE_OPTION_PATH_IMVEC2(nodes.label.padding);
    UPDATE_OPTION_PATH_FLOAT(nodes.label.rounding);
    UPDATE_OPTION_PATH_IMU32(nodes.label.backgroundColor);
    UPDATE_OPTION_PATH_INT(nodes.label.fadeout);
    UPDATE_OPTION_PATH_FLOAT(nodes.label.fadeoutCutoffLower);
    UPDATE_OPTION_PATH_FLOAT(nodes.label.fadeoutCutoffUpper);

    UPDATE_OPTION_PATH_FLOAT(edges.thickness);
    UPDATE_OPTION_PATH_FLOAT(edges.label.separation);
    UPDATE_OPTION_PATH_IMVEC2(edges.label.padding);
    UPDATE_OPTION_PATH_FLOAT(edges.label.rounding);
    UPDATE_OPTION_PATH_IMU32(edges.label.backgroundColor);
    UPDATE_OPTION_PATH_INT(edges.label.fadeout);
    UPDATE_OPTION_PATH_FLOAT(edges.label.fadeoutCutoffLower);
    UPDATE_OPTION_PATH_FLOAT(edges.label.fadeoutCutoffUpper);

    #undef UPDATE_OPTION_PATH_SIMPLE
    #undef UPDATE_OPTION_PATH_INT
    #undef UPDATE_OPTION_PATH_FLOAT
    #undef UPDATE_OPTION_PATH_IMVEC2
    #undef UPDATE_OPTION_PATH_IMU32

    this->sufficientlyStabilized = false;
}