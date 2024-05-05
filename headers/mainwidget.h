// MainWidget.h
#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

#include "openglWindowWidget.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
private:
    QLabel *openNDPLabel;
    QLabel *saveNDPLabel;
    QLabel *deleteLabel;
    QLabel *clearLabel;
    QLabel *switchLabel;
    QLabel *parseLabel;
    QLabel *analyseLabel;
    QLabel *compareNDPLabel;

    QPushButton *openNDPButton;
    QPushButton *saveNDPButton;
    QPushButton* deleteButton;
    QPushButton* clearButton;
    QPushButton* switchButton;
    QPushButton* parseButton;
    QPushButton* analyseButton;
    QPushButton* compareNDPButton;
    OpenGLWindowWidget *openGLWidget;
};

#endif // MAINWIDGET_H
