#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
}
MainWindow::~MainWindow() {}


// Event 処理 start ------------------------------
void MainWindow::keyPressEvent(QKeyEvent *key) {
    switch (viewrMode) {
    case ViewerMode::Normal:
        normalBranch(key);
        break;
    case ViewerMode::Zoom:
        zoomBranch(key);
        break;
    }
}

void MainWindow::normalBranch(QKeyEvent *key)
{
    switch(key->key())
    {
    // Zoom
    case Qt::Key_Plus:
    case Qt::Key_Z:
        zoomFactor += ZOOM_DELTA;
        if (zoomFactor >3.0) zoomFactor=3.0;
        viewrMode = ViewerMode::Zoom;
        update();
        break;

    case Qt::Key_X:
    case Qt::Key_Minus:
        zoomFactor -= ZOOM_DELTA;
        if (zoomFactor < ZOOM_DELTA) zoomFactor = ZOOM_DELTA;
        viewrMode = ViewerMode::Zoom;
        update();
        break;

    // paste
    case Qt::Key_V:
        if(!(Qt::ControlModifier==key->modifiers()))
            break;
        Q_FALLTHROUGH();

    case Qt::Key_P:
        pasteImage();
        break;

    // window always top
    case Qt::Key_T:
        alwaysOnTop();
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
    case Qt::Key_Escape:
    escape();
        break;

      //save
    case Qt::Key_S:
    case Qt::Key_W:
    save();
       break;

    // setOpacity
    case Qt::Key_Greater:
        opacity += OPACITY_DELTA;
        if(opacity > OPACITY_INIT) opacity = OPACITY_INIT;
        setWindowOpacity(opacity);
        break;

    case Qt::Key_Less:
        opacity -= OPACITY_DELTA;
        if(opacity < 0) opacity = 0;
        setWindowOpacity(opacity);
        break;

    // tab
    case Qt::Key_Tab:
        hideTabToggle();
        break;
   }
}

void MainWindow::zoomBranch(QKeyEvent *key)
{

    // state nomal
    switch (key->key()) {

    // Escape
    case Qt::Key_Escape:
       escape();
        break;

    // to nomal
    case Qt::Key_N:
    case Qt::Key_0:
        zoomFactor = ZOOM_INIT;
        imgOffset.setX(0);
        imgOffset.setY(0);
        viewrMode = ViewerMode::Normal;
        update();
        break;

    // Zoom
    case Qt::Key_Plus:
    case Qt::Key_Z:
        zoomFactor += ZOOM_DELTA;
        if (zoomFactor > ZOOM_MAX) zoomFactor=ZOOM_MAX;
        break;

    case Qt::Key_Minus:
    case Qt::Key_X:
        zoomFactor -= ZOOM_DELTA;
        if (zoomFactor < ZOOM_MIN) zoomFactor = ZOOM_MIN;
        break;

    // Right
    case Qt::Key_Right:
    case Qt::Key_L:
        imgOffset.setX(imgOffset.x()-OFFSET_DELTA);
        break;

     // Left
    case Qt::Key_Left:
    case Qt::Key_H:
        imgOffset.setX(imgOffset.x()+OFFSET_DELTA);
        break;

     // Up
    case Qt::Key_J:
        imgOffset.setY(imgOffset.y()-OFFSET_DELTA);
        break;
     // Down
    case Qt::Key_K:
        imgOffset.setY(imgOffset.y()+OFFSET_DELTA);
        break;

    // tab
    case Qt::Key_Tab:
        hideTabToggle();
        break;

    // setOpacity
    case Qt::Key_Greater:
        opacity += OPACITY_DELTA;
        if(opacity > OPACITY_INIT) opacity = OPACITY_INIT;
        setWindowOpacity(opacity);

        break;

    case Qt::Key_Less:
        opacity -= OPACITY_DELTA;
        if(opacity < 0) opacity = 0;
        setWindowOpacity(opacity);
        break;
    }
  update();
}


// render
void MainWindow::paintEvent(QPaintEvent *) {
    // 画像がなければ戻る
    if (curIndex==-1) return;

    // painter
    QPainter painter(this);
    QSize cntSize = contentsRect().size();
    QImage scaledImg = imgList[curIndex].data()->scaled(cntSize,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);

    int centerX = scaledImg.width() / 2 ;
    int centerY = scaledImg.height() / 2;

    QTransform transform;
    transform.translate(centerX,centerY);
    transform.scale(zoomFactor,zoomFactor);
    transform.translate(-centerX,-centerY);
    transform.translate(imgOffset.x(),imgOffset.y());


    painter.setTransform(transform);
    // 画像のレンダリング
    painter.drawImage(0,0,scaledImg);
}



void MainWindow::alwaysOnTop() {
    QSize size = this->size();
    if(flagTopHint)
    {
       // 通常
       setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
       hide();
       show();
       flagTopHint = false;
       resize(size);

    } else {
       //　最上面
       setWindowFlags(Qt::WindowStaysOnTopHint);
       show();
       flagTopHint = true;
       resize(size);
    }

}

// Event 処理 End ------------------------------

void MainWindow::pasteImage() {
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
        update();
}

void MainWindow::next() {
    curIndex ++;
    if (curIndex > maxIndex) curIndex = 0;
    update();
}

void MainWindow::back() {
    curIndex--;
    if(curIndex < 0) curIndex = maxIndex;
    update();
}

void MainWindow::escape() {
    close();
}

void MainWindow::save() {
    QString timeStamp = QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss");
    QString picturePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)
                          + "/" + timeStamp;
    // ディレクトリがなければディレクトリを作る
    QDir qDir;
    if (!qDir.exists(picturePath)) {
        if(!qDir.mkpath(picturePath)) {
            QMessageBox::critical(this,"エラー","ディレクトリが正しく作られませんでした");
            return;
        }
    }

    int index = 0;
    for(auto &img:imgList) {
        img.data()->save(picturePath +"/img_"+ QString::number(index) +".png");
        index++;
    }

    // クリップボードに保存パスを設定
     QClipboard *clip = QApplication::clipboard();
    clip->setText(picturePath);

    QMessageBox::information
        (this,"保存完了",picturePath + "に正しく画像を書き込めました\n保存後のパスはクリップボードに取得しました");
}


void MainWindow::hideTabToggle()
{
    int wndWidth = width();
    int wndHeight = height();
     if (flagFrameHide) {
         setWindowFlags(windowFlags() & ~Qt::FramelessWindowHint);
         flagFrameHide = false;
     } else {
         setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
         flagFrameHide = true;
     }
     show();

     resize(wndWidth,wndHeight);

}




