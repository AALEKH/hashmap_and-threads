#include <iostream>
#include <thread>
#include <list>
#include <chrono>

#include <algorithm>
#include <mutex>

using namespace std;

std::condition_variable m_condVar;

std::list<int>myList;

bool m_bDataLoaded;

std::mutex m_mutex;

void addToList(int max, int interval)
{
	while(true){
		std::lock_guard<std::mutex> guard(m_mutex);
		for (int i = 0; i < max; i++) {
			if( (i % interval) == 0) myList.push_back(i);
		}
		m_bDataLoaded = true;
		m_condVar.notify_one();
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
}

bool isDataLoaded() {
    return m_bDataLoaded;
}

void printList()
{
	while(true) {
		// std::cout << "Reached here" << std::endl;
		std::unique_lock<std::mutex> mlock(m_mutex);
		std::cout << "Entered Wait" << std::endl;
		m_condVar.wait(mlock, std::bind(&isDataLoaded));
		std::cout << "Skipped wait" << std::endl;
		for (auto itr = myList.begin(), end_itr = myList.end(); itr != end_itr; ++itr ) {
			cout << *itr << ",";
		}
		m_bDataLoaded = false;
		// std::cout << "Iterate Throught it" << std::endl;
		// m_condVar.wait(mlock, std::bind(&isDataLoaded));
	}

}

int main()
{
	int max = 100;

	std::thread t3(printList);
	std::thread t1(addToList, max, 1);
	std::thread t2(addToList, max, 10);

	t1.join();
	t2.join();
	t3.join();

	return 0;
}