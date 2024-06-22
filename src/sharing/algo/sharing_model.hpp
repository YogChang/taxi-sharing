

#include "ortools/constraint_solver/routing.h"
#include "src/sharing/wrapper/sharing_wrapper.hpp"
#include "src/sharing/dao/node.h"

#include <cmath>
#include <unordered_map>

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
  auto FindRoute(const std::size_t &node_from_index, const std::size_t &node_to_index) const -> const Route;
  auto GetRouteDistance(const std::size_t &node_from_index, const std::size_t &node_to_index) const -> const std::int64_t;
  auto GetRouteTime(const std::size_t &node_from_index, const std::size_t &node_to_index) const -> const std::int64_t;
  auto node(const std::size_t &node_index) const -> const Node { return this->nodes.at(node_index); }
  auto vehicle(const std::size_t &num_vehicle) const -> const Vehicle { return this->parameter.vehicles.at(num_vehicle); }
  auto demand(const std::size_t &node_index) const -> const std::int64_t;
  auto vehicle_capacities() const -> const std::vector<std::int64_t>;
  const Parameter &parameter;

 private:

  std::vector<Node> nodes;
  auto Initialize() -> void;
  std::unordered_map<std::string, Route> routes;
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
  }

  for (const auto &vehicle : this->parameter.vehicles) {
    auto tmp_vehicle_end = Node(vehicle, counter, NodeType::VEHICLE_END);
    this->nodes.push_back(tmp_vehicle_end);
    counter++;
  }

  DebugPrint << "this->nodes.size() : " << this->nodes.size() << std::endl;

  for (const auto &route: this->parameter.routes) {
    routes[route.code] = route;
  }

  DebugPrint << "this->routes.size() : " << this->routes.size() << std::endl;
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

auto SharingModel::demand(const std::size_t &node_index) const -> const std::int64_t {
  const auto node = this->nodes.at(node_index);

  if (node.nodetype == NodeType::ORDER_DIRECT)
    return std::int64_t{node.order.headcount};

  if (node.nodetype == NodeType::ORDER_DELIVERY)
    return std::int64_t{-node.order.headcount};

  return std::int64_t{0};
}

auto SharingModel::vehicle_capacities() const -> const std::vector<std::int64_t> {
  std::vector<std::int64_t> capacities;

  for (std::size_t i = 0; i < this->parameter.vehicles.size(); ++i)
  {
    auto vehicle = this->parameter.vehicles.at(i);

    capacities.push_back(vehicle.capacity);
  }

  return capacities;
}

auto SharingModel::FindRoute(const std::size_t &node_from_index, const std::size_t &node_to_index) const -> const Route {
  if (routes.empty())
    return UnreachableRoute;

  const auto from_node = this->nodes.at(node_from_index);
  const auto to_node = this->nodes.at(node_to_index);

  if (from_node.nodetype == NodeType::VEHICLE_END) {
    return UnreachableRoute;
  }

  if (to_node.nodetype == NodeType::VEHICLE_START) {
      return UnreachableRoute;
  }


  if (from_node.nodetype == NodeType::ORDER_DIRECT && to_node.nodetype == NodeType::ORDER_DIRECT) {
    const auto from_coordinates = from_node.order.direct_location;
    const auto to_coordinates = to_node.order.direct_location;
    const auto route_code = from_coordinates.code + separateSymbol + to_coordinates.code;

    if (routes.count(route_code) > 0)
      return routes.at(route_code);

    return UnreachableRoute;
  }

  if (from_node.nodetype == NodeType::ORDER_DIRECT && to_node.nodetype == NodeType::ORDER_DELIVERY) {
    const auto from_coordinates = from_node.order.direct_location;
    const auto to_coordinates = to_node.order.delivery_location;
    const auto route_code = from_coordinates.code + separateSymbol + to_coordinates.code;

    if (routes.count(route_code) > 0)
      return routes.at(route_code);

    return UnreachableRoute;
  }

  if (from_node.nodetype == NodeType::ORDER_DELIVERY && to_node.nodetype == NodeType::ORDER_DIRECT) {
    const auto from_coordinates = from_node.order.delivery_location;
    const auto to_coordinates = to_node.order.direct_location;
    const auto route_code = from_coordinates.code + separateSymbol + to_coordinates.code;

    if (routes.count(route_code) > 0)
      return routes.at(route_code);

    return UnreachableRoute;
  }

  if (from_node.nodetype == NodeType::ORDER_DELIVERY && to_node.nodetype == NodeType::ORDER_DELIVERY) {
    const auto from_coordinates = from_node.order.delivery_location;
    const auto to_coordinates = to_node.order.delivery_location;
    const auto route_code = from_coordinates.code + separateSymbol + to_coordinates.code;

    if (routes.count(route_code) > 0)
      return routes.at(route_code);

    return UnreachableRoute;
  }

  if (from_node.nodetype == NodeType::VEHICLE_START) {
    return ZeroRoute;
  }

  if (to_node.nodetype == NodeType::VEHICLE_END) {
    return ZeroRoute;
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