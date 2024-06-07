
#include <string>
#include <vector>


#ifndef GEOHASH_JSON_H_
#include "dependencies/geohash_json.hpp"
#define GEOHASH_JSON_H_
#endif

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
  std::int64_t passengers_num = -1;

  Destination(const Node &node) : node(node) {}
  auto ToJson() -> json {
    json ret = {
      {"node", node.ToJson()},
      {"arrival_time_early", arrival_time_early},
      {"arrival_time_lately", arrival_time_lately},
      {"arrival_distance", arrival_distance},
      {"passengers_num", passengers_num}
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
  std::int64_t memory_usage = -1;
  std::int64_t branches = -1;
  std::int64_t solutions = -1;
  std::int64_t droppeds_num = -1;
  std::int64_t minimum_obj_cost = -1;
  std::int64_t totally_vehicles_num = -1;
  std::int64_t totally_orders_num = -1;
  std::int64_t used_vehicles_num = 0;
  
  std::string version = "1.0.0";
  std::string routing_status = "UNKNOWN_STATUS";


  auto ToJson() -> json {
    json ret = {
      {"version", version},
      {"wall_time", wall_time},
      {"memory_usage", memory_usage},
      {"branches", branches},
      {"solutions", solutions},
      {"routing_status", routing_status},
      {"droppeds_num", droppeds_num},
      {"minimum_obj_cost", minimum_obj_cost},
      {"totally_vehicles_num", totally_vehicles_num},
      {"totally_orders_num", totally_orders_num},
      {"used_vehicles_num", used_vehicles_num}
    };

    return ret;
  }
};

class Solution {
 public:
  Solution() {}
  ~Solution() {}
  auto ToJson() -> json;
  auto ToGeoJson() -> json;

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

auto Solution::ToGeoJson() -> json {
  auto to_time_str = [] (const std::int64_t &i) {
    auto hour = i / 60;
    auto minute = i % 60;

    auto time_str = std::string();
    
    time_str += (hour < 10 ? "0" : "") + std::to_string(hour);
    time_str += ":";
    time_str += (minute < 10 ? "0" : "") + std::to_string(minute);

    return time_str;
  };

// =========================================
  auto solution_geo = geo_hash_json::GeoHashJson();

  for (auto v : this->valid_vehicles) {
    auto tmp_v = geo_hash_json::Vehicle(v.vehicle.code);

    for (auto d : v.destinations) {
      auto node = d.node;

      if (node.nodetype == NodeType::ORDER_DIRECT) {
        auto coordinates = node.order.direct_location;
        auto temp_node  = geo_hash_json::Node(coordinates.longitude, coordinates.latitude);
        temp_node.AddProperty("任務內容", node.order.code + "-上車");
        temp_node.AddProperty("預定上車時間", to_time_str(node.order.start_time));
        temp_node.AddProperty("抵達時間", to_time_str(d.arrival_time_early) + " ~ "+ to_time_str(d.arrival_time_lately));
        temp_node.AddProperty("累計里程", std::to_string(d.arrival_distance));
        temp_node.AddProperty("訂單人數", std::to_string(node.order.headcount));
        temp_node.AddProperty("乘客數 / 最大載客", std::to_string(d.passengers_num) + " / " + std::to_string(v.vehicle.capacity));
        if (d.passengers_num > v.vehicle.capacity)
          DebugPrint << "error!! " << v.vehicle.code << "-" << node.order.code << " passengers_num > vehicle.capacity" << std::endl;
        tmp_v.AddNode(temp_node);


      } else if (node.nodetype == NodeType::ORDER_DELIVERY) {
        auto coordinates = node.order.delivery_location;
        auto temp_node  = geo_hash_json::Node(coordinates.longitude, coordinates.latitude);
        temp_node.AddProperty("任務內容", node.order.code + "-下車");
        temp_node.AddProperty("預定最晚下車時間", to_time_str(node.order.end_time));
        temp_node.AddProperty("抵達時間", to_time_str(d.arrival_time_early) + " ~ "+ to_time_str(d.arrival_time_lately));
        temp_node.AddProperty("累計里程", std::to_string(d.arrival_distance));
        temp_node.AddProperty("訂單人數", std::to_string(node.order.headcount));
        temp_node.AddProperty("乘客數 / 最大載客", std::to_string(d.passengers_num) + " / " + std::to_string(v.vehicle.capacity));
        tmp_v.AddNode(temp_node);
      } else {

      }

    }

    solution_geo.AddVehicle(tmp_v);
  }

  return solution_geo.ToJson();
}


}  // namespace airsharing
}  // namespace airouting

#endif  // SRC_SHARING_DAO_SOLUTION_H_
