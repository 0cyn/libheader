//
// Created by kat on 7/13/22.
//

#include "Method.h"
#include "TypeProcessor.h"


vector<string> split(string str, string token);


Method::Method(const string& selector, const string& typeString, bool belongsToMetaClass) : m_selector(selector),
                                                                                m_typeString(typeString),
                                                                                m_belongsToMetaClass(belongsToMetaClass)
{
    m_types = TypeProcessor::Instance()->ProcessTypes(m_typeString);

    m_returnType = m_types.at(0)->GetRendered();
    int i = 0;
    for (EncodedType* type : m_types)
    {
        if (i == 0) { i++; continue; }
        m_argumentTypes.push_back(type->GetRendered());
    }

}

string Method::GetRendered()
{
    string rendered;
    rendered.append(m_belongsToMetaClass ? "+" : "-");
    rendered.append("(" + m_returnType + ")");

    if (m_argumentTypes.size() <= 2)
    {
        rendered.append(m_selector);
        return rendered;
    }

    vector<string> selSlots = split(m_selector, ":");

    int i = 0;
    for (string& selSlot : selSlots)
    {
        rendered.append(selSlot + ":(" + m_argumentTypes[i+2] + ")arg" + std::to_string(i) + " ");
    }

    return rendered;
}