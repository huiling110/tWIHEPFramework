/******************************************************************************
 * CutBTaggedJetN.hpp                                                          *
 *                                                                            *
 * Cuts on number of tagged jets in an event                                  *
 *                                                                            *
 * Derived from HistoCut which is in turn derived from BaseCut                *
 *                                                                            *
 * Public Member Functions of CutBTaggedJetN class                             *
 *    CutBTaggedJetN()                   -- Parameterized Constructor          *
 *    ~CutBTaggedJetN()                  -- Destructor                         *
 *    BookHistogram()                   -- Book histograms                    *
 *    Apply()                           -- Apply cuts and fill histograms     *
 *    GetCutName()                      -- Returns "CutBTaggedJetN"            *
 *                                                                            *
 * Private Data Members of CutBTaggedJetN                                      *
 *    myTH1F* _hbLooseJetNumberBefore         -- Hist of Num of jets before cuts    *
 *    myTH1F* _hbLooseJetNumberAfter          -- Hist of Num of jets before cuts    *
 *    Int_t _bLooseJetNumberMin;              -- Minimum number of jets to require  *
 *    Int_t _bLooseJetNumberMax;              -- Maximum number of jets allowed     *
 *                                                                            *
 * History                                                                    *
 *      23 Feb 2007 - Created by P. Ryan                                      *
 *****************************************************************************/


#include "SingleTopRootAnalysis/Cuts/TaggedJet/CutBTaggedJetN.hpp"
#include <iostream>
#include <sstream>

using namespace std;


/******************************************************************************
 * CutBTaggedJetN::CutBTaggedJetN(EventContainer *EventContainerObj)            *
 *                                                                            *
 * Parameterized Constructor                                                  *
 *                                                                            *
 * Input:  Event Object class                                                 *
 * Output: None                                                               *
 ******************************************************************************/
CutBTaggedJetN::CutBTaggedJetN(EventContainer *EventContainerObj, Int_t nbLooseJetsDefault, Int_t nbMediumJetsDefault)
{
  SetEventContainer(EventContainerObj);
  _bLooseJetNumberMin = nbLooseJetsDefault; // Overriding configuration file.
  _bLooseJetNumberMax = nbLooseJetsDefault;
  _bMediumJetNumberMin = nbMediumJetsDefault; // Overriding configuration file.
  _bMediumJetNumberMax = nbMediumJetsDefault;
  
} // CutBTaggedJetN


/******************************************************************************
 * CutBTaggedJetN::~CutBTaggedJetN()                                            *
 *                                                                            *
 * Destructor                                                                 *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
CutBTaggedJetN::~CutBTaggedJetN()
{
  
}//~CutBTaggedJetN

/******************************************************************************
 * void CutBTaggedJetN::BookHistogram()                                        *
 *                                                                            *
 * Book Histograms                                                            *
 *                                                                            *
 * Input:  None                                                               *
 * Output: None                                                               *
 ******************************************************************************/
void CutBTaggedJetN::BookHistogram(){
  
  // Histogram of events before cut on number of jets
  _hbLooseJetNumberBefore =  DeclareTH1F("BTaggedbLooseJetNumberBefore","Number of BTagged Jets before the cut",10, 0.0, 10.0);
  _hbLooseJetNumberBefore -> SetXAxisTitle("N_{Jet}^{BLooseTagged}");
  _hbLooseJetNumberBefore -> SetYAxisTitle("Events");

  // Histogram of events after cut on number of jets
  _hbLooseJetNumberAfter =  DeclareTH1F("BTaggedbLooseJetNumberAfter","Number of BTagged Jets after the cut",10, 0.0, 10.0);
  _hbLooseJetNumberAfter -> SetXAxisTitle("N_{Jet}^{BLooseTagged}");
  _hbLooseJetNumberAfter -> SetYAxisTitle("Events");
  
  // Get configuration
  EventContainer *EventContainerObj = GetEventContainer();
  TEnv *config = EventContainerObj -> GetConfig();

  // Use configuration to set min and max number of jets to cut on if default value used
  if (_bLooseJetNumberMax < 0.){
    _bLooseJetNumberMin = config -> GetValue("Cut.bLoose.Jet.Number.Min", 999);
    _bLooseJetNumberMax = config -> GetValue("Cut.bLoose.Jet.Number.Max", 999);
  }
  if (_bMediumJetNumberMax < 0.){
    _bMediumJetNumberMin = config -> GetValue("Cut.bMedium.Jet.Number.Min", 999);
    _bMediumJetNumberMax = config -> GetValue("Cut.bMedium.Jet.Number.Max", 999);
  }
  
  //
  // also add these two cuts to the cut flow table
  ostringstream titleStr;
  titleStr <<"BLooseTagged Jet : " << "N >=" << _bLooseJetNumberMin;
  TString tTitle(titleStr.str().c_str());
  GetCutFlowTable()->AddCutToFlow("BLooseTagged.Jet.Number.Min",tTitle);
  titleStr.str("");
  titleStr <<"BLooseTagged Jet : N <= "<<_bLooseJetNumberMax;
  tTitle=titleStr.str().c_str();
  GetCutFlowTable()->AddCutToFlow("BLooseTagged.Jet.Number.Max",tTitle);
  titleStr.str("");
  titleStr <<"BMediumTagged Jet : " << "N >=" << _bMediumJetNumberMin;
  tTitle=titleStr.str().c_str();
  GetCutFlowTable()->AddCutToFlow("BMediumTagged.Jet.Number.Min",tTitle);
  titleStr.str("");
  titleStr <<"BMediumTagged Jet : N <= "<<_bMediumJetNumberMax;
  tTitle=titleStr.str().c_str();
  GetCutFlowTable()->AddCutToFlow("BMediumTagged.Jet.Number.Max",tTitle);
  // also add the overal pass rate
  titleStr.str("");
  titleStr <<"BLooseTagged Jet : "<< _bLooseJetNumberMin<<" <= N <= "<<_bLooseJetNumberMax << " Or BMediumTagged Jet: "<<_bMediumJetNumberMin<<" <= N <= "<<_bMediumJetNumberMax ;
  tTitle=titleStr.str().c_str();
  GetCutFlowTable()->AddCutToFlow("bLooseOrMedium.Jet.Number.All",tTitle);
  
}//BookHistograms()

/******************************************************************************
 * Bool_t CutBTaggedJetN::Apply()                                              *
 *                                                                            *
 * Apply cuts and fill histograms                                             *
 *                                                                            *
 * Input:  None                                                               *
 * Output: kTrue if successful                                                *
 ******************************************************************************/
Bool_t CutBTaggedJetN::Apply()
{
  //cout<<"Begin of CutBTaggedJetN::Apply()"<<endl;

  // Get Event Tree
  EventContainer *evObj = GetEventContainer();

  Int_t bLooseJetNumber = 0;
  Int_t bMediumJetNumber = 0;
  _bLooseCut = evObj-> GetConfig() -> GetValue("ObjectID.BJet.LWPBTagCut",0.0);
  _bMediumCut = evObj-> GetConfig() -> GetValue("ObjectID.BJet.MWPBTagCut",0.0);
  for (auto jet : evObj->jets){
    if(jet.bDiscriminator() > _bLooseCut) bLooseJetNumber ++;
    if(jet.bDiscriminator() > _bMediumCut) bMediumJetNumber ++;
  }

  // Fill the histogram before the cut
  _hbLooseJetNumberBefore -> Fill(bLooseJetNumber);

  // Flags 
  Bool_t bLooseJetNumberMinPass    = kTRUE;   // Event passes min cut
  Bool_t bLooseJetNumberMaxPass    = kTRUE;   // Event passes max cut
  Bool_t bMediumJetNumberMinPass    = kTRUE;   // Event passes min cut
  Bool_t bMediumJetNumberMaxPass    = kTRUE;   // Event passes max cut

  // Cut on bLoose Min
  // Negative cut value for Min means there is no Min cut
  if( (_bLooseJetNumberMin != 999) && (bLooseJetNumber < _bLooseJetNumberMin) ){
     bLooseJetNumberMinPass = kFALSE;
     GetCutFlowTable()->FailCut("BLooseTagged.Jet.Number.Min");
  }//if
  else{
    GetCutFlowTable() -> PassCut("BLooseTagged.Jet.Number.Min");
  } //else

  // Cut on bLoose Max
  // Negative cut value for Min means there is no Min cut
  if( (_bLooseJetNumberMax != 999) && (bLooseJetNumber > _bLooseJetNumberMax) ){
     bLooseJetNumberMaxPass = kFALSE;
     GetCutFlowTable()->FailCut("BLooseTagged.Jet.Number.Max");
  } //if
  else{
    GetCutFlowTable() -> PassCut("BLooseTagged.Jet.Number.Max");
  } //else

  // Cut on bMedium Min
  // Negative cut value for Min means there is no Min cut
  if( (_bMediumJetNumberMin != 999) && (bMediumJetNumber < _bMediumJetNumberMin) ){
     bMediumJetNumberMinPass = kFALSE;
     GetCutFlowTable()->FailCut("BMediumTagged.Jet.Number.Min");
  }//if
  else{
    GetCutFlowTable() -> PassCut("BMediumTagged.Jet.Number.Min");
  } //else

  // Cut on bMedium Max
  // Negative cut value for Min means there is no Min cut
  if( (_bMediumJetNumberMax != 999) && (bMediumJetNumber > _bMediumJetNumberMax) ){
     bMediumJetNumberMaxPass = kFALSE;
     GetCutFlowTable()->FailCut("BMediumTagged.Jet.Number.Max");
  } //if
  else{
    GetCutFlowTable() -> PassCut("BMediumTagged.Jet.Number.Max");
  } //else


  // Fill Histogram after cuts
  if((bLooseJetNumberMinPass && bLooseJetNumberMaxPass) || (bMediumJetNumberMinPass && bMediumJetNumberMaxPass)){
    _hbLooseJetNumberAfter -> Fill(bLooseJetNumber);
    GetCutFlowTable() -> PassCut("bLooseOrMedium.Jet.Number.All");
  } //if
  else GetCutFlowTable() -> FailCut("bLooseOrMedium.Jet.Number.All");

  if((bLooseJetNumberMinPass && bLooseJetNumberMaxPass) || (bMediumJetNumberMinPass && bMediumJetNumberMaxPass)){
    //  cout<<"EventNumber : "<<evObj->eventNumber<<endl;
  }

  return((bLooseJetNumberMinPass && bLooseJetNumberMaxPass) || (bMediumJetNumberMinPass && bMediumJetNumberMaxPass));

} //Apply












