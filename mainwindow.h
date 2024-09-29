#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QImage>
#include <QKeyEvent>
#include <QList>
#include <QMainWindow>
#include <QMimeData>
#include <QPainter>
#include <QSharedPointer>
#include <QPaintEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    // private member
    QList<QSharedPointer<QImage>> imgList;
    int curIndex = -1;
    int maxIndex = -1;

    // Event
    void keyPressEvent(QKeyEvent*) override;
    void paintEvent(QPaintEvent*) override;

    // utility function
    void pasteImage();
    void next();
    void back();
    void escape();

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();
};
#endif // MAINWINDOW_H
