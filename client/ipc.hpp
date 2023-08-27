#ifndef ipc_hpp
#define ipc_hpp

#include "spec.hpp"

class IPC {
private:
  static constexpr size_t ESz = 32;
public:
  // Tupe definitions for all FIFOs used.
  using Int32 = RingBuf<Packet<int32_t>, ESz>;
  using Float = RingBuf<Packet<float>, ESz>;

  IPC(): // Constructor, no args required.
    // Float FIFOs
    AccelX{}, AccelY{}, AccelZ{},
    BarPsi{}, Precip{}, AirTmp{},
    H2OTmp{}, Weight{},
    // Int32 FIFOs
    Proxim{}
  {}

  /// Float FIFOs

  struct FloatFIFO: public Float {
    std::mutex _mutx;
    const bool push(const Packet<float> &fd) {
      std::lock_guard<std::mutex> lck(_mutx);
      return Float::push(fd);
    }
  } AccelX, AccelY, AccelZ,
    BarPsi, Precip, AirTmp,
    H2OTmp, Weight;

  /// Int32 FIFOs

  struct Int32FIFO: public Int32 {
    std::mutex _mutx;
    bool push(const Packet<int32_t> &id) {
      std::lock_guard<std::mutex> lck(_mutx);
      return Int32::push(id);
    }
  } Proxim;
};

#endif // ipc_hpp
