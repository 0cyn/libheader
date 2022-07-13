//
// Created by kat on 7/13/22.
//

#import <stdlib.h>
#include <string>
#include <optional>
#include <vector>
#include <map>

#ifndef LIBHEADER_TYPEPROCESSOR_H
#define LIBHEADER_TYPEPROCESSOR_H

using namespace std;

// needs to be a singleton so we can cache structs that we decode

enum EncodedTypeType { // sorry lazily avoiding naming conflicts
    StructEncodedTypeType = 0,
    NamedEncodedTypeType,
    IdEncodedTypeType,
    NormalEncodedTypeType
};

class EncodedType {
private:
    int m_pointerCount;
    EncodedTypeType m_type;

    string m_typeName; // even if its a struct, write this. it's going to be what we render, with structs,
                       //   we just need to make sure its a valid name. that will not be done in this context.
public:
    EncodedType();
    ~EncodedType();

    EncodedTypeType GetType() const { return m_type; };

    string GetRendered();
};

class TypeProcessor {
private:
    map<string, vector<string>> m_structCache;
    vector<string> tokenize(string type);
    void saveStruct();
public:
    TypeProcessor();
    ~TypeProcessor();

    static TypeProcessor* Instance();

    vector<EncodedType> ProcessTypes(string typeString);
};


#endif //LIBHEADER_TYPEPROCESSOR_H
