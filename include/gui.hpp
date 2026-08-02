#pragma once
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMap>
#include <QString>
#include <QList>
#include "circuit.hpp"

class GateItem : public QGraphicsItem {
public:
    GateItem(const QString& type, Circuit* circuit);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    QString gateType() const { return type_; }
    void setInputs(const QList<Wire*>& wires);
    void setOutput(Wire* wire);
    void evaluate();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    QString type_;
    Circuit* circuit_;
    QList<Wire*> inputs_;
    Wire* output_ = nullptr;
    Wire* localOut_ = nullptr;
};

class Canvas : public QGraphicsScene {
    Q_OBJECT
public:
    explicit Canvas(Circuit* circuit, QObject* parent = nullptr);

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    void dropEvent(QGraphicsSceneDragDropEvent* event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;

private:
    Circuit* circuit_;
};

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private slots:
    void runSimulation();
    void clearCanvas();

private:
    void buildPalette();
    QGraphicsView* view_ = nullptr;
    Canvas* scene_ = nullptr;
    Circuit circuit_;
};