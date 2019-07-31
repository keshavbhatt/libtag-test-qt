#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void loadMetaData(QString file);
    void on_pushButton_clicked();

    void on_clear_clicked();

private:
    Ui::MainWindow *ui;
    Ui::listItem listItem_Ui;
};

#endif // MAINWINDOW_H
