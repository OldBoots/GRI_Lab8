#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QBitArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Info
{
    bool flg_press;
    int x_min;
    int y_min;
    int x_max;
    int y_max;
    int depth;
    int size;
    int begin;
    int width;
    int height;
    QString version;
    void set_version(int ver){
        switch (ver) {
        case 0:
            version = "PC Paintbrush version 2.5";
            break;
        case 2:
            version = "PC Paintbrush 2.8 with palette information";
            break;
        case 3:
            version = "PC Paintbrush 2.8 without palette information";
            break;
        case 4:
            version = "PC Paintbrush for Windows";
            break;
        case 5:
            version = "PC Paintbrush 3.0 or later, PC Paintbrush Plus";
            break;
        default:
            version = "Version not defined";
            break;
        }
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void slot_load_img();
private:
    int sum_Byts(QByteArray arr, bool forward);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
