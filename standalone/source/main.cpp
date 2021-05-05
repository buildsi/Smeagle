#include <smeagle/smeagle.h>
#include <smeagle/version.h>

#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

auto main(int argc, char** argv) -> int {
  const std::unordered_map<std::string, smeagle::FormatCode> formats{
      {"terminal", smeagle::FormatCode::Terminal},
      {"json", smeagle::FormatCode::Json},
  };

  cxxopts::Options options(*argv, "Extract library metadata, the precious.");

  std::string library;
  std::string fmt;

  // clang-format off
  options.add_options()
    ("h,help", "Show help")
    ("v,version", "Print the current version number")
    ("l,library", "Library to inspect", cxxopts::value(library))
    ("f,fmt", "Format to output in", cxxopts::value(fmt)->default_value("terminal"))
  ;
  // clang-format on

  auto result = options.parse(argc, argv);

  if (result["help"].as<bool>()) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  if (result["version"].as<bool>()) {
    std::cout << "Smeagle, version " << SMEAGLE_VERSION << std::endl;
    return 0;
  }

  // Library is required
  if (result["library"].count() == 0) {
    std::cout << "A library is required.\n";
    return 0;
  }

  auto format = formats.find(fmt);
  if (format == formats.end()) {
    std::cerr << "unknown format: " << fmt << std::endl;
    return 1;
  }

  smeagle::Smeagle smeagle(library);
  //std::cout << smeagle.parse(format->second) << std::endl;

  return 0;
}
