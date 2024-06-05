
#include <string>
#include <vector>

#include "vehicle.h"
#include "order.h"


#ifndef SRC_SHARING_DAO_NODE_H_
#define SRC_SHARING_DAO_NODE_H_

namespace airouting {
namespace airsharing {

enum class NodeType : int { VEHICLE_START = 1, VEHICLE_END = 2, ORDER_DIRECT = 3, ORDER_DELIVERY = 4, UNKNOW = -1 };

class Node {
 public:
  Node() {}
  Node(const Order& order, const std::int64_t &system_index, const NodeType &nodetype);
  Node(const Vehicle& vehicle, const std::int64_t &system_index, const NodeType &nodetype);

  ~Node() {}
  auto ToJson() -> json;

  NodeType nodetype = NodeType::UNKNOW;
  Order order = DummyOrder;
  Vehicle vehicle = DummyVehicle;
  std::int64_t system_index = -1;
};

Node::Node(const Order& order, const std::int64_t &system_index, const NodeType &nodetype) {
  this->order = order;
  this->system_index = system_index;
  this->nodetype = nodetype;
}
Node::Node(const Vehicle& vehicle, const std::int64_t &system_index, const NodeType &nodetype) {
  this->vehicle = vehicle;
  this->system_index = system_index;
  this->nodetype = nodetype;
}

auto Node::ToJson() -> json {
  json ret;

  if (nodetype == NodeType::VEHICLE_START) {
    ret["nodetype"] = "VEHICLE_START";
    ret["vehicle"] = vehicle.ToJson();
  } else if (nodetype == NodeType::VEHICLE_END) {
    ret["nodetype"] = "VEHICLE_END";
    ret["vehicle"] = vehicle.ToJson();
  } else if (nodetype == NodeType::ORDER_DIRECT) {
    ret["nodetype"] = "ORDER_DIRECT";
    ret["order"] = order.ToJson();
  } else if (nodetype == NodeType::ORDER_DELIVERY) {
    ret["nodetype"] = "ORDER_DELIVERY";
    ret["order"] = order.ToJson();
  } else {
    ret["nodetype"] = "UNKNOW";
  }

  return ret;
}

}  // namespace airsharing
}  // namespace airouting

#endif  // SRC_SHARING_DAO_NODE_H_
