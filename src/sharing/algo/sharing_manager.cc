#include <algorithm>
#include <cstdint>
#include <sstream>
#include <vector>
#include <iostream>

#include "sharing_manager.hpp"


namespace airouting {
namespace airsharing {


SharingManager::SharingManager(const SharingModel &sharing_model) : sharing_model(sharing_model) {
  DebugPrint << "SharingManager::SharingManager()" << std::endl;

  DebugPrint << "this->sharing_model.nodes_size()" << this->sharing_model.nodes_size() << std::endl;
  DebugPrint << "this->sharing_model.vehicles_size()" << this->sharing_model.vehicles_size() << std::endl;

  auto manager = RoutingIndexManager(
      this->sharing_model.nodes_size(),
      this->sharing_model.vehicles_size(),
      this->sharing_model.vehicle_start_node_indexies(),
      this->sharing_model.vehicle_end_node_indexies());

  this->routing_managers.clear();
  this->routing_managers.push_back(manager);


  this->routing_model = new RoutingModel(manager);
}

auto SharingManager::StartCalculate() -> const Solution {
  Solution solution;

  return solution;
}



}  // namespace airpublic
}  // namespace airsharing

