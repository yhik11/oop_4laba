#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "circle.h"
#include "rectangle.h"
#include "triangle.h"
#include <QPainter>
#include <QToolBar>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ctrlPressed(false)
    , currentTool(CIRCLE) {
    ui->setupUi(this);

    createMenu();  // ← ВЫЗЫВАЕМ СОЗДАНИЕ МЕНЮ
    createToolbar();

    setFocusPolicy(Qt::StrongFocus);
    setWindowTitle("Визуальный редактор");
    resize(800, 600);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::createToolbar() {
    QToolBar* toolbar = addToolBar("Инструменты");

    // Кнопка для круга
    QAction* circleAction = toolbar->addAction("○");
    circleAction->setToolTip("Круг");
    connect(circleAction, &QAction::triggered, this, &MainWindow::setCircleTool);

    // Кнопка для прямоугольника
    QAction* rectAction = toolbar->addAction("□");
    rectAction->setToolTip("Прямоугольник");
    connect(rectAction, &QAction::triggered, this, &MainWindow::setRectangleTool);

    // Кнопка для треугольника
    QAction* triangleAction = toolbar->addAction("△");
    triangleAction->setToolTip("Треугольник");
    connect(triangleAction, &QAction::triggered, this, &MainWindow::setTriangleTool);

    // Разделитель
    toolbar->addSeparator();

    QAction* colorAction = toolbar->addAction("🎨");
    colorAction->setToolTip("Изменить цвет");
    connect(colorAction, &QAction::triggered, this, &MainWindow::changeColor);

    // Кнопка удаления
    QAction* deleteAction = toolbar->addAction("Удалить");
    deleteAction->setToolTip("Удалить выделенные фигуры");
    connect(deleteAction, &QAction::triggered, [this]() {
        for (int i = 0; i < storage.getCount(); i++) {
            if (storage.getObject(i)->isSelected()) {
                storage.remove(i);
                i--;
            }
        }
        update();
    });
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

    QAction* colorAction = editMenu->addAction("Изменить цвет...");
    connect(colorAction, &QAction::triggered, this, &MainWindow::changeColor);

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

void MainWindow::changeColor() {
    QColor newColor = QColorDialog::getColor(currentColor, this, "Выберите цвет фигур");

    if (newColor.isValid()) {
        currentColor = newColor;

        // Меняем цвет выделенных фигур
        for (int i = 0; i < storage.getCount(); i++) {
            if (storage.getObject(i)->isSelected()) {
                storage.getObject(i)->setColor(currentColor);
            }
        }
        update();
    }
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
        // Удаление выделенных фигур
        for (int i = 0; i < storage.getCount(); i++) {
            if (storage.getObject(i)->isSelected()) {
                storage.remove(i);
                i--;
            }
        }
        update();
    } else {
        // Управление стрелками для перемещения фигур
        int dx = 0, dy = 0;
        switch (event->key()) {
        case Qt::Key_Left: dx = -5; break;
        case Qt::Key_Right: dx = 5; break;
        case Qt::Key_Up: dy = -5; break;
        case Qt::Key_Down: dy = 5; break;
        }

        if (dx != 0 || dy != 0) {
            QRect area = this->rect();  // Границы окна

            // Перемещаем все выделенные фигуры с проверкой границ
            for (int i = 0; i < storage.getCount(); i++) {
                Shape* shape = storage.getObject(i);
                if (shape->isSelected()) {
                    // Получаем границы фигуры
                    QRect bounds = shape->getBounds();

                    // Проверяем, не выйдет ли фигура за границы
                    bool canMove = true;

                    // Создаем "призрачные" границы после перемещения
                    QRect newBounds = bounds.translated(dx, dy);

                    // Проверяем, что фигура полностью внутри окна
                    if (!area.contains(newBounds)) {
                        canMove = false;
                    }

                    if (canMove) {
                        shape->move(dx, dy);
                    }
                }
            }
            update();
        }
    }
}


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
    painter.drawText(10, 40, QString("Цвет: %1").arg(currentColor.name()));
    painter.drawText(10, 60, QString("Фигур: %1").arg(storage.getCount()));

    painter.drawText(width() - 250, 20, "Управление:");
    painter.drawText(width() - 250, 40, "Стрелки - перемещение");
    painter.drawText(width() - 250, 60, "Delete - удалить");
    painter.drawText(width() - 250, 80, "Ctrl+клик - несколько");
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        ctrlPressed = false;
    }
}

void MainWindow::createShape(int x, int y) {
    switch (currentTool) {
    case CIRCLE:
        storage.add(new Circle(x, y, 20, currentColor));
        break;
    case RECTANGLE:
        storage.add(new Rectangle(x, y, 40, 30, currentColor));
        break;
    case TRIANGLE:
        storage.add(new Triangle(x, y, 30, currentColor));
        break;
    }
}
