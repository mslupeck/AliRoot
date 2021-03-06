#ifndef ALIMUONCLUSTERFINDERPEAKFIT_H
#define ALIMUONCLUSTERFINDERPEAKFIT_H
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

/// \ingroup rec
/// \class AliMUONClusterFinderPeakFit
/// \brief Cluster finder in MUON arm of ALICE
///
//  Author Alexander Zinchenko, JINR Dubna; Laurent Aphecetche, SUBATECH
//

#include "AliMUONVClusterFinder.h"

#ifndef ROOT_TObjArray
#  include "TObjArray.h"
#endif
#ifndef ROOT_TVector2
#  include "TVector2.h"
#endif

class AliMUONMathieson;
//class AliMUONPad;

class TH2D;

class AliMUONClusterFinderPeakFit : public AliMUONVClusterFinder
{
public:
  AliMUONClusterFinderPeakFit(Bool_t plot, AliMUONVClusterFinder* clusterFinder); // Constructor
  virtual ~AliMUONClusterFinderPeakFit(); // Destructor

  /// It needs segmentation
  virtual Bool_t NeedSegmentation() const { return kTRUE; }

  using AliMUONVClusterFinder::Prepare;

  virtual Bool_t Prepare(Int_t detElemId, TObjArray* pads[2],
                         const AliMpArea& area, const AliMpVSegmentation* seg[2]);

  virtual AliMUONCluster* NextCluster();
  
  /// Return the number of local maxima
  Int_t GetNMax() const { return fNMax; }
  
  virtual void Print(Option_t* opt="") const;

private:
  /// Not implemented
  AliMUONClusterFinderPeakFit(const AliMUONClusterFinderPeakFit& rhs);
  /// Not implemented
  AliMUONClusterFinderPeakFit& operator=(const AliMUONClusterFinderPeakFit& rhs);

  Bool_t WorkOnPreCluster();

  /// Check precluster to simplify it (if possible), and return the simplified cluster
  AliMUONCluster* CheckPrecluster(const AliMUONCluster& cluster); 
  AliMUONCluster* CheckPreclusterTwoCathodes(AliMUONCluster* cluster); 
  
  /// Checks whether a pad and a pixel have an overlapping area.
  Bool_t Overlap(const AliMUONPad& pad, const AliMUONPad& pixel); 
  
  /// build array of pixels
  void BuildPixArray(AliMUONCluster& cluster); 
  void BuildPixArrayOneCathode(AliMUONCluster& cluster); 
  void PadOverHist(Int_t idir, Int_t ix0, Int_t iy0, AliMUONPad *pad, TH2D *h1, TH2D *h2);

  void RemovePixel(Int_t i);
  
  AliMUONPad* Pixel(Int_t i) const;
  
  Int_t FindLocalMaxima(TObjArray *pixArray, Int_t *localMax, Double_t *maxVal); // find local maxima 
  void  FlagLocalMax(TH2D *hist, Int_t i, Int_t j, Int_t *isLocalMax); // flag local max
  void  FindClusterCOG(AliMUONCluster& cluster, const Int_t *localMax, Int_t iMax); // find cluster around local max with COG
  void  FindClusterFit(AliMUONCluster& cluster, const Int_t *localMax, const Int_t *maxPos, Int_t nMax); // find cluster around local max with fitting
  void  PadsInXandY(AliMUONCluster& cluster, Int_t &nInX, Int_t &nInY) const; // get number of pads in X and Y

  void CheckOverlaps();

private:
  // Status flags for pads
  static const Int_t fgkZero; ///< pad "basic" state
  static const Int_t fgkMustKeep; ///< do not kill (for pixels)
  static const Int_t fgkUseForFit; ///< should be used for fit
  static const Int_t fgkOver; ///< processing is over
  static const Int_t fgkModified; ///< modified pad charge 
  static const Int_t fgkCoupled; ///< coupled pad    
    
  // Some constants
  static const Double_t fgkZeroSuppression; ///< average zero suppression value
  static const Double_t fgkDistancePrecision; ///< used to check overlaps and so on
  static const TVector2 fgkIncreaseSize; ///< idem
  static const TVector2 fgkDecreaseSize; ///< idem
  
  AliMUONVClusterFinder* fPreClusterFinder; //!<! the pre-clustering worker
  AliMUONCluster* fPreCluster; //!<! current pre-cluster
  TObjArray fClusterList; //!<! clusters corresponding to the current pre-cluster
  AliMUONMathieson* fMathieson; //!<! Mathieson to compute the charge repartition
  
  Int_t fEventNumber; //!<! current event being processed
  Int_t fDetElemId; //!<! current DE being processed
  Int_t fClusterNumber; //!<! current cluster number
  Int_t fNMax; //!<! number of local maxima
  TH2D *fHistAnode; //!<! histo for local maxima search
  
  const AliMpVSegmentation *fkSegmentation[2]; //!<! new segmentation
  
  TObjArray* fPixArray; //!<! collection of pixels
  Int_t fDebug; //!<! debug level
  Bool_t fPlot; //!<! whether we should plot thing (for debug only, quite slow!)
  
  Int_t fNClusters; //!<! total number of clusters
  Int_t fNAddVirtualPads; //!<! number of clusters for which we added virtual pads
  
  ClassDef(AliMUONClusterFinderPeakFit,0) // cluster finder in MUON arm of ALICE
};

#endif
