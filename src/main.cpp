// clang-format off
#include <winsock2.h>
#include <objidl.h>
#include <windows.h>
#include <gdiplus.h>
// clang-format on

#include <iostream>

#include "Parser.hpp"

using namespace rapidxml;

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <svg file>" << std::endl;
        return 1;
    }
    Parser *parser = Parser::getInstance(argv[1]);
    SVGElement *root = parser->getRoot();
    root->printData();
    delete parser;
    return 0;
}