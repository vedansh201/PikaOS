#pragma once

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>

class QLabel;

class PikachuWidget : public QWidget
{
public:
    explicit PikachuWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void setupUi();

    QLabel *m_image;

    bool m_dragging = false;
    QPoint m_dragOffset;
};
