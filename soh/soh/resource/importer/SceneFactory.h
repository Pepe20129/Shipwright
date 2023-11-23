#pragma once

#include "soh/resource/type/Scene.h"
#include "soh/resource/type/scenecommand/SceneCommand.h"
#include "soh/resource/importer/scenecommand/SceneCommandFactory.h"
#include "Resource.h"
#include "ResourceFactory.h"

namespace LUS {
class SceneFactory : public ResourceFactory {
  public:
    std::shared_ptr<IResource>
    ReadResource(std::shared_ptr<ResourceInitData> initData, std::shared_ptr<BinaryReader> reader) override;
    std::shared_ptr<IResource>
    ReadResourceXML(std::shared_ptr<ResourceInitData> initData, tinyxml2::XMLElement *reader) override;

    // Doing something very similar to what we do on the ResourceLoader.
    // Eventually, scene commands should be moved up to the ResourceLoader as well.
    // They can not right now because the exporter does not give them a proper resource type enum value,
    // and the exporter does not export the commands with a proper OTR header.
    static inline std::unordered_map<SceneCommandID, std::shared_ptr<SceneCommandFactory>> sceneCommandFactories;
};

class SceneFactoryV0 : public ResourceVersionFactory {
  public:
    void ParseFileBinary(std::shared_ptr<BinaryReader> reader, std::shared_ptr<IResource> resource) override;
    void ParseFileXML(tinyxml2::XMLElement* reader, std::shared_ptr<IResource> resource) override;
    void ParseSceneCommands(std::shared_ptr<Scene> scene, std::shared_ptr<BinaryReader> reader);
    void ParseSceneCommandsXML(std::shared_ptr<Scene> scene, tinyxml2::XMLElement* reader);
protected:
    std::shared_ptr<ISceneCommand> ParseSceneCommand(std::shared_ptr<Scene> scene, std::shared_ptr<BinaryReader> reader, uint32_t index);
    std::shared_ptr<ISceneCommand> ParseSceneCommandXML(std::shared_ptr<Scene> scene, tinyxml2::XMLElement* reader, uint32_t index);
};
}; // namespace LUS
