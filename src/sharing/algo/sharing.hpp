#include <algorithm>
#include <cstdint>
#include <sstream>
#include <vector>

#include "sharing_model.h"

#include "ortools/constraint_solver/routing.h"
#include "ortools/constraint_solver/routing_enums.pb.h"
#include "ortools/constraint_solver/routing_index_manager.h"
#include "ortools/constraint_solver/routing_parameters.h"

#ifndef VRP_HPP_
#define VRP_HPP_

namespace operations_research {
void VrpGlobalSpan();
}  // namespace operations_research


#endif