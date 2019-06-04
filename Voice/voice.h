#ifndef VOICE_H
#define VOICE_H

#include <QMainWindow>
#include <QTextToSpeech>

namespace Ui {
class voice;
}

class voice : public QMainWindow
{
    Q_OBJECT

public:
    explicit voice(QWidget *parent = nullptr);
    ~voice();

    void init();
public slots:
    void speak(QString str);
    void setVolume(int volume);
    void engineSelected(int index);
    void localeChanged(const QLocale &locale);
    void voiceSelected(int index);
    void languageSelected(int language);
private:
    Ui::voice *ui;
    QTextToSpeech *m_speech;
    QVector<QVoice> m_voices;
};

#endif // VOICE_H
