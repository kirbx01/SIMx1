#include <iostream>
#include <iomanip>
#include <string>
#include <bitset>
#include "circuit.hpp"

// Builds one full adder stage: sum = a^b^cin, cout = (a&b) | (cin&(a^b))
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

int main() {
    const int A = 6, B = 5;

    Circuit c;
    Wire* cin0 = c.addWire("cin0", 0);
    Wire* a[4];
    Wire* b[4];
    Wire* sum[4];
    Wire* carry[5];
    carry[0] = cin0;

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
    int carryOut = carry[4]->value();

    std::cout << "4-bit ripple-carry adder\n";
    std::cout << "A = " << A << " (" << std::bitset<4>(A) << ")\n";
    std::cout << "B = " << B << " (" << std::bitset<4>(B) << ")\n";
    std::cout << "Sum = " << result << ", Carry-out = " << carryOut << "\n";

    return 0;
}
