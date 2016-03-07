#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>



class Settings
{
public:
    Settings(QString);
    Settings();
    ~Settings();
    void saveSettings(QString name);
    void loadSettings(QString name);
    QString getValue(QString group, QString key, QVariant defaultValue = 0);
    void setValue(QString group, QString key, QVariant value = 0);
    QSettings *getSettings();
private:
    QSettings * _settings;
};

#endif // SETTINGS_H
