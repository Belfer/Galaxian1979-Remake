#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace NHTV {
namespace Util {
inline void readFile(const std::string &path, std::string &out) {
  std::ifstream ifs(path, std::ios::in);

  if (!ifs.is_open()) {
    std::cerr << "Could not read file " << path << ". File does not exist.\n";
    out = "";
  } else {
    std::string line = "";
    while (!ifs.eof()) {
      std::getline(ifs, line);
      out.append(line + "\n");
    }
  }

  ifs.close();
}

inline std::string readFile(const std::string &path) {
  std::string out;
  std::ifstream ifs(path, std::ios::in);

  if (!ifs.is_open()) {
    std::cerr << "Could not read file " << path << ". File does not exist.\n";
    out = "";
  } else {
    std::string line = "";
    while (!ifs.eof()) {
      std::getline(ifs, line);
      out.append(line + "\n");
    }
  }

  ifs.close();
  return out;
}
}
}
