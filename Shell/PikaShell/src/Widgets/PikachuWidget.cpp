#include "Widgets/PikachuWidget.hpp"

#include <QTransform>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <Qt>
#include <QDebug>
#include <QMouseEvent>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

#include "Core/PikaCommandParser.hpp"
#include "Core/PikaApplicationManager.hpp"
#include "Core/PikaController.hpp"

PikachuWidget::PikachuWidget(PikaApplicationManager *appManager,
                             QWidget *parent)
    : QWidget(parent),
      m_appManager(appManager)
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

    m_originalPixmap.load(":/assets/pikachu.png");

    qDebug() << "Pixmap null:" << m_originalPixmap.isNull();
    qDebug() << m_originalPixmap.size();

    m_image->setPixmap(
        m_originalPixmap.scaled(
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

void PikachuWidget::setController(PikaController *controller)
{
    m_controller = controller;
}

void PikachuWidget::setFacingLeft(bool left)
{
    QPixmap pixmap = m_originalPixmap;

    if (left)
    {
        pixmap = pixmap.transformed(QTransform().scale(-1, 1));
    }

    m_image->setPixmap(
        pixmap.scaled(
            140,
            140,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        )
    );
}

void PikachuWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (m_controller)
        {
            m_controller->setState(PikachuState::Interacting);
        }

        showInteraction();
    }

    QWidget::mousePressEvent(event);
}

void PikachuWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging)
    {
        move(mapToParent(event->pos() - m_dragOffset));
    }

    QWidget::mouseMoveEvent(event);
}

void PikachuWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragging = false;

        if (m_controller)
        {
            m_controller->setHomePosition(pos());
        }

        if (m_controller)
        {
            m_controller->setState(PikachuState::Idle);
        }
    }

    QWidget::mouseReleaseEvent(event);
}

void PikachuWidget::showInteraction()
{
    auto *dialog = new QDialog(nullptr);

    dialog->setWindowTitle("Pikachu");
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    auto *layout = new QVBoxLayout(dialog);

    auto *label = new QLabel(
        "Hi! What can I do?",
        dialog
    );

    label->setAlignment(Qt::AlignCenter);

    auto *input = new QLineEdit(dialog);
    input->setPlaceholderText("Type a command...");

    auto *sendButton = new QPushButton("Send", dialog);

    layout->addWidget(label);
    layout->addWidget(input);
    layout->addWidget(sendButton);

    dialog->resize(300, 150);

    QPoint globalPos = mapToGlobal(QPoint(0, 0));

    dialog->move(
        globalPos.x() - 60,
        globalPos.y() - dialog->height() - 10
    );

    auto *parser = new PikaCommandParser(m_appManager);

    auto executeCommand = [parser, input, label]()
    {
        QString command = input->text().trimmed();

        if (command.isEmpty())
        {
            return;
        }

        PikaCommandResult result =
            parser->execute(command);

        label->setText(result.message);
        input->clear();
    };

    connect(
        sendButton,
        &QPushButton::clicked,
        dialog,
        executeCommand
    );

    connect(
        input,
        &QLineEdit::returnPressed,
        dialog,
        executeCommand
    );

    connect(
        dialog,
        &QDialog::finished,
        dialog,
        [parser]()
        {
            delete parser;
        }
    );

    dialog->show();
    input->setFocus();
}