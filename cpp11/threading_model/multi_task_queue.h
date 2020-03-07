
template<class task_type>
struct multi_task_queue
{
    void submit(task_type t, int which)
    {
        {
            std::lock_guard<std::mutex> g(multi_tasks_in[which].m);
            multi_tasks_in[which].push_back(std::move(t));
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

    multi_task_queue(int size) : multi_tasks_in(size), multi_tasks_out(size) {]}
private:
    using Task_Queue = std::vector < task_type>;
    struct Task_Item {
        Task_Queue tasks;
        std::mutex m;
        std::condition_variable cv;
    };
    std::vector< Task_Item >                multi_tasks_in;   // input
    std::vector< Task_Queue >               multi_tasks_out;
};