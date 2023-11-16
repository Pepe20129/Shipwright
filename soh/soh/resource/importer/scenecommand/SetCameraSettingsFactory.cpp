#include "soh/resource/importer/scenecommand/SetCameraSettingsFactory.h"
#include "soh/resource/type/scenecommand/SetCameraSettings.h"
#include "spdlog/spdlog.h"

namespace LUS {
std::shared_ptr<IResource> SetCameraSettingsFactory::ReadResource(std::shared_ptr<ResourceInitData> initData,
                                                                 std::shared_ptr<BinaryReader> reader) {
    auto resource = std::make_shared<SetCameraSettings>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
    case 0:
	    factory = std::make_shared<SetCameraSettingsFactoryV0>();
	    break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load SetCameraSettings with version {}", resource->GetInitData()->ResourceVersion);
	return nullptr;
    }

    factory->ParseFileBinary(reader, resource);

    return resource;
}

std::shared_ptr<IResource>
SetCameraSettingsFactory::ReadResourceXML(std::shared_ptr<ResourceInitData> initData, tinyxml2::XMLElement *reader) {
    auto resource = std::make_shared<SetCameraSettings>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
        case 0:
            factory = std::make_shared<SetCameraSettingsFactoryV0>();
            break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load SetCameraSettings with version {}", resource->GetInitData()->ResourceVersion);
        return nullptr;
    }

    factory->ParseFileXML(reader, resource);

    return resource;
}

void LUS::SetCameraSettingsFactoryV0::ParseFileBinary(std::shared_ptr<BinaryReader> reader,
                                        std::shared_ptr<IResource> resource)
{
    std::shared_ptr<SetCameraSettings> setCameraSettings = std::static_pointer_cast<SetCameraSettings>(resource);
    ResourceVersionFactory::ParseFileBinary(reader, setCameraSettings);

    ReadCommandId(setCameraSettings, reader);
	
    setCameraSettings->settings.cameraMovement = reader->ReadInt8();
    setCameraSettings->settings.worldMapArea = reader->ReadInt32();

    LogCameraSettingsAsXML(setCameraSettings);
}

void LUS::SetCameraSettingsFactoryV0::ParseFileXML(tinyxml2::XMLElement* reader, std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetCameraSettings> setCameraSettings = std::static_pointer_cast<SetCameraSettings>(resource);

    setCameraSettings->settings.cameraMovement = reader->IntAttribute("CameraMovement");
    setCameraSettings->settings.worldMapArea = reader->IntAttribute("WorldMapArea");
}

void LogCameraSettingsAsXML(std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetCameraSettings> setCameraSettings = std::static_pointer_cast<SetCameraSettings>(resource);

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("SetCameraSettings");
    doc.InsertFirstChild(root);

    root->SetAttribute("CameraMovement", setCameraSettings->settings.cameraMovement);
    root->SetAttribute("WorldMapArea", setCameraSettings->settings.worldMapArea);

    tinyxml2::XMLPrinter printer;
    doc.Accept(&printer);
    
    SPDLOG_INFO("{}: {}", resource->GetInitData()->Path, printer.CStr());
}

} // namespace LUS
