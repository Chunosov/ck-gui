#include "ckjson.h"

#include "ck.h"
#include "utils.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

void CkJson::open(const QString& file)
{
    if (!QFile(file).exists())
    {
        qWarning() << "File not found:" << file;
        return;
    }
    auto doc = QJsonDocument::fromJson(Utils::loadTextFromFile(file));
    _json = doc.object();
    if (_json.isEmpty())
    {
        qWarning() << "Invalid CK-json file:" << file;
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

CkInfo::CkInfo(const QString& path)
{
    open(Utils::makePath({path, ".cm", "info.json" }));
}

//-----------------------------------------------------------------------------

CkEnvInfo::CkEnvInfo(const QString& uid) : CkInfo(CK::envPath(uid))
{
}

//-----------------------------------------------------------------------------

CkRepoMeta::CkRepoMeta(const QString& repoName)
{
    open(Utils::makePath({CK::repoPath(repoName), ".ckr.json" }));
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
