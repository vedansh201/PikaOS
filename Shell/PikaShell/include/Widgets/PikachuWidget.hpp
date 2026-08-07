#pragma once
#include <QPixmap>
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
    void setFacingLeft(bool left);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void setupUi();

    QLabel *m_image = nullptr;

    QPixmap m_originalPixmap;

    PikaController *m_controller = nullptr;

    bool m_dragging = false;
    QPoint m_dragOffset;
};