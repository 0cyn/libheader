//
// Created by kat on 7/13/22.
//

#include "Interface.h"

#include "ObjCClass.h"

using namespace std;

Interface::Interface(ObjCClass* objCClass) : m_class(objCClass)
{
    string rendered;

    rendered.append("@interface ");
    rendered.append(m_class->m_name);
    rendered.append(" : ");

    if (m_class->m_superclassName.has_value())
        rendered.append(m_class->m_superclassName.value());
    else
        rendered.append("NSObject");

    if (!m_class->m_ivars.empty())
    {
        rendered.append(" {\n");
        for (auto& ivar : m_class->m_ivars)
        {
            rendered.append(ivar->GetRendered());
            rendered.append(";\n");
        }
        rendered.append("}");
    }
    else
        rendered.append(" \n");

    if (!m_class->m_properties.empty())
    {
        rendered.append("\n");
        for (auto& prop : m_class->m_properties)
        {
            rendered.append(prop->GetRendered());
            rendered.append(";\n");
        }
        rendered.append("\n");
    }

    if (!(m_class->m_instanceMethods.empty() && m_class->m_classMethods.empty()))
    {
        rendered.append("\n");
        for (auto& meth : m_class->m_instanceMethods)
        {
            rendered.append(meth->GetRendered());
            rendered.append(";\n");
        }
        for (auto& meth : m_class->m_classMethods)
        {
            rendered.append(meth->GetRendered());
            rendered.append(";\n");
        }
        rendered.append("\n");
    }

    rendered.append("\n");

    rendered.append("@end");

    m_rendered = rendered;
}
