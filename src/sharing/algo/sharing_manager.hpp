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


class SharingManager {
 public:

  ~SharingManager() {}
  SharingManager(const SharingModel &sharing_model);
  auto StartCalculate() -> const Solution;

 private:
  const SharingModel &sharing_model;
  std::vector<RoutingIndexManager> routing_managers;
  RoutingModel *routing_model = nullptr;
};





}  // namespace airpublic
}  // namespace airsharing

#endif  // SRC_SHARING_ALGO_SHARING_HPP_