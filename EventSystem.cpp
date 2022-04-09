#include "event_sys.h"
#include <iostream>

sy::event_sys::EventSystem<int> intEventSys;

void Test(int val)
{
	std::cout << "Called : " << val << std::endl;
}

class TestClass
{
public:
	TestClass()
	{
		eventID = intEventSys.Subscribe([this](int val) {
			this->SomeEvent(val);
			});
	}

	void UnsubEvent()
	{
		intEventSys.Unsubscribe(eventID);
	}

	void SomeEvent(int val)
	{
		std::cout << "Test Class Some Event called : " << this << " ," << val << " , EventID : " << eventID << std::endl;
	}

private:
	sy::event_sys::EventID eventID = sy::event_sys::INVALID_EVENT_ID;

};

int main()
{

	std::vector<sy::event_sys::EventID> eventIDs;
	eventIDs.push_back(intEventSys.Subscribe(&Test));

	//{
	//	auto scoped = intEventSys.SubscribeWithScope(&Test);
	//	voidEventSys.Unsubscribe(std::move(scoped));
	//	intEventSys.Notify(4);
	//}

	std::vector<TestClass> objects{ TestClass(), TestClass() };
	objects[0].UnsubEvent();
	intEventSys.Notify(4);

	return 0;
}