#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "mystorage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    MyStorage storage;
    bool ctrlPressed;

    // ДОБАВЛЯЕМ ЭТИ ПОЛЯ:
    enum ToolType { CIRCLE, RECTANGLE, TRIANGLE, LINE };
    ToolType currentTool;
    QColor currentColor;  // если используешь в createShape

    void createShape(int x, int y);
    void createMenu();
    void createToolbar();
    void createLine();

private slots:  // ← ДОБАВЛЯЕМ ЭТОТ РАЗДЕЛ
    void setCircleTool();
    void setRectangleTool();
    void setTriangleTool();
    void setLineTool();
    void changeColor();

    void groupSelected();
    void ungroupSelected();
    void saveProject();
    void loadProject();
};



#endif // MAINWINDOW_H
