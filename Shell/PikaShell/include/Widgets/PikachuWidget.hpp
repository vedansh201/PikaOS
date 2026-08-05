#pragma once

#include <QWidget>
#include <QPoint>

class QLabel;
class QMouseEvent;
class PikaController;

class PikachuWidget : public QWidget
{

public:
    explicit PikachuWidget(QWidget *parent = nullptr);

    void setController(PikaController *controller);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void setupUi();

    QLabel *m_image = nullptr;

    PikaController *m_controller = nullptr;

    bool m_dragging = false;
    QPoint m_dragOffset;
};