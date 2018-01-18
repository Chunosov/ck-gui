#ifndef CKJSON_H
#define CKJSON_H

#include <QJsonObject>
#include <QStringList>

class CkJson
{
public:
    const QJsonObject& json() const { return _json; }
    bool ok() const { return _ok; }

    QStringList subkeys(const QString& key) const;
    QString valueStr(const QString& key) const;
    QString valueStr(QStringList path) const;

protected:
    QJsonObject _json;

    void open(const QString& fileName);
    void errorKeyNotFound(const QString& key) const;
    void warnKeyIsEmpty(const QString& key) const;

private:
    bool _ok = false;

    QString valueStr(const QJsonObject& json, const QString& key) const;
};

//-----------------------------------------------------------------------------

class CkEnvMeta : public CkJson
{
public:
    CkEnvMeta(const QString& uid);

    QStringList tags() const;
    QString full_path() const { return valueStr({"customize", "full_path"}); }
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
