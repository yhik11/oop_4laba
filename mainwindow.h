#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include "mystorage.h"
#include "objecttree.h"

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
    ObjectTree* objectTree;

    enum ToolType { CIRCLE, RECTANGLE, TRIANGLE, LINE, ARROW };
    ToolType currentTool;
    QColor currentColor;

    // ДОБАВЬ ЭТИ ДВА ПОЛЯ ДЛЯ СТРЕЛОК:
    std::shared_ptr<::Shape> arrowSource;
    bool creatingArrow;

    void createShape(int x, int y);
    void createMenu();
    void createToolbar();
    void createLine();

private slots:
    void setCircleTool();
    void setRectangleTool();
    void setTriangleTool();
    void setLineTool();
    void setArrowTool();  // ← ДОБАВЬ
    void changeColor();

    void groupSelected();
    void ungroupSelected();
    void saveProject();
    void loadProject();
    void updateTreeView();

    // ДОБАВЬ ЭТОТ МЕТОД:
    void createArrow(std::shared_ptr<::Shape> source, std::shared_ptr<::Shape> target);
};

#endif // MAINWINDOW_H
