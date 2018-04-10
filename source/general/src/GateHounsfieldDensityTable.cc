/*----------------------
  Copyright (C): OpenGATE Collaboration

  This software is distributed under the terms
  of the GNU Lesser General  Public Licence (LGPL)
  See LICENSE.md for further details
  ----------------------*/


/*!
  \class  GateHounsfieldDensityTable.cc
  \brief
  \author david.sarrut@creatis.insa-lyon.fr
*/

#include "GateHounsfieldDensityTable.hh"
#include "GateMiscFunctions.hh"
#include "G4SystemOfUnits.hh"
#include <fstream>

//-----------------------------------------------------------------------------
GateHounsfieldDensityTable::GateHounsfieldDensityTable()
{
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
GateHounsfieldDensityTable::~GateHounsfieldDensityTable()
{
  mH.clear();
  mD.clear();
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
double GateHounsfieldDensityTable::GetDensityFromH(double H)
{
  GateMessage("Geometry",3,"GateHounsfieldDensityTable::GetDensityFromH(double H): size of mH=" << mH.size() << Gateendl );
  GateMessage("Geometry",3,"GateHounsfieldDensityTable::GetDensityFromH(double H): size of mD=" << mD.size() << Gateendl );
  return LinearInterpolation(H, mH, mD);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
double GateHounsfieldDensityTable::FindMaxDensityDifference(double HMin, double HMax) {
  double dMin = GetDensityFromH(HMin);
  double dMax = GetDensityFromH(HMax);
  GateMessage("Geometry",3,"d(H=" << HMin << ")=" << dMin/(g/cm3) << ", d(H=" << HMax << ")=" << dMax/(g/cm3) << Gateendl);

  int i = 0;
  int n = mH.size();
  while (i<n && HMin>mH[i]) i++; //i--;
  int j=0;
  while (j<n && HMax>mH[j]) j++;
  j--;
  GateMessage("Geometry",3,"HMin=" << HMin << ", H[i=" << i << "]=" << mH[i] << ", H[j=" << j << "]=" << mH[j] << "HMax=" << HMax << Gateendl );
  for(int x=i; x<=j; x++) {
    // DD(G4BestUnit(mD[x], "Volumic Mass"));
    if (mD[x] < dMin) dMin = mD[x];
    if (mD[x] > dMax) dMax = mD[x];
  }
  GateMessage("Geometry",3,"dMin=" << dMin/(g/cm3) << ", dMax=" << dMax/(g/cm3) << Gateendl);

  return (dMax-dMin);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void GateHounsfieldDensityTable::Read(G4String filename)
{
  GateMessage("Geometry",4, "start reading " << filename << Gateendl);
  std::ifstream is;
  OpenFileInput(filename, is);
  while (is) {
    GateMessage("Geometry",4, "skipping comments" << Gateendl);
    skipComment(is);
    double h,d;
    is >> h;
    is >> d;
    GateMessage("Geometry",4, "h=" << h << " d=" << d << Gateendl);
    if (is) {
      mH.push_back(h);
      mD.push_back(d*g/cm3);
      if (mH.size() > 1) {
        if (h <= mH[mH.size()-2]) {
          GateError("Error Hounsfield must be in strict ascending order, while I read h="
                    << mH[mH.size()-2] << " and then h=" << h
                    << " (in file " << filename << ")\n");
        }
      }
    }
  }
  GateMessage("Geometry",4, "stop reading " << filename << Gateendl);
  GateMessage("Geometry",4, "Hounsfield table has " << mH.size() << " H and " << mD.size() << " D entries." << Gateendl);
  if (mH.size()<2){
    GateError("ERROR: Hounsfield density table should contain at least two entries, I got: " << mH.size() << Gateendl);
  }
  if (mH.size()!=mD.size()){
    GateError("ERROR: Hounsfield density table should have same number of H and D, I got #H=" << mH.size() << " and #D=" << mD.size() << Gateendl);
  }
}
//-----------------------------------------------------------------------------
