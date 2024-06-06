
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


// #include "dependencies/geohash_json.hpp"


// using namespace geo_hash_json;

// int main() {

//     std::vector<std::string> string_location = {
//         "wsmc2t2munmx",
//         "wsmc3377p",
//         "wsmbqtb7h3hb",
//     };
//     std::vector<std::pair<double, double>> coordinate_location = {
//         {120.60843268409371, 24.155837511643767},
//         {120.68716374553549, 24.171866494368953}
//     };
//     std::vector<std::string> arrival_reason = {
//         "到此一游",
//         "bj4"
//     };
//     std::vector<std::string> arrival_time = {
//         "08:30",
//         "13:30",
//         "16:00"
//     };

//     Vehicle v1("MyBike");
//     for (size_t i = 0; i < string_location.size(); i++)
//     {
//         Node temp_node(string_location.at(i));
//         temp_node.AddProperty("到訪理由", arrival_reason.at(i % arrival_reason.size()));
//         temp_node.AddProperty("到訪時間", arrival_time.at(i % arrival_time.size()));
//         v1.AddNode(temp_node);
//     }

//     Vehicle v2("TrashCar");
//     for (size_t i = 0; i < coordinate_location.size(); i++)
//     {
//         Node temp_node(coordinate_location.at(i).first, coordinate_location.at(i).second);
//         temp_node.AddProperty("到訪理由", arrival_reason.at(i % arrival_reason.size()));
//         temp_node.AddProperty("到訪時間", arrival_time.at(i % arrival_time.size()));
//         v2.AddNode(temp_node);
//     }

//     GeoHashJson geo_hash1;
//     geo_hash1.AddVehicle(v1);
//     geo_hash1.AddVehicle(v2);

//     nlohmann::json my_result = geo_hash1.ToJson();
//     std::cout << my_result.dump() << std::endl;

//     return 0;
// }
