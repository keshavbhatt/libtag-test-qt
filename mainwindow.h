#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "metadata.h"
#include "ui_songmeta.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_clear_clicked();

    void on_stopThread_clicked();

    void setMetaData(const QStringList metaList, const QPixmap pix);
private:
    metadata meta_thread;
    Ui::MainWindow *ui;
    Ui::listItem listItem_Ui;
    int tasksProgress = 0;
};

#endif // MAINWINDOW_H
