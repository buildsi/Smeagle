#include <fmt/format.h>
#include <smeagle/corpora.h>
#include <regex>
#include <iostream>
#include <fmt/core.h>

#include "Function.h"
#include "Symtab.h"

using namespace Dyninst;
using namespace SymtabAPI;
using namespace smeagle;

Corpus::Corpus(std::string _library) : library(std::move(_library)) {};

// Given a symbol, get a string representation of its type
std::string Corpus::getStringSymbolType(Symbol *symbol) {
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

// Get a string location from a Type
std::string Corpus::getStringLocationFromType(Type *paramType, int order) {
  // We need a string version of the type
  std::string paramTypeString = paramType->getName();

  // Signed and unsigned Bool,char,short,int,long,long long, and pointers
  std::regex checkinteger("(int|char|short|long|pointer|bool)");

  // Is it a constant?
  std::regex checkconstant("(const)");

  // float,double,_Decimal32,_Decimal64and__m64are in class SSE.
  std::regex checksse("(double|decimal|float|Decimal|m64)");

  // Does the type string match one of the types?
  bool isinteger = (std::regex_search(paramTypeString, checkinteger));
  bool isconst = (std::regex_search(paramTypeString, checkconstant));
  bool issse = (std::regex_search(paramTypeString, checksse));

  // Default to its own type (so we can see)
  std::string loc = paramTypeString;

  // I think constants are stored on the stack?
  if (isconst) {
    loc = "stack";
  } else if (issse) {
    loc = fmt::format("{}mm", order-1);
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

  return loc;
}

// dump all Type Locations to asp
void Corpus::toAsp() {

    std::cout << "corpus(" << library << ")," << std::endl; 
    for (auto &typeloc : typelocs) {

        std::cout << "abi_typelocation(" << library << ", " << typeloc.type 
                  << ", " << typeloc.parent << ", " << typeloc.name << ", "
                  << typeloc.type << "," << typeloc.location << std::endl;

    }
}

// parse a function for parameters and abi location
void Corpus::parseFunctionABILocation(Symbol *symbol) {
  // Get the name and type of the symbol
  std::string sname = symbol->getMangledName();
  std::string stype = Corpus::getStringSymbolType(symbol);

  Function *func = symbol->getFunction();
  std::vector<localVar *> params;

  // The function name looks equivalent to the symbol name
  std::string fname = func->getName();

  // Get parameters with types and names
  if (func->getParams(params)) {
    // We need to keep track of the order
    int order = 1;
    for (auto &param : params) {
      std::string paramName = param->getName();
      Type *paramType = param->getType();
      std::string loc = getStringLocationFromType(paramType, order);

      // Create a new typelocation to parse later
      TypeLocation typeloc;
      typeloc.name = paramName;
      typeloc.parent = fname;
      typeloc.type = paramType->getName();
      typeloc.exportOrImport = "export";
      typeloc.location = loc;
      typelocs.push_back (typeloc);
      order += 1;
    }
  }
}
