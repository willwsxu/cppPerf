#pragma once

template<class task_type>
struct task_queue
{
    void submit(task_type t)
    {
        {
            std::lock_guard<std::mutex> g(m);
            tasks.push_back(std::move(t));
        }
        cv.notify_one();
    }

    void submit_empty_task(int pool_size)
    {
        {
            std::lock_guard<std::mutex> g(m);
            for (size_t i = 0; i < pool_size; i++)
                tasks.push_back([]() { std::cout << "stop task\n"; });  // empty task to terminate thread in waiting
        }
        cv.notify_all();
    }

    task_type get_task() {
        std::unique_lock<std::mutex> g(m);
        cv.wait(g, [this]() { return !tasks.empty();  });
        task_type task = std::move(tasks.front());
        tasks.pop_front();
        return task;
    }
 
private:
    std::deque< task_type > tasks;
    std::mutex m;
    std::condition_variable cv;
};

template<typename task_type, typename task_queue>
struct thread_pool {  // single queue, locking
    thread_pool(int num) : pool(num)
    {
        int id = 0;
        for (auto& th : pool) {
            id++;
            th = std::thread([this, id]() {
                int call_count = 0;
                while (!stop_token) {
                    auto task = tasks.get_task();
                    //<<<<test code
                    using sys_clock = std::chrono::system_clock;
                    using std::chrono::duration_cast;
                    using std::chrono::microseconds;
                    auto start = sys_clock::now();
                    std::stringstream istr;
                    istr << id << " " << ++call_count << " basic_executor task start\n";
                    std::cout << istr.str();
                    //>>>>>test code
                    task();
                    //<<<<test code
                    auto end = sys_clock::now();
                    std::stringstream istr2;
                    istr2 << id << " " << call_count << " basic_executor task end. time took " << duration_cast<microseconds>(end - start).count() << "\n";
                    std::cout << istr2.str();
                    //>>>>>test code
                }
                });  // cannot use initilizer list
        }
    }
    thread_pool(thread_pool const&) = delete;
    thread_pool& operator=(thread_pool const&) = delete;

    ~thread_pool()
    {
        stop_token = true;
        tasks.submit_empty_task(pool.size());
        for (auto& th : pool) {
            if (th.joinable())
                th.join();
        }
    }
private:
    bool stop_token = false;
    std::vector <std::thread> pool;
    task_queue tasks;
};
