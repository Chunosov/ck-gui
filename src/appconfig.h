#ifndef APPCONFIG_H
#define APPCONFIG_H

#include <QString>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

class AppConfig
{
public:
    static QString ckReposPath();
    static QString ckBinPath();
    static QString ckExeName();

private:
    static QString configFileName();
    static QSettings& config();

    static int configValueInt(const QString& key, int defaultValue);
    static QString configValueStr(const QString& key, const QString& defaultValue);
};


#endif // APPCONFIG_H
