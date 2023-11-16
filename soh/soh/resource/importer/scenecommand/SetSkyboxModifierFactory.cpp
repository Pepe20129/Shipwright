#include "soh/resource/importer/scenecommand/SetSkyboxModifierFactory.h"
#include "soh/resource/type/scenecommand/SetSkyboxModifier.h"
#include "spdlog/spdlog.h"

namespace LUS {
std::shared_ptr<IResource> SetSkyboxModifierFactory::ReadResource(std::shared_ptr<ResourceInitData> initData,
                                                                 std::shared_ptr<BinaryReader> reader) {
    auto resource = std::make_shared<SetSkyboxModifier>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
    case 0:
	factory = std::make_shared<SetSkyboxModifierFactoryV0>();
	break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load SetSkyboxModifier with version {}", resource->GetInitData()->ResourceVersion);
        return nullptr;
    }

    factory->ParseFileBinary(reader, resource);

    return resource;
}

std::shared_ptr<IResource>
SetSkyboxModifierFactory::ReadResourceXML(std::shared_ptr<ResourceInitData> initData, tinyxml2::XMLElement *reader) {
    auto resource = std::make_shared<SetSkyboxModifier>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
        case 0:
            factory = std::make_shared<SetSkyboxModifierFactoryV0>();
            break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load SetSkyboxModifier with version {}", resource->GetInitData()->ResourceVersion);
        return nullptr;
    }

    factory->ParseFileXML(reader, resource);

    return resource;
}

void LUS::SetSkyboxModifierFactoryV0::ParseFileBinary(std::shared_ptr<BinaryReader> reader,
                                        std::shared_ptr<IResource> resource) {
	std::shared_ptr<SetSkyboxModifier> setSkyboxModifier = std::static_pointer_cast<SetSkyboxModifier>(resource);
	ResourceVersionFactory::ParseFileBinary(reader, setSkyboxModifier);

	ReadCommandId(setSkyboxModifier, reader);
	
    setSkyboxModifier->modifier.skyboxDisabled = reader->ReadInt8();
    setSkyboxModifier->modifier.sunMoonDisabled = reader->ReadInt8();

    LogSkyboxModifierAsXML(setSkyboxModifier);
}

void LUS::SetSkyboxModifierFactoryV0::ParseFileXML(tinyxml2::XMLElement* reader, std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetSkyboxModifier> setSkyboxModifier = std::static_pointer_cast<SetSkyboxModifier>(resource);

    setSkyboxModifier->modifier.skyboxDisabled = reader->IntAttribute("SkyboxDisabled");
    setSkyboxModifier->modifier.sunMoonDisabled = reader->IntAttribute("SunMoonDisabled");
}

void LogSkyboxModifierAsXML(std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetSkyboxModifier> setSkyboxModifier = std::static_pointer_cast<SetSkyboxModifier>(resource);

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("SetSkyboxModifier");
    doc.InsertFirstChild(root);

    root->SetAttribute("SkyboxDisabled", setSkyboxModifier->modifier.skyboxDisabled);
    root->SetAttribute("SunMoonDisabled", setSkyboxModifier->modifier.sunMoonDisabled);
    
    tinyxml2::XMLPrinter printer;
    doc.Accept(&printer);
    
    SPDLOG_INFO("{}: {}", resource->GetInitData()->Path, printer.CStr());
}

} // namespace LUS
