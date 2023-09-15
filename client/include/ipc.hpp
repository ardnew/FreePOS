#ifndef ipc_hpp
#define ipc_hpp

#include "spec.hpp"

template <typename T, size_t N = 32>
class PacketBuf: public RingBuf<Packet<T>, N> {
  using RingBuf<Packet<T>, N>::RingBuf;
  std::mutex _mutx;
public:
  bool push(const uint8_t *ptr, const size_t size) {
    std::lock_guard<std::mutex> lck(_mutx);
    return RingBuf<Packet<T>, N>::pushOverwrite(Packet<T>(ptr, size));
  }
};

class IPC {
public:
  IPC(): // Constructor, no args required.
    // UInt32 FIFOs
    Pulses{},
    // Int32 FIFOs
    Proxim{},
    // Float FIFOs
    BarPsi{}, Precip{}, AirTmp{},
    H2OTmp{}, Weight{},
    // Float 3D FIFOs
    AccXyz{}
  {}

  PacketBuf<uint32_t> Pulses;
  PacketBuf<int32_t> Proxim;
  PacketBuf<float> BarPsi, Precip, AirTmp, H2OTmp, Weight;
  PacketBuf<std::array<float, 3>> AccXyz;
};

#endif // ipc_hpp
