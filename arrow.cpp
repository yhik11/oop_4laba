#include "arrow.h"
#include "shape.h"
#include "circle.h"
#include "rectangle.h"
#include "triangle.h"
#include "line.h"
#include "group.h"
#include <QPainter>
#include <cmath>
#include <iostream>

Arrow::Arrow(std::shared_ptr<::Shape> src, std::shared_ptr<::Shape> tgt)
    : ::Shape(0, 0, Qt::blue), source(src), target(tgt), active(true) {

    // Подписываемся на изменения источника
    if (auto sourcePtr = source.lock()) {
        // Источник должен быть Subject (нужно будет добавить)
    }

    updatePosition();
}

Arrow::~Arrow() {
    // Отписываемся от источника
}

void Arrow::draw(QPainter& painter) const {
    if (!active) return;

    auto src = source.lock();
    auto tgt = target.lock();

    if (!src || !tgt) return;

    QPoint srcPoint = getSourcePoint();
    QPoint tgtPoint = getTargetPoint();

    // Рисуем линию
    painter.setPen(QPen(selected ? Qt::red : color, 2));
    painter.drawLine(srcPoint, tgtPoint);

    // Рисуем стрелку на конце
    const int arrowSize = 10;
    double angle = atan2(tgtPoint.y() - srcPoint.y(), tgtPoint.x() - srcPoint.x());

    QPoint arrowP1 = tgtPoint - QPoint(
                         arrowSize * cos(angle + M_PI / 6),
                         arrowSize * sin(angle + M_PI / 6)
                         );
    QPoint arrowP2 = tgtPoint - QPoint(
                         arrowSize * cos(angle - M_PI / 6),
                         arrowSize * sin(angle - M_PI / 6)
                         );

    painter.drawLine(tgtPoint, arrowP1);
    painter.drawLine(tgtPoint, arrowP2);
}

bool Arrow::contains(int mx, int my) const {
    if (!active) return false;

    auto src = source.lock();
    auto tgt = target.lock();

    if (!src || !tgt) return false;

    QPoint srcPoint = getSourcePoint();
    QPoint tgtPoint = getTargetPoint();

    // Упрощённая проверка - расстояние до линии
    const int tolerance = 5;

    // Вектор стрелки
    int vx = tgtPoint.x() - srcPoint.x();
    int vy = tgtPoint.y() - srcPoint.y();

    // Вектор от начала стрелки до точки
    int wx = mx - srcPoint.x();
    int wy = my - srcPoint.y();

    // Проекция
    double dot = wx * vx + wy * vy;
    double lenSq = vx * vx + vy * vy;

    if (lenSq == 0) return false;

    double proj = dot / lenSq;
    if (proj < 0 || proj > 1) return false;

    // Ближайшая точка на линии
    int closestX = srcPoint.x() + static_cast<int>(proj * vx);
    int closestY = srcPoint.y() + static_cast<int>(proj * vy);

    // Расстояние до линии
    int dx = mx - closestX;
    int dy = my - closestY;

    return (dx * dx + dy * dy) <= (tolerance * tolerance);
}

QRect Arrow::getBounds() const {
    if (!active) return QRect(0, 0, 0, 0);

    auto src = source.lock();
    auto tgt = target.lock();

    if (!src || !tgt) return QRect(0, 0, 0, 0);

    QPoint srcPoint = getSourcePoint();
    QPoint tgtPoint = getTargetPoint();

    int left = std::min(srcPoint.x(), tgtPoint.x()) - 10;
    int top = std::min(srcPoint.y(), tgtPoint.y()) - 10;
    int right = std::max(srcPoint.x(), tgtPoint.x()) + 10;
    int bottom = std::max(srcPoint.y(), tgtPoint.y()) + 10;

    return QRect(left, top, right - left, bottom - top);
}

void Arrow::resize(float scale) {
    // Для стрелки масштабирование не нужно
    // Положение определяется источниками
}

void Arrow::move(int dx, int dy) {
    // Стрелка не перемещается независимо
    // Её позиция определяется источниками
}

void Arrow::save(std::ofstream& out) const {
    // Пока сохраняем только индексы источников
    // Нужно будет доработать
    out << getTypeId() << " ";
    out << x << " " << y << " ";
    out << color.red() << " " << color.green() << " " << color.blue() << " ";
    out << "0 0" << std::endl;  // Заглушка
}

void Arrow::update(const std::string& message) {
    if (message == "source_moved") {
        updatePosition();

        // Если стрелка активна - перемещаем цель
        if (active) {
            auto src = source.lock();
            auto tgt = target.lock();

            if (src && tgt) {
                // Вычисляем смещение
                QPoint srcOld = getSourcePoint(); // Нужно хранить старое положение
                QPoint srcNew = src->getBounds().center();
                QPoint offset = srcNew - srcOld;

                // Перемещаем цель
                tgt->move(offset.x(), offset.y());
            }
        }
    }
}

void Arrow::updatePosition() {
    auto src = source.lock();
    auto tgt = target.lock();

    if (src && tgt) {
        QPoint srcCenter = src->getBounds().center();
        QPoint tgtCenter = tgt->getBounds().center();
        x = (srcCenter.x() + tgtCenter.x()) / 2;
        y = (srcCenter.y() + tgtCenter.y()) / 2;
    }
}

QPoint Arrow::getSourcePoint() const {
    auto src = source.lock();
    if (src) {
        return src->getBounds().center();
    }
    return QPoint(0, 0);
}

QPoint Arrow::getTargetPoint() const {
    auto tgt = target.lock();
    if (tgt) {
        return tgt->getBounds().center();
    }
    return QPoint(0, 0);
}

std::shared_ptr<::Shape> Arrow::getSource() const {
    return source.lock();
}

std::shared_ptr<::Shape> Arrow::getTarget() const {
    return target.lock();
}
