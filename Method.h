//
// Created by kat on 7/13/22.
//

#import <stdlib.h>
#include <string>
#include <vector>
#include <optional>

#ifndef LIBHEADER_METHOD_H
#define LIBHEADER_METHOD_H

using namespace std;

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


#endif //LIBHEADER_METHOD_H
