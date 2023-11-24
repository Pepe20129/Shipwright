#include "spdlog/spdlog.h"
#include <libultraship/libultraship.h>
#include "soh/resource/importer/SceneFactory.h"
#include "soh/resource/type/Scene.h"
#include "soh/resource/type/scenecommand/SceneCommand.h"
#include "soh/resource/importer/scenecommand/SetLightingSettingsFactory.h"
#include "soh/resource/importer/scenecommand/SetWindSettingsFactory.h"
#include "soh/resource/importer/scenecommand/SetExitListFactory.h"
#include "soh/resource/importer/scenecommand/SetTimeSettingsFactory.h"
#include "soh/resource/importer/scenecommand/SetSkyboxModifierFactory.h"
#include "soh/resource/importer/scenecommand/SetEchoSettingsFactory.h"
#include "soh/resource/importer/scenecommand/SetSoundSettingsFactory.h"
#include "soh/resource/importer/scenecommand/SetSkyboxSettingsFactory.h"
#include "soh/resource/importer/scenecommand/SetRoomBehaviorFactory.h"
#include "soh/resource/importer/scenecommand/SetCsCameraFactory.h"
#include "soh/resource/importer/scenecommand/SetCameraSettingsFactory.h"
#include "soh/resource/importer/scenecommand/SetRoomListFactory.h"
#include "soh/resource/importer/scenecommand/SetCollisionHeaderFactory.h"
#include "soh/resource/importer/scenecommand/SetEntranceListFactory.h"
#include "soh/resource/importer/scenecommand/SetSpecialObjectsFactory.h"
#include "soh/resource/importer/scenecommand/SetObjectListFactory.h"
#include "soh/resource/importer/scenecommand/SetStartPositionListFactory.h"
#include "soh/resource/importer/scenecommand/SetActorListFactory.h"
#include "soh/resource/importer/scenecommand/SetTransitionActorListFactory.h"
#include "soh/resource/importer/scenecommand/EndMarkerFactory.h"
#include "soh/resource/importer/scenecommand/SetAlternateHeadersFactory.h"
#include "soh/resource/importer/scenecommand/SetPathwaysFactory.h"
#include "soh/resource/importer/scenecommand/SetCutscenesFactory.h"
#include "soh/resource/importer/scenecommand/SetLightListFactory.h"
#include "soh/resource/importer/scenecommand/SetMeshFactory.h"

namespace LUS {

std::shared_ptr<IResource>
SceneFactory::ReadResource(std::shared_ptr<ResourceInitData> initData, std::shared_ptr<BinaryReader> reader) {
    if (SceneFactory::sceneCommandFactories.empty()) {
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetLightingSettings] = std::make_shared<SetLightingSettingsFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetWind] = std::make_shared<SetWindSettingsFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetExitList] = std::make_shared<SetExitListFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetTimeSettings] = std::make_shared<SetTimeSettingsFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetSkyboxModifier] = std::make_shared<SetSkyboxModifierFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetEchoSettings] = std::make_shared<SetEchoSettingsFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetSoundSettings] = std::make_shared<SetSoundSettingsFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetSkyboxSettings] = std::make_shared<SetSkyboxSettingsFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetRoomBehavior] = std::make_shared<SetRoomBehaviorFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetCsCamera] = std::make_shared<SetCsCameraFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetCameraSettings] = std::make_shared<SetCameraSettingsFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetRoomList] = std::make_shared<SetRoomListFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetCollisionHeader] = std::make_shared<SetCollisionHeaderFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetEntranceList] = std::make_shared<SetEntranceListFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetSpecialObjects] = std::make_shared<SetSpecialObjectsFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetObjectList] = std::make_shared<SetObjectListFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetStartPositionList] = std::make_shared<SetStartPositionListFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetActorList] = std::make_shared<SetActorListFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetTransitionActorList] = std::make_shared<SetTransitionActorListFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::EndMarker] = std::make_shared<EndMarkerFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetAlternateHeaders] = std::make_shared<SetAlternateHeadersFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetPathways] = std::make_shared<SetPathwaysFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetCutscenes] = std::make_shared<SetCutscenesFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetLightList] = std::make_shared<SetLightListFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetMesh] = std::make_shared<SetMeshFactory>();
    }

    auto resource = std::make_shared<Scene>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
    case 0:
	    factory = std::make_shared<SceneFactoryV0>();
	    break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load Scene with version {}", resource->GetInitData()->ResourceVersion);
        return nullptr;
    }

    factory->ParseFileBinary(reader, resource);

    return resource;
}

std::shared_ptr<IResource>
SceneFactory::ReadResourceXML(std::shared_ptr<ResourceInitData> initData, tinyxml2::XMLElement* reader) {
    if (SceneFactory::sceneCommandFactories.empty()) {
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetLightingSettings] = std::make_shared<SetLightingSettingsFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetWind] = std::make_shared<SetWindSettingsFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetExitList] = std::make_shared<SetExitListFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetTimeSettings] = std::make_shared<SetTimeSettingsFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetSkyboxModifier] = std::make_shared<SetSkyboxModifierFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetEchoSettings] = std::make_shared<SetEchoSettingsFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetSoundSettings] = std::make_shared<SetSoundSettingsFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetSkyboxSettings] = std::make_shared<SetSkyboxSettingsFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetRoomBehavior] = std::make_shared<SetRoomBehaviorFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetCsCamera] = std::make_shared<SetCsCameraFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetCameraSettings] = std::make_shared<SetCameraSettingsFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetRoomList] = std::make_shared<SetRoomListFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetCollisionHeader] = std::make_shared<SetCollisionHeaderFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetEntranceList] = std::make_shared<SetEntranceListFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetSpecialObjects] = std::make_shared<SetSpecialObjectsFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetObjectList] = std::make_shared<SetObjectListFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetStartPositionList] = std::make_shared<SetStartPositionListFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetActorList] = std::make_shared<SetActorListFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetTransitionActorList] = std::make_shared<SetTransitionActorListFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::EndMarker] = std::make_shared<EndMarkerFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetAlternateHeaders] = std::make_shared<SetAlternateHeadersFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetPathways] = std::make_shared<SetPathwaysFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetCutscenes] = std::make_shared<SetCutscenesFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetLightList] = std::make_shared<SetLightListFactory>();
        SceneFactory::sceneCommandFactories[LUS::SceneCommandID::SetMesh] = std::make_shared<SetMeshFactory>();
    }

    auto resource = std::make_shared<Scene>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
        case 0:
	        factory = std::make_shared<SceneFactoryV0>();
	        break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load Scene with version {}", resource->GetInitData()->ResourceVersion);
        return nullptr;
    }

    factory->ParseFileXML(reader, resource);

    return resource;
}

void SceneFactoryV0::ParseFileBinary(std::shared_ptr<BinaryReader> reader,
                                        std::shared_ptr<IResource> resource)
{
    std::shared_ptr<Scene> scene = std::static_pointer_cast<Scene>(resource);
    ResourceVersionFactory::ParseFileBinary(reader, scene);

    ParseSceneCommands(scene, reader);
}

void SceneFactoryV0::ParseFileXML(tinyxml2::XMLElement* reader, std::shared_ptr<IResource> resource) {
    std::shared_ptr<Scene> scene = std::static_pointer_cast<Scene>(resource);
    ResourceVersionFactory::ParseFileXML(reader, scene);

    ParseSceneCommandsXML(scene, reader);
}

void SceneFactoryV0::ParseSceneCommands(std::shared_ptr<Scene> scene, std::shared_ptr<BinaryReader> reader) {
    uint32_t commandCount = reader->ReadUInt32();
    scene->commands.reserve(commandCount);

    for (uint32_t i = 0; i < commandCount; i++) {
        scene->commands.push_back(ParseSceneCommand(scene, reader, i));
    }
}

void SceneFactoryV0::ParseSceneCommandsXML(std::shared_ptr<Scene> scene, tinyxml2::XMLElement* reader) {
    auto child = reader->FirstChildElement();

    int i = 0;

    while (child != nullptr) {
        scene->commands.push_back(ParseSceneCommandXML(scene, child, i));

        child = child->NextSiblingElement();
        i += 1;
    }
}

std::shared_ptr<ISceneCommand> SceneFactoryV0::ParseSceneCommand(std::shared_ptr<Scene> scene,
                                                                std::shared_ptr<BinaryReader> reader, uint32_t index) {
    SceneCommandID cmdID = (SceneCommandID)reader->ReadInt32();

    reader->Seek(-sizeof(int32_t), SeekOffsetType::Current);

    std::shared_ptr<ISceneCommand> result = nullptr;
    std::shared_ptr<SceneCommandFactory> commandFactory = SceneFactory::sceneCommandFactories[cmdID];

    if (commandFactory != nullptr) {
        auto initData = std::make_shared<ResourceInitData>();
        initData->Id = scene->GetInitData()->Id;
        initData->Type = ResourceType::SOH_SceneCommand;
        initData->Path = scene->GetInitData()->Path + "/SceneCommand" + std::to_string(index);
        initData->ResourceVersion = scene->GetInitData()->ResourceVersion;
        result = std::static_pointer_cast<ISceneCommand>(commandFactory->ReadResource(initData, reader));
        // Cache the resource?
    }

    if (result == nullptr) {
        SPDLOG_ERROR("Failed to load scene command of type {} in scene {}", (uint32_t)cmdID, scene->GetInitData()->Path);
    }

    return result;
}

std::vector<std::string> commandNames = {
    "SetStartPositionList",
    "SetActorList",
    "SetCsCamera",
    "SetCollisionHeader",
    "SetRoomList",
    "SetWind",
    "SetEntranceList",
    "SetSpecialObjects",
    "SetRoomBehavior",
    "Unused09",
    "SetMesh",
    "SetObjectList",
    "SetLightList",
    "SetPathways",
    "SetTransitionActorList",
    "SetLightingSettings",
    "SetTimeSettings",
    "SetSkyboxSettings",
    "SetSkyboxModifier",
    "SetExitList",
    "EndMarker",
    "SetSoundSettings",
    "SetEchoSettings",
    "SetCutscenes",
    "SetAlternateHeaders",
    "SetCameraSettings",
};

SceneCommandID GetCommandID(std::string commandName) {
    for (int i = 0; i < commandNames.size(); i++) {
        if (commandNames[i] == commandName) {
            return (SceneCommandID)i;
        }
    }

    return SceneCommandID::Error;
}

std::shared_ptr<ISceneCommand> SceneFactoryV0::ParseSceneCommandXML(std::shared_ptr<Scene> scene, tinyxml2::XMLElement* reader, uint32_t index) {
    
    std::string commandName = reader->Name();
    SceneCommandID cmdID = GetCommandID(commandName);
    
    if (cmdID == SceneCommandID::Error) {
        SPDLOG_ERROR("Failed to load scene command with name {} in scene {}", commandName, scene->GetInitData()->Path);
        return nullptr;
    }

    std::shared_ptr<ISceneCommand> result = nullptr;
    std::shared_ptr<SceneCommandFactory> commandFactory = SceneFactory::sceneCommandFactories[cmdID];
    
    if (commandFactory != nullptr) {
        auto initData = std::make_shared<ResourceInitData>();
        initData->Id = scene->GetInitData()->Id;
        initData->Type = ResourceType::SOH_SceneCommand;
        initData->Path = scene->GetInitData()->Path + "/SceneCommand" + std::to_string(index);
        initData->ResourceVersion = scene->GetInitData()->ResourceVersion;
        result = std::static_pointer_cast<ISceneCommand>(commandFactory->ReadResourceXML(initData, reader));
        // Cache the resource?
    }

    if (result == nullptr) {
        SPDLOG_ERROR("Failed to load scene command of type {} in scene {}", (uint32_t)cmdID, scene->GetInitData()->Path);
    }

    return result;
}

} // namespace LUS
