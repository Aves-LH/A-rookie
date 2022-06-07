#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    player = new QMediaPlayer;
    Playlist = new QMediaPlaylist();

    player->setPlaylist(Playlist);

    videoWidget = new QVideoWidget;

    player->setVideoOutput(videoWidget);

    ui->verticalLayout->addWidget(videoWidget);



    videoWidget->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Window,QColor(39,39,39));
    videoWidget->setPalette(palette);

    //进度条
    connect(player,&QMediaPlayer::durationChanged,
            [=](quint64 duration)
    {
            ui->horizontalSlider->setRange(0,duration);

    });

    connect(player,&QMediaPlayer::positionChanged,
            [=](quint64 position)
    {
            ui->horizontalSlider->setValue(position);
    });

    //全屏转化
    connect(videoWidget,&QVideoWidget::fullScreenChanged,
            [=](bool flag)
    {
        qDebug()<<"is full"<<flag;
    });
}

Widget::~Widget()
{
    delete player;
    delete Playlist;
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("选择视频文件"));
    qDebug()<<fileNames;
    addToPlaylist(fileNames);
}

void Widget::addToPlaylist(const QStringList& fileNames)
{
    foreach (QString const &argument, fileNames)
    {
        QFileInfo fileInfo(argument);
        if (fileInfo.exists())
        {
             QUrl url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
             if (fileInfo.suffix().toLower() == QLatin1String("m3u"))
                 Playlist->load(url);
             else
                 Playlist->addMedia(url);
        } else
        {
             QUrl url(argument);
             if (url.isValid())
                 Playlist->addMedia(url);
        }
    }
}

void Widget::on_pushButton_2_clicked()
{
    ui->horizontalSlider_2->setValue(50);
    player->setVolume(50);
    m_playerState = QMediaPlayer::PlayingState;
    player->play();
}

void Widget::on_pushButton_3_clicked()
{
     m_playerState = QMediaPlayer::PausedState;
     player->pause();
}

void Widget::on_pushButton_4_clicked()
{
    if(m_playerState == QMediaPlayer::PlayingState)
    {  

    }
    else
    {

    }
}


void Widget::on_horizontalSlider_sliderMoved(int position)
{
    player->setPosition(position);
}

void Widget::on_horizontalSlider_2_sliderMoved(int position)
{
    player->setVolume(position);
}

//双击鼠标事件
void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{
    videoWidget->setFullScreen(!isFullScreen());
    event->accept();
    qDebug()<<"mouse event";
}
