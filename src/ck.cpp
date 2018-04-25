#include "ck.h"

#include "appevents.h"
#include "ckjson.h"
#include "orion/tools/OriSettings.h"

#include <QFile>
#include <QDebug>
#include <QDir>

QString CK::_reposPath;

CK& CK::instance()
{
    static CK ck;
    return ck;
}

CK::CK()
{
    Ori::Settings settings;
    settings.beginDefaultGroup();

    _reposPath = settings.strValue("ckReposPath");
    if (_reposPath.isEmpty())
    {
        AppEvents::error("CK repository path not found in config");
        return;
    }

    QString ckExe;

#ifdef Q_OS_LINUX
    if (settings.value("ckInSystemPath").toBool())
    {
        ckExe = "ck";
    }
    else
#endif
    {
        auto ckDir = settings.strValue("ckBinPath");
        if (ckDir.isEmpty())
        {
            AppEvents::error("CK exe name not found in config");
            return;
        }
        _ck.setWorkingDirectory(ckDir);

#ifdef Q_OS_WIN32
        ckExe = "python";
        _args = QStringList { "-W", "ignore::DeprecationWarning", "..\\ck\\kernel.py" };
#else
        ckExe = settings.strValue("ckExeName");
        if (ckExe.isEmpty())
        {
            AppEvents::error("CK bin path not found in config");
            return;
        }
#endif
    }
    _ck.setProgram(ckExe);

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

QStringList CK::queryPackagesByTags(const QString& tags)
{
    return ck({ "search", "package", "--tags="+tags });
}

QStringList CK::queryPackagesByName(const QString& name)
{
    return ck({ "search", "package:" + name });
}

QStringList CK::queryProgramsByTags(const QString& tags)
{
    return ck({ "search", "program", "--tags="+tags });
}

QStringList CK::ck(const QStringList& args)
{
#if defined(Q_OS_WIN)
    static QString EOL("\r\n");
#elif defined(Q_OS_MAC)
    static QString EOL("\r");
#else
    static QString EOL("\n");
#endif

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
    auto lines = output.split(EOL, QString::SkipEmptyParts);
    for (const QString& line: lines)
        if (line.startsWith(errorMarker))
        {
            AppEvents::error(output);
            return QStringList();
        }
    return lines;
}

QString CK::repoPath(const QString& name)
{
    return makePath({ _reposPath, name });
}

QString CK::metaPath(const QString& entityPath)
{
    return makePath({ entityPath, ".cm", "meta.json" });
}

QString CK::envPath(const QString& uid)
{
    return makePath({ _reposPath, "local", "env", uid });
}

QString CK::envScriptPath(const QString& uid)
{
    return makePath({ envPath(uid), "env.sh" });
}

QString CK::envMetaPath(const QString& uid)
{
    return metaPath(envPath(uid));
}

QString CK::packagePath(const QString& name)
{
    QStringList parts = name.split(':');
    parts.insert(0, _reposPath);
    return makePath(parts);
}

QString CK::packageMetaPath(const QString& name)
{
    return metaPath(packagePath(name));
}

void CK::refreshEnv(const QString& uid)
{
    ck({ "refresh", "env:" + uid });
}

void CK::removeEnv(const QString& uid)
{
    ck({ "rm", "env:" + uid, "-f" });
}

bool CK::isFileExists(const QString& path)
{
    return QFile(path).exists();
}

QString CK::makePath(const QStringList& parts)
{
    return parts.join(QDir::separator());
}

QStringList CK::queryRepos()
{
    QDir repoDir(_reposPath);
    return repoDir.entryList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot);
}
