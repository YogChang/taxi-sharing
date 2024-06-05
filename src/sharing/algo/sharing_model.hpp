
#include "src/sharing/wrapper/sharing_wrapper.hpp"


#ifndef SRC_SHARING_WRAPPER_SHARINGMODEL_H_
#define SRC_SHARING_WRAPPER_SHARINGMODEL_H_

namespace airouting {
namespace airsharing {

class SharingModel {
 public:
  SharingModel() {}
  ~SharingModel() {}
  SharingModel(const Parameter &parameter);

 private:
  Parameter parameter;
  auto Initialize() -> void;
};

SharingModel::SharingModel(const Parameter &parameter) {
  this->parameter = parameter;
  this->Initialize();
}

auto SharingModel::Initialize() -> void {
  // todo
}

}  // namespace airpublic
}  // namespace airsharing

#endif  // SRC_SHARING_WRAPPER_SHARINGMODEL_H_