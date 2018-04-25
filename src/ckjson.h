#ifndef CKJSON_H
#define CKJSON_H

#include <QJsonObject>
#include <QStringList>

class CkJson
{
public:
    CkJson() {}
    CkJson(const QJsonObject& json): _json(json) {}

    const QJsonObject& json() const { return _json; }
    bool ok() const { return _ok; }

    QStringList subkeys(const QString& key) const;
    QString valueStr(const QString& key) const;
    QString valueStr(QStringList path) const;

protected:
    QJsonObject _json;

    void open(const QString& jsonFile);
    void errorKeyNotFound(const QString& key) const;
    void warnKeyIsEmpty(const QString& key) const;

private:
    bool _ok = false;

    QString valueStr(const QJsonObject& json, const QString& key) const;
};

//-----------------------------------------------------------------------------

class CkEnvDep : public CkJson
{
public:
    CkEnvDep() {}
    CkEnvDep(const QString& name, const QJsonObject& json): CkJson(json), _name(name) {}

    QString name() const { return valueStr("name"); }
    QString uoa() const { return valueStr("uoa"); }
    QString version() const { return valueStr("ver"); }

private:
    QString _name;
};

//-----------------------------------------------------------------------------

class CkEnvMeta : public CkJson
{
public:
    CkEnvMeta(const QString& uid);

    QStringList tags() const;
    QString full_path() const { return valueStr({"customize", "full_path"}); }
    QString version() const { return valueStr({"customize", "version"}); }
    QString env_script() const { return valueStr("env_script"); }
    QString packageUid() const { return valueStr({"customize", "used_package_uid"}); }
    QString packageUoa() const { return valueStr({"customize", "used_package_uoa"}); }
    QVector<CkEnvDep> deps() const;
};

//-----------------------------------------------------------------------------

class CkRepoDep : public CkJson
{
public:
    CkRepoDep() {}
    CkRepoDep(const QJsonObject& json): CkJson(json) {}

    QString uoa() const { return valueStr("repo_uoa"); }
    QString url() const { return valueStr("repo_url"); }
};

//-----------------------------------------------------------------------------

class CkRepoMeta : public CkJson
{
public:
    CkRepoMeta(const QString& repoName);

    QString name() const { return valueStr("data_name"); }
    QString uid() const { return valueStr("data_uid"); }
    QString uoa() const { return valueStr("data_uoa"); }
    QString url() const { return _dict.valueStr("url"); }
    QVector<CkRepoDep> deps() const;

public:
    CkJson _dict;
};

//-----------------------------------------------------------------------------

class CkInfo : public CkJson
{
public:
    CkInfo(const QString& path);

    QString backup_data_uid() const { return valueStr("backup_data_uid"); }
    QString data_name() const { return valueStr("data_name"); }
};

class CkEnvInfo : public CkInfo
{
public:
    CkEnvInfo(const QString& uid);
};

#endif // CKJSON_H
