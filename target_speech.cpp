#include <algorithm>
#include "target_speech.h"

namespace autobot {

TargetSpeech::TargetSpeech(const QString& speech_name)
  : AutobotUnit(speech_name),
    cur_word_index_(words_list_.begin()) {}

TargetSpeech::TargetSpeech(const QString& speech_name,
             const QStringList& words_list) :
  AutobotUnit(speech_name),
  words_list_(words_list),
  cur_word_index_(words_list_.begin()){}

void TargetSpeech::SetSpeechName(const QString& speech_name) {
  this->SetUnitName(speech_name);
}

const QString& TargetSpeech::SpeechName() const {
  return this->GetUnitName();
}

void TargetSpeech::AddNewWords(const QString& piece_of_words) {
  words_list_.append(piece_of_words);
}

void TargetSpeech::AddNewWords(const QStringList& piece_of_words_set) {
  words_list_.append(piece_of_words_set);
}

void TargetSpeech::SetWordsList(const QStringList& words_list) {
  words_list_ = words_list;
}


void TargetSpeech::RewindWord(bool is_random) {
  if (is_random) {
    std::random_shuffle(words_list_.begin(), words_list_.end());
  }
  cur_word_index_ = words_list_.begin();
}

const QString& TargetSpeech::GetNextWord(bool is_random) {
  if (cur_word_index_ == words_list_.end()) {
    RewindWord(is_random);
  } else {
    ++cur_word_index_;
  }
  return *(cur_word_index_);
}

const QStringList& TargetSpeech::GetWordsList() const {
  return words_list_;
}

QStringList& TargetSpeech::GetWordsListMutable() {
  return words_list_;
}

const ConnectionUnitList& TargetSpeech::GetAllAssignedRoom() const {
  return this->GetAllUpperConnections();
}

const QString TargetSpeech::GetTypeName() const {
  return GetTypeNameStatic();
}

const QString TargetSpeech::GetTypeNameStatic() {
  return "target_speech";
}
}
