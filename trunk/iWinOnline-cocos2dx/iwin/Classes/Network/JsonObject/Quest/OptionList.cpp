#include "OptionList.h"
#include "json/writer.h"
namespace iwinmesage
{
	int OptionList::LIST_TYPE_SINGLE_CHOICE = 0;
int OptionList::LIST_TYPE_MULTI_CHOICE = 1;

	OptionList:: OptionList ()
	{
		optionList.clear();
		listType = -1;
	}

	OptionList::~OptionList()
	{
		optionList.clear();
		listType = -1;
	}

	void OptionList::pushItem(OptionItem  op)
	{
		optionList.push_back(op);
	}

	void OptionList::changeValueItemAtIdx(int idx, bool value)
	{
		if (idx < 0 || idx >= optionList.size())
			return;
		auto temp = &(optionList[idx]);
		temp->setIsSelected(value);
	}

	rapidjson::Document OptionList::toJson()
	{

		rapidjson::Document document;
		document.SetObject();
		rapidjson::Document::AllocatorType& locator = document.GetAllocator();
		
		rapidjson::Value arroptionList(rapidjson::kArrayType);
		for (size_t i = 0; i < optionList.size(); i++)
		{
			rapidjson::Value opt(optionList.at(i).toJson(), locator);
			arroptionList.PushBack(opt, locator);
		}
		document.AddMember("optionList",arroptionList, locator);
		document.AddMember("listType", listType,locator);

		rapidjson::StringBuffer buffer;
		buffer.Clear();
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);
		return document;
	}
	void OptionList::toData(std::string json)
	{
		rapidjson::Document document;
		document.Parse(json.c_str());
		toData(document);
	}
	void OptionList::toData(rapidjson::Document & document)
	{
				if(document.HasMember("listType"))
		{
			setListType(document["listType"].GetInt());
		}
		if(document.HasMember("optionList") && !document["optionList"].IsNull() && document["optionList"].IsArray() )
		{
			const rapidjson::Value& arroptionList = document["optionList"];
			for (rapidjson::SizeType i = 0; i < arroptionList.Size(); i++)
			{
							const rapidjson::Value& c = arroptionList[i];	
			rapidjson::StringBuffer buffer;
			buffer.Clear();
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			c.Accept(writer);
			const char* json = buffer.GetString();
			OptionItem d;
			d.toData(json);
			optionList.push_back(d);

			}
		}


	}
}
