
#include <string>
#include <vector>

#include "src/sharing/config.h"

#ifndef SRC_SHARING_DAO_COORDINATES_H_
#define SRC_SHARING_DAO_COORDINATES_H_

namespace airouting {
namespace airsharing {


class Coordinates {
 public:
  Coordinates(const double &longitude, const double &latitude) : longitude(longitude), latitude(latitude) {}
  Coordinates() {}
  ~Coordinates() {}
  auto ToJson() -> json;

  // 經度
  double longitude = 0;
  // 緯度
  double latitude = 0;

} DummyCoordinates(-1, -1);

auto Coordinates::ToJson() -> json {
  json ret = {
    {"longitude", longitude},
    {"latitude", latitude}
  };

  return ret;
}


}  // namespace airsharing
}  // namespace airouting

#endif  // SRC_SHARING_DAO_COORDINATES_H_
