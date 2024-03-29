#include "popup.h"
#include <QPainter>
#include <QGuiApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QMediaPlayer>
#include <QDir>
#include <QUrl>
#include <QDebug>

PopUp::PopUp(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Tool |
                   Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    animation.setTargetObject(this);
    animation.setPropertyName("popupOpacity");
    connect(&animation, &QAbstractAnimation::finished, this, &PopUp::hide);



    label.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label.setStyleSheet("QLabel { color : white; "
                        "margin-top: 12px;"
                        "margin-bottom: 12px;"
                        "margin-left: 10px;"
                        "margin-right: 10px; }");


    layout.addWidget(&label, 0, 0);
    setLayout(&layout);


    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &PopUp::hideAnimation);
}

void PopUp::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect roundedRect;
    roundedRect.setX(rect().x() + 5);
    roundedRect.setY(rect().y() + 5);
    roundedRect.setWidth(rect().width() - 10);
    roundedRect.setHeight(rect().height() - 10);

    painter.setBrush(QBrush(QColor(0,0,0,180)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(roundedRect, 10, 10);
}

void PopUp::setPopupText(const QString &text)
{
    label.setText(text);
    adjustSize();
}

void PopUp::show()
{
    setWindowOpacity(0.0);

    animation.setDuration(150);
    animation.setStartValue(0.0);
    animation.setEndValue(1.0);

    setGeometry(QGuiApplication::screens().first()->geometry().width() - 530 - width() + QGuiApplication::screens().first()->geometry().x(),
                QGuiApplication::screens().first()->geometry().height() - 700 - height() + QGuiApplication::screens().first()->geometry().y(),
                width(),
                height());
    QWidget::show();
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(QDir::toNativeSeparators("message.mp3")));
    player->setVolume(100);
    player->play();
    animation.start();
    timer->start(3000);
}

void PopUp::hideAnimation()
{
    timer->stop();
    animation.setDuration(1000);
    animation.setStartValue(1.0);
    animation.setEndValue(0.0);
    animation.start();
}

void PopUp::hide()
{
    if(getPopupOpacity() == 0.0){
        QWidget::hide();
    }
}

void PopUp::setPopupOpacity(double opacity)
{
    popupOpacity = opacity;

    setWindowOpacity(opacity);
}

double PopUp::getPopupOpacity() const
{
    return popupOpacity;
}
