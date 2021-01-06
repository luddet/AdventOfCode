#pragma once
#include <vector>
#include <queue>

class Computer
{
public:
	using mem_item_t = long long;
	using mem_container_t = std::vector<mem_item_t>;
	enum class ComputerState
	{
		IDLE,
		WAITING_FOR_INPUT,
		DONE,
	};

	Computer(const mem_container_t& memory);
	void addInput(mem_item_t input);
	bool getOutput(mem_item_t& output);
	ComputerState getState() const { return m_state; }
	bool isDone() const;
	void run();

private:
	enum class Mode
	{
		POSITION = 0,
		IMMEDIATE = 1,
		RELATIVE = 2
	};

	mem_item_t getMemValue(Mode mode, size_t address);
	void setMemValue(mem_item_t value, size_t address, Mode mode);
	void resizeMemory(size_t newSize);

	size_t				m_ip; // instruction pointer
	size_t				m_relativeBase;
	mem_container_t	m_memory;
	std::queue<mem_item_t>		m_input;
	std::queue<mem_item_t>		m_output;
	ComputerState		m_state;
};
