#ifndef _MANDELBROTWORKQUEUE_
#define _MANDELBROTWORKQUEUE_

#include <mutex>
#include <tuple>

class WorkQueue {
public:
    WorkQueue();
    ~WorkQueue();

    // Set total number of tasks per iteration.
    void setTaskCount(unsigned int count);

    // Set task length.
    void setTaskLength(unsigned int length);

    // Get a tast from the queue.
    // If there are no tasks, returns -1.
    std::tuple<int, unsigned int> getTask();

private:
    unsigned int nextTask;
    unsigned int taskCount;
    unsigned int taskLength;

    // Just one mutex and lock_guards for thread safety.
    std::mutex accessMutex;
};

#endif
