testmain.out :
	g++ -std=c++11 ASTSemantic.cpp lex.yy.cc SymbolTable.cpp testmain.cpp tiger.tab.cc TigerAST.cpp TigerScanner.cpp -o testmain.exe

lex.yy.cc : tiger.l
	flex tiger.l

tiger.tab.cc tiger.tab.hh location.hh position.hh stack.hh: tiger.yy
	bison -tv tiger.yy
