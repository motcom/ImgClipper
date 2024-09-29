#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
}
MainWindow::~MainWindow() {}


// Event 処理 start ------------------------------
void MainWindow::keyPressEvent(QKeyEvent *key)
{
    switch(key->key())
    {
    // paste
    case Qt::Key_P:
        pasteImage();
        break;

   // next
    case Qt::Key_Right:
    case Qt::Key_L:
        next();
        break;

     // back
    case Qt::Key_Left:
    case Qt::Key_H:
        back();
        break;
    }

    // esc
case Qt::Key_Escape:
    escape();
    break;

}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (curIndex==-1) return;
    QImage* tmpImg = imgList[curIndex].data();
    painter.drawImage(0,0,*tmpImg);
}

// Event 処理 End ------------------------------

void MainWindow::pasteImage()
{
        qDebug() << "paste";
        QClipboard* clip = QApplication::clipboard();
        if (clip->mimeData()->hasImage())
        {

            QSharedPointer<QImage>
                imgPtr(QSharedPointer<QImage>::create(clip->image()));
            // QSharedPointerをQListに追加
            imgList.append(imgPtr);
        }
        curIndex = imgList.length()-1;
        maxIndex = curIndex;
        qDebug() << imgList.length();
        this->update();
}

void MainWindow::next()
{
    curIndex ++;
    if (curIndex > maxIndex) curIndex = 0;
    this->update();
}

void MainWindow::back()
{
    curIndex--;
    if(curIndex < 0) curIndex = maxIndex;
    this->update();
}

void MainWindow::escape()
{
    this->close();
}

