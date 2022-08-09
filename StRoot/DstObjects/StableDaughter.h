#ifndef STABLEDAUGHTER_H
#define STABLEDAUGHTER_H

// ROOT
#include "TLorentzVector.h"
// DstObjects
#include "Particle.h"
#include "StableDaughter.h"

class StableDaughter : public Particle{
private:
  TLorentzVector m_FourMomentumAtDecay;

public:
  StableDaughter();
  virtual ~StableDaughter();

  void FourMomentumAtDecay(const TLorentzVector &p){m_FourMomentumAtDecay = p;}
    TLorentzVector FourMomentumAtDecay(){return m_FourMomentumAtDecay;}

ClassDef(StableDaughter,1)
};
#endif