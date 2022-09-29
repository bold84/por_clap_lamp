
#include <functional>

#include <pico/stdlib.h>
#include <hardware/gpio.h>

#include "ClapDetector.h"

// Template for callback function. This is needed because the callback function
// needs to be a static function, but we want to be able to call a C++ member function
// from the callback.
template <typename T>
struct ClapCallback;

template <typename Ret, typename... Params>
struct ClapCallback<Ret(Params...)> {
    template <typename... Args>
    static Ret callback(Args... args) { return func(args...); }
    static std::function<Ret(Params...)> func;
};

// Initialize the static member.
template <typename Ret, typename... Params>
std::function<Ret(Params...)> ClapCallback<Ret(Params...)>::func;

// Constructor for the ClapDetector class.
ClapDetector::ClapDetector(int soundSensorPin)
  : soundSensorPin_(soundSensorPin)
{
  // Set the callback function for the clap detector.
  ClapCallback<void(uint, uint32_t)>::func = std::bind(&ClapDetector::clapCallback, this, std::placeholders::_1, std::placeholders::_2);
  // Enable the interrupt for the sound sensor pin.
  gpio_set_irq_enabled_with_callback(this->soundSensorPin_, GPIO_IRQ_EDGE_FALL, true, ClapCallback<void(uint, uint32_t)>::callback);
}
