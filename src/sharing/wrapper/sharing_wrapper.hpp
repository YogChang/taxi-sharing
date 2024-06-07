

#include "src/sharing/dao/parameter.h"


#ifndef SRC_SHARING_WRAPPER_SHARINGWRAPPER_HPP_
#define SRC_SHARING_WRAPPER_SHARINGWRAPPER_HPP_

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
  // define region parser

  const auto ParseStrategy = [] (const nlohmann::json &json_obj) -> const Strategy {
    auto strategy = Strategy();

    if (json_obj.contains("first_solution_strategy"))
      strategy.first_solution_strategy = json_obj.at("first_solution_strategy").get<std::int64_t>();
    if (json_obj.contains("metaheuristic"))
      strategy.metaheuristic = json_obj.at("metaheuristic").get<std::int64_t>();
    if (json_obj.contains("time_limit"))
      strategy.time_limit = json_obj.at("time_limit").get<std::int64_t>();

    return strategy;
  };

  const auto ParseVehicle = [] (const nlohmann::json &json_obj) -> const Vehicle {
    auto vehicle = Vehicle();

    if (json_obj.contains("code"))
      vehicle.code = json_obj.at("code").get<std::string>();
    if (json_obj.contains("on_work_time"))
      vehicle.on_work_time = json_obj.at("on_work_time").get<std::int64_t>();
    if (json_obj.contains("off_work_time"))
      vehicle.off_work_time = json_obj.at("off_work_time").get<std::int64_t>();
    if (json_obj.contains("capacity"))
      vehicle.capacity = json_obj.at("capacity").get<std::int64_t>();

    return vehicle;
  };

  const auto ParseCoordinates = [] (const nlohmann::json &json_obj) -> const Coordinates {
    auto coordinates = Coordinates();

    if (json_obj.contains("longitude"))
      coordinates.longitude = json_obj.at("longitude").get<double>();
    if (json_obj.contains("latitude"))
      coordinates.latitude = json_obj.at("latitude").get<double>();

    return coordinates;
  };

  const auto ParseOrder = [&ParseCoordinates] (const nlohmann::json &json_obj) -> const Order {
    auto order = Order();

    if (json_obj.contains("code"))
      order.code = json_obj.at("code").get<std::string>();
    if (json_obj.contains("start_time"))
      order.start_time = json_obj.at("start_time").get<std::int64_t>();
    if (json_obj.contains("end_time"))
      order.end_time = json_obj.at("end_time").get<std::int64_t>();
    if (json_obj.contains("headcount"))
      order.headcount = json_obj.at("headcount").get<std::int64_t>();


    if (json_obj.contains("direct_location"))
      order.direct_location = ParseCoordinates(json_obj.at("direct_location"));
    if (json_obj.contains("delivery_location"))
      order.delivery_location = ParseCoordinates(json_obj.at("delivery_location"));

    return order;
  };

  // =============================================================================================================

  // start to parse
  DebugPrint << " pass - FromJson(obj) !!! " << std::endl;
  auto parameter = Parameter();

  DebugPrint << " pass - region strategy !!! " << std::endl;
  if (json_obj.contains("strategy")) {
    parameter.strategy = ParseStrategy(json_obj.at("strategy"));
  }

  DebugPrint << " pass - region vehicles !!! " << std::endl;
  if (json_obj.contains("vehicles")) {
    const auto json_vehicles = json_obj.at("vehicles");
    parameter.vehicles.resize(json_vehicles.size());
    std::transform(json_vehicles.begin(), json_vehicles.end(), parameter.vehicles.begin(), ParseVehicle);
  }

  DebugPrint << " pass - region orders !!! " << std::endl;
  if (json_obj.contains("orders")) {
    const auto json_orders = json_obj.at("orders");
    parameter.orders.resize(json_orders.size());
    std::transform(json_orders.begin(), json_orders.end(), parameter.orders.begin(), ParseOrder);
  }

  return parameter;
}


}  // namespace airpublic
}  // namespace airsharing

#endif  // SRC_SHARING_WRAPPER_SHARINGWRAPPER_HPP_