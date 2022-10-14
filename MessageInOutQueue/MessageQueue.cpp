#include "MessageQueue.h"

MessageQueue::MessageQueue()
	:
	hInEvent(CreateEventW(NULL, FALSE, FALSE, NULL)),
	hOutEvent(CreateEventW(NULL, FALSE, FALSE, NULL))
{
	hEvents[0] = hInEvent;
	hEvents[1] = hOutEvent;
}

void MessageQueue::QueueIn(Message* Message)
{
	printf("Queueing Job\n");
	{
		std::unique_lock<std::mutex> lock(in_mutex);
		MessageIn.push(Message);
	}

	in_mutex_condition.notify_one();
	SetEvent(hInEvent);
}

void MessageQueue::QueueOut(Message* Message)
{
	printf("Queueing Job\n");
	{
		std::unique_lock<std::mutex> lock(out_mutex);
		MessageOut.push(Message);
	}

	out_mutex_condition.notify_one();
	SetEvent(hOutEvent);
}

Message* MessageQueue::WaitForMessage()
{
	Message* msg = nullptr;
	DWORD dwEvent = 0;
	dwEvent = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
	if (WAIT_OBJECT_0 == dwEvent) {
		std::unique_lock<std::mutex> lock(in_mutex);
		msg = MessageIn.front();
		MessageIn.pop();

		return msg;
	}
	else if (WAIT_OBJECT_0 + 1 == dwEvent) {
		std::unique_lock<std::mutex> lock(out_mutex);
		msg = MessageOut.front();
		MessageOut.pop();

		return msg;
	}
	else {
		printf("REEE: %ul\n", dwEvent);
	}
}

Message* MessageFactory(int in)
{
	// Make unique ptr??

	return new TestMessage(in);
}