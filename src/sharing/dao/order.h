
#include <string>
#include <vector>

#include "../config.h"
#include "coordinates.h"


#ifndef SRC_SHARING_DAO_ORDER_H_
#define SRC_SHARING_DAO_ORDER_H_

namespace airouting {
namespace airsharing {



class Order {
 public:
  Order() {}
  ~Order() {}
  json ToJson();

  // 編號
  std::string code = "";

  // 出發地點
  Coordinates direct_location = DummyCoordinates;
  // 抵達地點
  Coordinates delivery_location = DummyCoordinates;
  // 訂單開始時間
  std::int64_t start_time = airMinDatetime;
  // 訂單結束時間
  std::int64_t end_time = airMaxDatetime;


};

static const Order DummyOrder = Order();

}  // namespace airsharing
}  // namespace airouting

#endif  // SRC_SHARING_DAO_ORDER_H_
