#include "MessageQueue.h"

Message* MessageFactory(int in)
{
	// Make unique ptr??

	return new TestMessage(in);
}