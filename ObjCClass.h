//
// Created by kat on 7/13/22.
//

#import <stdlib.h>
#include <string>
#include <optional>
#include <vector>

#include "Method.h"
#include "Property.h"
#include "Ivar.h"

#include "Interface.h"

#ifndef LIBHEADER_OBJCCLASS_H
#define LIBHEADER_OBJCCLASS_H

using namespace std;

class ObjCClass {
private:
    string m_name;
    optional<string> m_superclassName;
    vector<string> m_protocolNames;

    vector<Method*> m_instanceMethods;
    vector<Method*> m_classMethods;
    vector<Property*> m_properties;
    vector<Ivar*> m_ivars;
public:
    void AddIvar(string name, string attr);
    void AddMethod(string sel, string attr, bool isMeta);
    void AddProperty(string name, string attr);

    Interface* RenderedInterface();
};


#endif //LIBHEADER_OBJCCLASS_H
