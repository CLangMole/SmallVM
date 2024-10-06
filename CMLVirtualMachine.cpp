//
// Created by probn on 29.09.2024.
//

#include <iostream>
#include "CMLVirtualMachine.h"

#define APPLY_OPERATION(AVAL, BVAL, OP)                                            \
m_stack.Push(std::visit([](auto x, auto y) -> CMLValue {                           \
    if constexpr(!std::is_same_v<decltype(x), decltype(y)>)                        \
    {                                                                              \
        throw;                                                                     \
    }                                                                              \
    else                                                                           \
    {                                                                              \
        return x OP y;                                                             \
    }                                                                              \
}, (AVAL), (BVAL)))                                                                \


CMLStack::CMLStack(uint64 size) : m_size(size), m_end(0) {
    m_data = new CMLValue[size];
}

CMLStack::~CMLStack() {
    delete[] m_data;
}

void CMLStack::Push(CMLValue entity) {
    m_data[m_end] = entity;
    m_end++;
}

CMLValue CMLStack::Pop() {
    m_end--;
    return m_data[m_end];
}

CMLVirtualMachine::CMLVirtualMachine(uint64 stackSize, CMLASM _asm) : m_stack(stackSize), m_asm(_asm) {}

CMLVirtualMachine::~CMLVirtualMachine() {
    delete[] m_asm.m_instructions;
}

void CMLVirtualMachine::Run(sint32 entryPoint) {
    for (sint32 i = entryPoint; i < m_asm.m_size; ++i) {
        auto &instruction = m_asm.m_instructions[i];

        switch (instruction.m_operation) {
            case HALT:{
                Dispose();
                return;
            }
            case PUSH: {
                m_stack.Push(instruction.m_value);
                break;
            }
            case POP: {
                m_stack.Pop();
                break;
            }
            case JMP: {
                auto instructionValue = instruction.m_value;

                i = std::get<int>(instructionValue);

                std::visit([&i, this](const auto t) {
                    if (std::is_same_v<decltype(t), sint32>) {
                        i = static_cast<uint8>(t);
                    } else {
                        LOGINSTERR ("JMP", INVALIDINSTRUCTIONTYPE);
                        Dispose();
                        return;
                    }
                }, instructionValue);

                break;
            }
            case JMPC: {
                auto instructionValue = instruction.m_value;

                std::visit([&](const auto t) {
                    if (std::is_same_v<decltype(t), sint32>) {
                        auto popped = m_stack.Pop();

                        std::visit([&i, t](auto x) {
                            if (std::is_same_v<decltype(x), uint8> ||
                                std::is_same_v<decltype(x), sint8> ||
                                std::is_same_v<decltype(x), sint32>) {
                                i = static_cast<uint8>(t);
                            }
                        }, popped);
                    } else {
                        LOGINSTERR ("JMP", INVALIDINSTRUCTIONTYPE);
                        Dispose();
                        return;
                    }
                }, instructionValue);

                break;
            }
            case TOC: {
                CMLValue value = m_stack.Pop();

                if (TryCast<sint8>(value)){
                    m_stack.Push(value);
                }

                break;
            }
            case TOUC: {
                CMLValue value = m_stack.Pop();

                if (TryCast<uint8>(value)){
                    m_stack.Push(value);
                }

                break;
            }
            case TOI16: {
                CMLValue value = m_stack.Pop();

                if (TryCast<sint16>(value)){
                    m_stack.Push(value);
                }

                break;
            }
            case TOU16: {
                CMLValue value = m_stack.Pop();

                if (TryCast<uint16>(value)){
                    m_stack.Push(value);
                }

                break;
            }
            case TOI32: {
                CMLValue value = m_stack.Pop();

                if (TryCast<sint32>(value)){
                    m_stack.Push(value);
                }

                break;
            }
            case TOU32: {
                CMLValue value = m_stack.Pop();

                if (TryCast<uint32>(value)){
                    m_stack.Push(value);
                }

                break;
            }
            case TOI64: {
                CMLValue value = m_stack.Pop();

                if (TryCast<sint64>(value)){
                    m_stack.Push(value);
                }

                break;
            }
            case TOU64: {
                CMLValue value = m_stack.Pop();

                if (TryCast<uint64>(value)){
                    m_stack.Push(value);
                }

                break;
            }
            case TOF32: {
                CMLValue value = m_stack.Pop();

                if (TryCast<dec32>(value)){
                    m_stack.Push(value);
                }

                break;
            }
            case TOF64: {
                CMLValue value = m_stack.Pop();

                if (TryCast<dec64>(value)){
                    m_stack.Push(value);
                }

                break;
            }
            case ADD: {
                auto a = m_stack.Pop();
                auto b = m_stack.Pop();

                APPLY_OPERATION(a, b, +);

                break;
            }
            case SUB: {
                auto a = m_stack.Pop();
                auto b = m_stack.Pop();

                APPLY_OPERATION(a, b, -);

                break;
            }
            case MUL: {
                auto a = m_stack.Pop();
                auto b = m_stack.Pop();

                APPLY_OPERATION(a, b, *);

                break;
            }
            case DIV: {
                auto a = m_stack.Pop();
                auto b = m_stack.Pop();

                APPLY_OPERATION(a, b, /);

                break;
            }
            case CMPE: {
                auto a = m_stack.Pop();
                auto b = m_stack.Pop();

                APPLY_OPERATION (a, b, ==);

                break;
            }
            case CMPNE: {
                auto a = m_stack.Pop ();
                auto b = m_stack.Pop ();

                APPLY_OPERATION (a, b, !=);

                break;
            }
            case CMPG: {
                auto a = m_stack.Pop ();
                auto b = m_stack.Pop ();

                APPLY_OPERATION (a, b, >);

                break;
            }
            case CMPL: {
                auto a = m_stack.Pop ();
                auto b = m_stack.Pop ();

                APPLY_OPERATION (a, b, <);

                break;
            }
            case CMPGE: {
                auto a = m_stack.Pop ();
                auto b = m_stack.Pop ();

                APPLY_OPERATION (a, b, >=);

                break;
            }
            case CMPLE: {
                auto a = m_stack.Pop ();
                auto b = m_stack.Pop ();

                APPLY_OPERATION (a, b, <=);

                break;
            }
            case PRINT: {
                std::visit([](const auto &x) { std::cout << x; }, m_stack.Pop());
                std::flush(std::cout);
            }
        }
    }
}

void CMLVirtualMachine::Dispose() const {
    delete[] m_asm.m_instructions;
}
