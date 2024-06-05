#include <algorithm>
#include <cstdint>
#include <sstream>
#include <vector>
#include <iostream>

#include "sharing_manager.hpp"


namespace airouting {
namespace airsharing {


const auto distance_cost_dimension_name = std::string{"Distance"};
const auto time_cost_dimension_name = std::string{"Time"};

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

auto SharingManager::AddTransitDistanceDimension() -> void {
  auto callback = [&] (int64_t from_index, int64_t to_index) -> std::int64_t {
    const auto node_from_index = this->routing_managers.at(0).IndexToNode(from_index).value();
    const auto node_to_index = this->routing_managers.at(0).IndexToNode(to_index).value();

    return this->sharing_model.GetRouteDistance(node_from_index, node_to_index);
  };

  auto callback_index = this->routing_model->RegisterTransitCallback(callback);


  std::vector<int> callback_index_mapping(sharing_model.vehicles_size(), callback_index);

  this->routing_model->AddDimensionWithVehicleTransits(
      callback_index_mapping,
      0,
      maxOfSystem,
      true,
      distance_cost_dimension_name);
}

auto SharingManager::AddTransitTimeDimension() -> void {
  auto callback = [&] (int64_t from_index, int64_t to_index) -> std::int64_t {
    const auto node_from_index = this->routing_managers.at(0).IndexToNode(from_index).value();
    const auto node_to_index = this->routing_managers.at(0).IndexToNode(to_index).value();

    return this->sharing_model.GetRouteTime(node_from_index, node_to_index);
  };

  auto callback_index = this->routing_model->RegisterTransitCallback(callback);


  std::vector<int> callback_index_mapping(sharing_model.vehicles_size(), callback_index);

  this->routing_model->AddDimensionWithVehicleTransits(
      callback_index_mapping,
      86400,
      maxOfSystem,
      false,
      time_cost_dimension_name);
}


auto SharingManager::AddSearchRecord() -> void {
  SearchRecord search_record;
  search_record.cost = this->routing_model->CostVar()->Min();
  search_record.time = this->routing_model->solver()->wall_time();
  this->search_records.push_back(search_record);

  if (search_record.cost < this->minimum_cost_record.cost) {
    this->minimum_cost_record = search_record;
  }
}

auto SharingManager::StartCalculate() -> const Solution {


  DebugPrint << "time_limit" << this->sharing_model.strategy().time_limit << std::endl;
  DebugPrint << "first_solution_strategy" << this->sharing_model.strategy().first_solution_strategy << std::endl;
  DebugPrint << "metaheuristic" << this->sharing_model.strategy().metaheuristic << std::endl;


  // 設定 log
  this->routing_search_parameter.set_log_search(true);

  //  設定初始解策略
  this->routing_search_parameter.set_first_solution_strategy(FirstSolutionStrategy_Value_PARALLEL_CHEAPEST_INSERTION);

  //  設定啟發式演算法
  this->routing_search_parameter.set_local_search_metaheuristic(LocalSearchMetaheuristic_Value_SIMULATED_ANNEALING);

  //  炫炮加速器
  this->routing_model->solver()->SetUseFastLocalSearch(true);
  this->routing_search_parameter.set_use_multi_armed_bandit_concatenate_operators(true);

  //  設定迭代時間上限
  this->routing_search_parameter.mutable_time_limit()->set_seconds(this->sharing_model.strategy().time_limit);

  // //  設定 callback
  // std::function<void()> callback = [&](){this->AddSearchRecord();};
  // this->routing_model->AddAtSolutionCallback(callback);

  //  開始計算
  auto assignment = this->routing_model->SolveWithParameters(this->routing_search_parameter);

  //  輸出結果

  return this->ConvertSolution(*assignment);
}

auto SharingManager::ConvertSolution(const operations_research::Assignment &assignment) -> const Solution {
  Solution solution;

  return solution;
}

}  // namespace airpublic
}  // namespace airsharing

