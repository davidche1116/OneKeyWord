#ifndef SAVETHREAD_H
#define SAVETHREAD_H

#include <QThread>
#include <QAxObject>
#include <QFileInfo>

class SaveThread : public QThread
{
    Q_OBJECT
public:
    explicit SaveThread(QObject *parent = nullptr);
    void SetInfo(const QString &name,
                 const QString &adminName,
                 const QString &adminId,
                 const QString &corpName,
                 const QString &corpId,
                 const QString &dept,
                 const QString &post,
                 const QString &phone,
                 QFileInfo fileInfo);

private:
    bool Open(QString Dir);
    void SaveFile(QString dir);
    void InputText(const QString &posName, const QString &text);

protected:
    void run() override;

private:
    QAxObject *m_WordFile;//指向整个Word应用程序
    QAxObject *Documents;//指向文档集，Word有很多文档
    QAxObject *m_Document;//指向激活文档，Word有很多文档,这是目前激活的那个文档

    QString mName;
    QString mAdminName;
    QString mAdminId;
    QString mCorpName;
    QString mCorpId;
    QString mDept;
    QString mPost;
    QString mPhone;
    QFileInfo mFileInfo;

signals:
    void SaveFinished(QString path);

};

#endif // SAVETHREAD_H
