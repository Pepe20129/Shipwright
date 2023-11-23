#include "soh/resource/importer/scenecommand/SetSpecialObjectsFactory.h"
#include "soh/resource/type/scenecommand/SetSpecialObjects.h"
#include "spdlog/spdlog.h"

namespace LUS {
std::shared_ptr<IResource> SetSpecialObjectsFactory::ReadResource(std::shared_ptr<ResourceInitData> initData,
                                                                 std::shared_ptr<BinaryReader> reader) {
    auto resource = std::make_shared<SetSpecialObjects>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
    case 0:
	    factory = std::make_shared<SetSpecialObjectsFactoryV0>();
	    break;
    }

    if (factory == nullptr){
        SPDLOG_ERROR("Failed to load SetSpecialObjects with version {}", resource->GetInitData()->ResourceVersion);
	return nullptr;
    }

    factory->ParseFileBinary(reader, resource);

    return resource;
}

std::shared_ptr<IResource>
SetSpecialObjectsFactory::ReadResourceXML(std::shared_ptr<ResourceInitData> initData, tinyxml2::XMLElement *reader) {
    auto resource = std::make_shared<SetSpecialObjects>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
        case 0:
            factory = std::make_shared<SetSpecialObjectsFactoryV0>();
            break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load SetSpecialObjects with version {}", resource->GetInitData()->ResourceVersion);
        return nullptr;
    }

    factory->ParseFileXML(reader, resource);

    return resource;
}

void LUS::SetSpecialObjectsFactoryV0::ParseFileBinary(std::shared_ptr<BinaryReader> reader,
                                        std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetSpecialObjects> setSpecialObjects = std::static_pointer_cast<SetSpecialObjects>(resource);
    ResourceVersionFactory::ParseFileBinary(reader, setSpecialObjects);

    ReadCommandId(setSpecialObjects, reader);
    
    setSpecialObjects->specialObjects.elfMessage = reader->ReadInt8();
    setSpecialObjects->specialObjects.globalObject = reader->ReadInt16();

    LogSpecialObjectsAsXML(setSpecialObjects);
}

void LUS::SetSpecialObjectsFactoryV0::ParseFileXML(tinyxml2::XMLElement* reader, std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetSpecialObjects> setSpecialObjects = std::static_pointer_cast<SetSpecialObjects>(resource);

    setSpecialObjects->cmdId = SceneCommandID::SetSpecialObjects;

    setSpecialObjects->specialObjects.elfMessage = reader->IntAttribute("ElfMessage");
    setSpecialObjects->specialObjects.globalObject = reader->IntAttribute("GlobalObject");
}

void LogSpecialObjectsAsXML(std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetSpecialObjects> setSpecialObjects = std::static_pointer_cast<SetSpecialObjects>(resource);

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("SetSpecialObjects");
    doc.InsertFirstChild(root);

    root->SetAttribute("ElfMessage", setSpecialObjects->specialObjects.elfMessage);
    root->SetAttribute("GlobalObject", setSpecialObjects->specialObjects.globalObject);
    
    tinyxml2::XMLPrinter printer;
    doc.Accept(&printer);
    
    SPDLOG_INFO("{}: {}", resource->GetInitData()->Path, printer.CStr());
}

} // namespace LUS
