#include "soh/resource/importer/scenecommand/SetSoundSettingsFactory.h"
#include "soh/resource/type/scenecommand/SetSoundSettings.h"
#include "spdlog/spdlog.h"

namespace LUS {
std::shared_ptr<IResource> SetSoundSettingsFactory::ReadResource(std::shared_ptr<ResourceInitData> initData,
                                                                std::shared_ptr<BinaryReader> reader) {
    auto resource = std::make_shared<SetSoundSettings>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
    case 0:
	    factory = std::make_shared<SetSoundSettingsFactoryV0>();
	    break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load SetSoundSettings with version {}", resource->GetInitData()->ResourceVersion);
	return nullptr;
    }

    factory->ParseFileBinary(reader, resource);

    return resource;
}

std::shared_ptr<IResource>
SetSoundSettingsFactory::ReadResourceXML(std::shared_ptr<ResourceInitData> initData, tinyxml2::XMLElement *reader) {
    auto resource = std::make_shared<SetSoundSettings>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
        case 0:
            factory = std::make_shared<SetSoundSettingsFactoryV0>();
            break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load SetSoundSettings with version {}", resource->GetInitData()->ResourceVersion);
        return nullptr;
    }

    factory->ParseFileXML(reader, resource);

    return resource;
}

void LUS::SetSoundSettingsFactoryV0::ParseFileBinary(std::shared_ptr<BinaryReader> reader,
                                        std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetSoundSettings> setSoundSettings = std::static_pointer_cast<SetSoundSettings>(resource);
    ResourceVersionFactory::ParseFileBinary(reader, setSoundSettings);

    ReadCommandId(setSoundSettings, reader);
	
    setSoundSettings->settings.reverb = reader->ReadInt8();
    setSoundSettings->settings.natureAmbienceId = reader->ReadInt8();
    setSoundSettings->settings.seqId = reader->ReadInt8();

    LogSoundSettingsAsXML(setSoundSettings);
}

void LUS::SetSoundSettingsFactoryV0::ParseFileXML(tinyxml2::XMLElement* reader, std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetSoundSettings> setSoundSettings = std::static_pointer_cast<SetSoundSettings>(resource);

    setSoundSettings->settings.reverb = reader->IntAttribute("Reverb");
    setSoundSettings->settings.natureAmbienceId = reader->IntAttribute("NatureAmbienceId");
    setSoundSettings->settings.seqId = reader->IntAttribute("SeqId");
}

void LogSoundSettingsAsXML(std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetSoundSettings> setSoundSettings = std::static_pointer_cast<SetSoundSettings>(resource);

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("SetSoundSettings");
    doc.InsertFirstChild(root);

    root->SetAttribute("Reverb", setSoundSettings->settings.reverb);
    root->SetAttribute("NatureAmbienceId", setSoundSettings->settings.natureAmbienceId);
    root->SetAttribute("SeqId", setSoundSettings->settings.seqId);
    
    tinyxml2::XMLPrinter printer;
    doc.Accept(&printer);
    
    SPDLOG_INFO("{}: {}", resource->GetInitData()->Path, printer.CStr());
}

} // namespace LUS
