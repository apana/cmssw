// Author:  Leonard Apanasevich
// Created:  Fri, 01 Dec 2017 19:48:43 GMT

#include <iostream>
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include "L1Trigger/L1TGlobal/interface/GTIntegrityFilter.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Math/interface/deltaR.h"

GTIntegrityFilter::GTIntegrityFilter(const edm::ParameterSet& iConfig)
{
  // JetSource_ = iConfig.getParameter<edm::InputTag>("JetSource");
  std::string gtDigiCollection_ = iConfig.getParameter<std::string>("GTDigiCollection");
  std::string muonDigiCollection_ = iConfig.getParameter<std::string>("MuonDigiCollection");
  std::string caloDigiCollection_ = iConfig.getParameter<std::string>("CaloDigiCollection");
  m_JustTag = iConfig.getParameter<bool>("JustTagDontFilter");
  m_verbosity     = iConfig.getParameter<bool>("Verbosity");

  m_gtMuons       = edm::InputTag(gtDigiCollection_, "Muon");
  m_gtEGammas     = edm::InputTag(gtDigiCollection_, "EGamma");
  m_gtTaus        = edm::InputTag(gtDigiCollection_, "Tau");
  m_gtJets        = edm::InputTag(gtDigiCollection_, "Jet");
  m_gtEtSums      = edm::InputTag(gtDigiCollection_, "EtSum");

  m_l2Muons       = edm::InputTag(muonDigiCollection_, "Muon");
  m_l2EGammas     = edm::InputTag(caloDigiCollection_, "EGamma");
  m_l2Taus        = edm::InputTag(caloDigiCollection_, "Tau");
  m_l2Jets        = edm::InputTag(caloDigiCollection_, "Jet");
  m_l2EtSums      = edm::InputTag(caloDigiCollection_, "EtSum");

  m_gtMuonToken   = consumes<l1t::MuonBxCollection>(m_gtMuons);
  m_gtEGammaToken = consumes<l1t::EGammaBxCollection>(m_gtEGammas);
  m_gtTauToken    = consumes<l1t::TauBxCollection>(m_gtTaus);
  m_gtJetToken    = consumes<l1t::JetBxCollection>(m_gtJets);
  m_gtEtSumToken  = consumes<l1t::EtSumBxCollection>(m_gtEtSums);

  m_l2MuonToken   = consumes<l1t::MuonBxCollection>(m_l2Muons);
  m_l2EGammaToken = consumes<l1t::EGammaBxCollection>(m_l2EGammas);
  m_l2TauToken    = consumes<l1t::TauBxCollection>(m_l2Taus);
  m_l2JetToken    = consumes<l1t::JetBxCollection>(m_l2Jets);
  m_l2EtSumToken  = consumes<l1t::EtSumBxCollection>(m_l2EtSums);

}

GTIntegrityFilter::~GTIntegrityFilter() = default;

void
GTIntegrityFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<std::string>("GTDigiCollection", "gtStage2");
  desc.add<std::string>("MuonDigiCollection", "gmtStage2Digis");
  desc.add<std::string>("CaloDigiCollection", "caloStage2Digis");
  desc.add<bool>("JustTagDontFilter",false);
  desc.add<bool>("Verbosity",false);
  descriptions.add("gtIntegrityFilter",desc);
}

//
// member functions
//

bool GTIntegrityFilter::filter(edm::StreamID, edm::Event & iEvent, edm::EventSetup const & iSetup) const
{
   using namespace edm;
   using namespace reco;
   if (m_verbosity) {
     // std::cout << "Verbosity flag set to true" << std::endl;
     // LogDebug("GTIntegrityFilter") << "\n\t Inside GTDigiIntegrityFilter\n" << std::endl;
   }
   Handle<l1t::MuonBxCollection> gtMuons,l2Muons;
   iEvent.getByToken(m_gtMuonToken,gtMuons);
   iEvent.getByToken(m_l2MuonToken,l2Muons);

   if (!gtMuons.isValid() or !l2Muons.isValid()) {
     if (m_verbosity) {
       edm::LogError("GTIntegrityFilter") << "Invalid Muon collection" << std::endl;
     }
     return false;
   }

   Handle<l1t::JetBxCollection> gtJets,l2Jets;
   iEvent.getByToken(m_gtJetToken,gtJets);
   iEvent.getByToken(m_l2JetToken,l2Jets);

   if (!gtJets.isValid() or !l2Jets.isValid()) {
     if (m_verbosity) {
       edm::LogError("GTIntegrityFilter") << "Invalid Jet collection" << std::endl;
     }
     return false;
   }

   Handle<l1t::EGammaBxCollection> gtEGammas,l2EGammas;
   iEvent.getByToken(m_gtEGammaToken,gtEGammas);
   iEvent.getByToken(m_l2EGammaToken,l2EGammas);

   if (!gtEGammas.isValid() or !l2EGammas.isValid()) {
     if (m_verbosity) {
       edm::LogError("GTIntegrityFilter") << "Invalid EGamma collection" << std::endl;
       return false;
     }
   }

   Handle<l1t::TauBxCollection> gtTaus,l2Taus;
   iEvent.getByToken(m_gtTauToken,gtTaus);
   iEvent.getByToken(m_l2TauToken,l2Taus);

   if (!gtTaus.isValid() or !l2Taus.isValid()) {
     if (m_verbosity) {
       edm::LogError("GTIntegrityFilter") << "Invalid Tau collection" << std::endl;
       return false;
     }
   }

   Handle<l1t::EtSumBxCollection> gtEtSums,l2EtSums;
   iEvent.getByToken(m_gtEtSumToken,gtEtSums);
   iEvent.getByToken(m_l2EtSumToken,l2EtSums);

   if (!gtEtSums.isValid() or !l2EtSums.isValid()) {
     if (m_verbosity) {
       edm::LogError("GTIntegrityFilter") << "Invalid EtSum collection" << std::endl;
     }
     return false;
   }


   //std::cout << "First: " << gtmuons->getFirstBX() << "\tLast: " << gtmuons->getLastBX() << std::endl;
   //std::cout << "First: " << l2muons->getFirstBX() << "\tLast: " << l2muons->getLastBX() << std::endl;

   //std::cout < "size of iterator: " << l2It->size() << std::endl;

   // also check size explicity???
   // if ((gtmuons->getFirstBX() != l2muons->getFirstBX()) or (gtmuons->getLastBX() != l2muons->getLastBX())){
   //   std::cout << "Size of Muon BXs do not agree" << std::endl;
   //   return (false);
   // }
   // 
   // for ( int ibx=gtmuons->getFirstBX(); ibx<=gtmuons->getLastBX(); ++ibx) {
   //   //l1t::MuonBxCollection::const_iterator mIt = g->begin();
   //   //std::vector<l1t::Muon> gtMuon =  gtmuons.(ibx);
   //   if (gtmuons->size(ibx) != l2muons->size(ibx)){
   //     std::cout << "Number of muons does not agree" << std::endl;
   //     return false;
   //   }
   //   std::cout << "IBX: " << ibx << "\tSize of muon collections: " << gtmuons->size(ibx) << "\t" << l2muons->size(ibx) <<std::endl;
   //   auto gtitr = gtmuons->begin(ibx);
   //   auto l2itr = l2muons->begin(ibx);
   //   //std::cout << "IBX: " << ibx << "\tSize of muon collection: " << gtitr->size() <<std::endl;
   //   for (; gtitr != gtmuons->end(ibx); ++gtitr, ++ l2itr ) {
   //     std::cout << "Muon : " << " BX= " << ibx << " ipt= " << gtitr->hwPt() << ", " << l2itr->hwPt()<< std::endl;
   //   }
   // }
   // 
   // std::cout << "Total size of muon collections: " << gtmuons->size() << std::endl;
   // auto gtitr = gtmuons->begin();
   // auto l2itr = l2muons->begin();
   // for (; gtitr != gtmuons->end(); ++gtitr, ++ l2itr ) {
   //   std::cout << "Muon : " << " ipt= " << gtitr->hwPt() << ", " << l2itr->hwPt()<< "\tkey: " << gtmuons->key(gtitr) << std::endl;
   // }

   bool muOK  = CheckCollections(gtMuons,l2Muons, "Muon");
   bool jetOK = CheckCollections(gtJets,l2Jets, "Jet");
   bool egOK  = CheckCollections(gtEGammas,l2EGammas, "EGamma");
   bool tauOK = CheckCollections(gtTaus,l2Taus, "Tau");
   bool sumOK = CheckCollections(gtEtSums,l2EtSums, "ETSum");

   bool aOK= muOK and jetOK and egOK and tauOK and sumOK;
   return aOK;
}

template<typename T>
bool GTIntegrityFilter::CheckCollections(T gtCollection, T l2Collection, std::string collectionName) const
{

  if (gtCollection->size() != l2Collection->size()){
    if (m_verbosity) 
      edm::LogError("GTIntegrityFilter") << collectionName << " collection sizes do not match!  GT has size " << gtCollection->size() 
		<< "\tL2 has size " << l2Collection->size() << std::endl;
    return false;
  }
  auto gtitr = gtCollection->begin();
  auto l2itr = l2Collection->begin();
  for (; gtitr != gtCollection->end(); ++gtitr, ++ l2itr ) {
    
    if ( gtitr->hwPt() != l2itr->hwPt() ) {
      if (m_verbosity) edm::LogError("GTIntegrityFilter") << "\tMismatch in " << collectionName  << " collection pT values: GT = " << gtitr->hwPt() << "\tL2 = " << l2itr->hwPt()<< std::endl;
      return false;
    }
    if ( gtitr->hwEta() != l2itr->hwEta() ) {
      if (m_verbosity) edm::LogError("GTIntegrityFilter") << "\tMismatch in " << collectionName  << " collection Eta values: GT = " << gtitr->hwEta() << "\tL2 = " << l2itr->hwEta()<< std::endl;
      return false;
    }
    if ( gtitr->hwPhi() != l2itr->hwPhi() ) {
      if (m_verbosity) edm::LogError("GTIntegrityFilter") << "\tMismatch in " << collectionName  << " collection Phi values: GT = " << gtitr->hwPhi() << "\tL2 = " << l2itr->hwPhi()<< std::endl;
      return false;
    }
    if ( gtitr->hwIso() != l2itr->hwIso() ) {
      if (m_verbosity) edm::LogError("GTIntegrityFilter") << "\tMismatch in " << collectionName  << " collection Iso values: GT = " << gtitr->hwIso() << "\tL2 = " << l2itr->hwIso()<< std::endl;
      return false;
    }
    if ( gtitr->hwQual() != l2itr->hwQual() ) {
      if (m_verbosity) edm::LogError("GTIntegrityFilter") << "\tMismatch in " << collectionName  << " collection Qual values: GT = " << gtitr->hwQual() << "\tL2 = " << l2itr->hwQual()<< std::endl;
      return false;
    }
  }

  return true;
}
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(GTIntegrityFilter);  
