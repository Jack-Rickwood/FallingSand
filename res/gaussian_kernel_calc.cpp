#include <cmath>
#include <iostream>
 
int main() {
  float sigma = 1.75;
  int N = 25;
  
  float total = std::erf((N+.5) / (std::sqrt(2.0) * sigma)) - std::erf((-N-.5) / (std::sqrt(2.0) * sigma));
  for (int i = -N; i <= +N; ++i) {
    std::cout << std::fixed << (std::erf((i+.5) / (std::sqrt(2.0) * sigma)) - std::erf((i-.5) / (std::sqrt(2.0) * sigma))) / total << '\n';
  }
}
