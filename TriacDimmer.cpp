
#include <functional> // for std::function

#include "pico/stdlib.h" // for sleep_ms
#include "hardware/gpio.h" // for gpio_set_irq_enabled_with_callback and GPIO_IRQ_EDGE_FALL, etc.

#include "TriacDimmer.h" // for TriacDimmer class definition

// Template for callback function. This is needed because the callback function
// needs to be a static function, but we want to be able to call a C++ member function
// from the callback.
template <typename T>
struct ZeroCrossCallback;

template <typename Ret, typename... Params>
struct ZeroCrossCallback<Ret(Params...)> {
    template <typename... Args>
    static Ret callback(Args... args) { return func(args...); }
    static std::function<Ret(Params...)> func;
};

// Initialize the static member.
template <typename Ret, typename... Params>
std::function<Ret(Params...)> ZeroCrossCallback<Ret(Params...)>::func;

// Constructor for the TriacDimmer class.
TriacDimmer::TriacDimmer(int zeroCrossPin, int psmPin)
  : zeroCrossPin_(zeroCrossPin)
  , psmPin_(psmPin)
{
  // Set the callback function for the zero cross detector.
  ZeroCrossCallback<void(uint, uint32_t)>::func = std::bind(&TriacDimmer::zeroCrossCallback, this, std::placeholders::_1, std::placeholders::_2);
  // Enable the interrupt for the zero cross pin.
  gpio_set_irq_enabled_with_callback(this->zeroCrossPin_, GPIO_IRQ_EDGE_RISE, true, ZeroCrossCallback<void(uint, uint32_t)>::callback);
  
  // Set the PSM pin to output.
  gpio_init(this->psmPin_);
  gpio_set_dir(this->psmPin_, GPIO_OUT);
}

void TriacDimmer::resetZeroCross()
{
  // Reset the zero cross flag.
  this->zeroCross_ = false;
}

int TriacDimmer::getDimPercentage()
{
  // Return the dim percentage by converting the PSM duration to a percentage.
  return this->map(this->psmDuration_, MIN_PSM_DURATION, MAX_PSM_DURATION, 0, 100);
}

void TriacDimmer::setDimPercentage(uint8_t dimPercentage)
{
  // Set the PSM duration by converting the dim percentage to a PSM duration. But first
  // make sure the dim percentage is within the allowed range.
  if(dimPercentage > 100)
  {
    dimPercentage = 100;
  }
  else if(dimPercentage < 0)
  {
    dimPercentage = 0;
  }
  this->psmDuration_ = this->map(dimPercentage, 0, 100, MIN_PSM_DURATION, MAX_PSM_DURATION);
}

int TriacDimmer::getPsmDuration() const
{
  // Return the PSM duration.
  return this->psmDuration_;
}

void TriacDimmer::setPsmDuration(int psmDuration)
{
  // Set the PSM duration. But make sure it is within the allowed range.
  if(psmDuration < MIN_PSM_DURATION)
  {
    this->psmDuration_ = MIN_PSM_DURATION;
  }
  else if(psmDuration > MAX_PSM_DURATION)
  {
    this->psmDuration_ = MAX_PSM_DURATION;
  }
  else
  {
    this->psmDuration_ = psmDuration;
  }
}

void TriacDimmer::dim()
{
  // Check if the zero cross flag is set.
  if(this->zeroCross_)
  {
    // Reset the zero cross flag.
    this->resetZeroCross();
    // Sleep for the PSM duration.
    sleep_us(this->psmDuration_);
    // Set the PSM pin high.
    gpio_put(this->psmPin_, 1);
    // Sleep for the rest of the zero cross period.
    sleep_us(8.333);
    // Set the PSM pin low.
    gpio_put(this->psmPin_, 0);
  }
}

int TriacDimmer::map(int x, int in_min, int in_max, int out_min, int out_max)
{
  // Map a value from one range to another.
  // Example: map(50, 0, 100, 0, 255) will return 127.
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}