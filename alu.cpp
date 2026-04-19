/*
    4-bit ALU Project v1.1
    Author: Huỳnh Quang Tuấn
    Date: 17/04/2026

    Features:
    - 4-bit addition
    - 4-bit subtraction using 2's complement
    - bitwise AND
    - bitwise OR
    - 4-bit multiplication (decimal result)

    Notes:
    - Inputs are accepted as 4 binary bits: A[0..3], B[0..3]
    - A[0] and B[0] are treated as the least significant bits
*/

#include <iostream>
#include <string>
#include <array>
#include <algorithm>

using namespace std;

static constexpr int WIDTH = 4;

/* Convert a 4-bit binary array to decimal.
   bits[0] is LSB, bits[3] is MSB. */
int bitsToInt(const array<int, WIDTH>& bits) {
    int value = 0;
    for (int i = 0; i < WIDTH; ++i) { 
        value += (bits[i] & 1) << i;
    }
    return value;
}

/* Convert decimal value to a 4-bit binary array.
   Only the lowest 4 bits are kept. */
array<int, WIDTH> intToBits(int value) {
    array<int, WIDTH> bits{};
    for (int i = 0; i < WIDTH; ++i) {
        bits[i] = (value >> i) & 1;
    }
    return bits;
}

/* Print a 4-bit binary value from MSB to LSB. */
void printBits(const array<int, WIDTH>& bits) {
    for (int i = WIDTH - 1; i >= 0; --i) {
        cout << bits[i];
    }
}

/* Base class for a 2-input logic gate. */
class LogicGate {
protected:
    int in1;
    int in2;

public:
    LogicGate() : in1(0), in2(0) {}

    LogicGate(int a, int b) : in1(a & 1), in2(b & 1) {}

    /* Set binary inputs for the gate. */
    void setInput(int a, int b) {
        in1 = a & 1;
        in2 = b & 1;
    }

    virtual int output() const = 0;
    virtual ~LogicGate() = default;
};

class XorGate : public LogicGate {
public:
    int output() const override {
        return in1 ^ in2;
    }
};

class AndGate : public LogicGate {
public:
    int output() const override {
        return in1 & in2;
    }
};

class OrGate : public LogicGate {
public:
    int output() const override {
        return in1 | in2;
    }
};

/* Binary shift-and-add multiplier for small unsigned values. */
class Multiplier {
private:
    int multiplicand;
    int multiplierValue;

public:
    Multiplier() : multiplicand(0), multiplierValue(0) {}

    void setInput(int a, int b) {
        multiplicand = a;
        multiplierValue = b;
    }

    int output() const {
        int result = 0;
        int tempMultiplicand = multiplicand;
        int tempMultiplier = multiplierValue;

        while (tempMultiplier > 0) {
            if (tempMultiplier & 1) {
                result += tempMultiplicand;
            }
            tempMultiplicand <<= 1;
            tempMultiplier >>= 1;
        }

        return result;
    }
};

/* Half adder: sum = A XOR B, carry = A AND B. */
class HalfAdder {
private:
    XorGate sumGate;
    AndGate carryGate;

public:
    void process(int a, int b) {
        sumGate.setInput(a, b);
        carryGate.setInput(a, b);
    }

    int getSum() const {
        return sumGate.output();
    }

    int getCarry() const {
        return carryGate.output();
    }
};

/* Full adder built from two half adders and one OR gate. */
class FullAdder {
private:
    HalfAdder ha1;
    HalfAdder ha2;
    OrGate carryGate;

    int sum;
    int carry;

public:
    FullAdder() : sum(0), carry(0) {}

    void process(int a, int b, int carryIn) {
        ha1.process(a, b);
        int s1 = ha1.getSum();
        int c1 = ha1.getCarry();

        ha2.process(s1, carryIn);
        sum = ha2.getSum();
        int c2 = ha2.getCarry();

        carryGate.setInput(c1, c2);
        carry = carryGate.output();
    }

    int getSum() const {
        return sum;
    }

    int getCarry() const {
        return carry;
    }
};

/* 4-bit ripple-carry adder. */
class RippleCarryAdder {
private:
    FullAdder adders[WIDTH];
    array<int, WIDTH> sumBits{};
    int carryOut;

public:
    RippleCarryAdder() : carryOut(0) {}

    void process(const array<int, WIDTH>& a, const array<int, WIDTH>& b, int carryIn) {
        int carry = carryIn;

        for (int i = 0; i < WIDTH; ++i) {
            adders[i].process(a[i], b[i], carry);
            sumBits[i] = adders[i].getSum();
            carry = adders[i].getCarry();
        }

        carryOut = carry;
    }

    array<int, WIDTH> getSumBits() const {
        return sumBits;
    }

    int getCarryOut() const {
        return carryOut;
    }

    void display() const {
        cout << "Result: " << carryOut << " ";
        printBits(sumBits);
        cout << '\n';
    }
};

/* Adder/Subtractor:
   mode = 0 -> A + B
   mode = 1 -> A - B (A + two's complement of B) */
class AdderSubtractor {
private:
    RippleCarryAdder rca;

public:
    void process(const array<int, WIDTH>& a, const array<int, WIDTH>& b, int mode) {
        array<int, WIDTH> bModified{};

        for (int i = 0; i < WIDTH; ++i) {
            bModified[i] = b[i] ^ (mode & 1);
        }

        rca.process(a, bModified, mode & 1);
    }

    void display() const {
        rca.display();
    }
};

/* Main ALU controller. */
class ALU {
private:
    AdderSubtractor adderSubtractor;
    Multiplier multiplier;
    AndGate andGate;
    OrGate orGate;

public:
    void execute(const array<int, WIDTH>& a, const array<int, WIDTH>& b, const string& opcode) {
        int aValue = bitsToInt(a);
        int bValue = bitsToInt(b);

        if (opcode == "ADD") {
            adderSubtractor.process(a, b, 0);
            adderSubtractor.display();
        }
        else if (opcode == "SUB") {
            adderSubtractor.process(a, b, 1);
            adderSubtractor.display();
        }
        else if (opcode == "AND") {
            int result = aValue & bValue;
            cout << "Result: " << result << '\n';
        }
        else if (opcode == "OR") {
            int result = aValue | bValue;
            cout << "Result: " << result << '\n';
        }
        else if (opcode == "MULTIPLY") {
            multiplier.setInput(aValue, bValue);
            cout << "Result: " << multiplier.output() << '\n';
        }
        else {
            cout << "Invalid opcode. Supported opcodes: ADD, SUB, AND, OR, MULTIPLY\n";
        }
    }
};

int main() {
    array<int, WIDTH> A{};
    array<int, WIDTH> B{};

    cout << "Enter 4 bits for A (0 or 1), LSB to MSB: ";
    for (int i = WIDTH - 1; i >= 0; --i) {
        cin >> A[i];
        A[i] = A[i] & 1;
    }

    cout << "Enter 4 bits for B (0 or 1), LSB to MSB: ";
    for (int i = WIDTH - 1; i >= 0; --i) {
        cin >> B[i];
        B[i] = B[i] & 1;
    }

    string opcode;
    cout << "Enter opcode (ADD, SUB, AND, OR, MULTIPLY): ";
    cin >> opcode;

    ALU myALU;
    myALU.execute(A, B, opcode);

    return 0;
}