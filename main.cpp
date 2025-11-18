#include "Coursework2.h"
#include <exception>
#include <iostream>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  try {
    EvaluationTest();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
