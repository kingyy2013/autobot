#ifndef TARGET_DEFAULT_SPEECH_H
#define TARGET_DEFAULT_SPEECH_H

#include <QHash>
#include <memory>

#include "target_speech.h"

namespace autobot {

typedef QHash<QString, std::shared_ptr<TargetSpeech>> TargetSpeechSet;

const QStringList female_speech = {
  "666",
  "声音好可爱",
  "小姐姐好棒",
  "敢开麦吗?",
};

const QStringList male_speech = {
  "666",
  "好猛",
  "小哥哥好棒"
};

std::unique_ptr<TargetSpeechSet> CreateDefaultTargetSpeechSet() {
  QList<std::shared_ptr<TargetSpeech>> speech_set_list = {
    std::make_shared<TargetSpeech>("默认（男）", male_speech),
    std::make_shared<TargetSpeech>("默认（女）", female_speech),
  };
  std::unique_ptr<TargetSpeechSet> singleton_speech_set;
  for (auto speech_set : speech_set_list) {
    (*singleton_speech_set)[speech_set->SpeechName()] = speech_set;
  }
  return singleton_speech_set;
}

static std::unique_ptr<TargetSpeechSet> singleton_speech_set
= CreateDefaultTargetSpeechSet();

} // namespace

#endif // TARGET_DEFAULT_SPEECH_H
