#include <cstdio>

#include "MessageQueue.h"


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

	temp = que.WaitForMessage();
}