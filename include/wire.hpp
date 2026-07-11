#pragma once
#include <string>

class Wire {
public:
    explicit Wire(std::string name, int value = 0);

    int value() const;
    void setValue(int v);
    const std::string& name() const;

private:
    std::string name_;
    int value_;
};
