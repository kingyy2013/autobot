#ifndef TARGET_DEFAULT_SPEECH_H
#define TARGET_DEFAULT_SPEECH_H

#include <QHash>
#include <QString>
#include <memory>

#include "target_speech.h"
#include "target_speech.h"

namespace autobot {

const QString kDefaultSpeechName = "默认";
const QString kDefaultMaleSpeechName = "默认（男）";
const QString kDefaultFemaleSpeechName = "默认（女）";

typedef QHash<QString, std::shared_ptr<TargetSpeech>> TargetSpeechSetMap;

std::shared_ptr<TargetSpeechSetMap> CreateDefaultTargetSpeechSet();

} // namespace

#endif // TARGET_DEFAULT_SPEECH_H
