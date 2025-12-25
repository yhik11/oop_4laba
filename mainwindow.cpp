#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "circle.h"
#include "rectangle.h"
#include "triangle.h"
#include "line.h"
#include "group.h"
#include "arrow.h"
#include "objecttree.h"
#include <QPainter>
#include <QToolBar>
#include <QColorDialog>
#include <QMenuBar>
#include <QFont>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QDockWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ctrlPressed(false)
    , currentTool(CIRCLE)
    , currentColor(Qt::red)
    , creatingArrow(false) {
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

    // СОЗДАЁМ И ДОБАВЛЯЕМ ДЕРЕВО ОБЪЕКТОВ
    objectTree = new ObjectTree(this);
    objectTree->setStorage(&storage);

    // Создаём док-виджет правильно
    QDockWidget* dockWidget = new QDockWidget(tr("Дерево объектов"), this);
    dockWidget->setWidget(objectTree);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

    createMenu();
    createToolbar();

    setFocusPolicy(Qt::StrongFocus);
    setWindowTitle("Визуальный редактор - Л.Р.7");
    resize(1000, 600);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::createToolbar() {
    QToolBar* toolbar = addToolBar("Инструменты");

    QAction* circleAction = toolbar->addAction("○");
    circleAction->setToolTip("Круг");
    connect(circleAction, &QAction::triggered, this, &MainWindow::setCircleTool);

    QAction* rectAction = toolbar->addAction("□");
    rectAction->setToolTip("Прямоугольник");
    connect(rectAction, &QAction::triggered, this, &MainWindow::setRectangleTool);

    QAction* triangleAction = toolbar->addAction("△");
    triangleAction->setToolTip("Треугольник");
    connect(triangleAction, &QAction::triggered, this, &MainWindow::setTriangleTool);

    QAction* lineAction = toolbar->addAction("─");
    lineAction->setToolTip("Линия");
    connect(lineAction, &QAction::triggered, this, &MainWindow::setLineTool);

    QAction* arrowAction = toolbar->addAction("➡️");
    arrowAction->setToolTip("Стрелка");
    connect(arrowAction, &QAction::triggered, this, &MainWindow::setArrowTool);

    toolbar->addSeparator();

    QAction* colorAction = toolbar->addAction("🎨");
    colorAction->setToolTip("Изменить цвет");
    connect(colorAction, &QAction::triggered, this, &MainWindow::changeColor);

    QAction* deleteAction = toolbar->addAction("Удалить");
    deleteAction->setToolTip("Удалить выделенные фигуры");
    connect(deleteAction, &QAction::triggered, [this]() {
        // Собираем индексы для удаления
        std::vector<int> toRemove;
        for (int i = 0; i < storage.getCount(); i++) {
            if (storage.getObject(i)->isSelected()) {
                toRemove.push_back(i);
            }
        }

        // Удаляем в обратном порядке
        for (auto it = toRemove.rbegin(); it != toRemove.rend(); ++it) {
            storage.remove(*it);
        }

        update();
    });

    QAction* groupAction = toolbar->addAction("Группа");
    groupAction->setToolTip("Группировать выделенные фигуры");
    connect(groupAction, &QAction::triggered, this, &MainWindow::groupSelected);

    QAction* ungroupAction = toolbar->addAction("Разгруппировать");
    ungroupAction->setToolTip("Разгруппировать выделенные группы");
    connect(ungroupAction, &QAction::triggered, this, &MainWindow::ungroupSelected);

    toolbar->addSeparator();

    QAction* saveAction = toolbar->addAction("💾");
    saveAction->setToolTip("Сохранить проект");
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveProject);

    QAction* loadAction = toolbar->addAction("📁");
    loadAction->setToolTip("Загрузить проект");
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadProject);
}

void MainWindow::createMenu() {
    QMenuBar* menuBar = this->menuBar();

    QMenu* fileMenu = menuBar->addMenu("Файл");
    QAction* saveAction = fileMenu->addAction("Сохранить проект...");
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveProject);
    QAction* loadAction = fileMenu->addAction("Загрузить проект...");
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadProject);
    fileMenu->addSeparator();
    QAction* exitAction = fileMenu->addAction("Выход");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    QMenu* toolsMenu = menuBar->addMenu("Инструменты");
    QAction* circleAction = toolsMenu->addAction("Круг");
    connect(circleAction, &QAction::triggered, this, &MainWindow::setCircleTool);
    QAction* rectangleAction = toolsMenu->addAction("Прямоугольник");
    connect(rectangleAction, &QAction::triggered, this, &MainWindow::setRectangleTool);
    QAction* triangleAction = toolsMenu->addAction("Треугольник");
    connect(triangleAction, &QAction::triggered, this, &MainWindow::setTriangleTool);
    QAction* lineAction = toolsMenu->addAction("Линия");
    connect(lineAction, &QAction::triggered, this, &MainWindow::setLineTool);
    QAction* arrowAction = toolsMenu->addAction("Стрелка");
    connect(arrowAction, &QAction::triggered, this, &MainWindow::setArrowTool);

    QMenu* editMenu = menuBar->addMenu("Правка");
    QAction* colorAction = editMenu->addAction("Изменить цвет...");
    connect(colorAction, &QAction::triggered, this, &MainWindow::changeColor);
    QAction* deleteAction = editMenu->addAction("Удалить выделенные");
    connect(deleteAction, &QAction::triggered, [this]() {
        // Собираем индексы для удаления
        std::vector<int> toRemove;
        for (int i = 0; i < storage.getCount(); i++) {
            if (storage.getObject(i)->isSelected()) {
                toRemove.push_back(i);
            }
        }

        // Удаляем в обратном порядке
        for (auto it = toRemove.rbegin(); it != toRemove.rend(); ++it) {
            storage.remove(*it);
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
    creatingArrow = false;
    arrowSource.reset();
}

void MainWindow::setRectangleTool() {
    currentTool = RECTANGLE;
    creatingArrow = false;
    arrowSource.reset();
}

void MainWindow::setTriangleTool() {
    currentTool = TRIANGLE;
    creatingArrow = false;
    arrowSource.reset();
}

void MainWindow::setLineTool() {
    currentTool = LINE;
    creatingArrow = false;
    arrowSource.reset();
}

void MainWindow::setArrowTool() {
    currentTool = ARROW;
    creatingArrow = false;
    arrowSource.reset();
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

        if (currentTool == ARROW) {
            // Режим создания стрелки
            if (ctrlPressed) {
                // Ctrl+клик в режиме стрелки - обычное выделение
                for (int i = storage.getCount() - 1; i >= 0; i--) {
                    if (storage.getObject(i)->contains(x, y)) {
                        bool current = storage.getObject(i)->isSelected();
                        storage.getObject(i)->setSelected(!current);
                        break;
                    }
                }
                update();
            } else {
                // Ищем фигуру под курсором
                std::shared_ptr<::Shape> clickedShape;
                int clickedIndex = -1;

                for (int i = storage.getCount() - 1; i >= 0; i--) {
                    if (storage.getObject(i)->contains(x, y)) {
                        clickedShape = storage.getSharedPtr(i);
                        clickedIndex = i;
                        break;
                    }
                }

                if (clickedShape) {
                    if (!creatingArrow) {
                        // Первый клик - выбираем источник
                        arrowSource = clickedShape;
                        creatingArrow = true;
                        // Визуально выделяем источник
                        storage.deselectAll();
                        arrowSource->setSelected(true);
                        storage.notifySelectionChanged();
                        update();
                    } else {
                        // Второй клик - создаём стрелку
                        if (arrowSource != clickedShape) {
                            createArrow(arrowSource, clickedShape);
                        }
                        // Сбрасываем состояние
                        arrowSource->setSelected(false);
                        arrowSource.reset();
                        creatingArrow = false;
                        storage.notifySelectionChanged();
                        update();
                    }
                } else {
                    // Кликнули по пустому месту - сбрасываем создание стрелки
                    if (creatingArrow && arrowSource) {
                        arrowSource->setSelected(false);
                    }
                    arrowSource.reset();
                    creatingArrow = false;
                    storage.deselectAll();
                    update();
                }
            }
        } else if (ctrlPressed) {
            // Ctrl+клик: добавляем/убираем выделение (для других инструментов)
            for (int i = storage.getCount() - 1; i >= 0; i--) {
                if (storage.getObject(i)->contains(x, y)) {
                    bool current = storage.getObject(i)->isSelected();
                    storage.getObject(i)->setSelected(!current);
                    break;
                }
            }
            storage.notifySelectionChanged();
            update();
        } else {
            // Обычный клик для других инструментов
            bool clickedOnShape = false;

            for (int i = 0; i < storage.getCount(); i++) {
                if (storage.getObject(i)->contains(x, y)) {
                    clickedOnShape = true;
                    break;
                }
            }

            if (clickedOnShape) {
                // Кликнули по фигуре - выделяем её
                storage.deselectAll();
                for (int i = storage.getCount() - 1; i >= 0; i--) {
                    if (storage.getObject(i)->contains(x, y)) {
                        storage.getObject(i)->setSelected(true);
                        break;
                    }
                }
                storage.notifySelectionChanged();
            } else {
                // Кликнули по пустому месту - создаём новую фигуру или сбрасываем выделение
                storage.deselectAll();
                storage.notifySelectionChanged();
                createShape(x, y);
            }
            update();
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        ctrlPressed = true;
    } else if (event->key() == Qt::Key_Delete) {
        // Удаление выделенных фигур
        std::vector<int> toRemove;

        for (int i = 0; i < storage.getCount(); i++) {
            if (storage.getObject(i)->isSelected()) {
                toRemove.push_back(i);
            }
        }

        for (auto it = toRemove.rbegin(); it != toRemove.rend(); ++it) {
            storage.remove(*it);
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
            QRect area = this->rect();

            for (int i = 0; i < storage.getCount(); i++) {
                ::Shape* shape = storage.getObject(i);
                if (shape && shape->isSelected()) {
                    QRect bounds = shape->getBounds();
                    QRect newBounds = bounds.translated(dx, dy);

                    if (area.contains(newBounds)) {
                        shape->move(dx, dy);
                    }
                }
            }
            storage.notifyUpdate("objects_moved");
            update();
        }
    }
    if (event->key() == Qt::Key_Plus || event->key() == Qt::Key_Minus) {
        float scale = (event->key() == Qt::Key_Plus) ? 1.1f : 0.9f;
        QRect area = this->rect();
        for (int i = 0; i < storage.getCount(); i++) {
            ::Shape* shape = storage.getObject(i);
            if (shape && shape->isSelected()) {
                QRect currentBounds = shape->getBounds();
                QRect newBounds = currentBounds;
                newBounds.setWidth(currentBounds.width() * scale);
                newBounds.setHeight(currentBounds.height() * scale);

                if (area.contains(newBounds)) {
                    shape->resize(scale);
                }
            }
        }
        storage.notifyUpdate("objects_resized");
        update();
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), Qt::white);

    // Рисуем все фигуры
    for (int i = 0; i < storage.getCount(); i++) {
        storage.getObject(i)->draw(painter);
    }

    painter.setFont(QFont("Arial", 9));
    painter.setPen(Qt::black);

    QString toolName;
    switch (currentTool) {
    case CIRCLE: toolName = "Круг"; break;
    case RECTANGLE: toolName = "Прямоугольник"; break;
    case TRIANGLE: toolName = "Треугольник"; break;
    case LINE: toolName = "Линия"; break;
    case ARROW: toolName = "Стрелка"; break;
    }

    painter.drawText(10, 20, QString("%1 | Цвет: %2").arg(toolName).arg(currentColor.name()));
    painter.drawText(10, 35, QString("Фигур: %1 | Выделено: %2").arg(storage.getCount()).arg(storage.countSelected()));

    // Подсказки для режима стрелки
    if (currentTool == ARROW) {
        if (creatingArrow && arrowSource) {
            painter.setPen(QPen(Qt::green, 1, Qt::DashLine));
            QRect bounds = arrowSource->getBounds();
            painter.drawRect(bounds);
            painter.drawText(10, 50, "Выберите цель для стрелки...");
        } else if (!creatingArrow) {
            painter.drawText(10, 50, "Выберите источник стрелки...");
        }
    }

    painter.setFont(QFont("Arial", 8));
    QStringList controls = {
        "ЛКМ: создать/выделить",
        "Ctrl+ЛКМ: несколько",
        "Стрелки: перемещение",
        "Delete: удалить",
        "+/-: размер",
        "Стрелка: два клика для связи"
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
    case ARROW:
        // Стрелки создаются через два клика, не здесь
        break;
    }
}

void MainWindow::createArrow(std::shared_ptr<::Shape> source, std::shared_ptr<::Shape> target) {
    if (source && target && source != target) {
        auto arrow = std::make_shared<Arrow>(source, target);
        storage.addShared(arrow);

        QMessageBox::information(this, "Стрелка создана",
                                 QString("Создана стрелка между объектами"));
    }
}

// ============ Методы для группировки ============

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

    // Собираем shared_ptr на фигуры В ОБРАТНОМ порядке
    for (auto it = selectedIndices.rbegin(); it != selectedIndices.rend(); ++it) {
        int idx = *it;
        if (auto shapePtr = storage.getSharedPtr(idx)) {
            group->addShape(shapePtr);
        }
    }

    // Удаляем оригинальные фигуры из хранилища (в обратном порядке)
    for (auto it = selectedIndices.rbegin(); it != selectedIndices.rend(); ++it) {
        storage.remove(*it);
    }

    // Выделяем группу
    group->setSelected(true);

    // Добавляем группу в хранилище
    storage.addShared(group);

    update();
    QMessageBox::information(this, "Группировка",
                             QString("Создана группа из %1 фигур").arg(selectedIndices.size()));
}

void MainWindow::ungroupSelected() {
    bool changed = false;

    // Собираем индексы групп для разгруппировки
    std::vector<int> groupIndices;

    for (int i = 0; i < storage.getCount(); i++) {
        if (storage.getObject(i)->isSelected() &&
            dynamic_cast<Group*>(storage.getObject(i))) {
            groupIndices.push_back(i);
        }
    }

    if (groupIndices.empty()) {
        QMessageBox::information(this, "Разгруппировка",
                                 "Нет выделенных групп для разгруппировки");
        return;
    }

    // Обрабатываем группы в обратном порядке
    for (auto it = groupIndices.rbegin(); it != groupIndices.rend(); ++it) {
        int idx = *it;

        if (auto groupPtr = std::dynamic_pointer_cast<Group>(storage.getSharedPtr(idx))) {
            // Извлекаем всех детей из группы
            for (int j = 0; j < groupPtr->getChildCount(); j++) {
                if (auto child = groupPtr->getChildSharedPtr(j)) {
                    child->setSelected(false);
                    storage.addShared(child);
                }
            }

            // Удаляем группу
            storage.remove(idx);
            changed = true;
        }
    }

    if (changed) {
        update();
        QMessageBox::information(this, "Разгруппировка",
                                 QString("Разгруппировано %1 групп").arg(groupIndices.size()));
    }
}

void MainWindow::saveProject() {
    QString filename = QFileDialog::getSaveFileName(
        this,
        "Сохранить проект",
        QDir::currentPath() + "/project.oop",
        "Файлы проекта (*.oop);;Все файлы (*)"
        );

    if (filename.isEmpty()) {
        return;
    }

    if (!filename.endsWith(".oop", Qt::CaseInsensitive)) {
        filename += ".oop";
    }

    if (storage.saveToFile(filename.toStdString())) {
        QMessageBox::information(this, "Сохранение",
                                 QString("Проект сохранён:\n%1").arg(filename));
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить проект");
    }
}

void MainWindow::loadProject() {
    QString filename = QFileDialog::getOpenFileName(
        this,
        "Загрузить проект",
        QDir::currentPath(),
        "Файлы проекта (*.oop);;Все файлы (*)"
        );

    if (filename.isEmpty()) {
        return;
    }

    if (storage.loadFromFile(filename.toStdString())) {
        update();
        QMessageBox::information(this, "Загрузка",
                                 QString("Проект загружен:\n%1").arg(filename));
    } else {
        QMessageBox::warning(this, "Ошибка",
                             QString("Не удалось загрузить проект:\n%1\nПроверьте файл.").arg(filename));
    }
}

void MainWindow::updateTreeView() {
    if (objectTree) {
        objectTree->update("manual_update");
    }
}
