#ifndef TARGET_DEFAULT_SPEECH_H
#define TARGET_DEFAULT_SPEECH_H

#include <QHash>
#include <memory>

#include "target_speech.h"
#include "target_speech.h"

namespace autobot {

typedef QHash<QString, std::shared_ptr<TargetSpeech>> TargetSpeechSetMap;

std::shared_ptr<TargetSpeechSetMap> CreateDefaultTargetSpeechSet();

} // namespace

#endif // TARGET_DEFAULT_SPEECH_H
