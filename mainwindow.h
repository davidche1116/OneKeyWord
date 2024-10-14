#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    bool verify();
    bool checkId(const QString &id);

private slots:
    void on_pushButton_clicked();
    void SaveFinished(QString path);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
