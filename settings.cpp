#include "settings.h"
#include <QSettings>
QString _appPath;
Settings::Settings(QString path)
{
    _appPath = path;
}

Settings::Settings()
{

}

Settings::~Settings()
{
    delete this;
}

void Settings::saveSettings(QString name)
{
    _settings->sync();
}

void Settings::loadSettings(QString name)
{
    _settings = new QSettings("QDChat_", name);
    saveSettings(name);
}

QString Settings::getValue(QString group, QString  key, QVariant defaultValue)
{
    _settings->beginGroup(group);
    QString value = _settings->value(key, defaultValue).toString();
    _settings->endGroup();

    return value;
}


void Settings::setValue(QString group, QString key, QVariant value)
{
    _settings->beginGroup(group);
    _settings->setValue(key, value);
    _settings->endGroup();
    saveSettings(group);
}



QSettings *Settings::getSettings()
{
    return _settings;
}

