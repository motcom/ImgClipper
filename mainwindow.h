#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QGraphicsWidget>
#include <QImage>
#include <QKeyEvent>
#include <QList>
#include <QMainWindow>
#include <QMessageBox>
#include <QMimeData>
#include <QPaintEvent>
#include <QPainter>
#include <QRect>
#include <QSharedPointer>
#include <QSize>
#include <QStandardPaths>
#include <QTransform>
#include <QVector2D>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    // private enum
    enum class ViewerMode{Normal,Zoom};
    ViewerMode viewrMode = ViewerMode::Normal;

    // const value
    const float ZOOM_INIT = 1.0;
    const float ZOOM_MAX = 3.0;
    const float ZOOM_MIN = 0.1;
    const float ZOOM_DELTA = 0.1;
    const float OPACITY_INIT = 1.0;
    const float OPACITY_DELTA = 0.1;
    const float OFFSET_DELTA = 10;


    // private member
    QTextEdit helpTextEdit;
    QList<QSharedPointer<QImage>> imgList;
    int curIndex = -1;
    int maxIndex = -1;
    bool flagTopHint = false;
    bool flagFrameHide = false;
    float opacity = OPACITY_INIT;
    float zoomFactor = ZOOM_INIT;
    QVector2D imgOffset{0,0};


    // Event
    void keyPressEvent(QKeyEvent*) override;
    void paintEvent(QPaintEvent*) override;

    // mode branch
    void normalBranch(QKeyEvent*);
    void zoomBranch(QKeyEvent*);

    // normal mode function
    void alwaysOnTop();
    void pasteImage();
    void next();
    void back();
    void escape();
    void save();
    void hideTabToggle();

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    const QString helpString =
      ""
      "";


};
#endif // MAINWINDOW_H
