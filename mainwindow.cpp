#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "savethread.h"
#include <QProgressDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->dateEdit->setDate(QDate::currentDate());
    ui->lineEditOrgName->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::verify()
{
    if (ui->lineEditOrgName->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("The organization name cannot be empty"));
        return false;
    }
    else if (ui->lineEditOrgPhone->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("The agency phone cannot be empty"));
        return false;
    }
    else if (ui->lineEditCorpName->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("The institutional legal person cannot be empty"));
        return false;
    }
    else if (!checkId(ui->lineEditCorpId->text()))
    {
        QMessageBox::critical(this, tr("Error"), tr("The corporate identity card is wrong"));
        return false;
    }
    else if (ui->lineEditAdminName->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("The administrator cannot be empty"));
        return false;
    }
    else if (!checkId(ui->lineEditAdminId->text()))
    {
        QMessageBox::critical(this, tr("Error"), tr("Incorrect administrator ID card"));
        return false;
    }
    else if (ui->lineEditDept->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("The administrator department cannot be empty"));
        return false;
    }
    else if (ui->lineEditPost->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("The administrator position cannot be empty"));
        return false;
    }
    return true;
}

bool MainWindow::checkId(const QString &id)
{
    // 长度
    if (id.length() != 18)
    {
        return false;
    }

    // 生日
    bool birthday = QDate::isValid(id.mid(6, 4).toInt(), id.mid(10, 2).toInt(), id.mid(12, 2).toInt());
    if (!birthday)
    {
        return false;
    }

    // 加权因子
    int weight[] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    // 校验码表
    char validateCodes[] = { '1', '0', 'X', '9', '8', '7', '6', '5', '4', '3', '2'};
    int sum = 0;
    for(int i = 0; i < 17; i++)
    {
        sum = sum + (id.at(i).digitValue()) * weight[i];
    }
    int mod = sum % 11;
    return validateCodes[mod] == id[17];
}


void MainWindow::on_pushButton_clicked()
{
    if (!verify())
    {
        return;
    }
    QString dir = QFileDialog::getExistingDirectory();
    if (dir.isEmpty())
    {
        return;
    }

    QString name = ui->lineEditOrgName->text();
    QString fileName = dir + "/" + name + ".docx";
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists())
    {
        int b = QMessageBox::question(this, tr("Wanning"), fileInfo.absoluteFilePath() + tr("The file already exists. Is it overwritten?"));
        if (b == QMessageBox::No)
        {
            return;
        }
    }

    QString admin = ui->lineEditAdminName->text();
    QString adminId = ui->lineEditAdminId->text();
    QString corp = ui->lineEditCorpName->text();
    QString corpid = ui->lineEditCorpId->text();
    QString dept = ui->lineEditDept->text();
    QString post = ui->lineEditPost->text();
    QString phone = ui->lineEditOrgPhone->text();

    QProgressDialog *d = new QProgressDialog(this);
    d->setLabelText(tr("Building, please wait..."));
    d->setRange(0, 0);

    SaveThread *t = new SaveThread(this);
    connect(t, &SaveThread::finished, d, &QProgressDialog::close);
    connect(t, &SaveThread::SaveFinished, this, &MainWindow::SaveFinished);

    t->SetInfo(name, admin, adminId, corp, corpid, dept, post, phone, fileInfo);
    t->start();
    d->exec();

}

void MainWindow::SaveFinished(QString path)
{
    QMessageBox::information(this, tr("Finish"), tr("The file has been stored in ") + path);
}
