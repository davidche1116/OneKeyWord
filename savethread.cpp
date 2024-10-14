#include "savethread.h"
#include <QFile>
#include <QDir>
#include <QDate>
#include <QDebug>

SaveThread::SaveThread(QObject *parent) : QThread(parent)
{

}

void SaveThread::SetInfo(const QString &name,
                         const QString &adminName,
                         const QString &adminId,
                         const QString &corpName,
                         const QString &corpId,
                         const QString &dept,
                         const QString &post,
                         const QString &phone, QFileInfo fileInfo)
{
    mName = name;
    mAdminName = adminName;
    mAdminId = adminId;
    mCorpName = corpName;
    mCorpId = corpId;
    mDept = dept;
    mPost = post;
    mPhone = phone;
    mFileInfo = fileInfo;
}

bool SaveThread::Open(QString Dir)
{
    // 新建一个word应用程序,并设置为可见
    m_WordFile = new QAxObject();//取代QAxWidget，使其在子线程中可用
    bool bFlag = m_WordFile->setControl("word.Application");
    if (NULL == m_WordFile)
    {
        qDebug() << "word opening false";
        // 尝试用wps打开
        bFlag = m_WordFile->setControl("kwps.Application");
        if (!bFlag)
        {
            return false;
        }
    }
    //设置打开的word应用可见，建议一开始可见。这样你可以看到这个过程。
    m_WordFile->setProperty("Visible", false);
    // 获取所有的工作文档
    Documents = m_WordFile->querySubObject("Documents");
    if (NULL == Documents)
    {
        qDebug() << "documents opening false";
        return false;
    }
    // 以文件template.dot为模版新建一个文档
    Documents->dynamicCall("Add(QString)", Dir);
    // 获取当前激活的文档
    m_Document = m_WordFile->querySubObject("ActiveDocument");
    qDebug() <<"ActiveDocument:"<< m_WordFile;
    /*cout << *m_WordFile << endl;*/
    if (NULL == m_Document)
    {
        qDebug()  << "Active Doc opening false";
        return false;
    }
    return true;
}

void SaveThread::SaveFile(QString dir)
{
    if (dir.isEmpty()) {
        qDebug() << "dir isEmpty";
        return;
    }
    qDebug() << "begin edit" << dir;
    // 将文件另存为outFileName,关闭工作文档，退出应用程序
    m_Document->dynamicCall("SaveAs (const QString&)", dir);
    m_Document->dynamicCall("Close (boolean)", true);  //关闭文本窗口
    m_WordFile->dynamicCall("Quit(void)");  //退出word
    delete m_Document;
    delete Documents;
    delete m_WordFile;

    emit SaveFinished(dir);
}

void SaveThread::InputText(const QString &posName, const QString &text)
{
    //根据bookmark找到书签位置
    QAxObject *bookmark_text = m_Document->querySubObject(QString("Bookmarks(%1)").arg(posName).toLocal8Bit().data());
    //根据bookmark找到selection，也就是光标位置
    bookmark_text->dynamicCall("Select(void)");

    QAxObject* selection = m_WordFile->querySubObject("Selection");
    if (!selection)
    {
        return;
    }
    selection->dynamicCall("TypeText(const QString&)", text);
}

void SaveThread::run()
{
    QString odt = QDir::tempPath() + "/1.odt";
    QFile::copy(":/odt/1.odt", odt);
    QString time = QDate::currentDate().toString(tr("yyyy-MM-dd"));

    if (Open(odt))
    {
        InputText("time1", time);
        InputText("time2", time);
        InputText("time3", time);
        InputText("time4", time);
        InputText("name1", mName);
        InputText("name2", mName);
        InputText("name3", mName);
        InputText("name4", mName);
        InputText("name5", mName);
        InputText("name6", mName);
        InputText("admin1", mAdminName);
        InputText("admin2", mAdminName);
        InputText("adminid1", mAdminId);
        InputText("adminid2", mAdminId);
        InputText("corp", mCorpName);
        InputText("corpid", mCorpId);
        InputText("dept", mDept);
        InputText("post", mPost);
        InputText("phone", mPhone);

        SaveFile(mFileInfo.absoluteFilePath());
        //        QMessageBox::information(this, tr("完成"), tr("文件已存储在") + fileInfo.absoluteFilePath());

    }
}
