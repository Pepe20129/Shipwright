#include "soh/resource/importer/scenecommand/SetEntranceListFactory.h"
#include "soh/resource/type/scenecommand/SetEntranceList.h"
#include "spdlog/spdlog.h"

namespace LUS {
std::shared_ptr<IResource>
SetEntranceListFactory::ReadResource(std::shared_ptr<ResourceInitData> initData, std::shared_ptr<BinaryReader> reader) {
    auto resource = std::make_shared<SetEntranceList>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
    case 0:
	factory = std::make_shared<SetEntranceListFactoryV0>();
	break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load SetEntranceList with version {}", resource->GetInitData()->ResourceVersion);
	return nullptr;
    }

    factory->ParseFileBinary(reader, resource);

    return resource;
}

std::shared_ptr<IResource>
SetEntranceListFactory::ReadResourceXML(std::shared_ptr<ResourceInitData> initData, tinyxml2::XMLElement *reader) {
    auto resource = std::make_shared<SetEntranceList>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
        case 0:
            factory = std::make_shared<SetEntranceListFactoryV0>();
            break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load SetEntranceList with version {}", resource->GetInitData()->ResourceVersion);
        return nullptr;
    }

    factory->ParseFileXML(reader, resource);

    return resource;
}

void LUS::SetEntranceListFactoryV0::ParseFileBinary(std::shared_ptr<BinaryReader> reader,
                                        std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetEntranceList> setEntranceList = std::static_pointer_cast<SetEntranceList>(resource);
    ResourceVersionFactory::ParseFileBinary(reader, setEntranceList);

    ReadCommandId(setEntranceList, reader);
	
    setEntranceList->numEntrances = reader->ReadUInt32();
    setEntranceList->entrances.reserve(setEntranceList->numEntrances);
    for (uint32_t i = 0; i < setEntranceList->numEntrances; i++) {
	    EntranceEntry entranceEntry;

	    entranceEntry.spawn = reader->ReadInt8();
	    entranceEntry.room = reader->ReadInt8();
		
        setEntranceList->entrances.push_back(entranceEntry);
    }

    LogEntranceListAsXML(setEntranceList);
}

void LUS::SetEntranceListFactoryV0::ParseFileXML(tinyxml2::XMLElement* reader, std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetEntranceList> setEntranceList = std::static_pointer_cast<SetEntranceList>(resource);

    setEntranceList->cmdId = SceneCommandID::SetEntranceList;

    auto child = reader->FirstChildElement();

    while (child != nullptr) {
        std::string childName = child->Name();
        if (childName == "EntranceEntry") {
            EntranceEntry entry;
            entry.spawn = child->IntAttribute("Spawn");
            entry.room = child->IntAttribute("Room");
            setEntranceList->entrances.push_back(entry);
        }

        child = child->NextSiblingElement();
    }

    setEntranceList->numEntrances = setEntranceList->entrances.size();
}

void LogEntranceListAsXML(std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetEntranceList> setEntranceList = std::static_pointer_cast<SetEntranceList>(resource);

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("SetEntranceList");
    doc.InsertFirstChild(root);

    for (size_t i = 0; i < setEntranceList->numEntrances; i++) {
        tinyxml2::XMLElement* entry = doc.NewElement("EntranceEntry");
        entry->SetAttribute("Spawn", setEntranceList->entrances[i].spawn);
        entry->SetAttribute("Room", setEntranceList->entrances[i].room);
        root->InsertEndChild(entry);
    }

    tinyxml2::XMLPrinter printer;
    doc.Accept(&printer);
    
    SPDLOG_INFO("{}: {}", resource->GetInitData()->Path, printer.CStr());
}

} // namespace LUS
