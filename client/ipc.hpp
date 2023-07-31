#ifndef ipc_hpp
#define ipc_hpp

#include <RingBuf.h>

#include <mutex>

// FreePOS API
#include "ble.hpp"

class SensorFIFO {
private:
  static constexpr size_t ESz = 32;
public:
  using Int32 = RingBuf<TaggedData<int32_t>, ESz>;
  using Float = RingBuf<TaggedData<float>, ESz>;

  struct: public Float {
    std::mutex _mutx;
    bool push(TaggedData<float> &fd) {
      std::lock_guard<std::mutex> lck(_mutx);
      return Float::push(fd);
    }
  } AccelX, AccelY, AccelZ, BarPsi, Precip, AirTmp, H2OTmp, Weight;

  struct: public Int32 {
    std::mutex _mutx;
    bool push(TaggedData<int32_t> &id) {
      std::lock_guard<std::mutex> lck(_mutx);
      return Int32::push(id);
    }
  } Proxim;



};

#endif // ipc_hpp
