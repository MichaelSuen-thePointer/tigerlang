#include <iostream>
#include <fstream>
#include "TigerScanner.hpp"
#include "tiger.tab.hh"
#include "TigerFrame.hpp"
#include "TigerIRT.hpp"
int main()
{
    tiger::TigerScanner scanner(&std::cin);
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
    }
    try
    {
        prog->semanticCheck();
        std::cout << "No syntax error or type error\n";
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << "\n";
        return -1;
    }
    tiger::Frame f;
    auto ir = prog->expression()->toIR(f);
    std::ofstream outFile("IROut.json");
    ir->dump(outFile);

    for (auto& func : f.functionFragments())
    {
        std::ofstream funcDump(std::string("IR") + func.first + ".json");
        func.second->dump(funcDump);
    }
}