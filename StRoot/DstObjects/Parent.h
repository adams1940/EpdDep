#ifndef PARENT_H
#define PARENT_H

// ROOT
#include "TVector3.h"
#include "TLorentzVector.h"
// DstObjects
#include "Particle.h"
#include "StableDaughter.h"

class Parent : public Particle{
private:
    TLorentzVector m_FourMomentum;
    TVector3 m_DecayPoint;
    StableDaughter m_DaughterProton, m_DaughterPion;

public:
  Parent();
  virtual ~Parent();

  void FourMomentum(const TLorentzVector &p){m_FourMomentum = p;}
    TLorentzVector FourMomentum(){return m_FourMomentum;}
  void DecayPoint(const TVector3 &p){m_DecayPoint = p;}
    TVector3 DecayPoint(){return m_DecayPoint;}
  void DaughterProton(const StableDaughter &d){m_DaughterProton = d;}
    StableDaughter DaughterProton(){return m_DaughterProton;}
  void DaughterPion(const StableDaughter &d){m_DaughterPion = d;}
    StableDaughter DaughterPion(){return m_DaughterPion;}

ClassDef(Parent,1)
};
#endif