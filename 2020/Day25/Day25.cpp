#include <iostream>

const uint64_t EX_PUB1 = 5764801;
const size_t EX_LOOPSIZE1 = 8;

const uint64_t EX_PUB2 = 17807724;
const size_t EX_LOOPSIZE2 = 11;

const uint64_t INPUT_PUB1 = 7573546;
const uint64_t INPUT_PUB2 = 17786549;

class Device
{
public:
	Device() : m_loopSize(0), m_publicKey(0) {}

	size_t publicKey() const { return m_publicKey; }

	size_t calculateLoopSize(size_t subject, size_t publicKey)
	{
		m_publicKey = publicKey;

		size_t value{ 1 };
		m_loopSize = 0;
		while (value != publicKey)
		{
			value *= subject;
			value %= 20201227;

			++m_loopSize;
		}
		return m_loopSize;
	}

	size_t transform(size_t subject) const
	{
		size_t value{ 1 };
		for (size_t i = 0; i < m_loopSize; ++i)
		{
			value *= subject;
			value %= 20201227;
		}
		return value;
	}

private:
	size_t m_loopSize;
	size_t m_publicKey;
};

int main()
{
	Device door, card;
	door.calculateLoopSize(7, INPUT_PUB1);
	card.calculateLoopSize(7, INPUT_PUB2);

	size_t key = door.transform(card.publicKey());

	std::cout << "Day25: " << key << std::endl;

}

