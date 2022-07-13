cmake_minimum_required(VERSION 3.21)
project(libHeader)

set(CMAKE_CXX_STANDARD 17)

add_library(libHeader library.cpp Header.cpp Header.h Interface.cpp Interface.h ObjCClass.cpp ObjCClass.h ImportManager.cpp ImportManager.h TypeHandler.cpp TypeHandler.h ObjCProtocol.cpp ObjCProtocol.h ObjCCategory.cpp ObjCCategory.h Method.cpp Method.h Property.cpp Property.h Ivar.cpp Ivar.h ObjCImage.cpp ObjCImage.h TypeProcessor.cpp TypeProcessor.h)
