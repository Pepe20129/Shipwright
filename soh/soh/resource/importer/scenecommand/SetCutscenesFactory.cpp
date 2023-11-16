#include "soh/resource/importer/scenecommand/SetCutscenesFactory.h"
#include "soh/resource/type/scenecommand/SetCutscenes.h"
#include <libultraship/libultraship.h>
#include "spdlog/spdlog.h"

namespace LUS {
std::shared_ptr<IResource>
SetCutscenesFactory::ReadResource(std::shared_ptr<ResourceInitData> initData, std::shared_ptr<BinaryReader> reader) {
    auto resource = std::make_shared<SetCutscenes>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
    case 0:
	    factory = std::make_shared<SetCutscenesFactoryV0>();
	    break;
    }

    if (factory == nullptr)
    {
        SPDLOG_ERROR("Failed to load SetCutscenes with version {}", resource->GetInitData()->ResourceVersion);
        return nullptr;
    }

    factory->ParseFileBinary(reader, resource);

    return resource;
}

std::shared_ptr<IResource>
SetCutscenesFactory::ReadResourceXML(std::shared_ptr<ResourceInitData> initData, tinyxml2::XMLElement *reader) {
    auto resource = std::make_shared<SetCutscenes>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
        case 0:
            factory = std::make_shared<SetCutscenesFactoryV0>();
            break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load SetCutscenes with version {}", resource->GetInitData()->ResourceVersion);
        return nullptr;
    }

    factory->ParseFileXML(reader, resource);

    return resource;
}

void LUS::SetCutscenesFactoryV0::ParseFileBinary(std::shared_ptr<BinaryReader> reader,
                                        		  std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetCutscenes> setCutscenes = std::static_pointer_cast<SetCutscenes>(resource);
    ResourceVersionFactory::ParseFileBinary(reader, setCutscenes);

    ReadCommandId(setCutscenes, reader);
    
    setCutscenes->fileName = reader->ReadString();
    setCutscenes->cutscene = std::static_pointer_cast<Cutscene>(LUS::Context::GetInstance()->GetResourceManager()->LoadResourceProcess(setCutscenes->fileName.c_str()));

    LogCutscenesAsXML(setCutscenes);
}

void LUS::SetCutscenesFactoryV0::ParseFileXML(tinyxml2::XMLElement* reader, std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetCutscenes> setCutscenes = std::static_pointer_cast<SetCutscenes>(resource);

    setCutscenes->fileName = reader->IntAttribute("FileName");
    setCutscenes->cutscene = std::static_pointer_cast<Cutscene>(LUS::Context::GetInstance()->GetResourceManager()->LoadResourceProcess(setCutscenes->fileName.c_str()));
}

void LogCutscenesAsXML(std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetCutscenes> setCutscenes = std::static_pointer_cast<SetCutscenes>(resource);

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("SetCutscenes");
    doc.InsertFirstChild(root);

    root->SetAttribute("FileName", setCutscenes->fileName.c_str());

    tinyxml2::XMLPrinter printer;
    doc.Accept(&printer);
    
    SPDLOG_INFO("{}: {}", resource->GetInitData()->Path, printer.CStr());
}

} // namespace LUS
