#ifndef HLTEcalPixelIsolTrackFilter_h
#define HLTEcalPixelIsolTrackFilter_h

#include "HLTrigger/HLTcore/interface/HLTFilter.h"
#include "DataFormats/HcalIsolatedTrack/interface/IsolatedPixelTrackCandidate.h"
/*
 This filter complements HLTPixelIsolTrackFilter in the trigger path
 IsoTrackHB(HE) to eliminates non-MIP tracks in ECAL and thus improving
 efficiency and reducing data rate significantly
 */
namespace edm {
  class ConfigurationDescriptions;
}

class HLTEcalPixelIsolTrackFilter : public HLTFilter {
public:
  explicit HLTEcalPixelIsolTrackFilter(const edm::ParameterSet&);
  ~HLTEcalPixelIsolTrackFilter() override;
  bool hltFilter(edm::Event&,
                 const edm::EventSetup&,
                 trigger::TriggerFilterObjectWithRefs& filterproduct) const override;
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  edm::EDGetTokenT<reco::IsolatedPixelTrackCandidateCollection> candTok;
  edm::InputTag candTag_;
  const double maxEnergyInEB_;
  const double maxEnergyInEE_;
  const double maxEnergyOutEB_;
  const double maxEnergyOutEE_;
  const int nMaxTrackCandidates_;
  const bool dropMultiL2Event_;
};

#endif
