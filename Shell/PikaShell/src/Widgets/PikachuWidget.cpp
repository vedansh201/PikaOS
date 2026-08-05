#include "Widgets/PikachuWidget.hpp"

#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <Qt>
#include <QDebug>


PikachuWidget::PikachuWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

void PikachuWidget::setupUi()
{
    auto *layout = new QVBoxLayout(this);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignCenter);

    m_image = new QLabel(this);
    m_image->setStyleSheet("background: transparent;");
    m_image->setAttribute(Qt::WA_TranslucentBackground);
    
    layout->setSpacing(0);

    setAutoFillBackground(false);
    m_image->setAutoFillBackground(false);
    
    QPixmap pixmap(":/assets/pikachu.png");

    qDebug() << "Pixmap null:" << pixmap.isNull();
    qDebug() << pixmap.size();

    m_image->setPixmap(
         pixmap.scaled(
             140,
             140,
             Qt::KeepAspectRatio,
             Qt::SmoothTransformation
         )
     );
    m_image->setAlignment(Qt::AlignCenter);

    layout->addWidget(m_image);

    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background: transparent;");

    setFixedSize(160, 160);
    setStyleSheet(R"(
         background: rgba(255,0,0,40);
         border: 2px solid yellow;
     )");
}
#include "Core/PikaController.hpp"

void PikachuWidget::setController(PikaController *controller)
{
    m_controller = controller;
}

void PikachuWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}

void PikachuWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
}

void PikachuWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
}
