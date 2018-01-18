#include "appconfig.h"

#include <QApplication>
#include <QSettings>
#include <QDir>
#include <QDebug>

QSettings& AppConfig::config()
{
    static QSettings cfg(configFileName(), QSettings::IniFormat);
    return cfg;
}

QString AppConfig::configFileName()
{
    return qApp->applicationDirPath() + QDir::separator() + "app.conf";
}

QString AppConfig::ckReposPath()
{
    return config().value("ck_repos_path").toString();
}

QString AppConfig::ckBinPath()
{
    return config().value("ck_bin_path").toString();
}

QString AppConfig::ckExeName()
{
    return config().value("ck_exe_name").toString();
}

int AppConfig::configValueInt(const QString& key, int defaultValue)
{
    bool ok;
    int value = config().value(key, defaultValue).toInt(&ok);
    return ok? value: defaultValue;
}

QString AppConfig::configValueStr(const QString& key, const QString& defaultValue)
{
    auto value = config().value(key, defaultValue).toString();
    return value.isEmpty()? defaultValue: value;
}
