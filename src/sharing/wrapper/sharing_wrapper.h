
#include "parameter.h"


#ifndef SRC_SHARING_WRAPPER_SHARINGWRAPPER_H_
#define SRC_SHARING_WRAPPER_SHARINGWRAPPER_H_

namespace airouting {
namespace airsharing {

class SharingWrapper {
 public:
  static const Parameter FromJson(const std::string &str);
  static const Parameter FromJson(const nlohmann::json &json_obj);

 private:
  SharingWrapper();
};




const Parameter SharingWrapper::FromJson(const std::string &str) {
  DebugPrint << " pass - FromJson(str) !!! " << std::endl;
  return SharingWrapper::FromJson(json::parse(str));
}

const Parameter SharingWrapper::FromJson(const nlohmann::json &json_obj) {
  DebugPrint << " pass - FromJson(obj) !!! " << std::endl;
  auto parameter = Parameter();

  DebugPrint << " pass - region strategy !!! " << std::endl;
  if (json_obj.contains("strategy")) {
    const auto json_strategy = json_obj.at("strategy");
    if (json_strategy.contains("first_solution_strategy")) 
      parameter.strategy.first_solution_strategy = json_strategy.at("first_solution_strategy").get<std::int64_t>();
    if (json_strategy.contains("metaheuristic")) 
      parameter.strategy.metaheuristic = json_strategy.at("metaheuristic").get<std::int64_t>();
    if (json_strategy.contains("time_limit")) 
      parameter.strategy.time_limit = json_strategy.at("time_limit").get<std::int64_t>();
  }

  DebugPrint << " pass - region vehicles !!! " << std::endl;
  if (json_obj.contains("vehicles")) {
    const auto json_vehicles = json_obj.at("vehicles");
    // auto vehicle_parameter = VehicleParameter(json_vehicles);
    // vehicle_parameter.WrapVrpParameter(vrp_parameter);
  }

  DebugPrint << " pass - region orders !!! " << std::endl;

  if (json_obj.contains("orders")) {
    const auto json_freights = json_obj.at("orders");
    // auto freights_parameter = FreightParameter(json_freights);
    // freights_parameter.WrapVrpParameter(vrp_parameter);
  }


  return parameter;
}


}  // namespace airpublic
}  // namespace airsharing

#endif  // SRC_SHARING_WRAPPER_SHARINGWRAPPER_H_