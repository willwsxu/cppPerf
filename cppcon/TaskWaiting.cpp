#include <functional>
#include <thread>
#include <future>
#include <memory>
#include <deque>
#include <mutex>
#include <iostream>
#include <chrono>
#include <vector>
#include <deque>
#include <optional>

using sys_clock = std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;

using task_type = std::function<void()>;

struct thread_pool {
    thread_pool() : pool(4)
    {
        int id = 0;
        for (auto& th : pool) {
            id++;
            th = std::thread([this, id]() {
                int call_count = 0;
                while (!stop_token) {
                    auto task = get_task();
                    auto start = sys_clock::now();
                    std::cout << id << " " << ++call_count << " basic_executor task start\n";
                    task();
                    auto end = sys_clock::now();
                    std::cout << id << " " << call_count << " basic_executor task end. time took " << duration_cast<microseconds>(end - start).count() << "\n";
                }
                });  // cannot use initilizer list
        }
    }
    thread_pool(thread_pool const&) = delete;
    thread_pool& operator=(thread_pool const&) = delete;

    void submit(task_type t)
    {
        {
            std::lock_guard<std::mutex> g(m);
            tasks.push_back(t);
        }
        cv.notify_one();
    }

    ~thread_pool()
    {
        stop_token = true;
        {
            std::lock_guard<std::mutex> g(m);
            for (size_t i=0; i<pool.size(); i++)
                tasks.push_back([]() {});  // empty task to terminate thread in waiting
        }
        cv.notify_all();
        for (auto& th : pool)
            th.join();
    }
private:
    void thread_loop()
    {
        static int call_count = 0;
        while (!stop_token) {
            auto task = get_task();
            auto start = sys_clock::now();
            std::cout << ++call_count << " basic_executor task start\n";
            task();
            auto end = sys_clock::now();
            std::cout << call_count << " basic_executor task end. time took " << duration_cast<microseconds>(end - start).count() << "\n";
        }
    }
    task_type get_task() {
        std::unique_lock<std::mutex> g(m);
        cv.wait(g, [this]() { return !tasks.empty();  });
        task_type task = std::move(tasks.front());
        tasks.pop_front();
        return task;
    }
    bool stop_token = false;
    std::vector <std::thread> pool;
    std::deque< task_type > tasks;
    std::mutex m;
    std::condition_variable cv;
};
struct basic_executor {
    void submit(task_type task) {
        pool->submit(task);
    }

    std::shared_ptr<thread_pool> pool = std::make_shared<thread_pool>(); // so class is default constructible and movable
};

struct wrapper_executor { 
    wrapper_executor(basic_executor& ex):exec(ex)
    {
    }
    std::future<void> submit2(std::function<void()> task)
    {
        return std::async(std::launch::async, [this,&task] { exec.submit(move(task)); });
    }

    std::future<void> submit(std::function<void()> task)
    {
        auto wrapper = std::make_shared<std::packaged_task< void() >>(task);
        exec.submit( [wrapper]() { (*wrapper)(); });
        return wrapper->get_future();  // get a future
    }

    struct Task_Handle;

    Task_Handle submit_cont(std::function<void()> task);

private:
    struct Chaining_internals {
        Chaining_internals(wrapper_executor& we_, task_type t) :wex(we_), current_task(std::move(t))
        {}
        void run()
        {
            current_task();
            std::lock_guard<std::mutex> g(m);
            complete = true;
            if (continuation)
                continuation->run();
        }
        void chain_next(std::shared_ptr<Chaining_internals> cont)
        {
            std::lock_guard<std::mutex> g(m);
            if (complete)
                cont->run();
            else
                continuation = cont;
        }
    private:
        friend struct Task_Handle; // need to access wex
        wrapper_executor& wex;
        task_type current_task;
        std::shared_ptr<Chaining_internals> continuation;
        std::mutex m;
        bool complete = false;
    };
    basic_executor exec;
};

struct wrapper_executor::Task_Handle
{
    Task_Handle(std::shared_ptr<Chaining_internals> _internals) : m_internals(_internals) {}

    Task_Handle then(std::function<void()> task)
    {
        auto next_internals = std::make_shared<Chaining_internals>(m_internals->wex, task);
        m_internals->chain_next(next_internals);
        std::cout << "Task_Handle then done\n";
        return Task_Handle(next_internals);
    }
private:
    std::shared_ptr<Chaining_internals> m_internals;
};

wrapper_executor::Task_Handle wrapper_executor::submit_cont(task_type task)
{
    auto internals = std::make_shared<Chaining_internals>(*this, task);
    exec.submit([internals]() { internals->run(); });

    std::cout << "submit_cont done\n";
    return Task_Handle(std::move(internals));  // Task_Handle and exec both own same internal obj
}


int main()
{
    auto task1 = []() { std::cout << "Task1\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); };
    auto task2 = []() { std::cout << "Task2\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); };
    auto task3 = []() { std::cout << "Task3\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(3000)); };
    
    basic_executor b;
    wrapper_executor we(b);
    we.submit_cont(task1).then(task2).then(task3);

    std::cout << "main complete test 1\n";

    auto task4 = []() { std::cout << "Task4\n";  };
    auto task5 = []() { std::cout << "Task5\n";  };
    auto task6 = []() { std::cout << "Task6\n";  };

    we.submit_cont(task4).then(task5).then(task6);

    std::cout << "main complete\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    return 0;
}
