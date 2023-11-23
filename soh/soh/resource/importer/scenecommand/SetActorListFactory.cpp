#include "soh/resource/importer/scenecommand/SetActorListFactory.h"
#include "soh/resource/type/scenecommand/SetActorList.h"
#include "spdlog/spdlog.h"

namespace LUS {
std::shared_ptr<IResource>
SetActorListFactory::ReadResource(std::shared_ptr<ResourceInitData> initData, std::shared_ptr<BinaryReader> reader) {
    auto resource = std::make_shared<SetActorList>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
    case 0:
	factory = std::make_shared<SetActorListFactoryV0>();
	break;
    }

    if (factory == nullptr)
    {
        SPDLOG_ERROR("Failed to load SetActorList with version {}", resource->GetInitData()->ResourceVersion);
	return nullptr;
    }

    factory->ParseFileBinary(reader, resource);

    return resource;
}

std::shared_ptr<IResource>
SetActorListFactory::ReadResourceXML(std::shared_ptr<ResourceInitData> initData, tinyxml2::XMLElement *reader) {
    auto resource = std::make_shared<SetActorList>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
        case 0:
            factory = std::make_shared<SetActorListFactoryV0>();
            break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load SetActorList with version {}", resource->GetInitData()->ResourceVersion);
        return nullptr;
    }

    factory->ParseFileXML(reader, resource);

    return resource;
}

void LUS::SetActorListFactoryV0::ParseFileBinary(std::shared_ptr<BinaryReader> reader,
                                        std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetActorList> setActorList = std::static_pointer_cast<SetActorList>(resource);
    ResourceVersionFactory::ParseFileBinary(reader, setActorList);

    ReadCommandId(setActorList, reader);
	
    setActorList->numActors = reader->ReadUInt32();
    setActorList->actorList.reserve(setActorList->numActors);
    for (uint32_t i = 0; i < setActorList->numActors; i++) {
	    ActorEntry entry;

	    entry.id = reader->ReadUInt16();
	    entry.pos.x = reader->ReadInt16();
	    entry.pos.y = reader->ReadInt16();
	    entry.pos.z = reader->ReadInt16();
	    entry.rot.x = reader->ReadInt16();
	    entry.rot.y = reader->ReadInt16();
	    entry.rot.z = reader->ReadInt16();
	    entry.params = reader->ReadUInt16();

        setActorList->actorList.push_back(entry);
    }

    LogActorListAsXML(setActorList);
}

void LUS::SetActorListFactoryV0::ParseFileXML(tinyxml2::XMLElement* reader, std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetActorList> setActorList = std::static_pointer_cast<SetActorList>(resource);

    setActorList->cmdId = SceneCommandID::SetActorList;

    auto child = reader->FirstChildElement();

    while (child != nullptr) {
        std::string childName = child->Name();
        if (childName == "ActorEntry") {
            ActorEntry entry;
            entry.id = child->IntAttribute("Id");
            entry.pos.x = child->IntAttribute("PosX");
            entry.pos.y = child->IntAttribute("PosY");
            entry.pos.z = child->IntAttribute("PosZ");
            entry.rot.x = child->IntAttribute("RotX");
            entry.rot.y = child->IntAttribute("RotY");
            entry.rot.z = child->IntAttribute("RotZ");
            entry.params = child->IntAttribute("Params");
            setActorList->actorList.push_back(entry);
        }

        child = child->NextSiblingElement();
    }

    setActorList->numActors = setActorList->actorList.size();
}

void LogActorListAsXML(std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetActorList> setActorList = std::static_pointer_cast<SetActorList>(resource);

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("SetActorList");
    doc.InsertFirstChild(root);

    for (size_t i = 0; i < setActorList->numActors; i++) {
        tinyxml2::XMLElement* entry = doc.NewElement("ActorEntry");
        entry->SetAttribute("Id", setActorList->actorList[i].id);
        entry->SetAttribute("PosX", setActorList->actorList[i].pos.x);
        entry->SetAttribute("PosY", setActorList->actorList[i].pos.y);
        entry->SetAttribute("PosZ", setActorList->actorList[i].pos.z);
        entry->SetAttribute("RotX", setActorList->actorList[i].rot.x);
        entry->SetAttribute("RotY", setActorList->actorList[i].rot.y);
        entry->SetAttribute("RotZ", setActorList->actorList[i].rot.z);
        entry->SetAttribute("Params", setActorList->actorList[i].params);
        root->InsertEndChild(entry);
    }

    tinyxml2::XMLPrinter printer;
    doc.Accept(&printer);
    
    SPDLOG_INFO("{}: {}", resource->GetInitData()->Path, printer.CStr());
}

} // namespace LUS
