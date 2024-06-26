#include <algorithm>
#include <cstdint>
#include <sstream>
#include <vector>
#include <iostream>

#include "sharing_manager.hpp"


namespace airouting {
namespace airsharing {

auto Dispatch(const std::string &str) -> const std::string {
  airouting::airsharing::DebugPrint << "start Dispatch" << std::endl;
  auto ret = json();

  auto parameter = airouting::airsharing::SharingWrapper::FromJson(str);

  if (!parameter) {
    ret["error"] = DebugPrint.ToJson();
    return ret.dump();
  }

  auto model = airouting::airsharing::SharingModel(parameter.value());

  auto manager = airouting::airsharing::SharingManager(model);

  manager.AddTransitDistanceDimension();
  manager.AddTransitTimeDimension();
  manager.AddCostEvaluatorOfDistance();
  manager.AddPriorityDisjunction();
  manager.AddVehicleTimeWindow();
  manager.AddOrderTimeWindow();
  manager.AddPickupAndDelivery();
  manager.AddDemandDimension();

  auto solution = manager.StartCalculate();

  ret = {
    {"geo_json", solution.ToGeoJson()},
    {"solution", solution.ToJson()}
  };


  return ret.dump();
}



const std::string StatusOfSearch(const operations_research::RoutingModel::Status &status) {
  switch (status) {
  case operations_research::RoutingModel::Status::ROUTING_NOT_SOLVED:
    return std::string{"ROUTING_NOT_SOLVED"};
  case operations_research::RoutingModel::Status::ROUTING_SUCCESS:
    return std::string{"ROUTING_SUCCESS"};
  case operations_research::RoutingModel::Status::ROUTING_FAIL:
    return std::string{"ROUTING_FAIL"};
  case operations_research::RoutingModel::Status::ROUTING_FAIL_TIMEOUT:
    return std::string{"ROUTING_FAIL_TIMEOUT"};
  case operations_research::RoutingModel::Status::ROUTING_INVALID:
    return std::string{"ROUTING_INVALID"};
  case operations_research::RoutingModel::Status::ROUTING_PARTIAL_SUCCESS_LOCAL_OPTIMUM_NOT_REACHED:
    return std::string{"ROUTING_PARTIAL_SUCCESS_LOCAL_OPTIMUM_NOT_REACHED"};
  case operations_research::RoutingModel::Status::ROUTING_INFEASIBLE:
    return std::string{"ROUTING_INFEASIBLE"};
  case operations_research::RoutingModel::Status::ROUTING_OPTIMAL:
    return std::string{"ROUTING_OPTIMAL"};
  }

  return std::string{"UNKNOWN_STATUS"};
}

const auto distance_cost_dimension_name = std::string{"Distance"};
const auto time_cost_dimension_name = std::string{"Time"};
const auto capacity_dimension_name = std::string{"Capacity"};

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

auto SharingManager::AddCostEvaluatorOfDistance() -> void {
  auto callback = [&] (int64_t from_index, int64_t to_index) -> int64_t {
    const auto node_from_index = this->routing_managers.at(0).IndexToNode(from_index).value();
    const auto node_to_index = this->routing_managers.at(0).IndexToNode(to_index).value();

    return this->sharing_model.GetRouteDistance(node_from_index, node_to_index);
  };

  auto callback_index = this->routing_model->RegisterTransitCallback(callback);

  for (std::size_t i = 0; i < this->sharing_model.vehicles_size(); ++i) {
    this->routing_model->SetArcCostEvaluatorOfVehicle(callback_index, i);
  }
}

auto SharingManager::AddPriorityDisjunction() -> void {
  std::int64_t max_cost = 100000;
  DebugPrint << "max_cost" << max_cost << std::endl;

  for (std::size_t i = 0; i < this->sharing_model.nodes_size(); ++i) {
    const auto node = this->sharing_model.node(i);

    if (node.nodetype == NodeType::ORDER_DIRECT || node.nodetype == NodeType::ORDER_DELIVERY) {
      this->routing_model->AddDisjunction({this->routing_managers.at(0).NodeToIndex(RoutingIndexManager::NodeIndex(i))}, max_cost);
    }
  }
}

auto SharingManager::AddVehicleTimeWindow() -> void {
  if (!this->routing_model->HasDimension(time_cost_dimension_name))
    this->AddTransitTimeDimension();

  auto dimension = this->routing_model->GetMutableDimension(time_cost_dimension_name);

  for (std::size_t i = 0; i < this->sharing_model.vehicles_size(); ++i) {
    const auto vehicle = this->sharing_model.vehicle(i);

    auto start_index = this->routing_model->Start(i);
    dimension->CumulVar(start_index)->SetRange(vehicle.on_work_time, vehicle.off_work_time);

    auto end_index = this->routing_model->End(i);
    dimension->CumulVar(end_index)->SetRange(vehicle.off_work_time, vehicle.off_work_time);
  }
}
auto SharingManager::AddOrderTimeWindow() -> void {
  if (!this->routing_model->HasDimension(time_cost_dimension_name))
    this->AddTransitTimeDimension();

  auto dimension = this->routing_model->GetMutableDimension(time_cost_dimension_name);

  for (std::size_t i = 0; i < this->sharing_model.nodes_size(); ++i) {
    const auto node = this->sharing_model.node(i);

    if (node.nodetype == NodeType::ORDER_DIRECT) {
      const auto this_order = node.order;
      auto index = this->routing_managers.at(0).NodeToIndex(RoutingIndexManager::NodeIndex(i));

      dimension->CumulVar(index)->SetRange(this_order.start_time -1 , this_order.start_time + 3);
    } else if (node.nodetype == NodeType::ORDER_DELIVERY) {
      const auto this_order = node.order;
      auto index = this->routing_managers.at(0).NodeToIndex(RoutingIndexManager::NodeIndex(i));

      dimension->CumulVar(index)->SetRange(this_order.start_time, this_order.end_time);
    }

  }
}

auto SharingManager::AddPickupAndDelivery() -> void {

  auto dimension = this->routing_model->GetMutableDimension(time_cost_dimension_name);

  for (std::size_t i = 0; i < this->sharing_model.nodes_size(); ++i) {
    const auto node = this->sharing_model.node(i);

    if (node.nodetype != NodeType::ORDER_DIRECT)
      continue;

    const auto pickup_index = this->routing_managers.at(0).NodeToIndex(RoutingIndexManager::NodeIndex(i));
    const auto delivery_index = this->routing_managers.at(0).NodeToIndex(RoutingIndexManager::NodeIndex(i+1));

    this->routing_model->AddPickupAndDelivery(pickup_index, delivery_index);

    this->routing_model->solver()->AddConstraint(this->routing_model->solver()->MakeEquality(
        this->routing_model->VehicleVar(pickup_index),
        this->routing_model->VehicleVar(delivery_index)));

    this->routing_model->solver()->AddConstraint(
        this->routing_model->solver()->MakeLessOrEqual(dimension->CumulVar(pickup_index),
        dimension->CumulVar(delivery_index)));

  }

}

auto SharingManager::AddDemandDimension() -> void {
  auto callback = [&](std::int64_t node_index) -> std::int64_t
  {
    int idx = this->routing_managers.at(0).IndexToNode(node_index).value();

    return this->sharing_model.demand(idx);
  };

  auto callback_index = this->routing_model->RegisterUnaryTransitCallback(callback);

  this->routing_model->AddDimensionWithVehicleCapacity(
      callback_index,
      1000,
      this->sharing_model.vehicle_capacities(),
      false,
      capacity_dimension_name);

}


auto SharingManager::StartCalculate() -> const Solution {


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


  //  開始計算
  auto assignment = this->routing_model->SolveWithParameters(this->routing_search_parameter);

  //  輸出結果

  return this->ConvertSolution(*assignment);
}

auto SharingManager::ConvertSolution(const operations_research::Assignment &assignment) -> const Solution {
  auto solution = Solution();
  DebugPrint << StatusOfSearch(this->routing_model->status()) << std::endl;

  if (this->routing_model->status() == RoutingModel::Status::ROUTING_SUCCESS || this->routing_model->status() == RoutingModel::Status::ROUTING_PARTIAL_SUCCESS_LOCAL_OPTIMUM_NOT_REACHED) {
    const auto &time_dimension = this->routing_model->GetDimensionOrDie(time_cost_dimension_name);


    // vehicle routes
    for (std::size_t v_num = 0; v_num < this->sharing_model.vehicles_size(); ++v_num) {
      auto vehicle = this->sharing_model.parameter.vehicles.at(v_num);

      auto valid_vehicle = ValidVehicle(vehicle);
      std::int64_t passengers_counter = 0;
      //  valid vehicle
      if (this->routing_model->IsVehicleUsed(assignment, v_num)) {
        solution.report.used_vehicles_num++;

        int64_t index = this->routing_model->Start(v_num);
        int64_t route_distance{0};

        while (!(this->routing_model->IsEnd(index))) {
          auto node = this->sharing_model.node(index);
          auto destination = Destination(node);
          destination.arrival_distance = route_distance;
          auto time_var = time_dimension.CumulVar(index);
          destination.arrival_time_early = assignment.Min(time_var);
          destination.arrival_time_lately = assignment.Max(time_var);
          if (node.nodetype == NodeType::ORDER_DIRECT) {
            passengers_counter += node.order.headcount;
            destination.passengers_num = passengers_counter;
          } else if (node.nodetype == NodeType::ORDER_DELIVERY) {
            passengers_counter -= node.order.headcount;
            destination.passengers_num = passengers_counter;
          }
          valid_vehicle.destinations.push_back(destination);


          const int64_t previous_index = index;
          index = assignment.Value(this->routing_model->NextVar(index));
          route_distance += this->routing_model->GetArcCostForVehicle(previous_index, index,
                                                        int64_t(v_num));
        }

        auto node = this->sharing_model.node(index);
        auto destination = Destination(node);
        destination.arrival_distance = route_distance;
        auto time_var = time_dimension.CumulVar(index);
        destination.arrival_time_early = assignment.Min(time_var);
        destination.arrival_time_lately = assignment.Max(time_var);
        if (node.nodetype == NodeType::ORDER_DIRECT) {
          passengers_counter++;
          destination.passengers_num = passengers_counter;
        } else if (node.nodetype == NodeType::ORDER_DELIVERY) {
          passengers_counter--;
          destination.passengers_num = passengers_counter;
        }
        valid_vehicle.destinations.push_back(destination);

      }

      solution.valid_vehicles.push_back(valid_vehicle);
    }

    // droppeds
    for (std::int64_t i = 0; i < this->routing_model->Size(); ++i) {
      if (this->routing_model->IsStart(i) || this->routing_model->IsEnd(i))
        continue;

      if (assignment.Value(this->routing_model->NextVar(i)) == i) {
        auto index = this->routing_managers.at(0).IndexToNode(i).value();
        auto node = this->sharing_model.node(index);
        auto destination = Destination(node);

        solution.droppeds.push_back(destination);
      }
    }

  } else {
    for (std::size_t i = 0; i < this->sharing_model.nodes_size(); ++i) {
      auto node = this->sharing_model.node(i);

      if (node.nodetype == NodeType::ORDER_DIRECT || node.nodetype == NodeType::ORDER_DELIVERY) {
        auto destination = Destination(node);
        solution.droppeds.push_back(destination);
      }
    }
  }

  const auto *solver = this->routing_model->solver();
  solution.report.routing_status = StatusOfSearch(this->routing_model->status());
  solution.report.wall_time = solver->wall_time();
  solution.report.memory_usage = solver->MemoryUsage();
  solution.report.branches = solver->branches();
  solution.report.solutions = solver->solutions();
  solution.report.droppeds_num = solution.droppeds.size();
  if (solver->solutions() > 0) {
    solution.report.minimum_obj_cost = assignment.ObjectiveValue();
  }
  solution.report.totally_vehicles_num = this->sharing_model.parameter.vehicles.size();
  solution.report.totally_orders_num = this->sharing_model.parameter.orders.size();

  DebugPrint << "ConvertSolution done" << std::endl;

  return solution;
}

}  // namespace airpublic
}  // namespace airsharing




std::string pydispatch_output = "{}";
extern "C" {
const char *PyDispatch(const char *json_ptr) {
  pydispatch_output.clear();
  std::string json_str = json_ptr;

  pydispatch_output = airouting::airsharing::Dispatch(json_str);
  return pydispatch_output.data();
}
};