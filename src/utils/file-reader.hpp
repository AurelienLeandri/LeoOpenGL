#pragma once

#include <string>

namespace leo {

class FileReader {
  public:
    static std::string readFile(std::string fileName);
};

}
