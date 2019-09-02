#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QSvgGenerator>

#include "svgreader.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    buildMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Слот для обновления сообщения в статус баре. Используется для вывода
 * координат курсора на рабочей области
 * @param string message текст сообщения
 */
void MainWindow::updateStatusBar(const QString &message)
{
    statusBarLabel->setText(message);
}

/**
 * @brief Функция сохранения текущей рабочей области в SVG файл
 */
void MainWindow::saveSvg()
{
    QString newPath = QFileDialog::getSaveFileName(this, "Сохранить как SVG",
        "", "SVG files (*.svg)");

    if (newPath.isEmpty())
        return;

    QSvgGenerator generator;
    generator.setFileName(newPath);
    generator.setSize(QSize(static_cast<int>(renderScene->width()),
                            static_cast<int>(renderScene->height())));
    generator.setViewBox(QRectF(0, 0, renderScene->width(), renderScene->height()));
    generator.setTitle("Векторный редактор");
    generator.setDescription("");

    QPainter painter;
    painter.begin(&generator);
    renderScene->render(&painter);
    painter.end();
}

/**
 * @brief Функция загрузки рабочей области из SVG файла
 */
void MainWindow::loadSvg()
{
    QString newPath = QFileDialog::getOpenFileName(this, "Открыть SVG",
                                                   "", "SVG files (*.svg)");
    if (newPath.isEmpty())
        return;

    renderScene->clear();

    renderScene->setSceneRect(SvgReader::getSizes(newPath));

    foreach (QGraphicsItem *item, SvgReader::getPolylines(newPath)) {
        switch (item->type()) {
        case Polyline::Type: {
            Polyline *polyline = qgraphicsitem_cast<Polyline*>(item);
            renderScene->addItem(polyline);
            break;
        }
        default:
            break;
        }
    }
}

/**
 * @brief Слот для обработки события изменения размера окна
 * @param event
 */
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    renderScene->setSceneRect(0,0, renderView->width() - 20, renderView->height() - 20);
}

/**
 * @brief Функция построения окна приложения и настройки сигналов
 */
void MainWindow::createActions()
{
    // Рабочая область
    renderScene = new RenderArea();
    connect(this, &MainWindow::activeToolChanged,
            renderScene, &RenderArea::setSelectedTool);
    renderView = new QGraphicsView(renderScene);
    renderView->setFrameStyle(0);
    renderScene->setSceneRect(0,0, 1024, 768);
    renderView->setMouseTracking(true);

    // Палитра
    paletteText = new QLabel("Цвет", this);
    paletteColor = new QPushButton(this);
    paletteColor->setStyleSheet("background-color : black");
    connect(paletteColor, &QPushButton::clicked,
            this, [this](){
        QColor color = QColorDialog::getColor(Qt::black);
        if (color.isValid()) {
            renderScene->setColor(color);
            paletteColor->setStyleSheet("background-color : " + color.name());
        }
    });

    // Толщина линии
    widthText = new QLabel("Толщина", this);
    widthSpinBox = new QSpinBox(this);
    widthSpinBox->setMinimum(1);
    widthSpinBox->setMaximum(72);
    connect(widthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            renderScene, &RenderArea::setLineWidth);

    // Инструмент "Выделение"
    selectionTool = new QPushButton("Выделение");
    connect(selectionTool, &QPushButton::clicked,
            this, [this]() {
        emit activeToolChanged(RenderArea::Tool::Selection);
    });

    // Инструмент "Ломаная линия"
    polylineTool = new QPushButton("Ломаная линия");
    connect(polylineTool, &QPushButton::clicked,
            this, [this]() {
        emit activeToolChanged(RenderArea::Tool::Polyline);
    });

    // Инструмент "Удаление"
    deleteTool = new QPushButton("Удаление");
    connect(deleteTool, &QPushButton::clicked,
            this, [this]() {
        emit activeToolChanged(RenderArea::Tool::Delete);
    });

    // Слой расположения элементов на панели инструментов
    toolsLayout = new QGridLayout;
    toolsLayout->addWidget(selectionTool, 0, 0);
    toolsLayout->addWidget(polylineTool, 0, 1);
    toolsLayout->addWidget(deleteTool, 1, 0);
    toolsLayout->addWidget(paletteText, 2, 0);
    toolsLayout->addWidget(paletteColor, 2, 1);
    toolsLayout->addWidget(widthText, 3, 0);
    toolsLayout->addWidget(widthSpinBox, 3, 1);
    toolsLayout->setRowStretch(4, 1);

    // Слой расположения элементов на окне приложения
    windowLayout = new QHBoxLayout;
    windowLayout->addItem(toolsLayout);
    windowLayout->addWidget(renderView);
    ui->centralWidget->setLayout(windowLayout);

    // Полоса статуса
    statusBarLabel = new QLabel("-");
    ui->statusBar->addWidget(statusBarLabel);
    connect(renderScene, &RenderArea::coordChanged,
            this, &MainWindow::updateStatusBar);
}

/**
 * @brief Функция для создания меню приложения
 */
void MainWindow::buildMenu()
{
    QMenu *fileMenu = ui->menuBar->addMenu("Файл");
    QAction *saveAction = fileMenu->addAction("Сохранить");
    connect(saveAction, &QAction::triggered,
            this, &MainWindow::saveSvg);

    QAction *loadAction = fileMenu->addAction("Загрузить");
    connect(loadAction, &QAction::triggered,
            this, &MainWindow::loadSvg);


}
