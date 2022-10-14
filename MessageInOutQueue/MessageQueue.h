#pragma once
// https://stackoverflow.com/questions/15752659/thread-pooling-in-c11

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <functional>
#include <Windows.h>


class Message
{
public:
	virtual int GetItem() = 0;
private:
};


class TestMessage : public Message
{
public:
	TestMessage(int in) : data(0) { data = in; };
	int GetItem() override { return data; };
private:
	int data;
};

Message* MessageFactory(int in);

class MessageQueue
{
public:
	MessageQueue();
	void QueueIn(Message* Message);
	void QueueOut(Message* Message);

	Message* WaitForMessage(void);

private:


	std::mutex in_mutex;
	std::condition_variable in_mutex_condition;

	std::mutex out_mutex;
	std::condition_variable out_mutex_condition;

	HANDLE hInEvent;
	HANDLE hOutEvent;

	HANDLE hEvents[2];

	std::queue<Message*> MessageIn;
	std::queue<Message*> MessageOut;

};
