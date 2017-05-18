#define YYDEBUG 1
#include <iostream>
#include <fstream>
#include "TigerScanner.hpp"
#include "tiger.tab.hh"

int main()
{
    std::ifstream ifs("../testcases/queens.tig");
    tiger::TigerScanner scanner(&ifs);
    std::unique_ptr<tiger::Program> prog;
    tiger::TigerParser parser(scanner, prog);
    //parser.set_debug_level(1);
    //scanner.set_debug(1);
    if (parser.parse() != 0)
    {
        std::cout << "Parse failed\n";
    }
    else
    {
        std::cout << "Parse succeed\n";
        prog->graphviz();
        std::cout << prog->graphvizCode;
    }
    try
    {
        prog->semanticCheck();
        std::cout << "No syntax error or type error\n";
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << "\n";
    }
}