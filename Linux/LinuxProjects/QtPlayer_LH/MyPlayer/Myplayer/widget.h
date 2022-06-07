#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>
#include <QKeyEvent>
#include <QDebug>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void addToPlaylist(const QStringList& fileNames);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

private:
    Ui::Widget *ui;

    QMediaPlayer *player;
    QMediaPlaylist *Playlist;
    QVideoWidget *videoWidget;
    QMediaPlayer::State  m_playerState;

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // WIDGET_H
