
#include <string>
#include <vector>

#include "src/sharing/config.h"

#ifndef SRC_SHARING_DAO_SOLUTION_H_
#define SRC_SHARING_DAO_SOLUTION_H_

namespace airouting {
namespace airsharing {


struct Destination {
  Node node;
  std::int64_t arrival_time_early = -1;
  std::int64_t arrival_time_lately = -1;
  std::int64_t arrival_distance = -1;

  Destination(const Node &node) : node(node) {}
  auto ToJson() -> json {
    json ret = {
      {"node", node.ToJson()},
      {"arrival_time_early", arrival_time_early},
      {"arrival_time_lately", arrival_time_lately},
      {"arrival_distance", arrival_distance},
    };

    return ret;
  }
};

struct ValidVehicle {
  const Vehicle vehicle;
  std::vector<Destination> destinations;

  ValidVehicle(const Vehicle &vehicle) : vehicle(vehicle) {}
  auto ToJson() -> json {
    json ret;

    ret["vehicle"] = vehicle.ToJson();

    std::vector<json> destinations_json(this->destinations.size());
    std::transform(this->destinations.begin(), this->destinations.end(), destinations_json.begin(), [] (Destination d) { return d.ToJson(); });
    ret["destinations"] = destinations_json;

    return ret;
  }
};

struct Report {
  std::int64_t wall_time = -1;
  std::string version = "1.0.0";
  std::string routing_status = "UNKNOWN_STATUS";
  auto ToJson() -> json {
    json ret = {
      {"version", version},
      {"wall_time", wall_time},
      {"routing_status", routing_status}
    };

    return ret;
  }
};

class Solution {
 public:
  Solution() {}
  ~Solution() {}
  auto ToJson() -> json;

  std::vector<ValidVehicle> valid_vehicles;
  std::vector<Destination> droppeds;
  Report report = Report();
};

auto Solution::ToJson() -> json {
  json ret;
  ret["report"] = report.ToJson();

  std::vector<json> valid_vehicles_json(this->valid_vehicles.size());
  std::transform(this->valid_vehicles.begin(), this->valid_vehicles.end(), valid_vehicles_json.begin(), [] (ValidVehicle v) { return v.ToJson(); });
  ret["valid_vehicles"] = valid_vehicles_json;

  std::vector<json> dropped_json(this->droppeds.size());
  std::transform(this->droppeds.begin(), this->droppeds.end(), dropped_json.begin(), [] (Destination d) { return d.ToJson(); });
  ret["droppeds"] = dropped_json;

  return ret;
}



}  // namespace airsharing
}  // namespace airouting

#endif  // SRC_SHARING_DAO_SOLUTION_H_
