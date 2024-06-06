#include <algorithm>
#include <cstdint>
#include <sstream>
#include <vector>

#include "sharing_model.hpp"
#include "src/sharing/dao/solution.h"

#include "ortools/constraint_solver/routing.h"
#include "ortools/constraint_solver/routing_enums.pb.h"
#include "ortools/constraint_solver/routing_index_manager.h"
#include "ortools/constraint_solver/routing_parameters.h"



#ifndef SRC_SHARING_ALGO_SHARING_HPP_
#define SRC_SHARING_ALGO_SHARING_HPP_

namespace airouting {
namespace airsharing {
using namespace operations_research;

auto Dispatch(const std::string &str) -> const std::string;

class SharingManager {
 public:

  ~SharingManager() {}
  SharingManager(const SharingModel &sharing_model);
  auto StartCalculate() -> const Solution;

  auto AddTransitDistanceDimension() -> void;
  auto AddTransitTimeDimension() -> void;
  auto AddCostEvaluatorOfDistance() -> void;
  auto AddPriorityDisjunction() -> void;
  auto AddVehicleTimeWindow() -> void;
  auto AddOrderTimeWindow() -> void;
  auto AddPickupAndDelivery() -> void;

 private:
  const SharingModel &sharing_model;

  std::vector<RoutingIndexManager> routing_managers;
  RoutingModel *routing_model = nullptr;
  RoutingSearchParameters routing_search_parameter = DefaultRoutingSearchParameters();


  auto ConvertSolution(const operations_research::Assignment &assignment) -> const Solution;
};





}  // namespace airpublic
}  // namespace airsharing

#endif  // SRC_SHARING_ALGO_SHARING_HPP_