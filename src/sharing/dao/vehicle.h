

#include <string>
#include <vector>

#include "src/sharing/config.h"


#ifndef SRC_SHARING_DAO_VEHICLE_H_
#define SRC_SHARING_DAO_VEHICLE_H_

namespace airouting {
namespace airsharing {



class Vehicle {
 public:
  Vehicle() {}
  ~Vehicle() {}
  auto ToJson() const -> json;


  // 編號
  std::string code = "";
  // 時窗開啟時間
  std::int64_t on_work_time = minDateTime;
  // 時窗結束時間
  std::int64_t off_work_time = maxDateTime;
  std::int64_t capacity = -1;

};

auto Vehicle::ToJson() const -> json {
  json ret = {
    {"code", code},
    {"on_work_time", on_work_time},
    {"off_work_time", off_work_time},
    {"capacity", capacity}
  };

  return ret;
}

static const Vehicle DummyVehicle = Vehicle();

}  // namespace airpublic
}  // namespace airsharing

#endif  // SRC_SHARING_DAO_VEHICLE_H_
