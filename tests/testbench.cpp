#include <iostream>
#include <string>
#include "circuit.hpp"

void buildFullAdder(Circuit& c, const std::string& tag,
                    Wire* a, Wire* b, Wire* cin, Wire* sum, Wire* cout) {
    Wire* axb = c.addWire("axb_" + tag);
    c.addGate<XorGate>("xor1_" + tag, {a, b}, axb);
    c.addGate<XorGate>("xor2_" + tag, {axb, cin}, sum);

    Wire* aAndB = c.addWire("aAndB_" + tag);
    c.addGate<AndGate>("and1_" + tag, {a, b}, aAndB);

    Wire* axbAndCin = c.addWire("axbAndCin_" + tag);
    c.addGate<AndGate>("and2_" + tag, {axb, cin}, axbAndCin);

    c.addGate<OrGate>("or1_" + tag, {aAndB, axbAndCin}, cout);
}

int runAdder(int A, int B) {
    Circuit c;
    Wire* a[4];
    Wire* b[4];
    Wire* sum[4];
    Wire* carry[5];
    carry[0] = c.addWire("cin0", 0);

    for (int i = 0; i < 4; ++i) {
        a[i] = c.addWire("a" + std::to_string(i), (A >> i) & 1);
        b[i] = c.addWire("b" + std::to_string(i), (B >> i) & 1);
        sum[i] = c.addWire("sum" + std::to_string(i));
        carry[i + 1] = c.addWire("carry" + std::to_string(i + 1));
        buildFullAdder(c, std::to_string(i), a[i], b[i], carry[i], sum[i], carry[i + 1]);
    }

    c.evaluate();

    int result = 0;
    for (int i = 0; i < 4; ++i) result |= (sum[i]->value() << i);
    result |= (carry[4]->value() << 4);
    return result;
}

int main() {
    int failures = 0;
    for (int A = 0; A < 16; ++A) {
        for (int B = 0; B < 16; ++B) {
            int expected = A + B;
            int got = runAdder(A, B);
            if (got != expected) {
                std::cout << "FAIL: " << A << " + " << B
                          << " expected " << expected << " got " << got << "\n";
                ++failures;
            }
        }
    }

    if (failures == 0) {
        std::cout << "All 256 combinations passed\n";
        return 0;
    }
    std::cout << failures << " combinations failed\n";
    return 1;
}
