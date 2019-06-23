#include <algorithm>
#include "target_speech.h"

namespace autobot {

TargetSpeech::TargetSpeech(const QString& speech_name)
  : speech_name_(speech_name),
    cur_word_index_(words_set_.begin()) {}

TargetSpeech::TargetSpeech(const QString& speech_name,
             const QStringList& piece_of_words) :
  words_set_(piece_of_words),
  speech_name_(speech_name),
  cur_word_index_(words_set_.begin()){}

const QString& TargetSpeech::SpeechName() const {
  return speech_name_;
}

void TargetSpeech::AddNewWords(const QString& piece_of_words) {
  words_set_.append(piece_of_words);
}

void TargetSpeech::AddNewWords(const QStringList& piece_of_words_set) {
  words_set_.append(piece_of_words_set);
}

void TargetSpeech::RewindWord(bool is_random) {
  std::random_shuffle(words_set_.begin(), words_set_.end());
  cur_word_index_ = words_set_.begin();
}

const QString& TargetSpeech::GetNextWord(bool is_random) {
  if (cur_word_index_ == words_set_.end()) {
    RewindWord(is_random);
  } else {
    ++cur_word_index_;
  }
  return *(cur_word_index_);
}

}
