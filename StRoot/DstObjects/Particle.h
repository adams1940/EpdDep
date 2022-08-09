#ifndef PARTICLE_H
#define PARTICLE_H

#include "TObject.h"

class Particle : public TObject{
private:
  int m_TrackId, m_Charge;

public:
  Particle();
  virtual ~Particle();

  void TrackId(int ID){m_TrackId = ID;}
    int TrackId(){return m_TrackId;}
  void Charge(int ID){m_Charge = ID;}
    int Charge(){return m_Charge;}

ClassDef(Particle,1)
};
#endif