#pragma once
// https://stackoverflow.com/questions/15752659/thread-pooling-in-c11

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <functional>
#include <Windows.h>

//handler
// single message queue

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

template <typename T>
class MyQueue
{
public:
	void Enqueue(T Data)
	{
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Queue.push(Data);
		}

		m_Condition.notify_one();
	}

	T Dequeue()
	{
		T Data;
		{
			std::unique_lock<std::mutex> lock(m_Mutex);

			m_Condition.wait(lock, [this] {
				return !m_Queue.empty();
				});
			Data = m_Queue.front();
			m_Queue.pop();
		}

		return Data;
	}

	DWORD Size() { return m_Queue.size(); }


private:
	std::mutex  m_Mutex;
	std::condition_variable m_Condition;
	std::queue<T> m_Queue;

};

template <typename T,typename F1, typename F2>
class InOutQueue
{
public:
	InOutQueue(F1 InHandler, F2 OutHandler)
		:
		m_InHandler(InHandler),
		m_OutHandler(OutHandler)

	{
		m_hEvents[0] = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
		m_hEvents[1] = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
	}

	void  QueueIn(T Data)
	{
		m_In.Enqueue(Data);
		ReleaseSemaphore(m_hEvents[In], 1, NULL);
	}

	void QueueOut(T Data)
	{
		m_Out.Enqueue(Data);
		ReleaseSemaphore(m_hEvents[Out], 1, NULL);
	}

	void WaitForMessage()
	{
		DWORD dwEvent = 0;
		for (;;) {
			dwEvent = WaitForMultipleObjects(2, m_hEvents, FALSE, INFINITE);

			if (WAIT_OBJECT_0 == dwEvent) {
				// Thread
				m_InHandler(m_In.Dequeue());
				
			}
			else if (WAIT_OBJECT_0 + 1 == dwEvent) {
				// Thread
				m_OutHandler(m_Out.Dequeue());
			}
			else {
				printf("REEE: %ul\n", dwEvent);
			}
		}
	}

private:
	enum m_Events{In, Out};
	HANDLE m_hEvents[2];
	MyQueue<T> m_In;
	MyQueue<T> m_Out;
	F1 m_InHandler;
	F2 m_OutHandler;
};