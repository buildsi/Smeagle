#include <fmt/format.h>
#include <smeagle/smeagle.h>
#include "Symtab.h"
#include "Function.h"

using namespace Dyninst;
using namespace SymtabAPI;
using namespace smeagle;

Smeagle::Smeagle(std::string _library) : library(std::move(_library)) {}

// Given a symbol, get a string representation of its type
std::string Smeagle::getStringSymbol(Symbol * symbol) {

  Symbol::SymbolType stype = symbol->getType();
  std::string sname;

  switch (stype) {
    case (Symbol::SymbolType::ST_FUNCTION): {
      sname = "function";
      break;
    }
    case (Symbol::SymbolType::ST_UNKNOWN): {
      sname = "unknown";
      break;
    }
    case (Symbol::SymbolType::ST_OBJECT): {
      sname = "object";
      break;
    }
    case (Symbol::SymbolType::ST_SECTION): {
      sname = "section";
      break;
    }
    case (Symbol::SymbolType::ST_MODULE): {
      sname = "module";
      break;
    }
    case (Symbol::SymbolType::ST_TLS): {
      sname = "tls";
      break;
    }
    case (Symbol::SymbolType::ST_DELETED): {
      sname = "deleted";
      break;
    }
    case (Symbol::SymbolType::ST_INDIRECT): {
      sname = "indirect";
      break;
    }
    case (Symbol::SymbolType::ST_NOTYPE): {
      sname = "notype";
      break;
    }
  }
  return sname;
}

// Parse the library with smeagle
int Smeagle::parse(FormatCode fmt) {

  switch(fmt) {
   default:
   case FormatCode::Json:
     std::cout << "You chose json.\n";
     break;
   case FormatCode::Terminal:
     std::cout << "You chose terminal.\n";
     break;
   case FormatCode::Asp:
     std::cout << "% You chose asp.\n";
     break;
  }
  
  // We are going to read functions and symbols
  Symtab *obj = NULL;
  std::vector <Symbol *> symbols;
  std::vector <Function *> funcs;

  // Read the library into the Symtab object, cut out early if there's error
  if (not Symtab::openFile(obj, library)) {
    std::cout << "There was a problem reading " << library << "\n";
    return 1;  
  }

  // Get all functions in the library
  if (not obj->getAllFunctions(funcs)) {
    std::cout << "There was a problem getting functions from " << library << "\n";
    return 1;
  }

  // Get all functions in the library
  // Note: looping through this doesn't seem to work
  if (not obj->getAllSymbols(symbols)) {
    std::cout << "There was a problem getting symbols from " << library << "\n";
    return 1;
  }

  // Loop through the vector and look at symbols
  for(auto & symbol : symbols) {
    
    // We are interested in symbols in the dynamic symbol table
    if (symbol->isInDynSymtab()){

      std::string sname = symbol->getMangledName();

      // If It's a function, get the parameters.
      if (symbol->isFunction()) {
          Function *func = symbol->getFunction();
          std::string fname = func->getName();
          std::cout << "function(" <<  sname << "," << fname << ")" << "\n";

          // TODO get parameters here
      }
    
      // Get the type of the symbol (this doesn't compile because Symbol.h cannot be found in include)
      // std::string stype = getStringSymbol(symbol);

      std::cout << "symbol(" <<  sname << ")" << "\n";
      // std::cout << "symbol_type(" <<  sname << "," <<  stype << ")" << "\n";
    }
  }
  return 0;
}


  // Get all the functions (call out of symtab, populates vector)
  // functions have symbols, so from symbols we can determine if from dynamic symbols table
  // export to those exporte dout of dynamic symbols table
  // functions also have parameters (can say get the params, will fill vector)
  // the parameters have type information, we'll want to look at the type, ABI standard, and
  // decide what location this type at this location would go into. 
  // Eventually we want to have a library - go back and replace main function
  // with something that takes a symtab
  // open target problem, do for all functions exported.

