#include "soh/resource/importer/scenecommand/SetRoomListFactory.h"
#include "soh/resource/type/scenecommand/SetRoomList.h"
#include "spdlog/spdlog.h"

namespace LUS {
std::shared_ptr<IResource>
SetRoomListFactory::ReadResource(std::shared_ptr<ResourceInitData> initData, std::shared_ptr<BinaryReader> reader) {
    auto resource = std::make_shared<SetRoomList>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
    case 0:
	factory = std::make_shared<SetRoomListFactoryV0>();
	break;
    }

    if (factory == nullptr)
    {
	SPDLOG_ERROR("Failed to load SetRoomList with version {}", resource->GetInitData()->ResourceVersion);
	return nullptr;
    }

    factory->ParseFileBinary(reader, resource);

    return resource;
}

std::shared_ptr<IResource>
SetRoomListFactory::ReadResourceXML(std::shared_ptr<ResourceInitData> initData, tinyxml2::XMLElement *reader) {
    auto resource = std::make_shared<SetRoomList>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
        case 0:
            factory = std::make_shared<SetRoomListFactoryV0>();
            break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load SetRoomList with version {}", resource->GetInitData()->ResourceVersion);
        return nullptr;
    }

    factory->ParseFileXML(reader, resource);

    return resource;
}

void LUS::SetRoomListFactoryV0::ParseFileBinary(std::shared_ptr<BinaryReader> reader,
                                        std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetRoomList> setRoomList = std::static_pointer_cast<SetRoomList>(resource);
    ResourceVersionFactory::ParseFileBinary(reader, setRoomList);

    ReadCommandId(setRoomList, reader);
	
    setRoomList->numRooms = reader->ReadInt32();
    setRoomList->rooms.reserve(setRoomList->numRooms);
    for (uint32_t i = 0; i < setRoomList->numRooms; i++) {
        RomFile room;

        setRoomList->fileNames.push_back(reader->ReadString());

        room.fileName = (char*)setRoomList->fileNames.back().c_str();
        room.vromStart = reader->ReadInt32();
        room.vromEnd = reader->ReadInt32();
		
        setRoomList->rooms.push_back(room);
    }

    LogRoomListAsXML(setRoomList);
}

void LUS::SetRoomListFactoryV0::ParseFileXML(tinyxml2::XMLElement* reader, std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetRoomList> setRoomList = std::static_pointer_cast<SetRoomList>(resource);

    auto child = reader->FirstChildElement();

    while (child != nullptr) {
        std::string childName = child->Name();
        if (childName == "Room") {
            RomFile room;

	        setRoomList->fileNames.push_back(child->Attribute("FileName"));

	        room.fileName = (char*)setRoomList->fileNames.back().c_str();
	        room.vromStart = child->IntAttribute("VromStart");
	        room.vromEnd = child->IntAttribute("VromEnd");

            setRoomList->rooms.push_back(room);
        }

        child = child->NextSiblingElement();
    }

    setRoomList->numRooms = setRoomList->rooms.size();
}

void LogRoomListAsXML(std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetRoomList> setRoomList = std::static_pointer_cast<SetRoomList>(resource);

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("SetRoomList");
    doc.InsertFirstChild(root);

    for (size_t i = 0; i < setRoomList->numRooms; i++) {
        tinyxml2::XMLElement* entry = doc.NewElement("RoomEntry");
        entry->SetAttribute("Path", setRoomList->fileNames[i].c_str());
        root->InsertEndChild(entry);
    }
    
    tinyxml2::XMLPrinter printer;
    doc.Accept(&printer);
    
    SPDLOG_INFO("{}: {}", resource->GetInitData()->Path, printer.CStr());
}

} // namespace LUS
