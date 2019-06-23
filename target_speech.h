#ifndef SPEECH_SET_H
#define SPEECH_SET_H

#include <QStringList>

namespace autobot {

class TargetSpeech
{
public:
  TargetSpeech(const QString& speech_name);
  TargetSpeech(const QString& speech_name,
               const QStringList& piece_of_words);

  const QString& SpeechName() const;
  void AddNewWords(const QString& piece_of_words);
  void AddNewWords(const QStringList& piece_of_words);
  void RewindWord(bool is_random = true);
  const QString& GetNextWord(bool is_random = true);
  void ClearAllWords();

private:
  QStringList words_set_;
  QString speech_name_;
  QStringList::Iterator cur_word_index_;
};

} // namespace

#endif // SPEECH_SET_H
