/****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** Commercial Usage
 ** Licensees holding valid Qt Commercial licenses may use this file in
 ** accordance with the Qt Commercial License Agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Nokia.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Nokia gives you certain additional
 ** rights.  These rights are described in the Nokia Qt LGPL Exception
 ** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 3.0 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU General Public License version 3.0 requirements will be
 ** met: http://www.gnu.org/copyleft/gpl.html.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Nokia at qt-info@nokia.com.
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 #include "imagewidget.h"

 #include <QtGui>
 #include <QGestureRecognizer>

 ImageWidget::ImageWidget(QWidget *parent)
     : QWidget(parent),
     position(0),
     horizontalOffset(0),
     verticalOffset(0),
     rotationAngle(0),
     scaleFactor(1),
     currentStepScaleFactor(1)

 {
     setMinimumSize(QSize(1200,800));

     grabGesture(Qt::PanGesture);
     grabGesture(Qt::PinchGesture);
     grabGesture(Qt::SwipeGesture);


     currentImage = loadImage("C:/Qt/Qt5.3.2/Tools/QtCreator/bin/build-PinchTest2-Desktop_Qt_5_3_MinGW_32bit-Debug/resources/cameraimage/WS000028.JPG");
     arrowImage   = loadImage("C:/Qt/Qt5.3.2/Tools/QtCreator/bin/build-PinchTest2-Desktop_Qt_5_3_MinGW_32bit-Debug/resources/arrow.png");
     azimuthImage = loadImage("C:/Qt/Qt5.3.2/Tools/QtCreator/bin/build-PinchTest2-Desktop_Qt_5_3_MinGW_32bit-Debug/resources/azimuth.png");
     rangeImage   = loadImage("C:/Qt/Qt5.3.2/Tools/QtCreator/bin/build-PinchTest2-Desktop_Qt_5_3_MinGW_32bit-Debug/resources/range.png");

     cmdDirectionX = 0;
     cmdDirectionY = 0;
     counter_image = 28;

     initialX      = 0;
     initialY      = 0;
     rotationAngle = -135;

     scaleFactor   = 1.3;

     modeTarget    = 0;
     posiTargetX   = 0;
     posiTargetY   = 0;

     QTimer *timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(updateImage()));
     timer->start(100);
 }

 ImageWidget::~ImageWidget()
 {
     QGestureRecognizer::unregisterRecognizer(m_gestureId);
 }

 bool ImageWidget::event(QEvent *event)
 {
     if (event->type() == QEvent::Gesture)
         return gestureEvent(static_cast<QGestureEvent*>(event));
     return QWidget::event(event);
 }

 void ImageWidget::paintEvent(QPaintEvent*)
 {
     QPainter p(this);

     float iw = currentImage.width();
     float ih = currentImage.height();
     float wh = height();
     float ww = width();

     p.save();
     p.translate(ww/2, wh/2);
     p.translate(100, 0);
     p.scale(currentStepScaleFactor * scaleFactor, currentStepScaleFactor * scaleFactor);
     p.translate(-iw/2, -ih/2);
     p.drawImage(0, 0, currentImage);
     p.restore();

     if(modeTarget==1){
         drawTargetArea(&p);
     }
     else{
         // DO NOTHING
     }
     drawStatus(&p);
     displayArrow(&p);
     displayAzimuth(&p);
 }

 void ImageWidget::nextAnimationFrame()
 {
     update();
 }

 void ImageWidget::mousePressEvent(QMouseEvent *e)
 {
     initialX = (float)e->pos().rx();
     initialY = (float)e->pos().ry();
 }

 void ImageWidget::mouseReleaseEvent(QMouseEvent *e)
 {
     cmdDirectionX = 0;
     cmdDirectionY = 0;
 }

 void ImageWidget::mouseMoveEvent(QMouseEvent *e)
 {

     if(modeTarget == 0){
         cmdDirectionX = ((float)e->pos().rx()-initialX)/200.0;
         cmdDirectionY = ((float)e->pos().ry()-initialY)/200.0;
         rotationAngle = rotationAngle + ((float)e->pos().rx()-initialX)/50;
     }
     else{
         posiTargetX = (float)e->pos().rx();
         posiTargetY = (float)e->pos().ry();
     }

     update();

     initialX = (float)e->pos().rx();
     initialY = (float)e->pos().ry();
 }

 void ImageWidget::mouseDoubleClickEvent(QMouseEvent *)
 {
     /*
     rotationAngle = 0;
     scaleFactor = 1;
     currentStepScaleFactor = 1;
     verticalOffset = 0;
     horizontalOffset = 0;
     update();
     */

     if(modeTarget == 0){
         modeTarget = 1;
     }
     else{
         modeTarget = 0;
     }

     //qDebug(" mode %d", modeTarget);
 }

 bool ImageWidget::gestureEvent(QGestureEvent *event)
 {
     if (QGesture *swipe = event->gesture(Qt::SwipeGesture)){
         //swipeTriggered(static_cast<QSwipeGesture *>(swipe));
         //qDebug(" swipeTriggered ");
     }
     else if (QGesture *pan = event->gesture(Qt::PanGesture)){
         panTriggered(static_cast<QPanGesture *>(pan));
         //qDebug(" panTriggered ");
     }

     if (QGesture *pinch = event->gesture(Qt::PinchGesture)){
         pinchTriggered(static_cast<QPinchGesture *>(pinch));
         //qDebug(" pinchTriggered ");
     }

     return true;
 }

 void ImageWidget::panTriggered(QPanGesture *gesture)
 {
 #ifndef QT_NO_CURSOR
     switch (gesture->state()) {
         case Qt::GestureStarted:
             //qDebug(" panTriggered started ");
         case Qt::GestureUpdated:
             setCursor(Qt::SizeAllCursor);
             //qDebug(" panTriggered updated ");
             break;
         default:
             setCursor(Qt::ArrowCursor);
     }
 #endif
     QPointF delta = gesture->delta();
     horizontalOffset += delta.x();
     verticalOffset += delta.y();

     update();
 }

 void ImageWidget::pinchTriggered(QPinchGesture *gesture)
 {
     QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
     //if (changeFlags & QPinchGesture::RotationAngleChanged) {
     //    qreal value = gesture->property("rotationAngle").toReal();
     //    qreal lastValue = gesture->property("lastRotationAngle").toReal();
     //    rotationAngle += value - lastValue;
     //}
     if (changeFlags & QPinchGesture::ScaleFactorChanged) {
         qreal value = gesture->property("totalScaleFactor").toReal();
         currentStepScaleFactor = value;
         //qDebug(" value %f currentStepScaleFactor %f", value , currentStepScaleFactor);
     }
     if (gesture->state() == Qt::GestureFinished) {
         scaleFactor *= currentStepScaleFactor;
         currentStepScaleFactor = 1;
         //qDebug(" scaleFactorChanged step %f scaleFactor %f ", currentStepScaleFactor, scaleFactor);
     }
     update();
 }

 void ImageWidget::resizeEvent(QResizeEvent*)
 {
     update();
 }

 void ImageWidget::openDirectory(const QString &path)
 {
     this->path = path;
     QDir dir(path);
     QStringList nameFilters;
     nameFilters << "*.jpg" << "*.png";
     files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);

     position = 0;
     goToImage(0);
     update();
 }

 QImage ImageWidget::loadImage(const QString &fileName)
 {
     QImageReader reader(fileName);
     if (!reader.canRead()) {
         qDebug() << fileName << ": can't load image";
         return QImage();
     }

     QImage image;
     if (!reader.read(&image)) {
         qDebug() << fileName << ": corrupted image";
         return QImage();
     }
     return image;
 }

 void ImageWidget::goNextImage()
 {
     if (files.isEmpty())
         return;

     if (position < files.size()-1) {
         ++position;
         prevImage = currentImage;
         currentImage = nextImage;
         if (position+1 < files.size())
             nextImage = loadImage(path+QLatin1String("/")+files.at(position+1));
         else
             nextImage = QImage();
     }
     update();
 }

 void ImageWidget::goPrevImage()
 {
     if (files.isEmpty())
         return;

     if (position > 0) {
         --position;
         nextImage = currentImage;
         currentImage = prevImage;
         if (position > 0)
             prevImage = loadImage(path+QLatin1String("/")+files.at(position-1));
         else
             prevImage = QImage();
     }
     update();
 }

 void ImageWidget::goToImage(int index)
 {
     if (files.isEmpty())
         return;

     if (index < 0 || index >= files.size()) {
         qDebug() << "goToImage: invalid index: " << index;
         return;
     }

     if (index == position+1) {
         goNextImage();
         return;
     }

     if (position > 0 && index == position-1) {
         goPrevImage();
         return;
     }

     position = index;

     if (index > 0)
         prevImage = loadImage(path+QLatin1String("/")+files.at(position-1));
     else
         prevImage = QImage();
     currentImage = loadImage(path+QLatin1String("/")+files.at(position));
     if (position+1 < files.size())
         nextImage = loadImage(path+QLatin1String("/")+files.at(position+1));
     else
         nextImage = QImage();
     update();
 }

 void ImageWidget::updateImage()
 {
     QString path_image;

     path_image = "C:/Qt/Qt5.3.2/Tools/QtCreator/bin/build-PinchTest2-Desktop_Qt_5_3_MinGW_32bit-Debug/resources/cameraimage/WS0000"
                  + QString::number(counter_image) + ".JPG";
     currentImage = loadImage(path_image);

     counter_image++;
     if(counter_image>86){
         counter_image = 28;
     }
     else{
         //DO NOTHING
     }

     update();
 }

 void ImageWidget::drawStatus(QPainter *painter)
 {
     QString text;

     QDateTime dateTime = QDateTime::currentDateTimeUtc();

     painter->save();
     painter->setPen(Qt::darkGray);
     painter->setBrush(Qt::SolidPattern);
     painter->setBrush(Qt::black);
     painter->drawRect(0, 0, 300, height());
     painter->restore();

     //-------------------//
     //----- 基本情報 -----//
     //-------------------//
     painter->save();
     painter->setPen(Qt::lightGray);
     painter->setFont(QFont("Arial", 12));
     painter->drawText(rect(), Qt::AlignLeft, "\n  プロトタイプ");

     text = "\n\n\n  日付 : " + dateTime.date().toString("yyyy/MM/dd");
     painter->drawText(rect(), Qt::AlignLeft, text);

     text = "\n\n\n\n  時刻 : " + dateTime.time().toString("hh:mm:ss UTC");
     painter->drawText(rect(), Qt::AlignLeft, text);
     painter->restore();


     //-------------------//
     //--- 操作指令情報 ----//
     //-------------------//
     painter->save();
     painter->setPen(Qt::lightGray);
     painter->setFont(QFont("Arial", 12));
     painter->translate(0, 200);

     text = "  操作指令";
     painter->drawText(rect(), Qt::AlignLeft, text);

     text = "\n\n  パン     : " + QString::number(rotationAngle + 135, 'g', 3) + " [deg]";
     painter->drawText(rect(), Qt::AlignLeft, text);
     text = "\n\n\n  チルト  : " + QString::number(cmdDirectionY);
     painter->drawText(rect(), Qt::AlignLeft, text);
     text = "\n\n\n\n  ズーム : " + QString::number((double)scaleFactor, 0, 2) + "  [倍]";
     painter->drawText(rect(), Qt::AlignLeft, text);

     text = "\n\n\n\n\n\n  モード : ";
     if(modeTarget==0){
         text = text + "パン・チルト操作";
     }
     else{
         text = text + "目標選択";
     }
     painter->drawText(rect(), Qt::AlignLeft, text);

     painter->restore();


 }


 void ImageWidget::drawTargetArea(QPainter* painter)
 {
     painter->save();
     painter->setPen(QPen(Qt::red, 2));
     painter->drawRect(posiTargetX, posiTargetY, 100, 100);
     painter->restore();
 }

 void ImageWidget::displayArrow(QPainter* painter)
 {
     if(cmdDirectionX < 0){// draw a right arrow
         painter->save();
         painter->translate(width()-arrowImage.width(), height()/2 - arrowImage.height()/2);
         painter->drawImage(0, 0, arrowImage);
         painter->restore();
     }
     else if(cmdDirectionX > 0){// draw a left arrow
         painter->save();
         painter->translate(arrowImage.width() + 300, height()/2 + arrowImage.height()/2);
         painter->rotate(180);
         painter->drawImage(0, 0, arrowImage);
         painter->restore();
     }
     else{ // no arrow
         //DO NOTHING
     }

     if(cmdDirectionY < 0){// draw a up arrow
         painter->save();
         painter->translate(width()/2 - arrowImage.height()/2 + 100, arrowImage.width());
         painter->rotate(-90);
         painter->drawImage(0, 0, arrowImage);
         painter->restore();
     }
     else if(cmdDirectionY > 0){// draw a down arrow
         painter->save();
         painter->translate(width()/2 + arrowImage.height()/2 + 100, height() - arrowImage.width());
         painter->rotate(90);
         painter->drawImage(0, 0, arrowImage);
         painter->restore();
     }
     else{ // no arrow
         //DO NOTHING
     }
 }

 void ImageWidget::displayAzimuth(QPainter* painter)
 {
     painter->save();
     painter->translate(0, this->height() - 300);
     painter->scale(0.415, 0.415);
     painter->drawImage(0, 0, azimuthImage);
     painter->restore();

     painter->save();
     painter->translate(azimuthImage.width()/4.8, this->height() - 300 + azimuthImage.height()/4.75);
     painter->scale(0.415, 0.415);
     painter->rotate(rotationAngle);
     painter->drawImage(0, 0, rangeImage);
     painter->restore();
 }

