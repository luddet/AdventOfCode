#include <cassert>
#include <iostream>
#include <string>

#include "Computer.h"

Computer::Computer(const mem_container_t& memory)
	:m_ip(0),
	m_relativeBase(0),
	m_memory(memory),
	m_state(ComputerState::IDLE)
{
}

void Computer::addInput(mem_item_t input)
{
	m_input.push(input);
}

bool Computer::getOutput(mem_item_t& output)
{
	if (!m_output.empty())
	{
		output = m_output.front();
		m_output.pop();
		return true;
	}
	else
		return false;
}

bool Computer::isDone() const
{
	return m_state == ComputerState::DONE;
}

void Computer::resizeMemory(size_t newSize)
{
	constexpr mem_item_t DEFAULT_NEW_MEM_VALUE = 0;
	size_t oldSize = m_memory.size();
	if (newSize <= oldSize)
		return;
	//std::cout << "oldsize: " << oldSize << ", newSize: " << newSize << std::endl;
	newSize = std::max(newSize, 2 * oldSize);

	m_memory.resize(newSize);
	std::fill(m_memory.begin() + oldSize, m_memory.end(), DEFAULT_NEW_MEM_VALUE);
}

Computer::mem_item_t Computer::getMemValue(Mode mode, size_t p)
{
	if (p >= m_memory.size())
		resizeMemory(p + 1);
	switch (mode)
	{
		case Mode::POSITION:
			if ((size_t)m_memory[p] >= m_memory.size())
				resizeMemory(m_memory[p] + 1);
			return m_memory[m_memory[p]];
		case Mode::IMMEDIATE:
			return m_memory[p];
		case Mode::RELATIVE:
			if (m_relativeBase + m_memory[p] > m_memory.size())
				resizeMemory(m_relativeBase + m_memory[p] + 1);
			return m_memory[m_relativeBase + m_memory[p]];
		default:
			throw std::exception("Unhandled mode.");
	}
}

void Computer::setMemValue(mem_item_t value, size_t address, Mode mode)
{
	assert(mode != Mode::IMMEDIATE);
	resizeMemory(address + 1);
	switch (mode)
	{
		case Mode::POSITION:
			resizeMemory(m_memory[address] + 1);
			m_memory[m_memory[address]] = value;
			break;
		case Mode::RELATIVE:
			resizeMemory(m_relativeBase + m_memory[address] + 1);
			m_memory[m_relativeBase + m_memory[address]] = value;
			break;
	}
}

void Computer::run()
{
	mem_item_t opCode = 0;
	while (m_ip < m_memory.size())
	{
		Mode mode[3]{ Mode::POSITION };
		opCode = m_memory[m_ip] % 100;
		mode[0] = (Mode)((m_memory[m_ip] / 100) % 10);
		mode[1] = (Mode)((m_memory[m_ip] / 1000) % 10);
		mode[2] = (Mode)((m_memory[m_ip] / 10000) % 10);

		mem_item_t param1Value, param2Value;
		switch (opCode)
		{
		case 1: // ADD
			param1Value = getMemValue(mode[0], m_ip + 1);
			param2Value = getMemValue(mode[1], m_ip + 2);
			setMemValue(param1Value + param2Value, m_ip + 3, mode[2]);
			m_ip += 4;
			break;
		case 2: // MUL
			param1Value = getMemValue(mode[0], m_ip + 1);
			param2Value = getMemValue(mode[1], m_ip + 2);
			setMemValue(param1Value * param2Value, m_ip + 3, mode[2]);
			m_ip += 4;
			break;
		case 3: // READ INPUT
			if (m_input.empty())
			{
				m_state = ComputerState::WAITING_FOR_INPUT;
				return;
			}
			setMemValue(m_input.front(), m_ip + 1, mode[0]);
			m_input.pop();
			m_ip += 2;
			break;
		case 4: // OUTPUT
			param1Value = getMemValue(mode[0], m_ip + 1);
			m_output.push(param1Value);
			m_ip += 2;
			break;
		case 5: // JUMP IF TRUE
			param1Value = getMemValue(mode[0], m_ip + 1);
			param2Value = getMemValue(mode[1], m_ip + 2);
			if (param1Value != 0)
				m_ip = param2Value;
			else
				m_ip += 3;
			break;
		case 6: // JUMP IF FALSE
			param1Value = getMemValue(mode[0], m_ip + 1);
			param2Value = getMemValue(mode[1], m_ip + 2);
			if (param1Value == 0)
				m_ip = param2Value;
			else
				m_ip += 3;
			break;
		case 7: // LESS THAN
			param1Value = getMemValue(mode[0], m_ip + 1);
			param2Value = getMemValue(mode[1], m_ip + 2);
			setMemValue((param1Value < param2Value) ? 1 : 0, m_ip + 3, mode[2]);
			m_ip += 4;
			break;
		case 8: // EQUALS
			param1Value = getMemValue(mode[0], m_ip + 1);
			param2Value = getMemValue(mode[1], m_ip + 2);
			setMemValue((param1Value == param2Value) ? 1 : 0, m_ip + 3, mode[2]);
			m_ip += 4;
			break;
		case 9: // Modify relative base
			param1Value = getMemValue(mode[0], m_ip + 1);
			m_relativeBase += param1Value;
			m_ip += 2;
			break;
		case 99:
			m_state = ComputerState::DONE;
			return;
		default:
			std::string msg("Unhandled opcode: ");
			msg += std::to_string(opCode);
			throw std::exception(msg.c_str());
		}
	}
}

Computer::mem_container_t readMemory(std::istream& stream)
{
	Computer::mem_container_t mem;
	Computer::mem_item_t opCode;
	while (stream >> opCode)
	{
		mem.push_back(opCode);
		char c; stream >> c; // eat comma
	}
	return mem;
}