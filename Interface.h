//
// Created by kat on 7/13/22.
//

#ifndef LIBHEADER_INTERFACE_H
#define LIBHEADER_INTERFACE_H

#include <string>

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
    std::string Rendered() const { return m_rendered; };
};


#endif //LIBHEADER_INTERFACE_H
