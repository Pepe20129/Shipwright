#pragma once

#include "soh/resource/importer/scenecommand/SceneCommandFactory.h"

namespace LUS {
class SetRoomListFactory : public SceneCommandFactory {
  public:
    std::shared_ptr<IResource>
    ReadResource(std::shared_ptr<ResourceInitData> initData, std::shared_ptr<BinaryReader> reader) override;
    std::shared_ptr<IResource>
    ReadResourceXML(std::shared_ptr<ResourceInitData> initData, tinyxml2::XMLElement *reader) override;
};

class SetRoomListFactoryV0 : public SceneCommandVersionFactory {
  public:
    void ParseFileBinary(std::shared_ptr<BinaryReader> reader, std::shared_ptr<IResource> resource) override;
    void ParseFileXML(tinyxml2::XMLElement* reader, std::shared_ptr<IResource> resource) override;
};

void LogRoomListAsXML(std::shared_ptr<IResource> resource);
}; // namespace LUS
