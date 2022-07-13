//
// Created by kat on 7/13/22.
//
#import <stdlib.h>
#include <string>
#include <vector>
#include <optional>

#ifndef LIBHEADER_PROPERTY_H
#define LIBHEADER_PROPERTY_H

using namespace std;

enum Attribute {
    RETAIN = 0,
    NONATOMIC,
    WEAK,
    READONLY,
    COPY
};

string attributeEncodingToRenderableAttribute();
Attribute encodedAttributeCharacterToAttribute();

class Property {
private:
    string m_name;
    string m_attrString;

    string m_typeString;
    string m_getter;
    string m_setter;

    vector<string> m_attributes;

    optional<string> m_ivarName;

    bool m_isDynamic; // TODO: i have literally no idea what this is, just that it exists and gets rendered differently.
    bool m_isClassType;
    string m_renderedType;

public:
    Property(string& name, string& attrString);
    ~Property();

    string GetRendered();
};


#endif //LIBHEADER_PROPERTY_H
