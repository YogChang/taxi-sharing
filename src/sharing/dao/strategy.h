

#include <string>
#include <vector>

#include "src/sharing/config.h"


#ifndef SRC_SHARING_DAO_STRATEGY_H_
#define SRC_SHARING_DAO_STRATEGY_H_

namespace airouting {
namespace airsharing {


class Strategy {
 public:
  Strategy() {}
  ~Strategy() {}
  auto ToJson() -> json;


  std::int64_t first_solution_strategy = 8;
  std::int64_t metaheuristic = 3;
  std::int64_t time_limit = 60;
  std::int64_t solution_limit = -1;
  std::int64_t dropped_penalty_cost = 100000000;
};

auto Strategy::ToJson() -> json {
  json ret = {
    {"first_solution_strategy", first_solution_strategy},
    {"metaheuristic", metaheuristic},
    {"time_limit", time_limit},
    {"solution_limit", solution_limit},
    {"dropped_penalty_cost", dropped_penalty_cost}
  };

  return ret;
}

}  // namespace airpublic
}  // namespace airsharing

#endif  // SRC_SHARING_DAO_STRATEGY_H_
