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
	
}