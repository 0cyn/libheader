//
// Created by kat on 7/20/22.
//

#include "Header.h"

using namespace HeaderParser;

int main()
{

    // Automatic full class serialization:

    auto oclass = new ObjCClass("TestClass1");

    oclass->AddIvar("_testPropertyTwo", "c");
    oclass->AddMethod("setTestPropertyOne:isCool:", "v48@0:8{CGRect={CGPoint=dd}{CGSize=dd}}16^B", false);
    oclass->AddMethod("testInstanceMethodOne", "Q16@0:8", false);
    oclass->AddMethod("testClassMethodOne", "c16@0:8", true);
    oclass->AddProperty("testPropertyOne", "T{CGRect={CGPoint=dd}{CGSize=dd}},V_testPropertyOne");
    oclass->AddProperty("testPropertyTwo", "Tc,R,V_testPropertyTwo");

    auto s = oclass->Interface()->GetRendered();

    printf("%s\n", s.c_str());

    // Single item serialization is also easy, and has the exact same method signature as above.
    // Maybe this would make "click to jump to IMP" easier to do

    auto prop = new Property("testPropertyThree", "TB,R,V_testPropertyThree");
    printf("%s;\n", prop->GetRendered().c_str());
}
