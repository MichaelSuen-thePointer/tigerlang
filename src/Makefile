testmain.out : *.hpp *.cpp *.cc *.hh
	g++ -std=c++14 ASTSemantic.cpp lex.yy.cc SymbolTable.cpp testmain.cpp tiger.tab.cc TigerAST.cpp TigerScanner.cpp ASTIR.cpp IRDump.cpp TigerIRT.cpp TigerFrame.cpp IRTGraphviz.cpp generate.cpp -o testmain.out

lex.yy.cc : tiger.l
	flex tiger.l

tiger.tab.cc tiger.tab.hh location.hh position.hh stack.hh: tiger.yy
	bison -tv tiger.yy
