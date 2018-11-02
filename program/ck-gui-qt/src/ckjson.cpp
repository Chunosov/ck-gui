#include "ckjson.h"

#include "appevents.h"
#include "ck.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

void CkJson::open(const QString& jsonFile)
{
    QFile file(jsonFile);
    if (!file.exists())
        return AppEvents::error(QString("File not found: %1").arg(jsonFile));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return AppEvents::error(QString("Unable to open file %1: %2").arg(jsonFile, file.errorString()));

    QJsonParseError status;
    auto doc = QJsonDocument::fromJson(file.readAll(), &status);
    if (status.error != QJsonParseError::NoError)
        return AppEvents::error(QString("Invalid CK-json file %1: %2").arg(jsonFile, status.errorString()));
    _json = doc.object();
    if (_json.isEmpty())
    {
        qWarning() << "Empty CK-json file:" << jsonFile;
        return;
    }
    _ok = true;
}

void CkJson::errorKeyNotFound(const QString& key) const
{
    qWarning() << "Required key not found:" << key;
}

void CkJson::warnKeyIsEmpty(const QString& key) const
{
    qWarning() << key << "is empty";
}

QString CkJson::valueStr(const QString& key) const
{
    return valueStr(_json, key);
}

QString CkJson::valueStr(QStringList path) const
{
    if (path.size() == 1)
        return valueStr(_json, path.first());
    auto json = _json;
    for (int i = 0; i < path.size()-1; i++)
    {
        if (!json.contains(path[i]))
        {
            errorKeyNotFound(path.join("/"));
            return QString();
        }
        json = json[path.at(i)].toObject();
    }
    auto key = path.last();
    if (!json.contains(key))
    {
        errorKeyNotFound(path.join("/"));
        return QString();
    }
    return valueStr(json, key);
}

QString CkJson::valueStr(const QJsonObject& json, const QString& key) const
{
    if (!json.contains(key))
    {
        errorKeyNotFound(key);
        return QString();
    }
    auto res = json[key].toString();
    if (res.isEmpty())
        warnKeyIsEmpty(key);
    return res;
}

//-----------------------------------------------------------------------------

CkEnvMeta::CkEnvMeta(const QString& uid)
{
    open(CK::envMetaPath(uid));
}

QStringList CkEnvMeta::tags() const
{
    auto json = _json["tags"].toArray();
    QStringList tags;
    for (auto it = json.constBegin(); it != json.constEnd(); it++)
        tags << (*it).toString();
    return tags;
}

QVector<CkEnvDep> CkEnvMeta::deps() const
{
    QVector<CkEnvDep> res;
    QJsonObject deps = _json["deps"].toObject();
    for (auto dep : deps.keys())
        res.append(CkEnvDep(dep, deps[dep].toObject()));
    return res;
}

//-----------------------------------------------------------------------------

QString CkInfo::infoFileFullPath(const QString& path)
{
    return CK::makePath({path, ".cm", "info.json" });
}

CkInfo::CkInfo(const QString& path)
{
    open(infoFileFullPath(path));
}

bool CkInfo::isValid(const QString& path)
{
    return QFile::exists(infoFileFullPath(path));
}

//-----------------------------------------------------------------------------

CkEnvInfo::CkEnvInfo(const QString& uid) : CkInfo(CK::envPath(uid))
{
}

bool CkEnvInfo::isValid(const QString& uid)
{
    return CkInfo::isValid(CK::envPath(uid));
}

//-----------------------------------------------------------------------------

CkRepoMeta::CkRepoMeta(const QString& repoName)
{
    open(CK::makePath({CK::repoPath(repoName), ".ckr.json" }));
    _dict = CkJson(_json["dict"].toObject());
}

QVector<CkRepoDep> CkRepoMeta::deps() const
{
    QVector<CkRepoDep> res;
    auto json = _dict.json()["repo_deps"].toArray();
    for (auto it = json.constBegin(); it != json.constEnd(); it++)
        res.append(CkRepoDep((*it).toObject()));
    return res;
}
