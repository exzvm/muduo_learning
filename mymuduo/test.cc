#include <iostream>
#include <cstring>

int main() {
  char str[] = "Hello, World!";

  size_t length = std::strlen(str);
  std::cout << "The length of the string is: " << length << std::endl;

  return 0;
}
