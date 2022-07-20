//
// Created by kat on 7/13/22.
//

#include "TypeProcessor.h"

#define startsWith(aVar, bVar) aVar.rfind(bVar, 0) == 0
#define stringFrom(str, start) str.substr(start, str.size()-start)
#define stringTill(str, till) str.substr(0, till)
#define stringSlice(str, startOffset, endOffset) str.substr(startOffset, str.size()-endOffset)
#define elif else if


vector<string> splitUntilLimit(string str, string token, int limit){
    /*
     * Split shit. Similar to python split so I dont have to think while writing this.
     * Removes token.
     * '*/
    vector<string>result;

    int timesSplit = 0;

    while(str.size()){

        int index = str.find(token);
        if (index!=string::npos && timesSplit < limit)
        {
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)
                result.push_back(str);
            timesSplit++;
        } else
        {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}


static map<string, string> kTypeEncodings = {
        {"c", "char"},
        {"i", "int"},
        {"s", "short"},
        {"l", "long"},
        {"q", "NSInteger"},
        {"C", "unsigned char"},
        {"I", "unsigned int"},
        {"S", "unsigned short"},
        {"L", "unsigned long"},
        {"A", "uint8_t"},
        {"Q", "NSUInteger"},
        {"f", "float"},
        {"d", "CGFloat"},
        {"b", "BOOL"},
        {"@", "id"},
        {"B", "BOOL"},
        {"v", "void"},
        {"*", "char *"},
        {"#", "Class"},
        {":", "SEL"},
        {"?", "unk"},
        {"T", "unk"}
};


EncodedStruct::EncodedStruct(string typeString)
{

}


EncodedType::EncodedType(const string &typeStr, int pointerCount) : m_typeString(typeStr), m_pointerCount(pointerCount)
{
    string start = stringTill(typeStr, 1);

    if (kTypeEncodings.count(start))
    {
        m_type = NormalEncodedTypeType;
        m_typeName = kTypeEncodings.at(start);
    }

    elif (start == "\"")
    {
        m_type = NamedEncodedTypeType;
        m_typeName = stringSlice(typeStr, 1, 1);
    }

    elif (start == "{")
    {
        m_type = StructEncodedTypeType;
        auto* s = new EncodedStruct(typeStr);
        m_typeName = s->m_name;
    }
}

EncodedType::~EncodedType() = default;


string EncodedType::GetRendered()
{
    string rendered;
    for (int i = 0; i < m_pointerCount; i++)
    {
        rendered.append("*");
    }
    rendered.append(m_typeName);
    return rendered;
}

/*
 *
 * This processor system is roughly translated from the python ktool one, and as such:
 * * is not perfect
 * * is not optimized for C++
 * * cannot handle crazy ObjC++ types
 *
 * */

vector<string> TypeProcessor::Tokenize(string& type)
{
    vector<string> tokens;
    bool parsing_brackets = false;
    int bracket_count = 0;
    string buffer;

    for (char& c : type)
    {
        if (parsing_brackets)
        {
            buffer.push_back(c);
            if (c == '{')
                bracket_count++;
            elif (c == '}')
            {
                bracket_count--;
                if (bracket_count == 0)
                {
                    tokens.push_back(buffer);
                    buffer.clear();
                    parsing_brackets = false;
                }
            }
        }
        elif (kTypeEncodings.count(to_string(c)) or c == '^')
        {
            tokens.push_back(to_string(c));
        }
        elif (c == '{')
        {
            buffer.push_back(c);
            parsing_brackets = true;
            bracket_count++;
        }
        elif (c == '"')
        {
            tokens.push_back(splitUntilLimit(type, "@", 1).at(1));
        }
    }

    return tokens;
}


vector<EncodedType*> TypeProcessor::ProcessTypes(string& typeString)
{
    if (m_typeStringCache.count(typeString))
        return m_typeStringCache.at(typeString);

    vector<EncodedType* > types;
    int pc = 0;

    vector<string> tokens = Tokenize(typeString);
    for (string& token : tokens)
    {
        if (token == "^")
            pc++;
        else
        {
            if (m_typeCache.count(token))
            {
                types.push_back(m_typeCache.at(token));
                continue;
            }
            auto type = new EncodedType(token, pc);
            pc = 0;
            m_typeCache[token] = type;
            types.push_back(type);
        }
    }

    return types;
}


