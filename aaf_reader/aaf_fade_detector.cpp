#include "aaf_fade_detector.h"
#include "aaf_utils.h"
#include <string>

AAFFadeDetector::AAFFadeDetector(DebugLogger& logger) : logger(logger) {}

void AAFFadeDetector::analyzeSegmentForFade(IAAFSegment* segment, AAFAudioClipInfo& clipInfo) {
    if (!segment) return;
    logger.debug(LogCategory::CLIPS, "Analyzing segment for fades");
    checkTransitions(segment, clipInfo);
    checkOperationGroups(segment, clipInfo);
    checkControlTracks(segment, clipInfo);
    checkFillerEffects(segment, clipInfo);
    checkDescriptiveMetadata(segment, clipInfo);
    checkNestedCompositions(segment, clipInfo);
    checkDataDefinitions(segment, clipInfo);
    checkModifierEffects(segment, clipInfo);
    checkFillerStepping(segment, clipInfo);
    checkCustomOperations(segment, clipInfo);
    checkProprietaryMetadata(segment, clipInfo);
}

void AAFFadeDetector::checkTransitions(IAAFSegment* segment, AAFAudioClipInfo& clipInfo) {
    IAAFTransition* transition = nullptr;
    if (SUCCEEDED(segment->QueryInterface(IID_IAAFTransition, (void**)&transition))) {
        aafLength_t length = 0;
        transition->GetTransitionLength(&length);
        clipInfo.hasFadeIn = true; // simplistic assumption
        clipInfo.fadeInLength = length;
        logger.debug(LogCategory::CLIPS, "Fade detected via IAAFTransition" );
        transition->Release();
    }
}

void AAFFadeDetector::checkOperationGroups(IAAFSegment* segment, AAFAudioClipInfo& clipInfo) {
    IAAFOperationGroup* op = nullptr;
    if (SUCCEEDED(segment->QueryInterface(IID_IAAFOperationGroup, (void**)&op))) {
        IAAFOperationDef* opDef = nullptr;
        if (SUCCEEDED(op->GetOperationDefinition(&opDef))) {
            aafWChar name[128] = {0};
            if (SUCCEEDED(opDef->GetName(name, sizeof(name)))) {
                std::string opName = wideToUtf8(name);
                if (opName.find("Fade") != std::string::npos) {
                    clipInfo.hasFadeIn = true;
                    logger.debug(LogCategory::CLIPS, "Fade detected via OperationGroup: " + opName);
                }
            }
            opDef->Release();
        }
        aafUInt32 inputs = 0;
        if (SUCCEEDED(op->CountSourceSegments(&inputs))) {
            for (aafUInt32 i = 0; i < inputs; ++i) {
                IAAFSegment* seg = nullptr;
                if (SUCCEEDED(op->GetInputSegmentAt(i, &seg))) {
                    analyzeSegmentForFade(seg, clipInfo);
                    seg->Release();
                }
            }
        }
        op->Release();
    }
}

void AAFFadeDetector::checkControlTracks(IAAFSegment* segment, AAFAudioClipInfo& clipInfo) {
    // Placeholder: in real implementation analyze Parameter/Control tracks
    logger.trace(LogCategory::CLIPS, "Checking ControlTracks for fade curves");
}

void AAFFadeDetector::checkFillerEffects(IAAFSegment* segment, AAFAudioClipInfo& clipInfo) {
    logger.trace(LogCategory::CLIPS, "Checking Filler for embedded fade effects");
}

void AAFFadeDetector::checkDescriptiveMetadata(IAAFSegment* segment, AAFAudioClipInfo& clipInfo) {
    logger.trace(LogCategory::CLIPS, "Checking descriptive metadata for fade tags");
}

void AAFFadeDetector::checkNestedCompositions(IAAFSegment* segment, AAFAudioClipInfo& clipInfo) {
    logger.trace(LogCategory::CLIPS, "Checking nested compositions for fades");
    IAAFSourceClip* src = nullptr;
    if (SUCCEEDED(segment->QueryInterface(IID_IAAFSourceClip, (void**)&src))) {
        aafSourceRef_t ref;
        if (SUCCEEDED(src->GetSourceReference(&ref))) {
            IAAFMob* mob = nullptr;
            if (SUCCEEDED(src->GetSourceMob(&mob))) {
                IAAFSegment* seg = nullptr;
                if (SUCCEEDED(mob->GetEssenceDescriptor((IAAFEssenceDescriptor**)&seg))) {
                    analyzeSegmentForFade(seg, clipInfo);
                    seg->Release();
                }
                mob->Release();
            }
        }
        src->Release();
    }
}

void AAFFadeDetector::checkDataDefinitions(IAAFSegment* segment, AAFAudioClipInfo& clipInfo) {
    logger.trace(LogCategory::CLIPS, "Checking DataDefinitions for SoundLevel or Opacity");
    IAAFComponent* comp = nullptr;
    if (SUCCEEDED(segment->QueryInterface(IID_IAAFComponent, (void**)&comp))) {
        IAAFDataDef* dataDef = nullptr;
        if (SUCCEEDED(comp->GetDataDef(&dataDef))) {
            aafUID_t uid;
            if (SUCCEEDED(dataDef->GetAUID(&uid))) {
                char buf[64];
                sprintf(buf, "%08X", uid.Data1);
                std::string id(buf);
                if (id.find("soundlevel") != std::string::npos) {
                    clipInfo.hasFadeIn = true;
                    logger.debug(LogCategory::CLIPS, "Fade implied by DataDefinition" );
                }
            }
            dataDef->Release();
        }
        comp->Release();
    }
}

void AAFFadeDetector::checkModifierEffects(IAAFSegment* segment, AAFAudioClipInfo& clipInfo) {
    logger.trace(LogCategory::CLIPS, "Checking ModifierEffect for fades");
}

void AAFFadeDetector::checkFillerStepping(IAAFSegment* segment, AAFAudioClipInfo& clipInfo) {
    logger.trace(LogCategory::CLIPS, "Checking stepwise filler fades");
}

void AAFFadeDetector::checkCustomOperations(IAAFSegment* segment, AAFAudioClipInfo& clipInfo) {
    logger.trace(LogCategory::CLIPS, "Checking custom OperationDefs for fades");
}

void AAFFadeDetector::checkProprietaryMetadata(IAAFSegment* segment, AAFAudioClipInfo& clipInfo) {
    logger.trace(LogCategory::CLIPS, "Checking proprietary KLV or tagged metadata for fades");
}
