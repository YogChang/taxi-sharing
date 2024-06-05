
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
  } else {
    std::cout << "reader.is_close" << std::endl;
  }
  reader.close();
  return strStream.str();
}

int main() {

  
  airouting::airsharing::DebugPrint << "before - ReadFile " << std::endl;
  std::string str = ReadFile("/mnt/test_data/test_input.json");
  std::cout << str << std::endl;

  auto parameter = airouting::airsharing::SharingWrapper::FromJson(str);
  std::cout << parameter.ToJson().dump(4) << std::endl;

  auto model = airouting::airsharing::SharingModel(parameter);

  auto manager = airouting::airsharing::SharingManager(model);

  auto solution = manager.StartCalculate();
  std::cout << solution.ToJson().dump(4) << std::endl;

  return 0;
}