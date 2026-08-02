#pragma once
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include "gate.hpp"
#include "wire.hpp"

class Circuit {
public:
    Wire* addWire(const std::string& name, int value = 0);

    template <typename GateT>
    void addGate(const std::string& name, std::vector<Wire*> inputs, Wire* output) {
        gates_.push_back(std::make_unique<GateT>(name, std::move(inputs), output));
    }

    // Gates must be added in dependency order (inputs before dependents).
    // evaluate() runs them in that order once, no cycle detection.
    void evaluate();

    Wire* wire(const std::string& name);

private:
    std::vector<std::unique_ptr<Wire>> wires_;
    std::unordered_map<std::string, Wire*> wireIndex_;
    std::vector<std::unique_ptr<Gate>> gates_;
};
