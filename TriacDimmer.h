#ifndef _TRIAC_DIMMER_H_
#define _TRIAC_DIMMER_H_

#define MIN_PSM_DURATION 4500
#define MAX_PSM_DURATION 7500

class TriacDimmer
{
public:
  TriacDimmer(int zeroCrossPin, int psmPin);
  ~TriacDimmer() = default;
  
  void resetZeroCross();
  
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
  
};


#endif // _TRIAC_DIMMER_H_
