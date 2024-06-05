
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
    
    return ret;
  }

  Strategy strategy;
  std::vector<Vehicle> vehicles;
  std::vector<Order> orders;

};



}  // namespace airpublic
}  // namespace airsharing

#endif  // SRC_SHARING_WRAPPER_PARAMETER_H_