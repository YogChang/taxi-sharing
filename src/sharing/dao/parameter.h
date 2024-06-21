



#include "src/sharing/dao/vehicle.h"
#include "src/sharing/dao/order.h"
#include "src/sharing/dao/strategy.h"
#include "src/sharing/dao/route.h"

#include <string>
#include <vector>




#ifndef SRC_SHARING_WRAPPER_PARAMETER_H_
#define SRC_SHARING_WRAPPER_PARAMETER_H_

namespace airouting {
namespace airsharing {

class Parameter {
 public:
  Parameter() {}
  ~Parameter() {}
  auto ToJson() -> json;


  Strategy strategy;
  std::vector<Vehicle> vehicles;
  std::vector<Order> orders;
  std::vector<Route> routes;

};

auto Parameter::ToJson() -> json {
  json ret;
  ret["strategy"] = strategy.ToJson();

  std::vector<json> vehicles_json(this->vehicles.size());
  std::transform(this->vehicles.begin(), this->vehicles.end(), vehicles_json.begin(), [](Vehicle v) { return v.ToJson(); });
  ret["vehicles"] = vehicles_json;

  std::vector<json> orders_json(this->orders.size());
  std::transform(this->orders.begin(), this->orders.end(), orders_json.begin(), [](Order o) { return o.ToJson(); });
  ret["orders"] = orders_json;

  std::vector<json> routes_json(this->routes.size());
  std::transform(this->routes.begin(), this->routes.end(), routes_json.begin(), [](Route r) { return r.ToJson(); });
  ret["routes"] = routes_json;

  return ret;
}

}  // namespace airpublic
}  // namespace airsharing

#endif  // SRC_SHARING_WRAPPER_PARAMETER_H_