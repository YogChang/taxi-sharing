
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

  
  airouting::airsharing::DebugPrint << "before - ReadFile " << std::endl;
  std::string str = ReadFile("/mnt/test_data/input/parameter.json");

  auto parameter = airouting::airsharing::SharingWrapper::FromJson(str);

  auto model = airouting::airsharing::SharingModel(parameter);

  auto manager = airouting::airsharing::SharingManager(model);

  manager.AddTransitDistanceDimension();
  manager.AddTransitTimeDimension();
  manager.AddCostEvaluatorOfDistance();
  manager.AddPriorityDisjunction();
  manager.AddVehicleTimeWindow();
  manager.AddOrderTimeWindow();
  manager.AddPickupAndDelivery();

  auto solution = manager.StartCalculate();
  SaveFile("/mnt/test_data/output/solution.json", solution.ToJson().dump(4));

  return 0;
}