//
// Created by probn on 29.09.2024.
//

#ifndef SMALLVM_CMLVIRTUALMACHINE_H
#define SMALLVM_CMLVIRTUALMACHINE_H

#include "CMLTypes.h"

#define INVALIDTYPETOCAST      "Invalid type to cast!"
#define INVALIDINSTRUCTIONTYPE "Invalid instruction value type!"
#define INVALIDSTACKVALUETYPE  "Invalid popped stack value type!"
#define LOGOUTOFBOUNDS(OBJ)    std::cerr << OBJ ": Element is out of bounds!" << std::endl
#define LOGINSTERR(INST, ERR)  std::cerr << INST ": " ERR << std::endl

using CMLValue = Variant<sint8, uint8, sint16, uint16, sint32, uint32, sint64, uint64, dec32, dec64>;

enum CMLOperation : sint8
{
    HALT,
    PUSH,
    POP,
    JMP,
    JMPC,

    TOC,
    TOUC,
    TOI16,
    TOU16,
    TOI32,
    TOU32,
    TOI64,
    TOU64,
    TOF32,
    TOF64,

    ADD,
    SUB,
    MUL,
    DIV,

    CMPE,
    CMPNE,
    CMPG,
    CMPL,
    CMPGE,
    CMPLE,

    PRINT
};

struct CMLInstruction
{
    CMLOperation m_operation;
    CMLValue m_value;
};

struct CMLASM
{
    CMLInstruction *m_instructions;
    uint64 m_size;
};

class CMLStack
{
    CMLValue *m_data;
    sint32 m_end;
public:
    explicit CMLStack(uint64 size);

    ~CMLStack();

    uint64 m_size;

    void Push(CMLValue entity);

    CMLValue Pop();
};

class CMLVirtualMachine
{
    CMLStack m_stack;
    CMLASM m_asm;

    template<Type T>
    sint32 TryCast(CMLValue &value) {
        if (std::is_convertible_v<T, CMLValue>) {
            value = std::visit([](auto x) {
                return static_cast<T>(x);
            }, value);

            return 1;
        } else {
            LOGINSTERR ("CAST", INVALIDTYPETOCAST);
            Dispose();
            return 0;
        }
    }

public:
    explicit CMLVirtualMachine(uint64 stackSize, CMLASM _asm);

    ~CMLVirtualMachine();

    void Run(sint32 entryPoint = 0);

    void Dispose() const;
};


#endif //SMALLVM_CMLVIRTUALMACHINE_H
