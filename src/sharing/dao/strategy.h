

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

  json ToJson() {
    json ret = {
      {"first_solution_strategy", first_solution_strategy},
      {"metaheuristic", metaheuristic},
      {"time_limit", time_limit}
    };

    return ret;
  }

  std::int64_t first_solution_strategy = 8;
  std::int64_t metaheuristic = 3;
  std::int64_t time_limit = 60;
};



}  // namespace airpublic
}  // namespace airsharing

#endif  // SRC_SHARING_DAO_STRATEGY_H_
