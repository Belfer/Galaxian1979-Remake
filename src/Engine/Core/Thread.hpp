#pragma once

#include "Timer.hpp"
#include <thread>

namespace NHTV {
namespace Thread {
template <typename Res, typename Ratio> void sleep(Time<Res, Ratio> duration) {
  std::this_thread::sleep_for(duration);
}
}
}
