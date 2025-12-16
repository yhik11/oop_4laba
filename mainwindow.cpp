#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "circle.h"
#include "rectangle.h"
#include "triangle.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ctrlPressed(false)
    , currentTool(CIRCLE) {
    ui->setupUi(this);

    createMenu();  // ← ВЫЗЫВАЕМ СОЗДАНИЕ МЕНЮ

    setFocusPolicy(Qt::StrongFocus);
    setWindowTitle("Визуальный редактор");
    resize(800, 600);
}

MainWindow::~MainWindow() {
    delete ui;
}

// Добавляем метод создания меню:
void MainWindow::createMenu() {
    QMenuBar* menuBar = this->menuBar();

    // Меню "Инструменты"
    QMenu* toolsMenu = menuBar->addMenu("Инструменты");

    QAction* circleAction = toolsMenu->addAction("Круг");
    connect(circleAction, &QAction::triggered, this, &MainWindow::setCircleTool);

    QAction* rectangleAction = toolsMenu->addAction("Прямоугольник");
    connect(rectangleAction, &QAction::triggered, this, &MainWindow::setRectangleTool);

    QAction* triangleAction = toolsMenu->addAction("Треугольник");
    connect(triangleAction, &QAction::triggered, this, &MainWindow::setTriangleTool);

    // Меню "Правка"
    QMenu* editMenu = menuBar->addMenu("Правка");
    QAction* deleteAction = editMenu->addAction("Удалить выделенные");
    connect(deleteAction, &QAction::triggered, [this]() {
        // Удаляем выделенные фигуры
        for (int i = 0; i < storage.getCount(); i++) {
            if (storage.getObject(i)->isSelected()) {
                storage.remove(i);
                i--;
            }
        }
        update();
    });
}

// Реализуем слоты:
void MainWindow::setCircleTool() {
    currentTool = CIRCLE;
}

void MainWindow::setRectangleTool() {
    currentTool = RECTANGLE;
}

void MainWindow::setTriangleTool() {
    currentTool = TRIANGLE;
}

// Обновляем paintEvent для отображения текущего инструмента:
void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Отрисовываем все фигуры
    for (int i = 0; i < storage.getCount(); i++) {
        storage.getObject(i)->draw(painter);
    }

    // Информационная панель
    painter.setPen(Qt::black);
    QString toolName;
    switch (currentTool) {
    case CIRCLE: toolName = "Круг"; break;
    case RECTANGLE: toolName = "Прямоугольник"; break;
    case TRIANGLE: toolName = "Треугольник"; break;
    }

    painter.drawText(10, 20, QString("Инструмент: %1").arg(toolName));
    painter.drawText(10, 40, QString("Фигур: %1").arg(storage.getCount()));
}



void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        int x = event->pos().x();
        int y = event->pos().y();

        if (ctrlPressed) {
            // Ctrl+клик: добавляем/убираем выделение
            for (int i = storage.getCount() - 1; i >= 0; i--) {
                if (storage.getObject(i)->contains(x, y)) {
                    bool current = storage.getObject(i)->isSelected();
                    storage.getObject(i)->setSelected(!current);
                    break;
                }
            }
        } else {
            // Обычный клик
            bool clickedOnShape = false;

            // Проверяем, попали ли в существующую фигуру
            for (int i = 0; i < storage.getCount(); i++) {
                if (storage.getObject(i)->contains(x, y)) {
                    clickedOnShape = true;
                    break;
                }
            }

            if (clickedOnShape) {
                // Кликнули по фигуре - выделяем её
                for (int i = 0; i < storage.getCount(); i++) {
                    storage.getObject(i)->setSelected(false);
                }
                for (int i = storage.getCount() - 1; i >= 0; i--) {
                    if (storage.getObject(i)->contains(x, y)) {
                        storage.getObject(i)->setSelected(true);
                        break;
                    }
                }
            } else {
                // Кликнули по пустому месту - создаём новую фигуру
                for (int i = 0; i < storage.getCount(); i++) {
                    storage.getObject(i)->setSelected(false);
                }
                createShape(x, y);
            }
        }
        update();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        ctrlPressed = true;
    } else if (event->key() == Qt::Key_Delete) {
        // Удаляем выделенные фигуры
        for (int i = 0; i < storage.getCount(); i++) {
            if (storage.getObject(i)->isSelected()) {
                storage.remove(i);
                i--; // Уменьшаем индекс т.к. массив сдвинулся
            }
        }
        update();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        ctrlPressed = false;
    }
}

void MainWindow::createShape(int x, int y) {
    switch (currentTool) {
    case CIRCLE:
        storage.add(new Circle(x, y, 20));
        break;
    case RECTANGLE:
        storage.add(new Rectangle(x, y, 40, 30));
        break;
    case TRIANGLE:
        storage.add(new Triangle(x, y, 30));
        break;
    }
}
