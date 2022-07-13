//
// Created by kat on 7/13/22.
//

#import <stdlib.h>
#include <string>
#include <vector>

#ifndef LIBHEADER_IVAR_H
#define LIBHEADER_IVAR_H

using namespace std;

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


#endif //LIBHEADER_IVAR_H
