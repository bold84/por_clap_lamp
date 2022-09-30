#ifndef _TRIAC_DIMMER_H_
#define _TRIAC_DIMMER_H_

#define MIN_PSM_DURATION 4600   // Minimum PSM duration in microseconds.
#define MAX_PSM_DURATION 8200   // Maximum PSM duration in microseconds.

/**
 * @brief Class for controlling a triac dimmer.
 * 
 * This class is used to control a triac dimmer. It uses the Pico's hardware timer to
 * generate a zero cross signal. The zero cross signal is used to trigger the triac.
 * The Pico's hardware timer is also used to generate the PSM signal. The PSM signal
 * is used to control the triac's firing angle.
 * 
 */
class TriacDimmer
{
public:
  
  /**
   * @brief Constructor.
   * 
   * @param psmPin The GPIO pin number for the PSM signal.
   */
  TriacDimmer(int psmPin);
  
  /**
   * @brief Destructor.
   * 
   */
  ~TriacDimmer() = default;
  
  /**
   * @brief Dim the light.
   * 
   * This method is used to reset the zero cross flag.
   * 
   */
  void resetZeroCross();
  
  /**
   * @brief Get the dim percentage.
   * 
   * This method is used to get the dim percentage.
   * 
   * @return int The dim percentage.
   */
  int getDimPercentage();
  
  /**
   * @brief Set the dim percentage.
   * 
   * This method is used to set the dim percentage.
   * 
   * @param dimPercentage The dim percentage.
   */
  void setDimPercentage(int dimPercentage);
  
  /**
   * @brief Increase the dim percentage.
   * 
   * @param increasePct 
   */
  void increaseDimPercentage(int increasePct = 20);
  
  /**
   * @brief Decrease the dim percentage.
   * 
   * @param descreasePct 
   */
  void decreaseDimPercentage(int descreasePct = 20);
  
  /**
   * @brief Get the PSM duration.
   * 
   * This method is used to get the PSM duration.
   * 
   * @return int The PSM duration
   */
  int getPsmDuration() const;
  
  /**
   * @brief Set the PSM duration.
   * 
   * This method is used to set the PSM duration.
   * 
   * @param psmDuration The PSM duration.
   */
  void setPsmDuration(int psmDuration);
  
  /**
   * @brief Get the power state
   * 
   * @return true 
   * @return false 
   */
  bool getPowerState() const;
  
  /**
   * @brief Set power on or off.
   * 
   * @param power 
   */
  void setPowerState(bool power);
  
  /**
   * @brief Dim the light.
   * 
   * This method is used to dim the light.
   * 
   */
  void dim();
  
  /**
   * @brief Map a value from one range to another.
   * 
   * This is the callback function that is called when a zero cross is detected.
   * 
   * @param gpio The GPIO pin that triggered the callback.
   * @param events The events that triggered the callback.
   */
  void zeroCrossCallback(uint gpio, uint32_t events)
  {
    this->zeroCross_ = true;
  }
  
private:

  int zeroCrossPin_ { 0 };                      // The pin that is connected to the zero cross detector.
  int psmPin_ { 0 };                            // The pin that is connected to the PSM.
  int psmDuration_ { MIN_PSM_DURATION };        // The duration of the PSM in microseconds.
  volatile bool zeroCross_ { false };           // Flag that indicates if a zero cross has been detected.
  bool powerState_ { true };                    // Flag that indicates if the light is on or off.
  
  /**
   * @brief Map a value from one range to another.
   * 
   * This method is used to map a value from one range to another.
   * 
   * @param x The value to map.
   * @param in_min The minimum value of the input range.
   * @param in_max The maximum value of the input range.
   * @param out_min The minimum value of the output range.
   * @param out_max The maximum value of the output range.
   * @return int The mapped value.
   */
  int map(int x, int in_min, int in_max, int out_min, int out_max);
  
};


#endif // _TRIAC_DIMMER_H_
