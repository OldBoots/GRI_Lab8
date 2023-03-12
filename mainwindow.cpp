#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(slot_load_img()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::sum_Byts(QByteArray arr, bool forward)
{
    if(forward){
        return QByteArray::fromRawData(arr, arr.size()).toHex().toInt(nullptr, 16);
    }else{
        int n = arr.size();
        if(n % 2){
            n--;
        }
        for(int i = 0; i < n / 2; i++){
            qSwap(arr[i], arr[arr.size() - 1 - i]);
        }
        return QByteArray::fromRawData(arr, arr.size()).toHex().toInt(nullptr, 16);
    }
}

void MainWindow::slot_load_img()
{
    qDebug("a");
    QByteArray arr;
    QFile file(QFileDialog::getOpenFileName());
    file.open(QIODevice::ReadOnly);
    arr = file.readAll();
    file.close();
    Info inf;
    inf.set_version(sum_Byts(QByteArray::fromRawData(&arr[1], 1), false));
    inf.flg_press = sum_Byts(QByteArray::fromRawData(&arr[2], 1), false);
    inf.depth = sum_Byts(QByteArray::fromRawData(&arr[3], 1), false);
    inf.x_min = sum_Byts(QByteArray::fromRawData(&arr[4], 2), false);
    inf.y_min = sum_Byts(QByteArray::fromRawData(&arr[6], 2), false);
    inf.x_max = sum_Byts(QByteArray::fromRawData(&arr[8], 2), false);
    inf.y_max = sum_Byts(QByteArray::fromRawData(&arr[10], 2), false);
    inf.width = inf.x_max + 1;
    inf.height = inf.y_max + 1;
    inf.size = arr.size();
    inf.begin = 128;
    ui->label_2->setText("version - " + inf.version +
                         "\npress - " + QString::number(inf.flg_press) +
                         "\ndepth - " + QString::number(inf.depth) +
                         "\nx_min - " + QString::number(inf.x_min) +
                         "\ny_min - " + QString::number(inf.y_min) +
                         "\nx_max - " + QString::number(inf.x_max) +
                         "\ny_max - " + QString::number(inf.y_max) +
                         "\nwidth - " + QString::number(inf.width) +
                         "\nheight - " + QString::number(inf.height) +
                         "\nsize - " + QString::number(inf.size) +
                         "\nbegin - " + QString::number(inf.begin));
    QImage img(inf.width, inf.height, QImage::Format_Indexed8);
    for(int i = 0; i < 256; i++){
        img.setColor(i, qRgb(QByteArray::fromRawData(&arr[arr.size() - 768 + i * 3], 1).toHex().toInt(nullptr, 16),
                     QByteArray::fromRawData(&arr[arr.size() - 768 + i * 3 + 1], 1).toHex().toInt(nullptr, 16),
                QByteArray::fromRawData(&arr[arr.size() - 768 + i * 3 + 2], 1).toHex().toInt(nullptr, 16)));
    }
    for(int i = inf.begin, x = 0, y = 0, g = 0; i < arr.size() - 769;){
        g = QByteArray::fromRawData(&arr[i], 1).toHex().toInt(nullptr, 16);
        if(g >= 192){
            i++;
            for(int j = 0; j < (g & 0x3f); j++){
                img.setPixel(x, y, QByteArray::fromRawData(&arr[i], 1).toHex().toInt(nullptr, 16));
                x++;
                if(x == inf.width){
                    x = 0;
                    y++;
                }
            }
        }else{
            img.setPixel(x, y, QByteArray::fromRawData(&arr[i], 1).toHex().toInt(nullptr, 16));
            x++;
            if(x == inf.width){
                x = 0;
                y++;
            }
        }
        i++;
    }
    ui->label->setPixmap(QPixmap().fromImage(img).scaled(400,
                                                         400,
                                                         Qt::KeepAspectRatio,
                                                         Qt::SmoothTransformation));
}
