#include "file-reader.hpp"
#include <fstream>
#include <iostream>

namespace leo {

std::string FileReader::readFile(std::string fileName) {
  std::ifstream ifs;
  try {
  ifs.open(fileName);
  if (!ifs.is_open())
    std::cerr << "<FileReader> ERROR: Cannot open file "
    << fileName << std::endl;
  std::string content((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
    return content;
  } catch (std::ifstream::failure &e) {
    std::cerr << "ERROR::FILE_READER::FILE_NOT_SUCCESSFULLY_READ("
      << fileName << ")" << std::endl;
  }
  return "";
}

}
