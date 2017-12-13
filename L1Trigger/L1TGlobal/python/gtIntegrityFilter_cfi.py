
import FWCore.ParameterSet.Config as cms

gtIntegrityFilter = cms.EDFilter("GTIntegrityFilter",
                                 JustTagDontFilter = cms.bool(False),
                                 Verbosity      = cms.bool(False),
                                 GTDigiCollection = cms.string("gtStage2Digis"),
                                 MuonDigiCollection = cms.string("gmtStage2Digis"),
                                 CaloDigiCollection = cms.string("caloStage2Digis"),
)

