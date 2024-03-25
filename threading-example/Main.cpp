#include <iostream>
#include <thread>
#include <vector>

// Function to be executed by each thread
void threadFunction(int threadId)
{
    std::cout << "Thread " << threadId << " started.\n";
    // Simulating some work
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Thread " << threadId << " finished.\n";
}

int main()
{
    // Vector to hold the thread objects
    std::vector<std::thread> threads;

    // Start 10 threads
    for (int i = 0; i < 10; ++i)
    {
        threads.emplace_back(threadFunction, i);
    }

    // Join all threads to wait for them to finish
    for (auto &thread : threads)
    {
        thread.join();
    }

    std::cout << "All threads have finished.\n";

    return 0;
}
