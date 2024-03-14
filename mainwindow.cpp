#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QWindow>
#include <QBitmap>
#include <QClipboard>
#include <QPushButton>
#include <QToolButton>
#include <QSystemTrayIcon>
#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), m_opacityValue(1)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlag(Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowOpacity(m_opacityValue);

    ui->verticalLayout->setContentsMargins(0,0,0,0);
    auto statusBar = this->statusBar();
    if(statusBar)statusBar->setVisible(false);

    m_paintRec = false;
    m_isFinishShoot = false;
    m_toolBarHeight = 50;
    m_toolBar = new QWidget(this);
    m_toolBar->setWindowOpacity(1);
    QHBoxLayout *barLayout = new QHBoxLayout(m_toolBar);

    QSlider *opacitySlider = new QSlider(m_toolBar);
    opacitySlider->installEventFilter(this);
    opacitySlider->setMaximumWidth(100);
    opacitySlider->setOrientation(Qt::Horizontal);
    opacitySlider->setMaximum(100);
    opacitySlider->setMinimum(40);
    opacitySlider->setValue(m_opacityValue*100);
    connect(opacitySlider, &QAbstractSlider::valueChanged, this,[&](int value){
        m_opacityValue = value/100.0;
        setWindowOpacity(m_opacityValue);
        m_toolBar->setWindowOpacity(1);

    });

    //QByteArray pinIcon = QByteArray::fromBase64(QString("iVBORw0KGgoAAAANSUhEUgAAAEcAAABHCAIAAADa0s1IAAAACXBIWXMAABYlAAAWJQFJUiTwAAAA"
    //                                                       "EXRFWHRTb2Z0d2FyZQBTbmlwYXN0ZV0Xzt0AAAQuSURBVGiB7ZpdSypRFIZ3pwNOZFEkRBAUIoEl"
    //                                                       "IhNGSFLZB4UUUZYMWRB5G9QP6aounfEyqB+RUCR0oYhISY6VRfYFEmkRJJyLARk6Hd0fK04TPleS"
    //                                                       "e615H9Sde4017+/v6Mfx638H+BKqVtqhaqUdqlbaoWqlHQCsCoXC8fFxJpNhbwXFb8b6bDYbCASy"
    //                                                       "2SxCyOfzORwOiFSsML1WaiWEUDgchogEAL3VByWEkCzLsVgMIhUrlFZ/Kykkk0nmSADQWP1LCSF0"
    //                                                       "d3fHHAkAYqsySgih29tb5kgAkFmVV0II5XK53d1d5lSs1OCfhSsqlTCbzT6fr7m5mS0bPbhW+EoK"
    //                                                       "LS0tS0tLXV1dDNnowbIiVSohCILT6aQKxkRlK2olhZGRkbm5ObpaaipYvby8bG5uUisp9PT0eL1e"
    //                                                       "g8HA0oSICnvg2dkZoxJCKJFIbG9vn5ycMPbBp4IV1D52f3+/tbUVCoVAulWk8ufq6OgoHA7Lsgxy"
    //                                                       "veHh4fn5eVmWU6mULMuyLBeLRY7jOI7T6XQcx9ntdovF0tTUxHIV3J09Foul0+l8Pp/P5+PxOMsl"
    //                                                       "cbBarWazmef5hoYGinKC/8Il9vf39/b2KC5GisFgcLlcQ0NDpIU0VgihnZ2dg4MDikIKjEbjxsZG"
    //                                                       "bW0tfgnlSUQQBJPJRFdLSjqdXltbe35+xi+hPzUKglBfX09dTkogEMBfTG/V1ta2uLhIXU6KxWLB"
    //                                                       "X8w0t7DZbNPT0ywdMOnt7R0bG8Nfzzo5m5iY6OvrY2xSHrvdvrq6SlQCMA/0eDzt7e3sfT6lv79/"
    //                                                       "ZWWFtArASq/XLywsEO28mDgcjuXlZYpCmIm0yWTyer0grUo4nU6fz0dXCzZnHxgYcLlcUN0GBwcF"
    //                                                       "QaAuh7x74PF4QPq4XC7GV551zq7m+vqavcno6Ojs7CxjE0iry8tLxg7j4+MzMzPsSSCtGEeck5OT"
    //                                                       "U1NTIEm+i5Xb7Xa73VBJvsu9xmg0CjjNhrTiOI669ubmJhgMPjw8gCSBtKqrqyMt4Xme53nl8dXV"
    //                                                       "lSiKj4+P7EkgrXQ6HdF6nuf9fr/f7y+JZTKZYDCYy+UYk/y3d6CipDxWi52fn4ui+PT0xJIE0gr/"
    //                                                       "YKdWUlCLpdNpURSJjvQfgLTq6OhobGysuOxvJQW1WCqVkiSpUCjQJQHe2bu7u8sv+JeSglosmUyK"
    //                                                       "ovj6+koRA9jKaDSWeba8koJa7PT0VJKkt7c30hjAVjabrbW19dOncJQU1GKJREKSJNKhJbCVXq//"
    //                                                       "dGyCr6SgFovH46IoFotF/HL4b0wOh8Nqtar/QqqkoBaLxWKSJOHXfsn3QPXxkU5JQS0WjUYPDw8x"
    //                                                       "Cynn7BW5uLgIhUKdnZ0Us/8PBAKBSCSCEFpfX8e8ff5VVrBEIhG9Xo//iwBtWJHyXc5XsFSttEPV"
    //                                                       "SjtUrbRD1Uo7VK20w8+0+gP5/K91qubSCAAAAABJRU5ErkJggg==").toUtf8());
    QString iconStr = "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"24\" height=\"24\" viewBox=\"0 0 24 24\""
                      " fill=\"none\" stroke=\"currentColor\" stroke-width=\"2\" stroke-linecap=\"round\" "
                      "stroke-linejoin=\"round\" class=\"feather feather-pen-tool\"><path d=\"M12 19l7-7 3 3-7 7-3-3z"
                      "\"></path><path d=\"M18 13l-1.5-7.5L2 2l3.5 14.5L13 18l5-5z\"></path><path d=\""
                      "M2 2l7.586 7.586\"></path><circle cx=\"11\" cy=\"11\" r=\"2\"></circle></svg>";
    QPixmap pix;
    pix.loadFromData(iconStr.toUtf8(),"SVG");
    QIcon icon1(pix);

    m_drawLineBtn = new QToolButton(m_toolBar);
    m_drawLineBtn->setCheckable(true);
    m_drawLineBtn->setIcon(icon1);


    iconStr="<svg role=\"img\" viewBox=\"0 0 24 24\" xmlns=\"http://www.w3.org/2000/svg\">"
              "<title>Pinboard</title><path d=\"M13.352 14.585l-4.509 4.614.72-4.062L3.428 7.57 0 "
              "7.753 7.58 0v2.953l7.214 6.646 4.513-1.105-4.689 4.982L24 24l-10.648-9.415z\"/></svg>";

    pix.loadFromData(iconStr.toUtf8(),"SVG");
    icon1 = (pix);
        auto alwaysOnBtn = new QToolButton(m_toolBar);
    alwaysOnBtn->setCheckable(true);
    auto tempAct = new QAction();
    tempAct->setCheckable(true);
    tempAct->setIcon(icon1);
    alwaysOnBtn->setDefaultAction(tempAct);
    connect(tempAct, &QAction::triggered,this,[&, alwaysOnBtn](){
        if(alwaysOnBtn->isChecked())
        {
            setWindowFlag(Qt::WindowStaysOnTopHint, true);
            show();
        }
        else{

            setWindowFlag(Qt::WindowStaysOnTopHint, false);
            show();

        }
    });

    iconStr="<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"24\" height=\"24\" viewBox=\"0 0 24 24\">"
                        "<path d=\"M17.133 16.202l1.596.923c-1.508 2.055-3.606 3.548-5.729 3.875v-12.102c0-2.39 2-2.619 "
                        "2-4.898 0-1.654-1.346-3-3-3s-3 1.346-3 3c0 2.297 2 2.487 2 4.898v12.102c-2.123-.327-4.221-1.799-"
                        "5.729-3.854l1.596-.944-4.867-2.811v5.621l1.5-.908c2.178 3.077 5.203 4.896 8.5 4.896s6.282-1.798"
                        " 8.458-4.875l1.542.887v-5.621l-4.867 2.811zm-6.134-12.202c0-.551.449-1 1.001-1s1 .449 1 1-.449 "
                        "1-1 1-1.001-.449-1.001-1z\"/></svg>";

    pix.loadFromData(iconStr.toUtf8(),"SVG");
    icon1 = (pix);
    //auto hookF1Btn = new QToolButton(m_toolBar);
    alwaysOnBtn->setCheckable(true);
    tempAct = new QAction();
    tempAct->setCheckable(true);
    tempAct->setIcon(icon1);
    //hookF1Btn->setDefaultAction(tempAct);
    connect(tempAct, &QAction::triggered,this,[&](){
        this->magHook();
    });


    barLayout->addStretch();
    barLayout->addWidget(m_drawLineBtn);
    barLayout->addWidget(alwaysOnBtn);
    //barLayout->addWidget(hookF1Btn);
    barLayout->addWidget(opacitySlider);

    //barLayout->addWidget(new QPushButton("TTTTTT"));

    m_toolBar->setLayout(barLayout);
    ui->verticalLayout->addStretch();
    ui->verticalLayout->addWidget(m_toolBar);
    //m_toolBar->hide();


    iconStr = "<svg role=\"img\" viewBox=\"0 0 24 24\" xmlns=\"http://www.w3.org/2000/svg\"><title>Packer</title>"
              "<path d=\"M7.844 0v3.38l5.75 3.32v10.148l2.705 1.552c1.676.967 3.045.388 3.045-1.285V9.668c-.014-1."
              "687-1.382-3.832-3.059-4.799L7.844 0zM4.656 2.932v16.574L12.436 24V7.426l-7.78-4.494Z\"/></svg>";
    QSystemTrayIcon *sysTray = new QSystemTrayIcon();
    pix.loadFromData(iconStr.toUtf8(),"SVG");
    icon1 = pix;
    sysTray->setIcon(icon1);
    sysTray->show();

    connect(sysTray, &QSystemTrayIcon::activated, this, [&](QSystemTrayIcon::ActivationReason reason){
        switch(reason)
        {
        case QSystemTrayIcon::DoubleClick:
        {
            this->startShoot();
        }
        case QSystemTrayIcon::Unknown:
        case QSystemTrayIcon::Context:
        {

            m_menu->exec(QCursor::pos());
        }
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::MiddleClick:
            break;
        }
    });

    m_menu = new QMenu(this);

    QAction *act;
    act = new QAction("Copy");
    connect(act, &QAction::triggered, this, [&](){
        QClipboard *clipBoard = QGuiApplication::clipboard();
        clipBoard->setPixmap(this->grab());
    });
    m_menu->addAction(act);

    //act = new QAction("ToolBar");
    //act->setCheckable(true);
    //connect(act, &QAction::triggered, this, [&, opacitySlider](bool checked){
    //    if(checked)
    //    {
    //        opacitySlider->setValue(m_opacityValue*100);
    //        m_toolBar->show();
    //        //m_toolBar->move(this->rect().width() - opacitySlider->width()*2, this->rect().height()- opacitySlider->height()*2);
    //    }
    //    else
    //        m_toolBar->hide();
    //});
    //m_menu->addAction(act);

    act = new QAction("exit");
    connect(act, &QAction::triggered, this, [&](){
        QApplication::exit();
    });
    m_menu->addAction(act);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::shootScreen()
{
    m_isFinishShoot = true;
    m_stopDrawBlue = true;
    QScreen *screen = QGuiApplication::screenAt(QCursor::pos());
    if (const QWindow *window = windowHandle())
        screen = window->screen();
    if (!screen)
        return;

    auto rect = QRectF(m_startPos,m_endPos);
    auto pixMap = screen->grabWindow(0, rect.x(),rect.y(),rect.width(),rect.height());
    m_originPix = pixMap;
    this->resize(rect.width(), rect.height()+m_toolBarHeight);
    setWindowFlag(Qt::WindowStaysOnTopHint, true);
    show();
}

void MainWindow::startShoot()
{
    //m_opacitySlider->setValue(100);
    m_isFinishShoot = false;
    m_stopDrawBlue = true;
    m_lines.clear();
    m_originPix = QPixmap();
    m_opacityValue = 1;

    m_startPos = QPointF(0,0);
    m_endPos = QPointF(0,0);
    QScreen *screen = QGuiApplication::screenAt(QCursor::pos());
    //auto screens = QGuiApplication::screens();
    auto rect =screen->geometry();
    this->move(rect.x(), rect.y());
    //this->showMaximized();
    //setWindowFlag(Qt::WindowStaysOnTopHint, true);
    ShowWindow((HWND)winId(), 1);
    show();
    setFocus();



    this->resize(rect.size());
    //setWindowFlag(Qt::WindowStaysOnTopHint, false);
    //show();
}

void MainWindow::stopShoot()
{
    m_stopDrawBlue = true;
    //setWindowFlag(Qt::WindowStaysOnTopHint, false);
    this->showMinimized();
    m_moved = false;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QMainWindow::mousePressEvent(event);
    m_stopDrawBlue = false;
    m_paintRec = true;
    m_moved = false;
    if (event->button() == Qt::LeftButton) {
        m_startPos = event->pos();
        m_moveStartPos = frameGeometry().topLeft() - event->globalPos();
        if(m_isFinishShoot && m_drawLineBtn->isChecked())
        {
            m_startPos = event->pos();
        }
    }
    setFocus();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_paintRec = false;
    QMainWindow::mouseReleaseEvent(event);
    if(!m_moved && !m_isFinishShoot)
    {
        stopShoot();
        this->update();
    }
    if(!m_isFinishShoot)
    {
        shootScreen();
        auto pos = this->mapToGlobal(m_startPos.toPoint());
        this->move(pos.x(), pos.y());
        this->update();

    }else if(event->button() == Qt::LeftButton)
    {
        if(m_drawLineBtn->isChecked())
        {
            m_endPos = event->pos();
            m_lines.append(QLineF(m_startPos, m_endPos));
            this->update();
        }
    }
    m_moved = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

    m_moved= true;
    if(m_isFinishShoot && !m_drawLineBtn->isChecked())
    {
        auto pos = event->globalPos() + m_moveStartPos;
        this->move(pos.x(), pos.y());
    }

    else{
        m_endPos = event->pos();
        this->update();
    }
    QMainWindow::mouseMoveEvent(event);

}

#define getPainter()\
auto painter = QPainter(this); painter.setRenderHints(QPainter::Antialiasing, true);\
QPen pen; pen.setWidth(4); pen.setColor(Qt::red);\
painter.setPen(pen);


void MainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    if(m_isFinishShoot)
    {
        getPainter();

        QSize size(this->size().width(),this->size().height() - m_toolBarHeight);
        auto pix = m_originPix.scaled(size);
        painter.drawPixmap(0,0, pix);
        painter.drawLines(m_lines);
    }
    else //if(m_stopDrawBlue)
    {
        getPainter();
        painter.fillRect(0,0, this->rect().width(), m_startPos.y(), QColor(51, 204, 255, 0x70));
        painter.fillRect(0, m_endPos.y(), this->rect().width(), this->rect().height() - m_endPos.y(), QColor(51, 204, 255, 0x70));
        painter.fillRect(0, m_startPos.y(), m_startPos.x(),m_endPos.y()- m_startPos.y(), QColor(51, 204, 255, 0x70));
        painter.fillRect(m_endPos.x(), m_startPos.y(), this->rect().width() - m_endPos.x() ,m_endPos.y()- m_startPos.y(), QColor(51, 204, 255, 0x70));


        //painter.fillRect(this->rect(), QColor(51, 204, 255, 0x70));
    }

    if(m_paintRec)
    {
        getPainter();

        if(!m_isFinishShoot)
        {
            if(m_moved)
            painter.drawRect(QRectF(m_startPos,m_endPos));
        }
        else if(m_drawLineBtn->isChecked())
        {
            //draw line when mouse move
            painter.drawLine(QLineF(m_startPos,m_endPos));
        }
    }




}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QMainWindow::keyPressEvent(event);
    if(event->key() == Qt::Key_Escape)
        this->stopShoot();
    if(event->key() == Qt::Key_Left)
        this->move(this->pos() + QPoint(-1,0));
    if(event->key() == Qt::Key_Right)
        this->move(this->pos() + QPoint(1,0));
    if(event->key() == Qt::Key_Up)
        this->move(this->pos() + QPoint(0,-1));
    if(event->key() == Qt::Key_Down)
        this->move(this->pos() + QPoint(0,1));
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Z)
    {
        m_lines.pop_back();
        update();
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{

    if(m_isFinishShoot)
    {
        auto pixelDelta = event->angleDelta();
        auto rect = this->rect();
        auto widthStep = rect.width()/15;
        auto heighStep = rect.height()/15;
        QSize newSize = this->size();
        if (pixelDelta.y() > 0) {
            newSize = QSize(rect.width() + widthStep, rect.height() + heighStep );

        } else if (pixelDelta.y() < 0) {
            if(rect.width() >200)
            newSize = QSize(rect.width() - widthStep, rect.height() - heighStep );
        }
        this->resize(newSize);
    }
    QWidget::wheelEvent(event);
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if(!m_isFinishShoot)
        return;


    m_menu->exec(event->globalPos());
}







