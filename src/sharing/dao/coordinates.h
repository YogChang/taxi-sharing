
#include <string>
#include <vector>

#include "../config.h"

#ifndef SRC_SHARING_DAO_COORDINATES_H_
#define SRC_SHARING_DAO_COORDINATES_H_

namespace airouting {
namespace airsharing {


class Coordinates {
 public:
  Coordinates() {}
  ~Coordinates() {}
  json ToJson() {
    json ret = {
      {"longitude", longitude},
      {"latitude", latitude}
    };

    return ret;
  }

  // 經度
  double longitude = 0.0;
  // 緯度
  double latitude = 0.0;

};

static const Coordinates DummyCoordinates = Coordinates();

}  // namespace airsharing
}  // namespace airouting

#endif  // SRC_SHARING_DAO_COORDINATES_H_
