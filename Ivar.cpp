//
// Created by kat on 7/13/22.
//

#include "Ivar.h"
#include "TypeProcessor.h"

#include <utility>

Ivar::Ivar(string name, string typeString) : m_name(std::move(name)), m_typeString(std::move(typeString))
{
    EncodedType ivarType = TypeProcessor::Instance()->ProcessTypes(m_typeString).front();

    m_isClassType = (ivarType.GetType() != StructEncodedTypeType && ivarType.GetType() != NormalEncodedTypeType);

    m_renderedType = ivarType.GetRendered();
}

Ivar::~Ivar() = default;

string Ivar::GetRendered() {
    string rendered;

    if (m_renderedType.rfind('<', 0) == 0)
        rendered.append("NSObject");
    rendered.append(m_renderedType + " ");
    if (m_isClassType)
        rendered.append("*");
    rendered.append(m_name);

    return rendered;
}



