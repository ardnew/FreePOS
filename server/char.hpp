#ifndef char_hpp
#define char_hpp

#include <sensor.h>

#include "spec.hpp"

class BLETaggedShortCharacteristic : public BLETypedCharacteristic<TaggedInt16> {
public:
  BLETaggedShortCharacteristic(const char* uuid, unsigned int properties):
    BLETypedCharacteristic<TaggedInt16>(uuid, properties) {}
};

class BLETaggedUnsignedShortCharacteristic : public BLETypedCharacteristic<TaggedUInt16> {
public:
  BLETaggedUnsignedShortCharacteristic(const char* uuid, unsigned int properties):
    BLETypedCharacteristic<TaggedUInt16>(uuid, properties) {}
};

class BLETaggedIntCharacteristic : public BLETypedCharacteristic<TaggedInt32> {
public:
  BLETaggedIntCharacteristic(const char* uuid, unsigned int properties):
    BLETypedCharacteristic<TaggedInt32>(uuid, properties) {}
};

class BLETaggedUnsignedIntCharacteristic : public BLETypedCharacteristic<TaggedUInt32> {
public:
  BLETaggedUnsignedIntCharacteristic(const char* uuid, unsigned int properties):
    BLETypedCharacteristic<TaggedUInt32>(uuid, properties) {}
};

class BLETaggedFloatCharacteristic : public BLETypedCharacteristic<TaggedFloat32> {
public:
  BLETaggedFloatCharacteristic(const char* uuid, unsigned int properties):
    BLETypedCharacteristic<TaggedFloat32>(uuid, properties) {}
};

#endif // char_hpp
