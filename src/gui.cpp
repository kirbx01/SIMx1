#include "gui.hpp"
#include <QApplication>
#include <QToolBar>
#include <QAction>
#include <QGraphicsSceneDragDropEvent>
#include <QPainter>
#include <QLinearGradient>
#include <QPen>
#include <QBrush>
#include <QMimeData>
#include <QDrag>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QDockWidget>
#include <QMessageBox>
#include <QRandomGenerator>

static const QMap<QString, QString> kGateLabels = {
    {"AND", "AND"}, {"OR", "OR"}, {"XOR", "XOR"}, {"NOT", "NOT"},
    {"NAND", "NAND"}, {"NOR", "NOR"}, {"XNOR", "XNOR"}, {"BUFFER", "BUF"},
};

GateItem::GateItem(const QString& type, Circuit* circuit)
    : type_(type), circuit_(circuit) {
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    QString wireName = type_ + "_" + QString::number(QRandomGenerator::global()->generate());
    localOut_ = circuit_->addWire(wireName.toStdString(), 0);
    output_ = localOut_;
}

QRectF GateItem::boundingRect() const {
    return QRectF(-50, -30, 100, 60);
}

void GateItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    QRectF rect = boundingRect().adjusted(2, 2, -2, -2);
    QLinearGradient grad(rect.topLeft(), rect.bottomLeft());
    grad.setColorAt(0, QColor(245, 247, 250));
    grad.setColorAt(1, QColor(220, 224, 230));
    painter->setBrush(QBrush(grad));
    painter->setPen(QPen(QColor(60, 70, 90), 2));
    painter->drawRoundedRect(rect, 8, 8);

    painter->setPen(QPen(QColor(30, 40, 60)));
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(10);
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignCenter, kGateLabels.value(type_, type_));

    painter->setPen(QPen(QColor(120, 130, 150), 2));
    painter->drawLine(QLineF(-50, -15, -40, -15));
    painter->drawLine(QLineF(-50, 15, -40, 15));
    painter->drawLine(QLineF(40, 0, 50, 0));

    if (output_ && output_->value()) {
        painter->setBrush(QBrush(QColor(80, 200, 120)));
        painter->setPen(QPen(QColor(40, 160, 80), 2));
    } else {
        painter->setBrush(QBrush(QColor(200, 80, 80)));
        painter->setPen(QPen(QColor(160, 40, 40), 2));
    }
    painter->drawEllipse(QPointF(45, 0), 4, 4);
}

void GateItem::setInputs(const QList<Wire*>& wires) {
    inputs_ = wires;
}

void GateItem::setOutput(Wire* wire) {
    output_ = wire;
}

void GateItem::evaluate() {
    if (inputs_.isEmpty() || !output_) return;
    std::vector<Wire*> in;
    for (Wire* w : inputs_) in.push_back(w);
    std::string name = (type_ + "_" + QString::number(pos().x())).toStdString();
    if (type_ == "AND") circuit_->addGate<AndGate>(name, in, output_);
    else if (type_ == "OR") circuit_->addGate<OrGate>(name, in, output_);
    else if (type_ == "XOR") circuit_->addGate<XorGate>(name, in, output_);
    else if (type_ == "NOT") circuit_->addGate<NotGate>(name, in, output_);
    else if (type_ == "NAND") circuit_->addGate<NandGate>(name, in, output_);
    else if (type_ == "NOR") circuit_->addGate<NorGate>(name, in, output_);
    else if (type_ == "XNOR") circuit_->addGate<XnorGate>(name, in, output_);
    else if (type_ == "BUFFER") circuit_->addGate<BufferGate>(name, in, output_);
    update();
}

QVariant GateItem::itemChange(GraphicsItemChange change, const QVariant& value) {
    if (change == ItemPositionHasChanged) update();
    return QGraphicsItem::itemChange(change, value);
}

Canvas::Canvas(Circuit* circuit, QObject* parent)
    : QGraphicsScene(parent), circuit_(circuit) {
    setSceneRect(-400, -300, 800, 600);
    setBackgroundBrush(QBrush(QColor(250, 250, 252)));
}

void Canvas::dragEnterEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasText()) event->acceptProposedAction();
    else event->ignore();
}

void Canvas::dragMoveEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasText()) event->acceptProposedAction();
    else event->ignore();
}

void Canvas::dropEvent(QGraphicsSceneDragDropEvent* event) {
    if (!event->mimeData()->hasText()) { event->ignore(); return; }
    QString type = event->mimeData()->text();
    GateItem* item = new GateItem(type, circuit_);
    addItem(item);
    item->setPos(event->scenePos());
    event->acceptProposedAction();
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("SIMx1 - Logic Gate Simulator");
    resize(900, 600);

    scene_ = new Canvas(&circuit_, this);
    view_ = new QGraphicsView(scene_);
    view_->setRenderHint(QPainter::Antialiasing);
    view_->setAcceptDrops(true);
    view_->setDragMode(QGraphicsView::RubberBandDrag);
    setCentralWidget(view_);

    buildPalette();

    QToolBar* toolbar = addToolBar("Controls");
    QAction* runAct = toolbar->addAction("Run");
    QAction* clearAct = toolbar->addAction("Clear");
    connect(runAct, &QAction::triggered, this, &MainWindow::runSimulation);
    connect(clearAct, &QAction::triggered, this, &MainWindow::clearCanvas);
}

void MainWindow::buildPalette() {
    QDockWidget* dock = new QDockWidget("Gates", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    QWidget* panel = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(panel);
    QStringList gates = {"AND", "OR", "XOR", "NOT", "NAND", "NOR", "XNOR", "BUFFER"};
    for (const QString& g : gates) {
        QLabel* label = new QLabel(kGateLabels.value(g, g));
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("QLabel { background: qlineargradient(x1:0,y1:0,x2:0,y2:1,stop:0 #f5f7fa, stop:1 #dce0e6); border: 1px solid #3c465a; border-radius: 6px; padding: 8px; font-weight: bold; }");
        label->setMinimumSize(80, 40);
        label->setCursor(Qt::OpenHandCursor);
        label->installEventFilter(this);
        label->setProperty("gateType", g);
        layout->addWidget(label);
    }
    layout->addStretch();
    dock->setWidget(panel);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QLabel* label = qobject_cast<QLabel*>(obj);
        if (label) {
            QString type = label->property("gateType").toString();
            QDrag* drag = new QDrag(this);
            QMimeData* mime = new QMimeData;
            mime->setText(type);
            drag->setMimeData(mime);
            drag->exec(Qt::CopyAction);
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::runSimulation() {
    circuit_.evaluate();
    for (QGraphicsItem* item : scene_->items()) {
        GateItem* g = dynamic_cast<GateItem*>(item);
        if (g) g->update();
    }
    QMessageBox::information(this, "Simulation", "Evaluation complete. Output LEDs updated.");
}

void MainWindow::clearCanvas() {
    for (QGraphicsItem* item : scene_->items()) {
        if (dynamic_cast<GateItem*>(item)) scene_->removeItem(item);
    }
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}