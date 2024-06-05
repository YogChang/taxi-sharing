
#include <string>
#include <vector>

#include "src/sharing/config.h"

#ifndef SRC_SHARING_DAO_SOLUTION_H_
#define SRC_SHARING_DAO_SOLUTION_H_

namespace airouting {
namespace airsharing {


class Solution {
 public:
  Solution() {}
  ~Solution() {}
  auto ToJson() -> json;



};

auto Solution::ToJson() -> json {
  json ret = {
    {"solution", "unknown"},
    {"time", 0}
  };

  return ret;
}



}  // namespace airsharing
}  // namespace airouting

#endif  // SRC_SHARING_DAO_SOLUTION_H_
