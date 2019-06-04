#include "voice.h"
#include "ui_voice.h"
#include <QLoggingCategory>

voice::voice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::voice),m_speech(nullptr)
{
    ui->setupUi(this);

    init(); //语音播报初始化
}

/**********************************************************************
作者：莫秋兰
时间：2019年5月8日19:03:36
功能：语音播放模块初始化
参数1：
参数2：
***********************************************************************/
void voice::init()
{
    QLoggingCategory::setFilterRules(QStringLiteral("qt.speech.tts=true\n qt.speech.tts.*=true"));

    ui->engine->addItem("Defaule",QString("default"));
    foreach(QString engine,QTextToSpeech::availableEngines()) //获取支持的文本到语音引擎插件的列表
        ui->engine->addItem(engine,engine); //在组合框或列表框中添加一个新数据项，并且可以指定数据项索引
    ui->engine->setCurrentIndex(0); //设定默认值
    engineSelected(0);

    //connect(ui->Speak,SIGNAL(clicked(bool)),this,SLOT(speak())); //点击Speak按钮就会进行语音播报

    //设置固定的音调、语速
    m_speech->setRate(0.0 / 20.0);
    m_speech->setPitch(5.0 / 10.0);
    m_speech->setVolume(100.0/100.0); //设置初始音量
    ui->Volume->setValue(100); //设置滑块的位置在末尾
    connect(ui->Volume,SIGNAL(valueChanged(int)),this,SLOT(setVolume(int)));
    connect(ui->engine,SIGNAL(currentIndexChanged(int)),this,SLOT(engineSelected()));

}

voice::~voice()
{
    delete ui;
}

/**********************************************************************
作者：莫秋兰
时间：2019年5月8日19:16:3
功能：语音播放
参数1：播放的内容
参数2：
***********************************************************************/
void voice::speak(QString str)
{

    //1)获取主界面中状态栏的关于成功、失败的内容
    if(m_speech->state() == QTextToSpeech::Ready)
    {
        //进行语音播报
        m_speech->say(str);

    }
}

/**********************************************************************
作者：莫秋兰
时间：2019年5月8日19:20:36
功能：设置音量
参数1：滑块值
参数2：
***********************************************************************/
void voice::setVolume(int volume) //音量
{
    m_speech->setVolume(volume / 100.0);
}

/**********************************************************************
作者：莫秋兰
时间：2019年5月8日19:25:6
功能：语言引擎选择
参数1：
参数2：
***********************************************************************/
void voice::engineSelected(int index)
{
    QString engineName = ui->engine->itemData(index).toString();
    delete m_speech;
    if(engineName == "default")
        m_speech = new QTextToSpeech(this);
    else {
        m_speech = new QTextToSpeech(engineName,this);
    }

    disconnect(ui->language,SIGNAL(currentIndexChanged(int)),this,SLOT(languageSelected()));
    ui->language->clear();

    QVector<QLocale> locales = m_speech->availableLocales();
    QLocale current = m_speech->locale();
    foreach(const QLocale &locale,locales)
    {
        QString name(QString("%1(%2)").arg(QLocale::languageToString(locale.language())).arg(QLocale::countryToString(locale.country())));
                QVariant localeVariant(locale);
        ui->language->addItem(name,localeVariant);
        if(locale.name() == current.name())
            current = locale;
    }

    m_speech->setRate(0.0 / 10.0);
    m_speech->setPitch(5.0 / 10.0);

    setVolume(ui->Volume->value());
    connect(m_speech,SIGNAL(stateChanged(QTextToSpeech::State)),this,SLOT(stateChanged(QTextToSpeech::State)));
    connect(m_speech,SIGNAL(localeChanged(QLocale)),this,SLOT(localeChanged(QLocale)));
    connect(ui->language,SIGNAL(currentIndexChanged(int)),this,SLOT(languageSelected(int)));
    localeChanged(current);
}

/**********************************************************************
作者：莫秋兰
时间：2019年5月8日19:30:26
功能：设置语言
参数1：
参数2：
***********************************************************************/
void voice::languageSelected(int language) //语言
{
    QLocale locale = ui->language->itemData(language).toLocale();
    m_speech->setLocale(locale);
}

/**********************************************************************
作者：莫秋兰
时间：2019年5月8日19:36:36
功能：选择语言
参数1：
参数2：
***********************************************************************/
void voice::voiceSelected(int index)
{
    m_speech->setVoice(m_voices.at(index));
}

/**********************************************************************
作者：莫秋兰
时间：2019年5月8日19:42:36
功能：语言区域的选择
参数1：
参数2：
***********************************************************************/
void voice::localeChanged(const QLocale &locale)
{
    QVariant localeVariant(locale);
    ui->language->setCurrentIndex(ui->language->findData(localeVariant));
    disconnect(ui->engine,SIGNAL(currentIndexChanged(int)),this,SLOT(voiceSelected(int)));
    ui->engine->clear();
    m_voices = m_speech->availableVoices();
    QVoice currentVoice = m_speech->voice();
    foreach(const QVoice &voice,m_voices)
    {
        ui->engine->addItem(QString("%1-%2-%3").arg(voice.name()).arg(QVoice::genderName(voice.gender())).arg(QVoice::ageName(voice.age())));
        if(voice.name() == currentVoice.name())
            ui->engine->setCurrentIndex(ui->engine->count() - 1);
    }
    connect(ui->engine,SIGNAL(currentIndexChanged(int)),this,SLOT(voiceSelected(int)));
}

