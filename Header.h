//
// Created by kat on 7/13/22.
//

#include <stdlib.h>
#include <string>
#include <vector>
#include <optional>
#include <map>
#include <set>

#ifndef LIBHEADER_HEADER_H
#define LIBHEADER_HEADER_H


#define startsWith(aVar, bVar) aVar.rfind(bVar, 0) == 0
#define stringFrom(str, start) str.substr(start, str.size()-start)
#define stringTill(str, till) str.substr(0, till)
#define stringSlice(str, startOffset, endOffset) str.substr(startOffset, str.size()-endOffset)
#define elif else if



using namespace std;

namespace HeaderParser
{

    vector<string> split(string str, string token);
    vector<string> splitUntilLimit(string str, string token, int limit);

    class ObjCClass;
    class Property;
    class Method;
    class Ivar;

    class Interface {
    private:
        ObjCClass* m_class;
        std::string m_rendered;
    public:
        explicit Interface(ObjCClass *objCClass);
        std::string GetRendered() const { return m_rendered; };
    };

    class Ivar {
    private:
        string m_name;
        string m_typeString;

        bool m_isClassType;
        string m_renderedType;
    public:
        Ivar(string name, string typeString);
        ~Ivar();

        string GetRendered();
    };

    class EncodedType;

    class Method {
    private:
        bool m_belongsToMetaClass; // if then render with + instead of -
        string m_selector;

        string m_typeString;

        string m_returnType;
        vector<string> m_argumentTypes; // FIRST TWO ARE SELF + CMD!

        vector<EncodedType*> m_types;

    public:
        Method(const string& selector, const string& typeString, bool belongsToMetaClass);
        ~Method();

        string GetRendered();
    };

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

        explicit ObjCClass(string name);
        ~ObjCClass();

        void SetSuperclass(string superclassName) { m_superclassName = superclassName; };

        void AddProtocolName(const string& protocolName);

        void AddIvar(const string& name, const string& attr);
        void AddMethod(const string& sel, const string& attr, bool isClassMethod);
        void AddProperty(const string& name, const string& attr);

        Interface* Interface();
    };

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
        Property(const string& name, const string& attrString);
        ~Property();

        string GetRendered();
    };

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
        TypeProcessor() = default;
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
}


#endif //LIBHEADER_HEADER_H
