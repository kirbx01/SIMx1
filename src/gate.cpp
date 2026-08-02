#include "gate.hpp"

Gate::Gate(std::string name, std::vector<Wire*> inputs, Wire* output)
    : name_(std::move(name)), inputs_(std::move(inputs)), output_(output) {}

const std::string& Gate::name() const { return name_; }

void AndGate::evaluate() {
    int result = 1;
    for (Wire* w : inputs_) result &= w->value();
    output_->setValue(result);
}

void OrGate::evaluate() {
    int result = 0;
    for (Wire* w : inputs_) result |= w->value();
    output_->setValue(result);
}

void XorGate::evaluate() {
    int result = 0;
    for (Wire* w : inputs_) result ^= w->value();
    output_->setValue(result);
}

void NotGate::evaluate() {
    output_->setValue(!inputs_[0]->value());
}

void NandGate::evaluate() {
    int result = 1;
    for (Wire* w : inputs_) result &= w->value();
    output_->setValue(!result);
}

void NorGate::evaluate() {
    int result = 0;
    for (Wire* w : inputs_) result |= w->value();
    output_->setValue(!result);
}

void XnorGate::evaluate() {
    int result = 0;
    for (Wire* w : inputs_) result ^= w->value();
    output_->setValue(!result);
}

void BufferGate::evaluate() {
    output_->setValue(inputs_[0]->value());
}