#include "soh/resource/importer/scenecommand/SetLightListFactory.h"
#include "soh/resource/type/scenecommand/SetLightList.h"
#include "spdlog/spdlog.h"
#include "z64light.h"

namespace LUS {
std::shared_ptr<IResource>
SetLightListFactory::ReadResource(std::shared_ptr<ResourceInitData> initData, std::shared_ptr<BinaryReader> reader) {
    auto resource = std::make_shared<SetLightList>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
    case 0:
	factory = std::make_shared<SetLightListFactoryV0>();
	break;
    }

    if (factory == nullptr)
    {
        SPDLOG_ERROR("Failed to load SetLightList with version {}", resource->GetInitData()->ResourceVersion);
	return nullptr;
    }

    factory->ParseFileBinary(reader, resource);

    return resource;
}

std::shared_ptr<IResource>
SetLightListFactory::ReadResourceXML(std::shared_ptr<ResourceInitData> initData, tinyxml2::XMLElement *reader) {
    auto resource = std::make_shared<SetLightList>(initData);
    std::shared_ptr<ResourceVersionFactory> factory = nullptr;

    switch (resource->GetInitData()->ResourceVersion) {
        case 0:
            factory = std::make_shared<SetLightListFactoryV0>();
            break;
    }

    if (factory == nullptr) {
        SPDLOG_ERROR("Failed to load SetLightList with version {}", resource->GetInitData()->ResourceVersion);
        return nullptr;
    }

    factory->ParseFileXML(reader, resource);

    return resource;
}

void LUS::SetLightListFactoryV0::ParseFileBinary(std::shared_ptr<BinaryReader> reader,
                                        		  std::shared_ptr<IResource> resource)
{
    std::shared_ptr<SetLightList> setLightList = std::static_pointer_cast<SetLightList>(resource);
    ResourceVersionFactory::ParseFileBinary(reader, setLightList);

    ReadCommandId(setLightList, reader);
    
    setLightList->numLights = reader->ReadUInt32();
    setLightList->lightList.reserve(setLightList->numLights);
    for (uint32_t i = 0; i < setLightList->numLights; i++) {
        LightInfo light;

        light.type = reader->ReadUByte();
		
	    light.params.point.x = reader->ReadInt16();
        light.params.point.y = reader->ReadInt16();
        light.params.point.z = reader->ReadInt16();

        light.params.point.color[0] = reader->ReadUByte(); // r
        light.params.point.color[1] = reader->ReadUByte(); // g
        light.params.point.color[2] = reader->ReadUByte(); // b

        light.params.point.drawGlow = reader->ReadUByte();
	    light.params.point.radius = reader->ReadInt16();

        setLightList->lightList.push_back(light);
    }

    LogLightListAsXML(setLightList);
}

void LUS::SetLightListFactoryV0::ParseFileXML(tinyxml2::XMLElement* reader, std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetLightList> setLightList = std::static_pointer_cast<SetLightList>(resource);

    setLightList->cmdId = SceneCommandID::SetLightList;

    auto child = reader->FirstChildElement();

    while (child != nullptr) {
        std::string childName = child->Name();
        if (childName == "LightInfo") {
            LightInfo light;
            light.type = child->IntAttribute("Type");
            if (light.type == LIGHT_DIRECTIONAL) {
                light.params.dir.x = child->IntAttribute("X");
                light.params.dir.y = child->IntAttribute("Y");
                light.params.dir.z = child->IntAttribute("Z");
                light.params.dir.color[0] = child->IntAttribute("ColorR");
                light.params.dir.color[1] = child->IntAttribute("ColorG");
                light.params.dir.color[2] = child->IntAttribute("ColorB");
            } else {
                light.params.point.x = child->IntAttribute("X");
                light.params.point.y = child->IntAttribute("Y");
                light.params.point.z = child->IntAttribute("Z");
                light.params.point.color[0] = child->IntAttribute("ColorR");
                light.params.point.color[1] = child->IntAttribute("ColorG");
                light.params.point.color[2] = child->IntAttribute("ColorB");
                light.params.point.drawGlow = child->IntAttribute("DrawGlow");
                light.params.point.radius = child->IntAttribute("Radius");
            }
            setLightList->lightList.push_back(light);
        }

        child = child->NextSiblingElement();
    }

    setLightList->numLights = setLightList->lightList.size();
}

void LogLightListAsXML(std::shared_ptr<IResource> resource) {
    std::shared_ptr<SetLightList> setLightList = std::static_pointer_cast<SetLightList>(resource);

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("SetLightList");
    doc.InsertFirstChild(root);

    for (size_t i = 0; i < setLightList->numLights; i++) {
        tinyxml2::XMLElement* light = doc.NewElement("LightInfo");
        light->SetAttribute("Type", setLightList->lightList[i].type);
        if (setLightList->lightList[i].type == LIGHT_DIRECTIONAL) {
            light->SetAttribute("X", setLightList->lightList[i].params.dir.x);
            light->SetAttribute("Y", setLightList->lightList[i].params.dir.y);
            light->SetAttribute("Z", setLightList->lightList[i].params.dir.z);
            light->SetAttribute("ColorR", setLightList->lightList[i].params.dir.color[0]);
            light->SetAttribute("ColorG", setLightList->lightList[i].params.dir.color[1]);
            light->SetAttribute("ColorB", setLightList->lightList[i].params.dir.color[2]);
        } else {
            light->SetAttribute("X", setLightList->lightList[i].params.point.x);
            light->SetAttribute("Y", setLightList->lightList[i].params.point.y);
            light->SetAttribute("Z", setLightList->lightList[i].params.point.z);
            light->SetAttribute("ColorR", setLightList->lightList[i].params.point.color[0]);
            light->SetAttribute("ColorG", setLightList->lightList[i].params.point.color[1]);
            light->SetAttribute("ColorB", setLightList->lightList[i].params.point.color[2]);
            light->SetAttribute("DrawGlow", setLightList->lightList[i].params.point.drawGlow);
            light->SetAttribute("Radius", setLightList->lightList[i].params.point.radius);
        }
        root->InsertEndChild(light);
    }

    tinyxml2::XMLPrinter printer;
    doc.Accept(&printer);
    
    SPDLOG_INFO("{}: {}", resource->GetInitData()->Path, printer.CStr());
}

} // namespace LUS
