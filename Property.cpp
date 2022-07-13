//
// Created by kat on 7/13/22.
//

#include "Property.h"
#include "TypeProcessor.h"

vector<string> split(string str, string token){
    vector<string>result;
    while(str.size()){
        int index = str.find(token);
        if (index!=string::npos)
        {
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)
                result.push_back(str);
        } else
        {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

#define startsWith(aVar, bVar) aVar.rfind(bVar, 0) == 0
#define stringFrom(str, start) str.substr(start, attr.size()-start)

Property::Property(string& name, string& attrString) : m_name(name), m_attrString(attrString)
{
    map<string, string> attributeEncodings = {
            {"&", "retain"},
            {"N", "nonatomic"},
            {"W", "weak"},
            {"R", "readonly"},
            {"C", "copy"}
    };

    vector<string> attributeStrings = split(m_attrString, ",");
    for (string& attr : attributeStrings)
    {
        if (startsWith(attr, 'T')) // Type Field
        {
            m_typeString = stringFrom(attr, 1);
            EncodedType propType = TypeProcessor::Instance()->ProcessTypes(m_typeString).front();
            m_renderedType = propType.GetRendered();
        }
        else if (startsWith(attr, 'V'))
        {
            m_ivarName = stringFrom(attr, 1);
        }
        else if (startsWith(attr, 'G'))
        {
            m_getter = stringFrom(attr, 1);
        }
        else if (startsWith(attr, 'S'))
        {
            m_setter = stringFrom(attr, 1);
        }
        else if (startsWith(attr, 'D'))
            m_isDynamic = true;
        else if (attributeEncodings.count(attr.substr(0, 1)))
            m_attributes.push_back(attributeEncodings.at(attr.substr(0, 1)));
        if (!m_getter.empty())
            m_attributes.push_back("getter=" + m_getter);
        if (!m_setter.empty())
            m_attributes.push_back("setter=" + m_setter);
    }
}

string Property::GetRendered()
{
    string rendered = "@property ";

    if (!m_attributes.empty())
    {
        rendered.append("(");
        int i = 0;
        int max = m_attributes.size() - 1;
        for (string& attr : m_attributes)
        {
            rendered.append(attr);
            if (i != max)
                rendered.append(", ");
            i++;
        }
        rendered.append(") ");
    }

    if (m_renderedType.rfind('<', 0) == 0)
        rendered.append("NSObject");
    rendered.append(m_renderedType + " ");
    if (m_isClassType)
        rendered.append("*");
    rendered.append(m_name);

    return rendered;
}

Property::~Property() = default;


