
#include <functional>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "TriacDimmer.h"

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

TriacDimmer::TriacDimmer(int zeroCrossPin, int psmPin)
  : zeroCrossPin_(zeroCrossPin)
  , psmPin_(psmPin)
{
  ZeroCrossCallback<void(uint, uint32_t)>::func = std::bind(&TriacDimmer::zeroCrossCallback, this, std::placeholders::_1, std::placeholders::_2);
  gpio_set_irq_enabled_with_callback(this->zeroCrossPin_, GPIO_IRQ_EDGE_RISE, true, ZeroCrossCallback<void(uint, uint32_t)>::callback);
  
  gpio_init(this->psmPin_);
  gpio_set_dir(this->psmPin_, GPIO_OUT);
}

void TriacDimmer::resetZeroCross()
{
  this->zeroCross_ = false;
}

int TriacDimmer::getDimPercentage()
{
  return this->map(this->psmDuration_, MIN_PSM_DURATION, MAX_PSM_DURATION, 0, 100);
}

void TriacDimmer::setDimPercentage(uint8_t dimPercentage)
{
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
  return this->psmDuration_;
}

void TriacDimmer::setPsmDuration(int psmDuration)
{
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
  if(this->zeroCross_)
  {
    this->resetZeroCross();
    sleep_us(this->psmDuration_);
    gpio_put(this->psmPin_, 1);
    sleep_us(8.333);
    gpio_put(this->psmPin_, 0);
  }
}

int TriacDimmer::map(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}