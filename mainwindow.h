#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QStatusBar>
#include <QColorDialog>

#include <QDebug>

#include "renderarea.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindow *ui;

    // Кнопки на панели инструментов
    QPushButton *selectionTool;
    QPushButton *polylineTool;
    QPushButton *deleteTool;

    // Палитра
    QLabel *paletteText;
    QPushButton *paletteColor;

    // Толщина линии
    QLabel *widthText;
    QSpinBox *widthSpinBox;

    // Слои для расположения элементов
    QGridLayout *toolsLayout;
    QHBoxLayout *windowLayout;

    // Отрисовка графической сцены
    RenderArea *renderScene;
    QGraphicsView *renderView;

    // Статус бар
    QLabel *statusBarLabel;

    void buildMenu();
    void createActions();

signals:
    void renderAreaClicked(qreal xpos, qreal ypos);
    void activeToolChanged(RenderArea::Tool tool);

public slots:
    void updateStatusBar(const QString &message);
    void saveSvg();
    void loadSvg();
};

#endif // MAINWINDOW_H
