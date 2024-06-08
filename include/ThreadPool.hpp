#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>


/**
 * @brief The ThreadPool class represents a pool of worker threads for parallel task execution.
 * 
 * This class manages a collection of worker threads that can execute tasks concurrently. Tasks are
 * enqueued into the thread pool and executed by available worker threads, allowing for efficient
 * utilization of system resources and parallel execution of tasks.
 * 
 * @note This class provides a mechanism for parallelizing task execution and improving system performance
 * by leveraging multiple worker threads.
 */

class ThreadPool {
public:

    /**
     * @brief Constructs a new ThreadPool object with the specified number of worker threads.
     * 
     * @param numThreads The number of worker threads to create in the thread pool.
     */
    ThreadPool(size_t numThreads);
    
    /**
     * @brief Destroys the ThreadPool object and terminates all worker threads.
     * 
     * This method joins all worker threads and cleans up associated resources upon destruction
     * of the ThreadPool object.
     */
    ~ThreadPool();

    /**
     * @brief Enqueues a task into the thread pool for execution by an available worker thread.
     * 
     * @param task The task to be executed by the thread pool. This task is represented as a 
     * std::function<void()> object.
     * 
     * This method adds the task to the task queue and notifies an available worker thread
     * to execute the task.
     */
    void enqueueTask(std::function<void()> task);

private:
    std::vector<std::thread> workers; ///< The collection of worker threads in the thread pool.
    std::queue<std::function<void()>> tasks; ///< The queue of tasks to be executed by worker threads.

    std::mutex queueMutex; ///< Mutex for thread-safe access to the task queue.
    std::condition_variable condition; ///< Condition variable for task synchronization.
    bool stop; ///< Flag indicating whether the thread pool should stop accepting new tasks.

    /**
     * @brief Worker thread function that continuously executes tasks from the task queue.
     * 
     * This method is the main function executed by each worker thread in the thread pool. It
     * continuously waits for tasks to be enqueued into the task queue and executes them.
     */
    void workerThread();
};

#endif // THREADPOOL_HPP
