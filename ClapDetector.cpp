
#include <functional>

#include <pico/stdlib.h>
#include <hardware/gpio.h>

#include "ClapDetector.h"



// Constructor for the ClapDetector class.
ClapDetector::ClapDetector(int soundSensorPin)
  : soundSensorPin_(soundSensorPin)
{
  // Set the callback function for the clap detector.
  //ClapCallback<void(uint, uint32_t)>::func = std::bind(&ClapDetector::clapCallback, this, std::placeholders::_1, std::placeholders::_2);
  // Enable the interrupt for the sound sensor pin.
  //gpio_set_irq_enabled_with_callback(this->soundSensorPin_, GPIO_IRQ_EDGE_FALL, true, ClapCallback<void(uint, uint32_t)>::callback);
}

void ClapDetector::clapCallback(uint gpio, uint32_t events)
{
  // Implement the callback function for the clap detector. There can be multiple claps
  // in a short time, so we need to debounce the claps. We do this by checking if the
  // time since the last clap is greater than the debounce time. If it is, then we
  // increment the clap count.
  uint32_t currentTime = to_ms_since_boot(get_absolute_time());
  if(currentTime - this->lastClapTime_ > this->clapDebounceTime_)
  {
    this->lastClapTime_ = currentTime;
    this->clapCount_++;
  }
}

int ClapDetector::getClapCount()
{
  // check if the clapSequenceTimeout_ has expired. If it has, then reset the clap count
  // and return the clap count. If it hasn't, then return 0.
  uint32_t currentTime = to_ms_since_boot(get_absolute_time());
  if(currentTime - this->lastClapTime_ > this->clapSequenceTimeout_)
  {
    int clapCount = this->clapCount_;
    this->clapCount_ = 0;
    return clapCount;
  }
  else
  {
    return 0;
  }
}
