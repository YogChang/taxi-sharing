
#include "../dao/vehicle.h"
#include "../dao/order.h"
#include "../dao/strategy.h"




#include <string>
#include <vector>

#include "../config.h"


#ifndef SRC_SHARING_WRAPPER_PARAMETER_H_
#define SRC_SHARING_WRAPPER_PARAMETER_H_

namespace airouting {
namespace airsharing {

class Parameter {
 public:
  Parameter() {}
  ~Parameter() {}

  json ToJson() {
    json ret;
    ret["strategy"] = strategy.ToJson();

    std::vector<json> vehicles_json(this->vehicles.size());
    std::transform(this->vehicles.begin(), this->vehicles.end(), vehicles_json.begin(), [](Vehicle v) { return v.ToJson(); });
    ret["vehicles"] = vehicles_json;

    std::vector<json> orders_json(this->orders.size());
    std::transform(this->orders.begin(), this->orders.end(), orders_json.begin(), [](Order o) { return o.ToJson(); });
    ret["orders"] = orders_json;

    return ret;
  }

  Strategy strategy;
  std::vector<Vehicle> vehicles;
  std::vector<Order> orders;

};



}  // namespace airpublic
}  // namespace airsharing

#endif  // SRC_SHARING_WRAPPER_PARAMETER_H_