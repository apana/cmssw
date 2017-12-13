#ifndef GTIntegrityFilter_h
#define GTIntegrityFilter_h
// -*- C++ -*-
//
// Package:    GTIntegrityFilter
// Class:      GTIntegrityFilter
//
/**\class GTIntegrityFilter GTIntegrityFilter.cc

Description: <one line class summary>

Implementation:
<Notes on implementation>
*/
//
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/global/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/InputTag.h"

//#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"
//#include "DataFormats/L1TGlobal/interface/GlobalExtBlk.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/L1Trigger/interface/EGamma.h"
#include "DataFormats/L1Trigger/interface/Tau.h"
#include "DataFormats/L1Trigger/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"

class GTIntegrityFilter : public edm::global::EDFilter<> {
   public:
      explicit GTIntegrityFilter(const edm::ParameterSet&);
      ~GTIntegrityFilter() override;
      static void fillDescriptions(edm::ConfigurationDescriptions & descriptions);

   private:
      bool filter(edm::StreamID, edm::Event &, edm::EventSetup const &) const final;
      //edm::EDGetTokenT<reco::CaloJetCollection> JetSourceToken_;
      edm::InputTag m_gtMuons, m_gtEGammas, m_gtTaus, m_gtJets, m_gtEtSums;
      edm::InputTag m_l2Muons, m_l2EGammas, m_l2Taus, m_l2Jets, m_l2EtSums;

      edm::EDGetToken m_gtMuonToken,m_gtEGammaToken,m_gtTauToken,m_gtJetToken,m_gtEtSumToken;
      edm::EDGetToken m_l2MuonToken,m_l2EGammaToken,m_l2TauToken,m_l2JetToken,m_l2EtSumToken;
      bool m_JustTag;
      bool m_verbosity;
      template<typename T> bool CheckCollections(T,T, std::string) const;
};

#endif
