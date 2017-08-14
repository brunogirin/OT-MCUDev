/*
 * sensors.h
 *
 *  Created on: 11 Aug 2017
 *      Author: denzo
 */

#ifndef SRC_SENSORS_H_
#define SRC_SENSORS_H_

#include <stdint.h>


namespace OTV0P2BASE {

#if 0
// Minimal lightweight sensor subset.
// Contains just enough to check availability and to name and get the latest value.
template <class T>
class SensorCore
  {
  public:
    // Type of sensed data.
    typedef T data_t;

    // Return last value fetched by read(); undefined before first read().
    // Usually fast.
    // Often likely to be thread-safe or usable within ISRs (Interrupt Service Routines),
    // BUT READ IMPLEMENTATION DOCUMENTATION BEFORE TREATING AS thread/ISR-safe.
    virtual T get() const = 0;

    // Returns true if this sensor is currently available.
    // True by default unless implementation overrides.
    // For those sensors that need starting this will be false before begin().
    virtual bool isAvailable() const { return(true); }

    // Returns a suggested (JSON) tag/field/key name including units of get(); NULL means no recommended tag.
    // The lifetime of the pointed-to text must be at least that of the Sensor instance.
//    virtual Sensor_tag_t tag() const { return(NULL); }

  };

// Base sensor type.
// Templated on sensor value type, typically uint8_t or uint16_t or int16_t.
template <class T>
class Sensor : public SensorCore<T>
  {
  public:
    // Force a read/poll of this sensor and return the value sensed.
    // May be expensive/slow.
    // For many implementations read() should be called at a reasonably steady rate,
    // see preferredPollInterval_s().
    // Unlikely to be thread-safe or usable within ISRs (Interrupt Service Routines).
    // Individual implementations can document alternative behaviour.
    virtual T read() = 0;

    // Returns true if this sensor reading value passed is potentially valid, eg in-range.
    // Default is to always return true, ie all values potentially valid.
    virtual bool isValid(T /*value*/) const { return(true); }

    // Returns non-zero if this implementation requires a regular call to read() to operate correctly.
    // Preferred poll interval (in seconds) or 0 if no regular poll() call required.
    // Default returns 0 indicating regular call to read() not required,
    // only as required to fetch new values from the underlying sensor.
    virtual uint_fast8_t preferredPollInterval_s() const { return(0); }

//    // Returns a suggested privacy/sensitivity level of the data from this sensor.
//    // The default sensitivity is set to just forbid transmission at default (255) leaf settings.
//    virtual uint8_t sensitivity() const { return(254 /* stTXsecOnly */ ); }

    // Handle simple interrupt for this sensor.
    // Must be fast and ISR (Interrupt Service Routine) safe.
    // Returns true if interrupt was successfully handled and cleared
    // else another interrupt handler in the chain may be called
    // to attempt to clear the interrupt.
    // By default does nothing (and returns false).
    virtual bool handleInterruptSimple() { return(false); }
  };


// Abstract temperature sensor in 1/16th of one degree Celsius.
// Nominally covers a range from well below 0C to at least 100C
// for room and DHW temperature monitoring.
// May cover a wider range for other specialist monitoring.
// Some devices may indicate an error by returning a zero or (very) negative value.
// A returned value can be tested for validity with isErrorValue().
class TemperatureC16Base : public Sensor<int_fast16_t>
  {
  public:
    // Error value returned if device unavailable or not yet read.
    // Negative and below minimum value that DS18B20 can return legitimately (-55C).
    static constexpr int_fast16_t DEFAULT_INVALID_TEMP = -128 * 16; // Nominally -128C.

  protected:
    // Room temperature in 16*C, eg 1 is 1/16 C, 32 is 2C, -64 is -4C.
    // Never expected to be updated or used in an ISR, so not marked volatile.
    int_fast16_t value = DEFAULT_INVALID_TEMP;

    // Prevent instantiation of a naked instance.
    // Starts off with a detectably-invalid value, eg for before read() is called first.
    constexpr TemperatureC16Base() { }

  public:
    // Returns true if the given value indicates, or may indicate, an error.
    // If false then the value passed is likely legitimate.
//    virtual bool isErrorValue(int_fast16_t value) const { return(DEFAULT_INVALID_TEMP == value); }

    // Returns number of useful binary digits after the binary point; default is 4.
    // May be negative if some of the digits BEFORE the binary point are not usable.
    // Some sensors may dynamically return fewer places.
    virtual int_fast8_t getBitsAfterPoint() const { return(4); }

    // Returns true if fewer than 4 bits of useful data after the binary point.
    bool isLowPrecision() const { return(getBitsAfterPoint() < 4); }

    // Preferred poll interval (in seconds).
    // This should be called at a regular rate, usually 1/60, so make stats such as velocity measurement easier.
    virtual uint_fast8_t preferredPollInterval_s() const override { return(60); }

    // Return last value fetched by read(); undefined before first read().
    // Fast.
    // Not thread-safe nor usable within ISRs (Interrupt Service Routines).
    virtual int_fast16_t get() const override { return(value); }

    // Returns a suggested (JSON) tag/field/key name including units of get(); NULL means no recommended tag.
    // The lifetime of the pointed-to text must be at least that of the Sensor instance.
//    virtual OTV0P2BASE::Sensor_tag_t tag() const override { return(V0p2_SENSOR_TAG_F("T|C16")); }
  };
#endif

class RoomTemperatureC16_SHT21 final //: public TemperatureC16Base
  { public: int_fast16_t value = -2048; int_fast16_t read();};



}
#endif /* SRC_SENSORS_H_ */
