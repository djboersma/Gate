/*----------------------
  Copyright (C): OpenGATE Collaboration

  This software is distributed under the terms
  of the GNU Lesser General  Public Licence (LGPL)
  See GATE/LICENSE.txt for further details
  ----------------------*/


/*!
  \class GateSparseImageT
  \ingroup data_structures
  \author thibault.frisson@creatis.insa-lyon.fr
  laurent.guigues@creatis.insa-lyon.fr
  david.sarrut@creatis.insa-lyon.fr
*/

#ifndef __GATESPARSEIMAGET_HH__
#define __GATESPARSEIMAGET_HH__

// g4
#include <globals.hh>

// std
#include <fstream>
#include <iomanip>
#include <cstdint>

// gate
#include "GateVImage.hh"
#include "GateMachine.hh"
#include "GateMHDImage.hh"
#include "GateMiscFunctions.hh"

// root
#ifdef G4ANALYSIS_USE_ROOT
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#endif

template<class PixelType>
bool second_less(typename const std::map<int64_t,PixelType>::value_type& p1,
                 typename const std::map<int64_t,PixelType>::value_type& p2){
  return p1.second < p2.second;
}

/// \brief 3D images of PixelType values
template<class PixelType>
class GateSparseImageT : public GateVImage
{
public:

  GateSparseImageT();
  virtual ~GateSparseImageT();

  // Define some iterator types
  typedef typename std::map<int64_t,PixelType> data_container_type;
  typedef data_container_type::iterator iterator;
  typedef data_container_type::const_iterator const_iterator;

  /// Allocates the data
  virtual void Allocate();

  // Access to the image values
  /// Returns the value of the image at voxel of index provided
  inline PixelType GetValue(int index) const {
    if (index<0) return mOutsideValue;
    iterator i = data.find(index);
    return (i==data.end()) ? mInsideValue : i->second;
  }

  /// Returns a reference on the value of the image at voxel of index provided
  // inline PixelType& GetValue(int index)       { return data[index]; }

  /// Returns the value of the image at voxel of coordinates provided
  inline PixelType GetValue(int i, int j, int k) const;

  /// Returns the value of the image at voxel of position provided
  inline PixelType GetValue(const G4ThreeVector& position) const {
    return GetValue(GetIndexFromPosition(position));
  }

  /// Returns a reference on the value of the image at voxel of position provided
  // inline PixelType& GetValue(const G4ThreeVector& position) { return data[GetIndexFromPosition(position)]; }
  /// Sets the value of the voxel of coordinates x,y,z

  inline int64_t GetIndex( int x, int y, int z) {
    if (x<0) return -1;
    if (y<0) return -1;
    if (z<0) return -1;
    if (x>=resolution.x()) return -1;
    if (y>=resolution.x()) return -1;
    if (z>=resolution.x()) return -1;
    return x+y*lineSize+z*planeSize;
  }

  inline void SetValue ( int x, int y, int z, PixelType v ) {
    int64_t index = GetIndex(x,y,z);
    if (index<0) return;
    SetValue(index,v);
  }
  /// Sets the value of the voxel of index i

  inline void SetValue ( int64_t i, PixelType v ) {
    data[i]=v;
    iterator i = data.find(index);
    if (i == data.end()){
      if (v!=mInsideValue){
        data.insert(std::make_pair(index,v));
      }
      // if index does not occur in map and v is the default inside value
      // then we do not need to do anything
    } else {
      if (v==mInsideValue){
        data.erase(i);
      } else {
        i->second = v;
      }
    }
  }

  /// Adds a value to the voxel of index provided
  inline void AddValue(int64_t index, PixelType value) {
    if (index<0) return;
    if (value==0) return;
    iterator i = data.find(index);
    if (i == data.end()){
      data.insert(std::make_pair(index,mInsideValue+v));
    } else {
      PixelType new_value = i->second + value;
      if (new_value == mInsideValue){
        data.erase(i);
      } else {
        i->second = new_value;
      }
    }
  }

  /// Fills the image with a value
  // inline void Fill(PixelType v) { for (iterator i=begin(); i!=end(); ++i) (*i)=v; }
  // inline void Fill(PixelType v) { fill(data.begin(), data.end(), v); }
  inline void Fill(PixelType v) { mInsideValue = v; data.clear(); }

  inline PixelType GetMinValue() const;
  inline PixelType GetMaxValue() const;

  inline PixelType GetInsideValue()   { return mInsideValue; } //The default HU value inside the phantom
  inline void SetInsideValue( PixelType v ) { mInsideValue=v; }

  inline PixelType GetOutsideValue()   { return mOutsideValue; } //The HU value that must be considered not part of the phantom
  inline void SetOutsideValue( PixelType v ) { mOutsideValue=v; }
  PixelType GetNeighborValueFromCoordinate(const ESide & side, const G4ThreeVector & coord);

  void MergeDataByAddition(G4String filename);

  // iterators. NOTE: these ONLY iterate over the "interesting" voxels.
  iterator begin() { return data.begin(); }
  iterator end()   { return data.end(); }
  const_iterator begin() const { return data.begin(); }
  const_iterator end() const  { return data.end(); }

  // IO
  /// Writes the image to a file with comment (the format is detected automatically)
  virtual void Write(G4String filename, const G4String & comment = "");

  /// Reads the image from a file (the format is detected automatically)
  virtual void Read(G4String filename);


  /// Displays info about the image to standard output
  virtual void PrintInfo();


  //-----------------------------------------------------------------------------
protected:
  std::map<int64_t,PixelType> data;
  PixelType mOutsideValue;
  PixelType mInsideValue;

  void ReadAscii(G4String filename);
  void ReadAnalyze(G4String filename);
  void ReadMHD(G4String filename);
  void ReadInterfile(G4String fileName);

  void WriteVox(std::ofstream & os);
  void WriteAscii(std::ofstream & os, const G4String & comment);
  void WriteBin(std::ofstream & os);
  void WriteAnalyzeHeader(G4String filename);
  void WriteRoot(G4String filename);
  void WriteMHD(std::string filename);
private:
  // implementation details
  PixelType FindMostCommonValue(typename const std::vector<PixelType>& temp);
  template<typename VoxelType> void GetRawDataFromFile(istream& is);
  void ConvertVectorToMap( typename std::vector<PixelType>& temp);

};

#include "GateSparseImageT.icc"

// vim: ai sw=2 ts=2 et
#endif
