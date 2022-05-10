#include <iostream>
#include <string>
#include "smeagle/smeagle.h"

int main(int argc, char *argv[]) {
   if (argc < 2) {
      std::cerr << "Usage: " << argv[0] << " binary0 [binary1] [binary2] ...";
      return -1;
   }

   for (int i = 1; i < argc; i++) {
      smeagle::Smeagle parseit(std::string(argv[i]));
      smeagle::Corpus corpus = parseit.parse();
      corpus.toJson();
      if (i + 1 != argc)
         std::cout << "------------------\n";
   }
}
