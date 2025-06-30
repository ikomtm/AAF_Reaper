#pragma once
#include "data_structures.h"
#include "debug_logger.h"
#include <AAF.h>

class AAFFadeDetector {
public:
    explicit AAFFadeDetector(DebugLogger& logger);

    // Analyze a segment for fade information and update clipInfo
    void analyzeSegmentForFade(IAAFSegment* segment, AAFAudioClipInfo& clipInfo);

private:
    DebugLogger& logger;

    void checkTransitions(IAAFSegment* segment, AAFAudioClipInfo& clipInfo);
    void checkOperationGroups(IAAFSegment* segment, AAFAudioClipInfo& clipInfo);
    void checkControlTracks(IAAFSegment* segment, AAFAudioClipInfo& clipInfo);
    void checkFillerEffects(IAAFSegment* segment, AAFAudioClipInfo& clipInfo);
    void checkDescriptiveMetadata(IAAFSegment* segment, AAFAudioClipInfo& clipInfo);
    void checkNestedCompositions(IAAFSegment* segment, AAFAudioClipInfo& clipInfo);
    void checkDataDefinitions(IAAFSegment* segment, AAFAudioClipInfo& clipInfo);
    void checkModifierEffects(IAAFSegment* segment, AAFAudioClipInfo& clipInfo);
    void checkFillerStepping(IAAFSegment* segment, AAFAudioClipInfo& clipInfo);
    void checkCustomOperations(IAAFSegment* segment, AAFAudioClipInfo& clipInfo);
    void checkProprietaryMetadata(IAAFSegment* segment, AAFAudioClipInfo& clipInfo);
};
