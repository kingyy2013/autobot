#include "target_speech_set.h"

namespace autobot {

const QStringList default_speech = {
  "666",
  "来了老哥",
  "我就看看不说话"
};

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

std::shared_ptr<TargetSpeechSetDict> CreateDefaultTargetSpeechSet() {
  QList<std::shared_ptr<TargetSpeech>> speech_set_list = {
    std::make_shared<TargetSpeech>(kDefaultSpeechName, default_speech),
    std::make_shared<TargetSpeech>(kDefaultMaleSpeechName, male_speech),
    std::make_shared<TargetSpeech>(kDefaultFemaleSpeechName, female_speech),
  };
  std::shared_ptr<TargetSpeechSetDict> singleton_speech_set
      = std::make_shared<TargetSpeechSetDict>();
  for (auto speech_set : speech_set_list) {
    (*singleton_speech_set)[speech_set->SpeechName()] = speech_set;
  }
  return singleton_speech_set;
}

}// namespace
