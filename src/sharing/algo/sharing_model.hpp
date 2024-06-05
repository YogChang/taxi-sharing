

#include "ortools/constraint_solver/routing.h"
#include "src/sharing/wrapper/sharing_wrapper.hpp"
#include "src/sharing/dao/node.h"

#ifndef SRC_SHARING_ALGO_SHARINGMODEL_HPP_
#define SRC_SHARING_ALGO_SHARINGMODEL_HPP_

namespace airouting {
namespace airsharing {
using namespace operations_research;


struct Route {
  Route(const Coordinates &coordinates_from, const Coordinates &coordinates_to) {
    distance = std::abs(coordinates_from.longitude - coordinates_to.longitude) + std::abs(coordinates_from.latitude - coordinates_to.latitude);
    time = distance;
  }
  Route(std::string type) {
    if (type.compare("ZeroRoute") == 0) {
      distance = 0;
      time = time;
    }
  }

  std::int64_t distance = maxOfSystem;
  std::int64_t time = maxOfSystem;

} ZeroRoute("ZeroRoute"), UnreachableRoute("UnreachableRoute");



class SharingModel {
 public:
  ~SharingModel() {}
  SharingModel(const Parameter &parameter);

  auto nodes_size() const -> const std::size_t { return this->nodes.size(); }
  auto vehicles_size() const -> const std::size_t { return this->parameter.vehicles.size(); }
  auto vehicle_start_node_indexies() const -> const std::vector<RoutingIndexManager::NodeIndex>;
  auto vehicle_end_node_indexies() const -> const std::vector<RoutingIndexManager::NodeIndex>;
  auto strategy() const -> const Strategy { return this->parameter.strategy; }
  auto FindRoute(const std::size_t &node_from_index, const std::size_t &node_to_index) const -> const Route;
  auto GetRouteDistance(const std::size_t &node_from_index, const std::size_t &node_to_index) const -> const std::int64_t;
  auto GetRouteTime(const std::size_t &node_from_index, const std::size_t &node_to_index) const -> const std::int64_t;
  auto node(const std::size_t &node_index) const -> const Node { return this->nodes.at(node_index); }
  auto vehicle(const std::size_t &num_vehicle) const -> const Vehicle { return this->parameter.vehicles.at(num_vehicle); }
  const Parameter &parameter;

 private:

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
}

auto SharingModel::vehicle_end_node_indexies() const -> const std::vector<RoutingIndexManager::NodeIndex> {
  std::vector<RoutingIndexManager::NodeIndex> indexies;

  for (std::size_t i = 0; i < this->nodes.size(); ++i) {
    auto tmp_node = this->nodes.at(i);

    if (tmp_node.nodetype == NodeType::VEHICLE_END) {
      indexies.push_back(RoutingIndexManager::NodeIndex(i));
    }
  }

  return indexies;
}

auto SharingModel::FindRoute(const std::size_t &node_from_index, const std::size_t &node_to_index) const -> const Route {

  const auto from_node = this->nodes.at(node_from_index);
  const auto to_node = this->nodes.at(node_to_index);


  if (from_node.nodetype == NodeType::ORDER_DIRECT) {
    const auto from_coordinates = from_node.order.direct_location;

    if (to_node.nodetype == NodeType::ORDER_DIRECT) {
      const auto to_coordinates = to_node.order.direct_location;
      return Route(from_coordinates, to_coordinates);
    } else if (to_node.nodetype == NodeType::ORDER_DELIVERY) {
      const auto to_coordinates = to_node.order.delivery_location;
      return Route(from_coordinates, to_coordinates);
    } else if (to_node.nodetype == NodeType::VEHICLE_START) {
      return UnreachableRoute;
    } else if (to_node.nodetype == NodeType::VEHICLE_END) {
      return ZeroRoute;
    }

  } else if (from_node.nodetype == NodeType::ORDER_DELIVERY) {
    const auto from_coordinates = from_node.order.delivery_location;

    if (to_node.nodetype == NodeType::ORDER_DIRECT) {
      const auto to_coordinates = to_node.order.direct_location;
      return Route(from_coordinates, to_coordinates);
    } else if (to_node.nodetype == NodeType::ORDER_DELIVERY) {
      const auto to_coordinates = to_node.order.delivery_location;
      return Route(from_coordinates, to_coordinates);
    } else if (to_node.nodetype == NodeType::VEHICLE_START) {
      return UnreachableRoute;
    } else if (to_node.nodetype == NodeType::VEHICLE_END) {
      return ZeroRoute;
    }
  } else if (from_node.nodetype == NodeType::VEHICLE_START) {

    if (to_node.nodetype == NodeType::ORDER_DIRECT || to_node.nodetype == NodeType::ORDER_DELIVERY) {
      return ZeroRoute;
    } else {
      return UnreachableRoute;
    }

  } else if (from_node.nodetype == NodeType::VEHICLE_END) {
    return UnreachableRoute;
  }

  return UnreachableRoute;
}

auto SharingModel::GetRouteDistance(const std::size_t &node_from_index, const std::size_t &node_to_index) const -> const std::int64_t {
  return FindRoute(node_from_index, node_to_index).distance;
}
auto SharingModel::GetRouteTime(const std::size_t &node_from_index, const std::size_t &node_to_index) const -> const std::int64_t {
  return FindRoute(node_from_index, node_to_index).time;
}

}  // namespace airpublic
}  // namespace airsharing

#endif  // SRC_SHARING_ALGO_SHARINGMODEL_HPP_