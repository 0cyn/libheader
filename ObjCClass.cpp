//
// Created by kat on 7/13/22.
//

#include "ObjCClass.h"

ObjCClass::ObjCClass(string& name) : m_name(name)
{

}


ObjCClass::~ObjCClass() = default;


void ObjCClass::AddProtocolName(const string& protocolName)
{
    m_protocolNames.push_back(protocolName);
}


void ObjCClass::AddIvar(const string& name, const string& attr)
{
    auto ivar = new Ivar(name, attr);
    m_ivars.push_back(ivar);
}


void ObjCClass::AddMethod(const string& sel, const string& attr, bool isClassMethod)
{
    auto method = new Method(sel, attr, isClassMethod);
    if (isClassMethod)
        m_classMethods.push_back(method);
    else
        m_instanceMethods.push_back(method);
}


void ObjCClass::AddProperty(const string& name, const string& attr)
{
    auto prop = new Property(name, attr);
    m_properties.push_back(prop);
}


Interface* ObjCClass::RenderedInterface()
{
    auto interface = new Interface(this);
    return interface;
}
