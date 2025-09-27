#pragma once

#include <string>

class Error {
public:
  static void Check (const std::string& msg);
};
