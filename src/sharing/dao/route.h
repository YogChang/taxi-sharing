
#include <string>
#include <vector>

#include "src/sharing/config.h"

#ifndef SRC_SHARING_DAO_ROUTE_H_
#define SRC_SHARING_DAO_ROUTE_H_

namespace airouting {
namespace airsharing {

struct Coordinates {

  Coordinates(const double &longitude, const double &latitude) : longitude(longitude), latitude(latitude) {}
  Coordinates(const std::string code) : code(code) {}
  Coordinates() {}
  ~Coordinates() {}
  auto ToJson() -> json {
    return {
      {"code", code},
      {"longitude", longitude},
      {"latitude", latitude}
    };
  }

  // code
  std::string code = "";
  // 經度
  double longitude = 0;
  // 緯度
  double latitude = 0;

} DummyCoordinates(-1, -1);


const char* kZeroRoute = "ZeroRoute";
const char* kUnreachableRoute = "UnreachableRoute";
struct Route {

  Route(const std::string code) : code(code) {
    if (code.compare(kZeroRoute) == 0) {
      distance = 0;
      time = 0;
    } else if (code.compare(kZeroRoute) == 0) {
      distance = maxOfSystem;
      time = maxOfSystem;
    }
  }
  Route() {}

  auto ToJson() -> json;


  std::string code = "";
  std::int64_t distance = -1;
  std::int64_t time = -1;

} ZeroRoute(kZeroRoute), UnreachableRoute(kUnreachableRoute);

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
