#include "group.h"
#include <algorithm>

Group::Group(const QColor& color) : Shape(0, 0, color) {
}

Group::~Group() {
    children.clear();
}

void Group::draw(QPainter& painter) const {
    for (const auto& child : children) {
        child->draw(painter);
    }

    if (selected) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(Qt::red, 2, Qt::DashLine));
        QRect bounds = getBounds();
        painter.drawRect(bounds);
    }
}

bool Group::contains(int mx, int my) const {
    for (const auto& child : children) {
        if (child->contains(mx, my)) return true;
    }
    return false;
}

QRect Group::getBounds() const {
    if (children.empty()) return QRect(0, 0, 0, 0);

    QRect bounds = children[0]->getBounds();
    for (size_t i = 1; i < children.size(); i++) {
        bounds = bounds.united(children[i]->getBounds());
    }
    return bounds;
}

void Group::resize(float scale) {
    if (children.empty()) return;

    QRect oldBounds = getBounds();
    QPoint center = oldBounds.center();

    for (auto& child : children) {
        QRect childBounds = child->getBounds();
        QPoint childCenter = childBounds.center();

        int offsetX = childCenter.x() - center.x();
        int offsetY = childCenter.y() - center.y();
        int newOffsetX = (int)(offsetX * scale);
        int newOffsetY = (int)(offsetY * scale);

        child->move(newOffsetX - offsetX, newOffsetY - offsetY);
        child->resize(scale);
    }

    QRect newBounds = getBounds();
    x = newBounds.center().x();
    y = newBounds.center().y();
}

void Group::move(int dx, int dy) {
    for (auto& child : children) {
        child->move(dx, dy);
    }
    if (!children.empty()) {
        QRect bounds = getBounds();
        x = bounds.center().x();
        y = bounds.center().y();
    }
}

void Group::addShape(std::shared_ptr<Shape> shape) {
    children.push_back(shape);
    if (!children.empty()) {
        QRect bounds = getBounds();
        x = bounds.center().x();
        y = bounds.center().y();
    }
}

void Group::removeShape(int index) {
    if (index >= 0 && index < (int)children.size()) {
        children.erase(children.begin() + index);
    }
}

Shape* Group::getChild(int index) const {
    if (index >= 0 && index < (int)children.size()) {
        return children[index].get();
    }
    return nullptr;
}

void Group::save(std::ofstream& out) const {
    out << getTypeId() << " ";
    out << x << " " << y << " ";
    out << color.red() << " " << color.green() << " " << color.blue() << " ";
    out << children.size() << " ";

    for (const auto& child : children) {
        child->save(out);
    }
}

std::shared_ptr<Shape> Group::load(std::ifstream& in) {
    int x, y, r, g, b, childCount;
    in >> x >> y >> r >> g >> b >> childCount;

    auto group = std::make_shared<Group>(QColor(r, g, b));

    for (int i = 0; i < childCount; i++) {
        auto child = Shape::load(in);
        if (child) {
            group->addShape(child);
        }
    }

    return group;
}
