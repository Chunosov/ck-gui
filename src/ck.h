#ifndef CK_H
#define CK_H

#include <QProcess>
#include <QStringList>

class CkEntry
{
public:
    QString uid;
    QString name;

    QString str() const { return QString("%1: %2").arg(uid, name); }

    bool isEmpty() const { return uid.isEmpty(); }
};

//-----------------------------------------------------------------------------

class CK
{
public:
    CK();

    static CK& instance();

    CkEntry queryEnvByUid(const QString& uid);
    QList<CkEntry> queryEnvsByTags(const QString& tags);
    void refreshEnv(const QString& uid);
    void removeEnv(const QString& uid);

    QStringList queryPackagesByTags(const QString& tags);
    QStringList queryProgramsByTags(const QString& tags);

    static QString envPath(const QString& uid);
    static QString envScriptPath(const QString& uid);
    static QString envMetaPath(const QString& uid);

    static bool isFileExists(const QString& path);

private:
    QProcess _ck;
    QString _reposPath;
    QStringList _args;

    QStringList ck(const QStringList &args);
};

#endif // CK_H
