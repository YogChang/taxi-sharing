
#include <float.h>
#include <cstdint>
#include "nlohmann/json.hpp"

#ifndef AIROUTING_AIRSHARING_CONFIG_H_
#define AIROUTING_AIRSHARING_CONFIG_H_





namespace airouting {
namespace airsharing {
using json = nlohmann::json;

struct ErrorCode {
    const int code = 0;
    const std::string message = "";
    ErrorCode(const int code, const std::string& message) : code(code), message(message) {}
    auto ToJson() -> json {
        return json {
            {"code", code},
            {"message", message}
        };
    }
};

class DebugConfig {
    const bool is_debug_;
    std::vector<ErrorCode> error_codes_;
public:
    template<typename T>
    DebugConfig& log(const T& data) {
        if (is_debug_) {
            std::cout << data;
        }
        return *this;
    }
    DebugConfig(bool is_debug) : is_debug_(is_debug) {}

    DebugConfig& log(std::ostream& (*manipulator)(std::ostream&)) {
      if (is_debug_) {
          std::cout << manipulator;
      }
      return *this;
    }

    DebugConfig& log(const ErrorCode& error_code) {
        error_codes_.push_back(error_code);

        return *this;
    }
    json ToJson() {
        std::vector<json> json_error_codes_(this->error_codes_.size());
        std::transform(this->error_codes_.begin(), this->error_codes_.end(), json_error_codes_.begin(), [](ErrorCode e) { return e.ToJson(); });

        return json_error_codes_;
    }

} DebugPrint(true);

// 使之可以連續使用 << 語法
template<typename T>
DebugConfig& operator<<(DebugConfig& config, const T& data) {
    return config.log(data);
}
// 支援插入特殊操作符 std::endl
DebugConfig& operator<<(DebugConfig& config, std::ostream& (*manipulator)(std::ostream&)) {
    return config.log(manipulator);
}





// 0L = 01/01/1970 @ 12:00am (UTC)
static const std::int64_t minDateTime = 0L;

// 253402300799L = 12/31/9999 @ 11:59pm (UTC)
static const std::int64_t maxDateTime = 253402300799L;

static const std::int64_t maxOfSystem = INT64_MAX;


}  // namespace airpublic
}  // namespace airsharing

#endif