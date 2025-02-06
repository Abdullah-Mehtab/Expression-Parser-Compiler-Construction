## Expression Parser with Assembly Code Generation  

### 📌 Overview  
This project is a **Compiler Construction** assignment that implements a three-stage **Expression Parser** using **Lexical Analysis, Syntax Analysis (Operator Precedence Parsing), and Assembly Code Generation**.  

It processes **arithmetic expressions** from the command line, validates them, and generates equivalent **assembly code** for evaluation. The project consists of **three tasks**, each building on the previous one.  

---

## 📜 Project Tasks  

### ✅ **Task 1: Lexical Analysis**  
The first task performs **lexical analysis** by identifying and categorizing tokens from an input arithmetic expression.  

### 🔹 Features  
- Accepts an **expression** as input via command-line arguments.  
- Identifies **identifiers** (variable names), **operators** (`+`, `-`, `*`, `/`), and **sentinel (`$`)** symbols.  
- Outputs recognized **tokens** along with their types.  
- Detects and reports **invalid characters**.  

### 🖥️ **Example Execution**  
```bash
$ ./Project_Task1 "a+b$"
Program finds following tokens in the expression:
Expression received: a+b$
int literal found: a
Arithmetic operator: +
int literal found: b
Sentinel: $
```
---
### ✅ **Task 2: Syntax Analysis (Operator Precedence Parsing)**  
This task extends **Task 1** by implementing **operator precedence parsing** using a **stack-based approach** to validate expressions.  

### 🔹 Features  
- Ensures **correct syntax** based on the given **Context-Free Grammar (CFG)**.  
- Implements **left recursion elimination** to handle operator precedence correctly.  
- Maintains a **stack implementation table** to show parsing steps.  
- Accepts user **input values for identifiers** to compute expression results.  

### 🖥️ **Example Execution**  
```bash
$ ./Project_Task2 "a+b$"
Program finds following tokens in the expression:
Expression received: a+b$
Enter integer values of the following identifiers:
Value of a: 2
Value of b: 3

The stack implementation table for operator precedence parser:
Stack                Input                Action
$                   a+b$                 Push
$a$                 +b$                   Push
$+a$                b$                    Push
$b+a$               $                     Push
$b$                 $                     Pop
$                   $                     Accepted

The output of the given expression is: 5
```
---
### ✅ **Task 3: Assembly Code Generation**  
This final task **extends Task 2** by generating equivalent **assembly code** during parsing.  

### 🔹 Features  
- **Syntax-Directed Translation (SDT)** to generate assembly instructions dynamically.  
- Uses **registers** (`R0`, `R1`, etc.) for temporary storage.  
- Implements **efficient memory reuse** for intermediate calculations.  
- Generates an **assembly file (`assembly_code.asm`)** containing translation output.  

### 🖥️ **Example Execution**  
```bash
$ ./Project_Task3 "a+b$"
Program finds following tokens in the expression:
Expression received: a+b$
Enter integer values of the following identifiers:
Value of a: 4
Value of b: 6

The output of the given expression is: 10

Generated Assembly Code:
LI R0, a
LI R1, b
ADD R2, R0, R1
```
*(Stored in `assembly_code.asm`)*  

---
## 📂 File Structure  

```
📦 Expression Parser Project
 ┣ 📜 Project_Task1.c      # Lexical Analysis Implementation
 ┣ 📜 Project_Task2.c      # Syntax Analysis with Operator Precedence Parsing
 ┣ 📜 Project_Task3.c      # Assembly Code Generation via Syntax-Directed Translation
 ┣ 📜 assembly_code.asm    # Generated Assembly Code Output
 ┣ 📜 README.md            # Project Documentation (This file)
 ┣ 📜 Project Report.docx  # Detailed Project Report
```

---
## 🛠️ **How to Run the Project**  

### ✅ **Compile the Programs**  
Each task can be compiled separately:  
```bash
gcc Project_Task1.c -o Project_Task1
gcc Project_Task2.c -o Project_Task2
gcc Project_Task3.c -o Project_Task3
```

### ✅ **Run with Input Expressions**  
```bash
./Project_Task1 "a+b$"
./Project_Task2 "a+b$"
./Project_Task3 "a+b$"
```

---
## 📖 **Technical Details**  

### 🔹 **Token Classification**
| Token Type  | Description |
|------------|------------|
| **IDENTIFIER** | Variable name (e.g., `a`, `b`) |
| **OPERATOR** | Arithmetic operations (`+`, `-`, `*`, `/`) |
| **SENTINEL** | Marks the end of expression (`$`) |
| **INVALID** | Unrecognized characters |

### 🔹 **Parsing Approach (Operator Precedence Parsing)**  
- Uses **CFG rules** to ensure correct syntax.  
- Eliminates **left recursion** for proper parsing.  
- Maintains **operator precedence** (`*, /` > `+, -`).  

### 🔹 **Assembly Code Generation**  
- Uses **Syntax-Directed Translation (SDT)** to produce assembly.  
- Allocates **registers dynamically** (`R0`, `R1`, etc.).  
- Optimizes **register reuse** for efficiency.  
- Generates an `assembly_code.asm` file.  

---
## ⚠️ Error Handling  
- **Lexical Errors**: Invalid characters in the input.  
- **Syntax Errors**: Incorrect operator placements, unmatched identifiers.  
- **Division by Zero**: Prevents invalid calculations.  
- **Stack Overflow/Underflow**: Properly managed in parsing.  

---
## 📌 Conclusion  
This project successfully implements **Lexical Analysis, Operator Precedence Parsing, and Assembly Code Generation** for arithmetic expressions. It provides a **step-by-step parsing table** and **assembly instructions**, making it a complete **Compiler Construction** project. 🎯

---

## 🤝 **Contributors**  
👤 **Abdullah Mehtab** (241607845)  

🚀 **Submitted for COMP-451 (Compiler Construction) @ Forman Christian College**
