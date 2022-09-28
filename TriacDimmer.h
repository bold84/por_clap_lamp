#ifndef _TRIAC_DIMMER_H_
#define _TRIAC_DIMMER_H_

#define MIN_PSM_DURATION 4600
#define MAX_PSM_DURATION 8200

class TriacDimmer
{
public:
  TriacDimmer(int zeroCrossPin, int psmPin);
  ~TriacDimmer() = default;
  
  void resetZeroCross();
  
  int getDimPercentage();
  void setDimPercentage(uint8_t dimPercentage);
  
  int getPsmDuration() const;
  void setPsmDuration(int psmDuration);
  
  void dim();
  
private:
  int zeroCrossPin_ { 0 };
  int psmPin_ { 0 };
  int psmDuration_ { 4500 };
  volatile bool zeroCross_ { false };
  
  void zeroCrossCallback(uint gpio, uint32_t events)
  {
    this->zeroCross_ = true;
  }
  
  int map(int x, int in_min, int in_max, int out_min, int out_max);
  
};


#endif // _TRIAC_DIMMER_H_
