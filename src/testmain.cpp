#include <iostream>
#include <fstream>
#include "TigerScanner.hpp"
#include "tiger.tab.hh"
#include "TigerIRT.hpp"
#include "TigerFrame.hpp"

int main()
{
    freopen("../src/testcases/queens.tig", "r", stdin);
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
        prog->graphviz();
        std::ofstream outAST("AST.gv");
        outAST << prog->graphvizCode;
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << "\n";
        return -1;
    }
    tiger::Frame f;
    auto ir = prog->expression()->toIR(f);
    std::ofstream outFile("IROut.json");
    std::ofstream outFile2("IROut.gv");
    ir->dump(outFile);
    ir->graphviz(outFile2);

    for (auto& func : f.functionFragments())
    {
        std::ofstream funcDump(std::string("IR") + func.first + ".json");
        func.second->dump(funcDump);
        std::ofstream funcDump2(std::string("IR") + func.first + ".gv");
        func.second->graphviz(funcDump2);
    }

    std::ofstream outFile3("strings.json");
    outFile3 << "{\n";
    int c = 0;
    for (auto& str : f.stringFragments()) {
        if (c != 0) outFile3 << ",\n";
        outFile3 << "\"" << str.first << "\":" << str.second;
        c++;
    }
    outFile3 << "}\n";
}