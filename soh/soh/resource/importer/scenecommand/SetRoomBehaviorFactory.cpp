#include "soh/resource/importer/scenecommand/SetRoomBehaviorFactory.h"
#include "soh/resource/type/scenecommand/SetRoomBehavior.h"
#include "spdlog/spdlog.h"

namespace LUS {
std::shared_ptr<IResource>
SetRoomBehaviorFactory::ReadResource(std::shared_ptr<ResourceInitData> initData, std::shared_ptr<BinaryReader> reader) {
    auto resource = std::make_shared<SetRoomBehavior>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
    case 0:
	factory = std::make_shared<SetRoomBehaviorFactoryV0>();
	break;
    }

    if (factory == nullptr) {
	SPDLOG_ERROR("Failed to load SetRoomBehavior with version {}", resource->GetInitData()->ResourceVersion);
	return nullptr;
    }

    factory->ParseFileBinary(reader, resource);

    return resource;
}

std::shared_ptr<IResource>
SetRoomBehaviorFactory::ReadResourceXML(std::shared_ptr<ResourceInitData> initData, tinyxml2::XMLElement *reader) {
    auto resource = std::make_shared<SetRoomBehavior>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
        case 0:
            factory = std::make_shared<SetRoomBehaviorFactoryV0>();
            break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load SetRoomBehavior with version {}", resource->GetInitData()->ResourceVersion);
        return nullptr;
    }

    factory->ParseFileXML(reader, resource);

    return resource;
}

void LUS::SetRoomBehaviorFactoryV0::ParseFileBinary(std::shared_ptr<BinaryReader> reader,
                                        std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetRoomBehavior> setRoomBehavior = std::static_pointer_cast<SetRoomBehavior>(resource);
    ResourceVersionFactory::ParseFileBinary(reader, setRoomBehavior);

    ReadCommandId(setRoomBehavior, reader);
	
    setRoomBehavior->roomBehavior.gameplayFlags = reader->ReadInt8();
    setRoomBehavior->roomBehavior.gameplayFlags2 = reader->ReadInt32();

    LogRoomBehaviorAsXML(setRoomBehavior);
}

void LUS::SetRoomBehaviorFactoryV0::ParseFileXML(tinyxml2::XMLElement* reader, std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetRoomBehavior> setRoomBehavior = std::static_pointer_cast<SetRoomBehavior>(resource);
	
    setRoomBehavior->roomBehavior.gameplayFlags = reader->IntAttribute("GameplayFlags1");
    setRoomBehavior->roomBehavior.gameplayFlags2 = reader->IntAttribute("GameplayFlags2");
}

void LogRoomBehaviorAsXML(std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetRoomBehavior> setRoomBehavior = std::static_pointer_cast<SetRoomBehavior>(resource);

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("SetRoomBehavior");
    doc.InsertFirstChild(root);

    root->SetAttribute("GameplayFlags1", setRoomBehavior->roomBehavior.gameplayFlags);
    root->SetAttribute("GameplayFlags2", setRoomBehavior->roomBehavior.gameplayFlags2);

    tinyxml2::XMLPrinter printer;
    doc.Accept(&printer);
    
    SPDLOG_INFO("{}: {}", resource->GetInitData()->Path, printer.CStr());
}

} // namespace LUS
