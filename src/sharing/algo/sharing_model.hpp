

#include "ortools/constraint_solver/routing.h"
#include "src/sharing/wrapper/sharing_wrapper.hpp"
#include "src/sharing/dao/node.h"

#ifndef SRC_SHARING_ALGO_SHARINGMODEL_HPP_
#define SRC_SHARING_ALGO_SHARINGMODEL_HPP_

namespace airouting {
namespace airsharing {
using namespace operations_research;

class SharingModel {
 public:
  ~SharingModel() {}
  SharingModel(const Parameter &parameter);

  auto nodes_size() const -> const std::size_t { return this->nodes.size(); }
  auto vehicles_size() const -> const std::size_t { return this->parameter.vehicles.size(); }
  auto vehicle_start_node_indexies() const -> const std::vector<RoutingIndexManager::NodeIndex>;
  auto vehicle_end_node_indexies() const -> const std::vector<RoutingIndexManager::NodeIndex>;
  auto strategy() const -> const Strategy { return this->parameter.strategy; }
  auto GetRouteDistance(const std::size_t &node_from_index, const std::size_t &node_to_index) const -> const std::int64_t;
  auto GetRouteTime(const std::size_t &node_from_index, const std::size_t &node_to_index) const -> const std::int64_t;

 private:
 
  const Parameter &parameter;
  std::vector<Node> nodes;
  auto Initialize() -> void;
};

SharingModel::SharingModel(const Parameter &parameter) : parameter(parameter) {
  this->Initialize();
}

auto SharingModel::Initialize() -> void {
  DebugPrint << "SharingModel::Initialize()" << std::endl;

  std::int64_t counter = 0;

  for (const auto &order : this->parameter.orders) {
    auto tmp_order_direct = Node(order, counter, NodeType::ORDER_DIRECT);
    this->nodes.push_back(tmp_order_direct);
    counter++;
  
    auto tmp_order_deliver = Node(order, counter, NodeType::ORDER_DELIVERY);
    this->nodes.push_back(tmp_order_deliver);
    counter++;
  }

  for (const auto &vehicle : this->parameter.vehicles) {
    auto tmp_vehicle_start = Node(vehicle, counter, NodeType::VEHICLE_START);
    this->nodes.push_back(tmp_vehicle_start);
    counter++;

    auto tmp_vehicle_end = Node(vehicle, counter, NodeType::VEHICLE_END);
    this->nodes.push_back(tmp_vehicle_end);
    counter++;
  }

  DebugPrint << "this->nodes.size() : " << this->nodes.size() << std::endl;
}

auto SharingModel::vehicle_start_node_indexies() const -> const std::vector<RoutingIndexManager::NodeIndex> {
  std::vector<RoutingIndexManager::NodeIndex> indexies;

  for (std::size_t i = 0; i < this->nodes.size(); ++i) {
    auto tmp_node = this->nodes.at(i);

    if (tmp_node.nodetype == NodeType::VEHICLE_START) {
      indexies.push_back(RoutingIndexManager::NodeIndex(i));
    }
  }

  return indexies;
};

auto SharingModel::vehicle_end_node_indexies() const -> const std::vector<RoutingIndexManager::NodeIndex> {
  std::vector<RoutingIndexManager::NodeIndex> indexies;

  for (std::size_t i = 0; i < this->nodes.size(); ++i) {
    auto tmp_node = this->nodes.at(i);

    if (tmp_node.nodetype == NodeType::VEHICLE_END) {
      indexies.push_back(RoutingIndexManager::NodeIndex(i));
    }
  }

  return indexies;
};

auto SharingModel::GetRouteDistance(const std::size_t &node_from_index, const std::size_t &node_to_index) const -> const std::int64_t {
  return 1;
}
auto SharingModel::GetRouteTime(const std::size_t &node_from_index, const std::size_t &node_to_index) const -> const std::int64_t {
  return 1;
}

}  // namespace airpublic
}  // namespace airsharing

#endif  // SRC_SHARING_ALGO_SHARINGMODEL_HPP_