
#include <string>
#include <vector>

#include "src/sharing/config.h"

#ifndef SRC_SHARING_DAO_ROUTE_H_
#define SRC_SHARING_DAO_ROUTE_H_

namespace airouting {
namespace airsharing {


struct Route {

  Route(const Coordinates &coordinates_from, const Coordinates &coordinates_to) {
    auto sum = std::abs(coordinates_from.longitude - coordinates_to.longitude) + std::abs(coordinates_from.latitude - coordinates_to.latitude);
    distance = std::max(std::int64_t(sum * 100), std::int64_t{1});
    time = distance;

  }
  Route(std::string type) {
    if (type.compare("ZeroRoute") == 0) {
      distance = 0;
      time = 0;
    } else if (type.compare("UnreachableRoute") == 0) {
      distance = maxOfSystem;
      time = maxOfSystem;
    }
  }
  Route() {}

  auto ToJson() -> json;


  std::string code = "";
  std::int64_t distance = -1;
  std::int64_t time = -1;

} ZeroRoute("ZeroRoute"), UnreachableRoute("UnreachableRoute");

auto Route::ToJson() -> json {
  json ret = {
    {"code", code},
    {"distance", distance},
    {"time", time},
  };

  return ret;
}


}  // namespace airsharing
}  // namespace airouting

#endif  // SRC_SHARING_DAO_ROUTE_H_
