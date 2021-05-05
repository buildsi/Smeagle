#include <fmt/format.h>
#include <smeagle/smeagle.h>
#include "Symtab.h"

using namespace Dyninst;
using namespace SymtabAPI;

using namespace smeagle;

Smeagle::Smeagle(std::string _library) : library(std::move(_library)) {}

int Smeagle::parse(FormatCode fmt) {

  switch(fmt) {
   default:
   case FormatCode::Json:
     std::cout << "You chose json.\n";
     break;
   case FormatCode::Terminal:
     std::cout << "You chose terminal.\n";
     break;
  }
  
  // Read the library into the Symtab object
  Symtab *obj = NULL;
  bool err = Symtab::openFile(obj, library);
  if (!err){
    std::cout << "There was a problem reading " << library << "\n";
    return 1;
  }

  return 0;
}
