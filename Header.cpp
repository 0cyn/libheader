/*
 *
 * Copyright 2022 kat
 * gh/cxnder
 * twitter/arm64e
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *      following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *      disclaimer.
 *
 * 2. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 *      products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *      INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *      ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *      INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *      SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *      ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *      ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/
#include "Header.h"

using namespace HeaderParser;
using namespace std;


namespace HeaderParser
{

vector<string> split(string str, string token)
{
    vector<string> result;
    while (str.size())
    {
        int index = str.find(token);
        if (index != string::npos)
        {
            result.push_back(str.substr(0, index));
            str = str.substr(index + token.size());
            if (str.size() == 0)
                result.push_back(str);
        }
        else
        {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}


vector<string> splitUntilLimit(string str, string token, int limit)
{
    /*
     * Similar to python split so I dont have to think while writing this.
     * Removes token.
     * '*/
    vector<string> result;

    int timesSplit = 0;

    while (str.size())
    {
        int index = str.find(token);
        if (index != string::npos && timesSplit < limit)
        {
            result.push_back(str.substr(0, index));
            str = str.substr(index + token.size());
            if (str.size() == 0)
                result.push_back(str);
            timesSplit++;
        }
        else
        {
            result.push_back(str);
            str = "";
        }
    }
    return result;
}


Interface::Interface(ObjCClass *objCClass) : m_class(objCClass)
{
    string rendered;
    string renderedHTML;

    rendered.append("@interface ");
    renderedHTML.append(R"(<div class="builtin-fg">@interface</div> )");
    rendered.append(m_class->m_name);
    renderedHTML.append(R"(<div class="name">)" + m_class->m_name + "</div>");
    rendered.append(" : ");
    renderedHTML.append(" <div class=\"builtin-bg\">:</div> ");

    if (m_class->m_superclassName.has_value())
    {
        rendered.append(m_class->m_superclassName.value());
        renderedHTML.append(R"(<div class="type">)" + m_class->m_superclassName.value() + "</div>");
    }
    else
    {
        rendered.append("NSObject");
        renderedHTML.append(R"(<div class="type">NSObject</div>)");
    }
    if (!m_class->m_ivars.empty())
    {
        rendered.append(" {\n");
        renderedHTML.append( " <div class=\"builtin-bg\">{</div>\n\n");
        for (auto &ivar: m_class->m_ivars)
        {
            rendered.append("\t" + ivar->GetRendered());
            renderedHTML.append("\t" + ivar->GetRenderedHTML());
            rendered.append(";\n");
            renderedHTML.append(HTML_SEMICOLON + "\n\n");
        }
        rendered.append("}\n");
        renderedHTML.append("<div class=\"builtin-bg\">}</div>\n\n");
    }
    else
    {
        rendered.append(" \n");
        renderedHTML.append(" \n\n");
    }


    if (!m_class->m_properties.empty())
    {
        rendered.append("\n");
        renderedHTML.append("\n\n");
        for (auto &prop: m_class->m_properties)
        {
            rendered.append(prop->GetRendered());
            renderedHTML.append(prop->GetRenderedHTML());
            rendered.append(";\n");
            renderedHTML.append(HTML_SEMICOLON + "\n\n");
        }
        rendered.append("\n");
        renderedHTML.append("\n\n");
    }

    if (!(m_class->m_instanceMethods.empty() && m_class->m_classMethods.empty()))
    {
        rendered.append("\n");
        renderedHTML.append("\n\n");
        for (auto &meth: m_class->m_instanceMethods)
        {
            rendered.append(meth->GetRendered());
            renderedHTML.append(meth->GetRenderedHTML());
            rendered.append(";\n");
            renderedHTML.append(HTML_SEMICOLON + "\n\n");
        }
        for (auto &meth: m_class->m_classMethods)
        {
            rendered.append(meth->GetRendered());
            renderedHTML.append(meth->GetRenderedHTML());
            rendered.append(";\n");
            renderedHTML.append(HTML_SEMICOLON + "\n\n");
        }
        rendered.append("\n");
        renderedHTML.append("\n\n");
    }

    rendered.append("\n");
    renderedHTML.append("\n\n");

    rendered.append("@end");
    renderedHTML.append(R"(<div class="builtin-fg">@end</div> )");

    m_rendered = rendered;
    m_renderedHTML = renderedHTML;
}


Ivar::Ivar(string name, string typeString) : m_name(name), m_typeString(typeString)
{
    EncodedType *ivarType = TypeProcessor::Instance()->ProcessTypes(m_typeString).at(0);

    m_isClassType = (ivarType->GetType() != StructEncodedTypeType && ivarType->GetType() != NormalEncodedTypeType);

    m_renderedType = ivarType->GetRendered();
}

Ivar::~Ivar() = default;

string Ivar::GetRendered()
{
    string rendered;

    if (m_renderedType.rfind('<', 0) == 0)
        rendered.append("NSObject");
    rendered.append(m_renderedType + " ");
    if (m_isClassType)
        rendered.append("*");
    rendered.append(m_name);

    return rendered;
}

string Ivar::GetRenderedHTML()
{
    string rendered;

    if (m_renderedType.rfind('<', 0) == 0)
    {
        rendered.append("<div class=\"type\">NSObject</div>");
        rendered.append(R"(<div class="builtin-bg">&lt;</div><div class="type">)" + stringSlice(m_renderedType, 1, 1) + "</div><div class=\"builtin-bg\">&gt;</div> ");
    }
    else
        rendered.append(R"(<div class="type">)" + m_renderedType + "</div> ");
    if (m_isClassType)
        rendered.append("<div class=\"builtin-bg\">*</div>");
    rendered.append("<div class=\"name\">" + m_name + "</div>");

    return rendered;
}


Method::Method(const string &selector, const string &typeString, bool belongsToMetaClass) : m_selector(selector),
                                                                                            m_typeString(typeString),
                                                                                            m_belongsToMetaClass(belongsToMetaClass)
{
    m_types = TypeProcessor::Instance()->ProcessTypes(m_typeString);

    m_returnType = m_types.at(0)->GetRendered();
    int i = 0;
    for (EncodedType *type: m_types) {
        if (i == 0) {
            i++;
            continue;
        }
        m_argumentTypes.push_back(type->GetRendered());
    }

}

string Method::GetRendered() {
    string rendered;
    rendered.append(m_belongsToMetaClass ? "+" : "-");
    rendered.append("(" + m_returnType + ")");

    if (m_argumentTypes.size() <= 2)
    {
        rendered.append(m_selector);
        return rendered;
    }

    vector<string> selSlots = split(m_selector, ":");

    int i = 0;
    for (string &selSlot: selSlots)
    {
        if (m_argumentTypes.size() > i + 2)
            rendered.append(selSlot + ":(" + m_argumentTypes[i + 2] + ")arg" + std::to_string(i) + " ");
        else
            rendered.append(selSlot);
        i++;
    }

    return rendered;
}


string Method::GetRenderedHTML()
{
    string rendered;
    rendered.append(m_belongsToMetaClass ? "<div class=\"builtin-bg\">+</div>" : "<div class=\"builtin-bg\">-</div>");
    rendered.append(R"(<div class="builtin-bg">(</div><div class="type">)" + m_returnType + "</div><div class=\"builtin-bg\">)</div>");

    if (m_argumentTypes.size() <= 2)
    {
        rendered.append("<div class=\"name\">" + m_selector + "</div>");
        return rendered;
    }

    vector<string> selSlots = split(m_selector, ":");

    int i = 0;
    for (string &selSlot: selSlots)
    {
        if (m_argumentTypes.size() > i + 2)
            rendered.append("<div class=\"name\">" + selSlot + R"(</div><div class="name">:(</div><div class="type">)" + m_argumentTypes[i + 2] + "</div><div class=\"name\">)arg" + std::to_string(i) + "</div> ");
        else
            rendered.append("<div class=\"name\">" + selSlot + "</div>");
        i++;
    }

    return rendered;
}


ObjCClass::ObjCClass(string name) : m_name(name) {}


ObjCClass::~ObjCClass() = default;


void ObjCClass::AddProtocolName(const string &protocolName)
{
    m_protocolNames.push_back(protocolName);
}


void ObjCClass::AddIvar(const string &name, const string &attr)
{
    auto ivar = new Ivar(name, attr);
    m_ivars.push_back(ivar);
}


void ObjCClass::AddMethod(const string &sel, const string &attr, bool isClassMethod)
{
    auto method = new Method(sel, attr, isClassMethod);
    if (isClassMethod)
        m_classMethods.push_back(method);
    else
        m_instanceMethods.push_back(method);
}


void ObjCClass::AddProperty(const string &name, const string &attr)
{
    auto prop = new Property(name, attr);
    m_properties.push_back(prop);
}


Interface *ObjCClass::Interface()
{
    auto interface = new class Interface(this);
    return interface;
}


Property::Property(const string &name, const string &attrString) : m_name(name), m_attrString(attrString)
{
    map<string, string> attributeEncodings = {
            {"&", "retain"},
            {"N", "nonatomic"},
            {"W", "weak"},
            {"R", "readonly"},
            {"C", "copy"}
    };

    vector<string> attributeStrings = split(m_attrString, ",");
    for (string &attr: attributeStrings)
    {
        if (startsWith(attr, 'T')) // Type Field
        {
            m_typeString = stringFrom(attr, 1);
            EncodedType *propType = TypeProcessor::Instance()->ProcessTypes(m_typeString).front();
            m_renderedType = propType->GetRendered();
        }
        else if (startsWith(attr, 'V'))
            m_ivarName = stringFrom(attr, 1);
        else if (startsWith(attr, 'G'))
            m_getter = stringFrom(attr, 1);
        else if (startsWith(attr, 'S'))
            m_setter = stringFrom(attr, 1);
        else if (startsWith(attr, 'D'))
            m_isDynamic = true;
        else if (attributeEncodings.count(attr.substr(0, 1)))
            m_attributes.push_back(attributeEncodings.at(attr.substr(0, 1)));
        if (!m_getter.empty())
            m_attributes.push_back("getter=" + m_getter);
        if (!m_setter.empty())
            m_attributes.push_back("setter=" + m_setter);
    }
}

string Property::GetRendered()
{
    string rendered = "@property ";

    if (!m_attributes.empty())
    {
        rendered.append("(");
        int i = 0;
        int max = m_attributes.size() - 1;
        for (string &attr: m_attributes)
        {
            rendered.append(attr);
            if (i != max)
                rendered.append(", ");
            i++;
        }
        rendered.append(") ");
    }

    if (m_renderedType.rfind('<', 0) == 0)
        rendered.append("NSObject");
    rendered.append(m_renderedType + " ");
    if (m_isClassType)
        rendered.append("*");
    rendered.append(m_name);

    return rendered;
}

string Property::GetRenderedHTML() {

    string rendered = "<div class=\"builtin-fg\">@property</div> ";

    if (!m_attributes.empty())
    {
        rendered.append("<div class=\"builtin_bg\">(</div>");
        int i = 0;
        int max = m_attributes.size() - 1;
        for (string &attr: m_attributes)
        {
            rendered.append("<div class=\"builtin-fg\">" + attr + "</div>");
            if (i != max)
                rendered.append("<div class=\"builtin-bg\">,</div> ");
            i++;
        }
        rendered.append("<div class=\"builtin_bg\">)</div> ");
    }

    if (m_renderedType.rfind('<', 0) == 0)
    {
        rendered.append("<div class=\"type\">NSObject</div>");
        rendered.append(R"(<div class="builtin-bg">&lt;</div><div class="type">)" + stringSlice(m_renderedType, 1, 1) + "</div><div class=\"builtin-bg\">&gt;</div> ");
    }
    else
        rendered.append(R"(<div class="type">)" + m_renderedType + "</div> ");
    if (m_isClassType)
        rendered.append("<div class=\"builtin-bg\">*</div>");
    rendered.append("<div class=\"name\">" + m_name + "</div>");

    return rendered;

}

Property::~Property() = default;


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
    m_name = split(stringSlice(typeString, 1, 1), "=").at(0);

}


EncodedType::EncodedType(const string &typeStr, int pointerCount) : m_typeString(typeStr),
                                                                    m_pointerCount(pointerCount)
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
        auto *s = new EncodedStruct(typeStr);
        m_typeName = s->m_name;
    }
}

EncodedType::~EncodedType() = default;


string EncodedType::GetRendered()
{
    string rendered;
    rendered.append(m_typeName);
    for (int i = 0; i < m_pointerCount; i++)
    {
        rendered.append("*");
    }
    return rendered;
}

/*
*
* This processor system is roughly translated from the python ktool one, and as such:
* * is not perfect
* * is not optimized for C++
*
* */

vector<string> TypeProcessor::Tokenize(string &type) {
    vector<string> tokens;
    bool parsing_brackets = false;
    int bracket_count = 0;
    string buffer;

    for (char &c: type)
    {
        string ch = {c};
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
        } elif (kTypeEncodings.count(ch) or c == '^')
        {
            tokens.push_back(ch);
        } elif (c == '{')
        {
            buffer.push_back(c);
            parsing_brackets = true;
            bracket_count++;
        } elif (c == '"')
        {
            tokens.push_back(splitUntilLimit(type, "@", 1).at(1));
        }
    }

    return tokens;
}


vector<EncodedType *> TypeProcessor::ProcessTypes(string &typeString)
{
    if (m_typeStringCache.count(typeString))
        return m_typeStringCache.at(typeString);

    vector<EncodedType *> types;
    int pc = 0;

    vector<string> tokens = Tokenize(typeString);
    for (string &token: tokens)
    {
        if (token == "^")
            pc++;
        else
        {
            // In the type cache, we append the pointer count to key since that isn't included as a token we see.
            if (m_typeCache.count(token + to_string(pc)))
            {
                types.push_back(m_typeCache.at(token + to_string(pc)));
                continue;
            }
            auto type = new EncodedType(token, pc);
            m_typeCache[token + to_string(pc)] = type;
            pc = 0;
            types.push_back(type);
        }
    }

    return types;
}

}