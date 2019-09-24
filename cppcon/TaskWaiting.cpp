#include <functional>
#include <thread>
#include <future>
#include <memory>
#include <deque>
#include <mutex>
#include <iostream>

struct basic_executor { 
    void submit(std::function<void()> task) {
        /*std::thread t([task]() { 

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            task(); 
            });
        t.detach();*/

        std::cout << "basic_executor start\n";
        std::async(std::launch::async, [this, &task] { task(); });
        std::cout << "basic_executor done\n";
    }

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

    struct Task_Handle
    {
        Task_Handle(wrapper_executor* w) : executor(w) {}
        std::shared_ptr<wrapper_executor> executor;
        std::deque< std::function<void()>> q;

        void exec();
        Task_Handle& then(std::function<void()> task)
        {
            {
                std::lock_guard<std::mutex> lk(m);
                q.push_back(task);
            }
            auto wrapper = std::make_shared<std::packaged_task< void() >>([this]() { exec(); });
            executor->exec.submit([wrapper]() {
                (*wrapper)();
                }
            );
            std::cout << "then done\n";
            return *this;
        }
        std::mutex m;
    };

    Task_Handle& submit_cont(std::function<void()> task)
    {
        handle = std::make_shared<Task_Handle>(new wrapper_executor(*this));
        handle->q.push_back(task);
        auto wrapper = std::make_shared<std::packaged_task< void() >>([this]() { handle->exec(); });
        exec.submit([wrapper]() {
            (*wrapper)();
            }
        );
        std::cout << "submit_cont done\n";
        return *handle;
    }

private:
    basic_executor exec;
    std::shared_ptr<Task_Handle> handle;
};

void wrapper_executor::Task_Handle::exec()
{
    {
        std::lock_guard<std::mutex> lk(m);
        if (q.empty())
            return;
        q.front()();
        q.pop_front();
    }
}


int main()
{
    auto task1 = []() { std::cout << "Task1\n";  };
    auto task2 = []() { std::cout << "Task2\n";  };
    auto task3 = []() { std::cout << "Task3\n";  };
    
    basic_executor b;
    wrapper_executor we(b);
    we.submit_cont(task1).then(task2).then(task3);

    std::cout << "main complete\n";
    return 0;
}
