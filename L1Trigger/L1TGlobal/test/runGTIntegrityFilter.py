#!/usr/bin/env python

import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras
process = cms.Process('GTIntegrityFilter',eras.Run2_2017)

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
##process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
##process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
##process.load('Configuration.StandardSequences.RawToDigi_Data_cff')

process.load('L1Trigger.L1TGlobal.GlobalParameters_cff')

## process.load('L1Trigger/L1TGlobal/debug_messages_cfi')
## process.MessageLogger.l1t_debug.l1t.limit = cms.untracked.int32(100000)
## ## process.MessageLogger.categories.append('l1t|Global')
## process.MessageLogger.categories.append('L1TGlobalSummary')
## ## process.MessageLogger.debugModules = cms.untracked.vstring('L1TGlobalSummary','gtIntegrityFilter')
## process.MessageLogger.categories.append('GTIntegrityFilter')
## process.MessageLogger.l1t_debug.append(GTIntegrityFilter = cms.untracked.PSet(limit = cms.untracked.int32(100000)))
## process.MessageLogger.debugModules = cms.untracked.vstring('GTIntegrityFilter')
## process.MessageLogger.l1t_global.threshold = cms.untracked.string('INFO')
## process.MessageLogger.cerr.threshold = cms.untracked.string('ERROR')
## process.MessageLogger.cerr.threshold = cms.untracked.string('DEBUG')
process.MessageLogger.cerr.FwkReport.reportEvery = 100

## process.MessageLogger = cms.Service("MessageLogger",
##                      destinations       =  cms.untracked.vstring('debugmessages'),
##                      categories         = cms.untracked.vstring('l1t'),
##                      debugModules  = cms.untracked.vstring('L1TGlobalSummary'),
##
##                      debugmessages          = cms.untracked.PSet(
##                                                 threshold =  cms.untracked.string('DEBUG'),
##                                                 ##INFO       =  cms.untracked.int32(0),
##                                                 ## DEBUG   = cms.untracked.int32(0),
##                          l1t = cms.untracked.PSet (
##                              limit = cms.untracked.int32(100)
##                          )
##                      )
## )

# Input source
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1000) )

process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring(
        "root://cmseos.fnal.gov//store/user/lpctrig/apana/uGT/Validation/run304777_ErrorStream_RAW.root",
        "root://cmseos.fnal.gov//store/user/lpctrig/apana/uGT/Validation/run304776_ErrorStream_RAW.root"
        ## "/store/data/Run2017E/HLTPhysics/RAW/v1/000/304/776/00000/047B8195-BEAD-E711-9E6E-02163E019B83.root",

	),
    skipEvents = cms.untracked.uint32(0)
    )


## process.options = cms.untracked.PSet()
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '94X_dataRun2_HLT_relval_v6', '')

### unpack the calo, muon, and gt collections
process.load("EventFilter.L1TRawToDigi.gmtStage2Digis_cfi")
process.load("EventFilter.L1TRawToDigi.caloStage2Digis_cfi")
process.load("EventFilter.L1TRawToDigi.gtStage2Digis_cfi")

### setup the GTIntegrityFilter
process.load('L1Trigger.L1TGlobal.gtIntegrityFilter_cfi')
process.gtIntegrityFilter.Verbosity = cms.bool(True)

process.unpack_l1digis  = cms.Path(process.gmtStage2Digis * process.caloStage2Digis * process.gtStage2Digis)
process.p               = cms.Path(process.gtIntegrityFilter)

process.schedule = cms.Schedule(process.unpack_l1digis,
                                process.p
)

dump=False
if dump:
    outfile = open('dump_config.py','w')
    print >> outfile,process.dumpPython()
    outfile.close()
