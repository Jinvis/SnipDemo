#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QPixmap>
#include <QSlider>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class QToolButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void (*magHook)();

public Q_SLOTS:
    void shootScreen();
    void startShoot();
    void stopShoot();
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    QPainter createPainter();

private:
    Ui::MainWindow *ui;
    QPointF m_startPos;
    QPointF m_endPos;
    QPointF m_moveStartPos;
    QPixmap m_originPix;
    QSlider *m_opacitySlider;
    QMenu *m_menu;
    int m_toolBarHeight;
    QWidget *m_toolBar;
    bool m_moved;
    bool m_stopDrawBlue;

    QToolButton *m_drawLineBtn;

    QVector<QLineF> m_lines;



    bool m_paintRec;
    bool m_isFinishShoot;
    float m_opacityValue;

};



#endif // MAINWINDOW_H
