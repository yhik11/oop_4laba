#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "circle.h"
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , ctrlPressed(false) {
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);

    for (int i = 0; i < storage.getCount(); i++) {
        storage.getObject(i)->draw(painter);
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        ctrlPressed = true;
    } else if (event->key() == Qt::Key_Delete) {
        storage.removeSelected();
        update();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        ctrlPressed = false;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        int x = event->pos().x();
        int y = event->pos().y();

        if (ctrlPressed) {
            // Ctrl+ЛКМ: добавляем/убираем выделение
            for (int i = storage.getCount() - 1; i >= 0; i--) {
                if (storage.getObject(i)->contains(x, y)) {
                    bool current = storage.getObject(i)->isSelected();
                    storage.getObject(i)->setSelected(!current);
                    break;
                }
            }
        } else {
            // Обычный клик
            bool clickedOnCircle = false;

            // Проверяем, попали ли в круг
            for (int i = 0; i < storage.getCount(); i++) {
                if (storage.getObject(i)->contains(x, y)) {
                    clickedOnCircle = true;
                    break;
                }
            }

            if (clickedOnCircle) {
                // Кликнули по кругу - выделяем его
                storage.deselectAll();
                for (int i = storage.getCount() - 1; i >= 0; i--) {
                    if (storage.getObject(i)->contains(x, y)) {
                        storage.getObject(i)->setSelected(true);
                        break;
                    }
                }
            } else {
                // Кликнули по пустому месту - создаём новый круг
                storage.deselectAll();
                storage.add(new CCircle(x, y, 25));
            }
        }
        update();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    update();
}

MainWindow::~MainWindow()
{
    delete ui;
}
