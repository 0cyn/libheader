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
    Interface* m_interface;
public:
    string m_name;
    optional<string> m_superclassName;
    vector<string> m_protocolNames;
    vector<Method*> m_instanceMethods;
    vector<Method*> m_classMethods;
    vector<Property*> m_properties;
    vector<Ivar*> m_ivars;

    explicit ObjCClass(string& name);
    ~ObjCClass();

    void SetSuperclass(string superclassName) { m_superclassName = superclassName; };

    void AddProtocolName(const string& protocolName);

    void AddIvar(const string& name, const string& attr);
    void AddMethod(const string& sel, const string& attr, bool isClassMethod);
    void AddProperty(const string& name, const string& attr);

    Interface* RenderedInterface();
};


#endif //LIBHEADER_OBJCCLASS_H
