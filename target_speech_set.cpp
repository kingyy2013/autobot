#include "target_speech_set.h"

namespace autobot {

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

std::shared_ptr<TargetSpeechSetMap> CreateDefaultTargetSpeechSet() {
  QList<std::shared_ptr<TargetSpeech>> speech_set_list = {
    std::make_shared<TargetSpeech>("默认（男）", male_speech),
    std::make_shared<TargetSpeech>("默认（女）", female_speech),
  };
  std::shared_ptr<TargetSpeechSetMap> singleton_speech_set
      = std::make_shared<TargetSpeechSetMap>();
  for (auto speech_set : speech_set_list) {
    (*singleton_speech_set)[speech_set->SpeechName()] = speech_set;
  }
  return singleton_speech_set;
}

}// namespace
