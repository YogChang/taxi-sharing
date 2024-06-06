
#include <iostream>
#include <fstream>
#include <string>

#include "src/sharing/algo/sharing_manager.hpp"

void SaveFile(std::string save_path, std::string file_str) {
  std::ofstream writer(save_path);
  writer.clear();
  writer << file_str;
  writer.close();
}
std::string ReadFile(std::string read_path) {
  std::string file_str;
  std::ifstream reader = std::ifstream(read_path);
  std::stringstream strStream;
  if (reader.is_open()) {
    strStream << reader.rdbuf();
  }

  reader.close();
  return strStream.str();
}

int main() {

  std::string parameter_str = ReadFile("/mnt/test_data/input/parameter.json");


  std::string solution_str = airouting::airsharing::Dispatch(parameter_str);

  SaveFile("/mnt/test_data/output/solution.json", solution_str);

  return 0;
}


