# 4-bit ALU Simulator in C++



A 4-bit Arithmetic Logic Unit (ALU) simulator implemented in C++ using fundamental digital design principles.
This project models core hardware components such as logic gates, adders, and a ripple carry architecture.

---

##  Features

- 4-bit ADD and SUB (using 2's complement)
- Bitwise AND / OR operations
- Integer multiplication
- Modular design using logic gate abstraction
- Ripple Carry Adder (RCA) implementation

---

##  Supported Operations

- `ADD`
- `SUB`
- `AND`
- `OR`
- `MULTIPLY`

---

##  Concepts Applied

- Logic Gates (AND, OR, XOR)
- Half Adder & Full Adder
- Ripple Carry Adder (RCA)
- Bit-level manipulation
- Object-Oriented Programming (C++)

---

##  Architecture Overview

The ALU is built hierarchically:

- Basic logic gates form half adders
- Half adders form full adders
- Full adders are chained into a 4-bit Ripple Carry Adder
- Higher-level modules integrate arithmetic and logic operations

This structure mimics real-world digital circuit design.

---

##  Input Format

- Users enter two 4-bit binary numbers
- Input is provided in **MSB → LSB order** (natural reading format)
- Each bit must be either `0` or `1`

### Example:

```text
A = 0101
B = 0011
```

---

##  How to Run

### Compile:

```bash
g++ alu.cpp -o alu
```

### Execute:

```bash
./alu
```

---

##  Example Output

```text
Enter A: 0101
Enter B: 0011
Operation: ADD
Result: 1000
```

---

##  Project Structure

```
.
|── README.md
|── alu.cpp
```

---

##  Purpose

This project is part of a learning path in:

- Digital Design
- Computer Architecture
- Hardware-oriented programming
- Foundational preparation for Digital Verification workflows ***

---

##  Author

Huỳnh Quang Tuấn
University of Science, VNU-HCM (HCMUS)
