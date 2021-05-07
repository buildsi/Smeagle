#include <fmt/format.h>
#include <smeagle/smeagle.h>

#include <regex>

#include "Function.h"
#include "Symtab.h"

using namespace Dyninst;
using namespace SymtabAPI;
using namespace smeagle;

Smeagle::Smeagle(std::string _library) : library(std::move(_library)) {}

// Get a string location from a Type
std::string Smeagle::getStringLocationFromType(Type *paramType, int order) {
  // We need a string version of the type
  std::string paramTypeString = paramType->getName();

  // Signed and unsigned Bool,char,short,int,long,long long, and pointers
  std::regex checkinteger("(int|char|short|long|pointer|bool)");

  // Is it a constant?
  std::regex checkconstant("const");

  // Does the type string match one of the types?
  bool isinteger = (std::regex_match(paramTypeString, checkinteger));
  bool isconst = (std::regex_match(paramTypeString, checkconstant));

  // This is the final location string we will return
  std::string loc = "unknown";

  // I think constants are stored on the stack?
  // TODO this regex is not working
  if (isconst) {
    loc = "stack";
  } else if (isinteger) {
    switch (order) {
      case 1: {
        loc = "%rdi";
        break;
      }
      case 2: {
        loc = "%rsi";
        break;
      }
      case 3: {
        loc = "%rdx";
        break;
      }
      case 4: {
        loc = "%rcx";
        break;
      }
      case 5: {
        loc = "%r8";
        break;
      }
      case 6: {
        loc = "%r9";
        break;
      }
      // Greater than 6 is stored in memory
      default: { loc = "memory"; }
    }
  }

  // TODO check for double and float
  // float,double,_Decimal32,_Decimal64and__m64are in class SSE.
  // SSE = ['double', 'decimal']

  // elif die_type in SSE and order <= 8:
  //    return "%xmm" + str(order - 1)

  return loc;
}

// Given a symbol, get a string representation of its type
std::string Smeagle::getStringSymbolType(Symbol *symbol) {
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

// parse a function for parameters and abi location
void Smeagle::parseFunctionABILocation(Symbol *symbol) {
  // Get the name and type of the symbol
  std::string sname = symbol->getMangledName();
  std::string stype = Smeagle::getStringSymbolType(symbol);

  Function *func = symbol->getFunction();
  std::vector<localVar *> params;

  // The function name looks equivalent to the symbol name
  std::string fname = func->getName();
  std::cout << "interface(" << fname << ")"
            << "\n";

  // Get parameters with types and names
  if (func->getParams(params)) {
    // We need to keep track of the order
    int order = 1;
    for (auto &param : params) {
      std::string paramName = param->getName();
      Type *paramType = param->getType();
      std::string loc = getStringLocationFromType(paramType, order);

      // Note that we don't need style or paramName, this is for debugging
      std::cout << "abi_typelocation(" << stype << ", " << library << ", " << fname << ", "
                << paramName << ", " << paramType->getName() << ", " << loc << ")"
                << "\n";
      order += 1;
    }
  }
}

// Parse the library with smeagle
// Eventually we want to have a library - go back and replace main function
// with something that takes a symtab
int Smeagle::parse(FormatCode fmt) {
  // This isn't used yet, we would eventually want to generate outpyt
  switch (fmt) {
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
  std::vector<Symbol *> symbols;
  std::vector<Function *> funcs;

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

  // Define the corpus
  std::cout << "corpus(" << library << ")"
            << "\n";

  // Loop through the vector and look at symbols
  for (auto &symbol : symbols) {
    // We are interested in symbols in the dynamic symbol table
    if (symbol->isInDynSymtab()) {
      // If It's a function, parse the parameters
      if (symbol->isFunction()) {
        parseFunctionABILocation(symbol);
        // The symbol is something else (we likely want a subset of these)
      }  // else {
         // std::cout << "symbol_notparsed(" <<  sname << ")" << "\n";
      //}
    }
  }
  return 0;
}
