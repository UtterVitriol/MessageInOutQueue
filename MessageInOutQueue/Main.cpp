#include <cstdio>

#include <thread>
#include <map>
#include "MessageQueue.h"
#include "MyThread.h"


void do_thing(MyQueue<int>* que)
{
	Sleep(3000);
	que->Enqueue(69);
}

void in(int lel)
{
	printf("In: %d\n", lel);
}

void out(int lel)
{
	printf("Out: %d\n", lel);
}


void test_task(my_thread_t thr, void* data)
{
	// do thing
	int myNum = *(int*)data;
	int iRes = 0;

	for (;;)
	{

		printf("task: %d\n", myNum);

		iRes = thr->task_callback();

		switch (iRes)
		{
		case 0:
			// No erro, continue;
			break;

		case 1:	// exit
			printf("Exit: %d\n", myNum);
		case 2: // timeout
			printf("Timeout: %d\n", myNum);
		case 3: // error

			// cleanup
			return;
		}
	}
}


int main()
{

	MyThreadHandler tHand;
	int one = 1;
	int two = 2;
	tHand.create("one", test_task, &one);
	tHand.create("two", test_task, &two);
	
	Sleep(2000);

	tHand.update("one", false);
	tHand.exit("one");

	Sleep(2000);

	tHand.update("one", false);


	Sleep(20000);

	/*InOutQueue<int, std::function<void(int)>, std::function<void(int)>> a{
		[](int lel) {printf("In: %d\n", lel); },
			[](int lel) {printf("Out: %d\n", lel); }
	};


	std::map<std::string, int> map;




	std::thread t2 = std::thread([&a] {for (int x = 1; x < 101; x++) { a.QueueOut(x); }});
	std::thread t1 = std::thread([&a] {for (int x = 1; x < 101; x++) { a.QueueIn(x); }});

	a.WaitForMessage();*/

}