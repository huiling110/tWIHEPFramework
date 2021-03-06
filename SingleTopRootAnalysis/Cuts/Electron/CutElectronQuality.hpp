/******************************************************************************
 * CutElectronQuality.hpp                                                           *
 *                                                                            *
 * Cuts on electron N (can cut on tight or veto Electrons)               *
 * Must pass tight or veto to constructor                                     *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 *                                                                            *
 * Public Member Functions of CutElectronQuality class                              *
 *    CutElectronQuality()                     -- Parameterized Constructor         *
 *    ~CutElectronQuality()                    -- Destructor                        *
 *    BookHistogram()                -- Book histograms                       *
 *    Apply()                        -- Apply cuts and fill histograms        *
 *    GetCutName()                   -- Returns "CutElectronQuality"                *
 *                                                                            *
 * Private Data Members of CutElectronQuality class                                 *
 *    myTH1F* _hElectronQualityBefore;    -- Hist mu N before cut         *
 *    myTH1F* _hElectronQualityAfter;     -- Hist mu N of jets after cut  *
 *                                                                            *
 *    Int_t _ElectronQualityMin;          -- Minimum Electron N           *
 *    Int_t _ElectronQualityMax;          -- Maximum Electron N           *
 *                                                                            *
 * History                                                                    *
 *      15 Jan 2007 - Created by P. Ryan                                      *
 *****************************************************************************/

#ifndef CutElectronQuality_h
#define CutElectronQuality_h

#include "SingleTopRootAnalysis/Base/Dictionary/HistoCut.hpp"
#include "SingleTopRootAnalysis/Base/Dictionary/EventContainer.hpp"

#include <string>
#include <vector>
#include <sstream>

using namespace std;

class CutElectronQuality : public HistoCut 
{

public:

  // Parameterized Constructor
  CutElectronQuality(EventContainer *EventContainerObj, TString electronType);
  
  // Destructor
  ~CutElectronQuality();

  // Get the name of describing the cut
  inline std::string GetCutName() { return "CutElectronQuality"; };

  // Book Histograms
  void BookHistogram();

  // Apply cut and fill histogram
  Bool_t Apply();


private:

  // Histograms
  myTH1F* _hElectronQualityBefore;   // Histogram Electron N before the cut
  myTH1F* _hElectronQualityAfter;    // Histogram Electron N of jets after the cut

  // Cut parameters
  Bool_t _ElectronQuality;        
 
  // Electron Type (veto, tight)
  TString electronType;

};


#endif







