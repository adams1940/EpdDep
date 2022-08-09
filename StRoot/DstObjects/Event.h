#ifndef EVENT_H
#define EVENT_H

#include <vector>
// ROOT
#include "TObject.h"
#include "TVector3.h"
// DstObjects
#include "Parent.h"

class Event : public TObject {
private:
  int m_RefMult;
  TVector3 m_PrimaryVertex;
  float m_BField;

public:
  Event();
  virtual ~Event();

  void RefMult(const int &Mult){m_RefMult = Mult;}
    int RefMult(){return m_RefMult;}
  void PrimaryVertex(const TVector3 &PV){m_PrimaryVertex = PV;}
    TVector3 PrimaryVertex(){return m_PrimaryVertex;}
  void BField(float B){m_BField = B;}
    float BField(){return m_BField;}

  std::vector<Parent> m_LambdaCollection;

ClassDef(Event,1)
};
#endif