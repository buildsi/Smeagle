// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <iomanip>
#include <sstream>
#include <iosfwd>
#include <string>
#include <utility>
#include <vector>

namespace smeagle::x86_64::callsites {

  // Test function for printing call sites (from Jim, tweaked by @vsoch)
  void PrintCallSite(Dyninst::SymtabAPI::CallSite * cs) {
    int indent = 1;
    if (auto called = cs -> GetCalled()) {
      std::cout << std::setw(4 * indent) << "" << std::left << std::setw(10) << "calls" << called -> GetName() << "\n";
      ++indent;
      std::cout << "returnType" << called -> GetReturnType() << std::endl;
      auto numParams = called -> GetNumParams();
      for (int i = 0; i < numParams; ++i) {
        auto theType = called -> GetParamNType(i);
        std::string theTypeName = "<NONE>";
        if (theType) {
          theTypeName = theType -> getName();
        }
        std::ostringstream os;
        os << "param[" << i << "].type";
        std::cout << os.str() << theType << std::endl;

      }
    } else {
      // the called function is unknown
      std::cout << std::setw(4 * indent) << "" << std::left << std::setw(10) << "calls" << "<UNKNOWN>\n";
    }
  }

} // namespace smeagle::x86_64::callsites
