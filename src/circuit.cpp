#include "circuit.hpp"

Wire* Circuit::addWire(const std::string& name, int value) {
    wires_.push_back(std::make_unique<Wire>(name, value));
    Wire* w = wires_.back().get();
    wireIndex_[name] = w;
    return w;
}

void Circuit::evaluate() {
    for (auto& g : gates_) g->evaluate();
}

Wire* Circuit::wire(const std::string& name) {
    return wireIndex_.at(name);
}
