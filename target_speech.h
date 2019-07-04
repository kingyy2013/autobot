#ifndef SPEECH_SET_H
#define SPEECH_SET_H

#include <QStringList>
#include "autobot_unit.h"

namespace autobot {
class TargetSpeech : public AutobotUnit {
public:
  TargetSpeech() = default;
  TargetSpeech(const QString& speech_name);
  TargetSpeech(const QString& speech_name,
               const QStringList& piece_of_words);

  void SetSpeechName(const QString& speech_name);
  const QString& SpeechName() const;

  void AddNewWords(const QString& piece_of_words);
  void AddNewWords(const QStringList& piece_of_words);
  void RewindWord(bool is_random = true);

  const QString& GetNextWord(bool is_random = true);
  void ClearAllWords();

  const QStringList& GetWordsList() const;
  void SetWordsList(const QStringList& word_list);

  QStringList& GetWordsListMutable();

  const AutobotUnitDict& GetAssignedRooms() const;

  const QString GetTypeName() const ;
  static const QString GetTypeNameStatic();
private:
  QStringList words_list_;
  QStringList::Iterator cur_word_index_;
};

} // namespace

#endif // SPEECH_SET_H
