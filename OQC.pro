#-------------------------------------------------
#
# Project created by QtCreator 2019-04-26T09:47:31
# 作者: 刘维球，梁家榕，莫秋兰
# 版权：惠州市几米物联所有
# 版本：1.0.0
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += serialport
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OQC
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
    MainWindow/mainwindow.cpp \
    LogIn/qdlglogin.cpp \
    DataBase/database.cpp \
    INI/ini.cpp \
    Setting/qwdialogconfig.cpp

HEADERS += \
    MainWindow/mainwindow.h \
    LogIn/qdlglogin.h \
    LogIn/qdlglogin.h \
    DataBase/database.h \
    Test_Current/extcode.h \
    Test_Current/fundtypes.h \
    Test_Current/hosttype.h \
    Test_Current/ILVDataInterface.h \
    Test_Current/ILVTypeInterface.h \
    Test_Current/IT6400.h \
    Test_Current/Keysight34461A.h \
    Test_Current/lv_epilog.h \
    Test_Current/lv_prolog.h \
    Test_Current/Relay32.h \
    INI/ini.h \
    INI/ini.h \
    Setting/qwdialogconfig.h

FORMS += \
    MainWindow/mainwindow.ui \
    LogIn/qdlglogin.ui \
    Setting/qwdialogconfig.ui \
    Setting/qwdialogconfig.ui


# 配置图标（仅Windows有效）
# 本程序使用两个图标，这样其快捷方式可以有更多选择，也可以给关联文件提供图标选择
RC_ICONS=3.ico



# 版本号
VERSION = 1.0.0

# 语言
# 0x0004 表示 简体中文
# 详见 https://msdn.microsoft.com/en-us/library/dd318693%28vs.85%29.aspx
RC_LANG = 0x0004

# 公司名
QMAKE_TARGET_COMPANY = 惠州市几米物联公司
# 产品名称
QMAKE_TARGET_PRODUCT = 测试
# 详细描述
QMAKE_TARGET_DESCRIPTION = 本软件用于合作工厂的抽检
# 版权
QMAKE_TARGET_COPYRIGHT = Copyright(C) 2019 00001


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc









