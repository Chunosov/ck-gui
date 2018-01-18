#include "ck.h"

#include "appevents.h"
#include "appconfig.h"
#include "ckjson.h"
#include "utils.h"

#include <QFile>
#include <QDebug>
#include <QDir>

CK::CK()
{
    _reposPath = AppConfig::ckReposPath();
    if (_reposPath.isEmpty())
        AppEvents::error("CK repository path not found in config");

    auto ckDir = AppConfig::ckBinPath();
    if (ckDir.isEmpty())
        AppEvents::error("CK exe name not found in config");

#ifdef Q_OS_WIN32
    _ck.setProgram("python");
    _args = QStringList { "-W", "ignore::DeprecationWarning", ckDir + "\\..\\ck\\kernel.py" };
#else
    auto ckExe = AppConfig::ckExeName();
    if (ckExe.isEmpty())
        AppEvents::error("CK bin path not found in config");

    _ck.setProgram(ckExe);
    _ck.setWorkingDirectory(ckDir);
#endif

    qDebug() << "CK repos path:" << _reposPath;
    qDebug() << "CK bin path:" << _ck.workingDirectory();
    qDebug() << "CK executable:" << _ck.program();
    qDebug() << "CK default args:" << _args.join(" ");
}

QList<CkEntry> CK::queryEnvsByTags(const QString& tags)
{
    auto args = QStringList{ "search", "env", "--tags="+tags };
    auto results = ck(args);
    if (results.isEmpty())
        return QList<CkEntry>();

    QList<CkEntry> entries;
    for (auto result: results)
    {
        // result = "local:env:fff6cd1bb4dc78f2"
        auto uid = result.section(':', -1);
        auto entry = queryEnvByUid(uid);
        if (!entry.isEmpty())
            entries << entry;
    }
    return entries;
}

CkEntry CK::queryEnvByUid(const QString& uid)
{
    if (uid.isEmpty())
        return CkEntry();

    auto info = CkEnvInfo(uid);
    if (!info.ok()) return CkEntry();

    auto name = info.data_name();
    if (name.isEmpty()) return CkEntry();

    CkEntry entry { uid, name };
    return entry;
}

QStringList CK::ck(const QStringList& args)
{
    static QString errorMarker("CK error:");

#ifdef Q_OS_WIN32
    QStringList fullArgs = _args;
    fullArgs.append(args);
    _ck.setArguments(fullArgs);
#else
    _ck.setArguments(args);
#endif
    qDebug() << "Run CK command:" << _ck.program() + ' ' +  _ck.arguments().join(" ");
    _ck.start();
    _ck.waitForFinished();
    auto error = _ck.errorString();
    auto errors = QString::fromLatin1(_ck.readAllStandardError());
    auto output = QString::fromLatin1(_ck.readAllStandardOutput());
//    qDebug() << "STDERR: " + errors;
//    qDebug() << "STDOUT: " + output;
    if (output.isEmpty() && (_ck.error() != QProcess::UnknownError || !errors.isEmpty()))
    {
        AppEvents::error(QString("Error running program %1/%2: %3\n%4")
            .arg(_ck.workingDirectory()).arg(_ck.program()).arg(error).arg(errors));
        return QStringList();
    }
    auto lines = output.split(Utils::EOL(), QString::SkipEmptyParts);
    for (const QString& line: lines)
        if (line.startsWith(errorMarker))
        {
            AppEvents::error(output);
            return QStringList();
        }
    return lines;
}

QString CK::envPath(const QString& uid)
{
    return Utils::makePath({ AppConfig::ckReposPath(), "local", "env", uid });
}

QString CK::envScriptPath(const QString& uid)
{
    return Utils::makePath({ envPath(uid), "env.sh" });
}

QString CK::envMetaPath(const QString& uid)
{
    return Utils::makePath({ envPath(uid), ".cm", "meta.json" });
}

void CK::refreshEnv(const QString& uid)
{
    ck({ "refresh", "env:" + uid });
}
