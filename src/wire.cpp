#include "wire.hpp"

Wire::Wire(std::string name, int value) : name_(std::move(name)), value_(value) {}

int Wire::value() const { return value_; }

void Wire::setValue(int v) { value_ = v & 1; }

const std::string& Wire::name() const { return name_; }
