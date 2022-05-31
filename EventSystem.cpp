#include "event_sys.h"
#include <iostream>
using namespace sy;

auto intEventSys = EventSystem<int>::Create();

void Test(int val)
{
	std::cout << "Called : " << val << std::endl;
}

class TestClass
{
public:
	TestClass()
	{
		event = std::move(intEventSys->Subscribe([this](int val) {
			this->SomeEvent(val);
			}));
	}

	void UnsubEvent()
	{
		event.Unsubscribe();
	}

	void SomeEvent(int val)
	{
		std::cout << "Called : Test Class Some Event called : " << this << "; " << val << "; EventID : " << static_cast<uint64_t>(event.ID()) << std::endl;
	}

private:
	Event<int> event;

};

int main()
{
	std::cout << "* Print Event case 1" << std::endl;
	std::vector<Event<int>> events;
	events.emplace_back(intEventSys->Subscribe(&Test));
	intEventSys->Notify(0);
	std::cout << std::endl;

	{
		auto scoped = intEventSys->Subscribe(&Test);
		std::cout << "* Print Event case 2-1" << std::endl;
		intEventSys->Notify(1);
		std::cout << std::endl;
	}
	std::cout << "* Print Event case 2-2" << std::endl;
	intEventSys->Notify(2);
	std::cout << std::endl;

	{
		// std::vector<TestClass> objects; 
		// << This case will make undefined behaviour cause of capture of this pointer.

		std::vector<std::unique_ptr<TestClass>> objects;
		objects.emplace_back(std::make_unique<TestClass>());
		objects.emplace_back(std::make_unique<TestClass>());

		std::cout << "* Print Event case 3" << std::endl;
		intEventSys->Notify(3);
		std::cout << std::endl;
	}

	std::cout << "* Print Event case 4" << std::endl;
	intEventSys->Notify(4);

	return 0;
}