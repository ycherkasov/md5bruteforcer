#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <algorithm>


/// @Thread-pool class
class thread_pool {
public:

    /// @brief Create thred pool with passed size
    /// @param threads: number of threads that could be executed concurrently
    thread_pool(size_t threads);

    /// Place task to the queue
    /// @param f: function or method to execute in the thread
    /// @param args: function argumants
    /// @note: Throws an exception if try to add new task to stopeed pool
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)->std::future<typename std::result_of<F(Args...)>::type>;

    /// @brief destroy pool with joining all executed threads
    ~thread_pool();

private:

    // need to keep track of threads so we can join them
    std::vector< std::thread > workers;

    // the task queue
    std::queue< std::function<void()> > tasks;

    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;

    // flag to stop
    bool stop;
};

// the constructor just launches some amount of workers
inline thread_pool::thread_pool(size_t threads) : stop(false) {

    for (size_t i = 0; i < threads; ++i)
        workers.emplace_back(
        [this] {

        for (;;) {
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            while (!this->stop && this->tasks.empty())
                this->condition.wait(lock);
            if (this->stop && this->tasks.empty())
                return;
            std::function<void()> task(this->tasks.front());
            this->tasks.pop();
            lock.unlock();
            task();
        }
    });
}

// add new work item to the pool
template<class F, class... Args>
auto thread_pool::enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {

    typedef typename std::result_of<F(Args...)>::type return_type;

    // don't allow enqueueing after stopping the pool
    if (stop){
        throw std::runtime_error("Enqueue on stopped thread_pool");
    }

    auto task = std::make_shared< std::packaged_task<return_type()> >(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> res = task->get_future();

    /* wrap queue lock */{
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.push([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}

// the destructor joins all threads
inline thread_pool::~thread_pool() {

    /* wrap queue lock */ {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }

    condition.notify_all();

    // gently wait for all workers to finish
    std::for_each(workers.begin(), workers.end(), [](std::thread& w){w.join();});
}

#endif // THREAD_POOL_H
