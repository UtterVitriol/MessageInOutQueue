#include <cstdio>
#include <thread>


#include "MessageQueue.h"

void test(MessageQueue *que)
{
	Sleep(2000);

	Message* ree = MessageFactory(28);

	que->QueueIn(ree);
}


int main()
{
	MessageQueue que{};
	Message* ree = MessageFactory(23);

	Message* in = MessageFactory(10);
	Message* out = MessageFactory(20);

	que.QueueIn(in);
	que.QueueOut(out);

	Message* temp = que.WaitForMessage();

	printf("Temp: %d\n", temp->GetItem());

	temp = que.WaitForMessage();
	printf("Temp: %d\n", temp->GetItem());
	std::thread b = std::thread(test, &que);

	temp = que.WaitForMessage();
	printf("End Temp: %d\n", temp->GetItem());
	b.join();
}