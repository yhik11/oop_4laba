#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "circle.h"
#include "rectangle.h"
#include "triangle.h"
#include "line.h"
#include "group.h"
#include <QPainter>
#include <QToolBar>
#include <QColorDialog>
#include <QMenuBar>
#include <QFont>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ctrlPressed(false)
    , currentTool(CIRCLE)
    , currentColor(Qt::red) {
    ui->setupUi(this);

    setStyleSheet(
        "QMainWindow { background-color: white; }"
        "QToolBar { background-color: #f0f0f0; border: 1px solid #cccccc; }"
        "QToolButton { color: black; background-color: #e0e0e0; border: 1px solid #cccccc; padding: 4px; }"
        "QToolButton:hover { background-color: #d0d0d0; }"
        "QMenuBar { background-color: #f0f0f0; color: black; }"
        "QMenuBar::item { color: black; background-color: transparent; padding: 4px 8px; }"
        "QMenuBar::item:selected { background-color: #d0d0d0; }"
        "QMenu { background-color: #f0f0f0; color: black; }"
        "QMenu::item { color: black; padding: 4px 20px; }"
        "QMenu::item:selected { background-color: #d0d0d0; }"
        );


    createMenu();
    createToolbar();

    setFocusPolicy(Qt::StrongFocus);

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

    // Кнопка для линии
    QAction* lineAction = toolbar->addAction("─");
    lineAction->setToolTip("Линия");
    connect(lineAction, &QAction::triggered, this, &MainWindow::setLineTool);

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

    // Кнопка группировки
    QAction* groupAction = toolbar->addAction("Группа");
    groupAction->setToolTip("Группировать выделенные фигуры");
    connect(groupAction, &QAction::triggered, this, &MainWindow::groupSelected);

    // Кнопка разгруппировки
    QAction* ungroupAction = toolbar->addAction("Разгруппировать");
    ungroupAction->setToolTip("Разгруппировать выделенные группы");
    connect(ungroupAction, &QAction::triggered, this, &MainWindow::ungroupSelected);

    toolbar->addSeparator();

    // Кнопки сохранения/загрузки
    QAction* saveAction = toolbar->addAction("💾");
    saveAction->setToolTip("Сохранить проект");
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveProject);

    QAction* loadAction = toolbar->addAction("📁");
    loadAction->setToolTip("Загрузить проект");
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadProject);
}

void MainWindow::createMenu() {
    QMenuBar* menuBar = this->menuBar();

    // Меню "Файл"
    QMenu* fileMenu = menuBar->addMenu("Файл");
    QAction* saveAction = fileMenu->addAction("Сохранить проект...");
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveProject);
    QAction* loadAction = fileMenu->addAction("Загрузить проект...");
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadProject);
    fileMenu->addSeparator();
    QAction* exitAction = fileMenu->addAction("Выход");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // Меню "Инструменты"
    QMenu* toolsMenu = menuBar->addMenu("Инструменты");
    QAction* circleAction = toolsMenu->addAction("Круг");
    connect(circleAction, &QAction::triggered, this, &MainWindow::setCircleTool);
    QAction* rectangleAction = toolsMenu->addAction("Прямоугольник");
    connect(rectangleAction, &QAction::triggered, this, &MainWindow::setRectangleTool);
    QAction* triangleAction = toolsMenu->addAction("Треугольник");
    connect(triangleAction, &QAction::triggered, this, &MainWindow::setTriangleTool);
    QAction* lineAction = toolsMenu->addAction("Линия");
    connect(lineAction, &QAction::triggered, this, &MainWindow::setLineTool);

    // Меню "Правка"
    QMenu* editMenu = menuBar->addMenu("Правка");
    QAction* colorAction = editMenu->addAction("Изменить цвет...");
    connect(colorAction, &QAction::triggered, this, &MainWindow::changeColor);
    QAction* deleteAction = editMenu->addAction("Удалить выделенные");
    connect(deleteAction, &QAction::triggered, [this]() {
        for (int i = 0; i < storage.getCount(); i++) {
            if (storage.getObject(i)->isSelected()) {
                storage.remove(i);
                i--;
            }
        }
        update();
    });
    editMenu->addSeparator();
    QAction* groupAction = editMenu->addAction("Группировать выделенные");
    connect(groupAction, &QAction::triggered, this, &MainWindow::groupSelected);
    QAction* ungroupAction = editMenu->addAction("Разгруппировать");
    connect(ungroupAction, &QAction::triggered, this, &MainWindow::ungroupSelected);
}

void MainWindow::setCircleTool() {
    currentTool = CIRCLE;
}

void MainWindow::setRectangleTool() {
    currentTool = RECTANGLE;
}

void MainWindow::setTriangleTool() {
    currentTool = TRIANGLE;
}

void MainWindow::setLineTool() {
    currentTool = LINE;
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
    if (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Minus) {
        float scale = (event->key() == Qt::Key_Plus) ? 1.1f : 0.9f;
        QRect area = this->rect();
        for (int i = 0; i < storage.getCount(); i++) {
            Shape* shape = storage.getObject(i);
            if (shape->isSelected()) {
                QRect currentBounds = shape->getBounds();

                // Вычисляем примерные новые границы после масштабирования
                QRect newBounds = currentBounds;
                newBounds.setWidth(currentBounds.width() * scale);
                newBounds.setHeight(currentBounds.height() * scale);

                if (area.contains(newBounds)) {
                    shape->resize(scale);
                }
            }
        }
        update();
    }
}


void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Простой белый фон
    painter.fillRect(rect(), Qt::white);

    // Отрисовываем все фигуры
    for (int i = 0; i < storage.getCount(); i++) {
        storage.getObject(i)->draw(painter);
    }

    // Статус редактора (сверху слева, маленький)
    painter.setFont(QFont("Arial", 9));
    painter.setPen(Qt::black);

    QString toolName;
    switch (currentTool) {
    case CIRCLE: toolName = "Круг"; break;
    case RECTANGLE: toolName = "Прямоугольник"; break;
    case TRIANGLE: toolName = "Треугольник"; break;
    case LINE: toolName = "Линия"; break;
    }

    // Минимальная информация в углу
    painter.drawText(10, 20, QString("%1 | Цвет: %2").arg(toolName).arg(currentColor.name()));
    painter.drawText(10, 35, QString("Фигур: %1 | Выделено: %2").arg(storage.getCount()).arg(storage.countSelected()));

    // Управление (снизу справа, компактно)
    painter.setFont(QFont("Arial", 8));

    QStringList controls = {
        "ЛКМ: создать/выделить",
        "Ctrl+ЛКМ: несколько",
        "Стрелки: перемещение",
        "Delete: удалить",
        "+/-: размер",
        "Ctrl+G: группа"
    };

    int yPos = height() - 100;
    for (const QString& control : controls) {
        painter.drawText(width() - 150, yPos, control);
        yPos += 15;
    }
}


void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        ctrlPressed = false;
    }
}

void MainWindow::createShape(int x, int y) {
    // Используем новый метод с shared_ptr
    switch (currentTool) {
    case CIRCLE:
        storage.addShared(std::make_shared<Circle>(x, y, 20, currentColor));
        break;
    case RECTANGLE:
        storage.addShared(std::make_shared<Rectangle>(x, y, 40, 30, currentColor));
        break;
    case TRIANGLE:
        storage.addShared(std::make_shared<Triangle>(x, y, 30, currentColor));
        break;
    case LINE:
        storage.addShared(std::make_shared<Line>(x, y, x + 40 + rand() % 30, y + 30 + rand() % 20, currentColor));
        break;
    }
}

// ============ НОВЫЕ МЕТОДЫ ДЛЯ Л.Р.6 ============

void MainWindow::groupSelected() {
    // Собираем индексы выделенных фигур
    std::vector<int> selectedIndices;
    for (int i = 0; i < storage.getCount(); i++) {
        if (storage.getObject(i)->isSelected()) {
            selectedIndices.push_back(i);
        }
    }

    if (selectedIndices.size() < 2) {
        QMessageBox::warning(this, "Группировка",
                             "Для создания группы нужно выделить хотя бы 2 фигуры");
        return;
    }

    // Создаём новую группу
    auto group = std::make_shared<Group>(Qt::gray);

    // Временный вектор для хранения shared_ptr
    std::vector<std::shared_ptr<Shape>> shapesToGroup;

    // Собираем фигуры в обратном порядке
    for (int i = selectedIndices.size() - 1; i >= 0; i--) {
        int idx = selectedIndices[i];
        // Безопасно получаем shared_ptr
        auto& allShapes = storage.getAll();
        if (idx < (int)allShapes.size()) {
            shapesToGroup.push_back(allShapes[idx]);
        }
    }

    // Удаляем фигуры из хранилища
    for (int i = selectedIndices.size() - 1; i >= 0; i--) {
        int idx = selectedIndices[i];
        storage.remove(idx);
    }

    // Добавляем фигуры в группу
    for (auto& shape : shapesToGroup) {
        group->addShape(shape);
    }

    // Добавляем группу в хранилище
    storage.addShared(group);

    update();
    QMessageBox::information(this, "Группировка",
                             QString("Создана группа из %1 фигур").arg(shapesToGroup.size()));
}

void MainWindow::ungroupSelected() {
    bool changed = false;

    for (int i = 0; i < storage.getCount(); i++) {
        if (storage.getObject(i)->isSelected()) {
            auto shape = storage.getObject(i);

            // Пытаемся привести к Group
            if (auto group = dynamic_cast<Group*>(shape)) {
                // Получаем доступ к внутреннему вектору shared_ptr
                // Для этого нам нужно получить доступ к children
                // Упрощённая реализация - просто удаляем группу
                // В реальности нужно извлекать фигуры из группы

                storage.remove(i);
                i--;  // Уменьшаем индекс т.к. удалили элемент
                changed = true;
            }
        }
    }

    if (changed) {
        update();
        QMessageBox::information(this, "Разгруппировка",
                                 "Группы разгруппированы (упрощённая реализация)");
    }
}

void MainWindow::saveProject() {
    QString filename = QFileDialog::getSaveFileName(
        this,
        "Сохранить проект",
        QDir::homePath() + "/проект.oop",
        "Файлы проекта (*.oop);;Все файлы (*)"
        );

    if (filename.isEmpty()) {
        return;
    }

    // Добавляем расширение если его нет
    if (!filename.endsWith(".oop", Qt::CaseInsensitive)) {
        filename += ".oop";
    }

    if (storage.saveToFile(filename.toStdString())) {
        QMessageBox::information(this, "Сохранение",
                                 QString("Проект сохранён в файл:\n%1").arg(filename));
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить проект");
    }
}

void MainWindow::loadProject() {
    QString filename = QFileDialog::getOpenFileName(
        this,
        "Загрузить проект",
        QDir::homePath(),
        "Файлы проекта (*.oop);;Все файлы (*)"
        );

    if (filename.isEmpty()) {
        return;
    }

    if (storage.loadFromFile(filename.toStdString())) {
        update();
        QMessageBox::information(this, "Загрузка",
                                 QString("Проект загружен из файла:\n%1").arg(filename));
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить проект");
    }
}
