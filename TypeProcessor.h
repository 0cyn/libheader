//
// Created by kat on 7/13/22.
//

#import <stdlib.h>
#include <string>
#include <set>
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

class EncodedStruct {
public:
    string m_name;
    set<string> m_fields;
    set<string> m_fieldNames;

    EncodedStruct(string typeString);
    ~EncodedStruct();

    string GetRendered();
};

class EncodedType {
private:
    int m_pointerCount;
    EncodedTypeType m_type;
    string m_typeString;

    string m_typeName; // even if its a struct, write this. it's going to be what we render, with structs,
                       //   we just need to make sure its a valid name. that will not be done in this context.
public:
    explicit EncodedType(const string& typeStr, int pointerCount = 0);
    ~EncodedType();

    [[nodiscard]] EncodedTypeType GetType() const { return m_type; };

    string GetRendered();
};

class TypeProcessor {
private:
    map<string, EncodedStruct*> m_structCache;
    map<string, vector<EncodedType*>> m_typeStringCache;
    map<string, EncodedType*> m_typeCache;

    void saveStruct(EncodedStruct* structType);
public:
    TypeProcessor() {};
    ~TypeProcessor() = default;

    TypeProcessor(TypeProcessor const&) = delete;
    TypeProcessor& operator=(TypeProcessor const&) = delete;

    static shared_ptr<TypeProcessor> Instance()
    {
        static shared_ptr<TypeProcessor> instance{new TypeProcessor};
        return instance;
    }

    vector<EncodedType *> ProcessTypes(string& typeString);
    static vector<string> Tokenize(string& type);
};


#endif //LIBHEADER_TYPEPROCESSOR_H
