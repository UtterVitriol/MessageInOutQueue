#pragma once

#include <thread>
#include <map>
#include "MessageQueue.h"




class MyThread
{
public:
	std::thread thr;
	HANDLE hEvent;
	bool bExit;
	

	std::function<void(std::shared_ptr<MyThread>, void*)> task;

	MyThread(std::function<void(std::shared_ptr<MyThread>, void*)> task)
		:
		bExit(false),
		hEvent(CreateEventW(NULL, FALSE, FALSE, NULL)),
		task(task)
	{
	}

	int task_callback()
	{
		DWORD dwEvent = 0;

		// wait for ack

		dwEvent = WaitForSingleObject(hEvent, 3000);

		if (WAIT_OBJECT_0 == dwEvent)
		{
			if (bExit)
			{
				return 1;
			}

			return 0;
		}
		else if (WAIT_TIMEOUT == dwEvent)
		{
			// timeout
			return 2;
		}
		else
		{
			// error
			return 3;
		}

	}

private:
};


using my_thread_t = std::shared_ptr<MyThread>;


class MyThreadHandler
{
public:
	bool create(std::string id, std::function<void(my_thread_t, void*)> task, void* data)
	{
		my_thread_t p = std::make_shared<MyThread>(task);
		p->thr = std::thread([p, data] {p->task(p, data); });
		p->thr.detach();
		map[id] = p;

		return true;
	}

	bool update(std::string id, bool bExit = false)
	{
		map[id]->bExit = bExit;
		SetEvent(map[id]->hEvent);
		return true;
	}

	bool exit(std::string id)
	{
		map[id]->bExit = true;
		return true;
	}

private:
	std::map<std::string, std::shared_ptr<MyThread>> map;

};