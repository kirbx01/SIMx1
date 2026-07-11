#pragma once
#include <string>
#include <vector>
#include "wire.hpp"

class Gate {
public:
    Gate(std::string name, std::vector<Wire*> inputs, Wire* output);
    virtual ~Gate() = default;

    virtual void evaluate() = 0;
    const std::string& name() const;

protected:
    std::string name_;
    std::vector<Wire*> inputs_;
    Wire* output_;
};

class AndGate : public Gate {
public:
    using Gate::Gate;
    void evaluate() override;
};

class OrGate : public Gate {
public:
    using Gate::Gate;
    void evaluate() override;
};

class XorGate : public Gate {
public:
    using Gate::Gate;
    void evaluate() override;
};

class NotGate : public Gate {
public:
    using Gate::Gate;
    void evaluate() override;
};
