#include <thread>
#include <vector>
#include <string>
#include <deque>
#include <mutex>
#include <map>
#include <variant>
#include <chrono>

// FORK=Chopstick
struct FORK_REQUEST
{
    int fork;
    FORK_REQUEST(int f) : fork(f) {}
};
struct FORK_RESPONSE
{
    int fork;
    bool granted;
    FORK_RESPONSE(int f, bool g) : fork(f), granted(g) {}
};
struct FORK_RELEASE
{
    int fork1;
    int fork2;
};
using ActorMessages = std::variant<FORK_REQUEST, FORK_RESPONSE, FORK_RELEASE>;

class Actor
{
public:
    void start()
    {
        t = std::thread([this]() { run(); });
    }
    virtual void run() {};

    //rule of five
    ~Actor()
    {
        if (t.joinable())
            t.join();
    }

    Actor(const Actor& copy_construct) = delete;
    Actor(Actor&& move_construct) = default;
    Actor& operator=(const Actor&) = delete;
    Actor& operator=(Actor&&) = default;

    Actor() = default;

    void send(ActorMessages&& msg, Actor&) {}
    virtual void receive(const ActorMessages& msg) {}

private:
    std::thread t;
protected:
    std::deque<ActorMessages>  messages;
    std::mutex m;

    void enqueue(ActorMessages&& msg) {
        std::lock_guard<std::mutex> g(m);
        messages.push_back(std::move(msg));
    }
    void enqueue(const ActorMessages& msg) {
        std::lock_guard<std::mutex> g(m);
        messages.push_back(msg);
    }
};

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

class Philosopher : public Actor
{
public:
    Philosopher(int id, int N, Actor& table) : the_table(&table){
        left.chop_id = id;
        right.chop_id = (id + 1) % N;
    }
    Philosopher() = default;
    void run() override {
        while (true) {
            {
                std::lock_guard<std::mutex> lg(m);
                while (!messages.empty()) {
                    auto& msg = messages.front();
                    receive(msg);
                    messages.pop_front();
                }
            }
            if (left.is_acquired && right.is_acquired) {
                eat();
            }
            else if (left.is_acquired)
                request_fork(right.chop_id);
            else
                request_fork(left.chop_id);
        }

    }

    void request_fork(int f) {
        send(FORK_REQUEST(f), *the_table);
    }
    void eat() {
        send(FORK_RELEASE(), *the_table);
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1ms);
    }
    void receive(const ActorMessages& msg) override {
        std::visit([](auto&& arg){
            }, msg);
    }

    Philosopher(const Philosopher& copy_construct) = delete;
    Philosopher(Philosopher&& move_construct) = default;
    Philosopher& operator=(const Philosopher&) = delete;
    Philosopher& operator=(Philosopher&&) = default;
private:
    struct Chopstick_state
    {
        int chop_id;
        bool is_acquired = false;
    };
    Chopstick_state left, right;
    Actor * the_table;
};

class DiningTable : public Actor
{
public:
    DiningTable(int N):chopsticks_state(N, true){
        for (int i = 0; i < N; i++) {
            philosophers.emplace(std::piecewise_construct,
                std::forward_as_tuple(i),
                std::forward_as_tuple(i, N, *this)
            );
        }
    }

    void receive(const ActorMessages& msg) override {
        std::visit([](auto&& arg) {
            }, msg);
    }
    
    DiningTable(const DiningTable& copy_construct) = delete;
    DiningTable(DiningTable&& move_construct) = default;
    DiningTable& operator=(const DiningTable&) = delete;
    DiningTable& operator=(DiningTable&&) = default;

protected:
    void run() override {
        while (true) {
            {
                std::lock_guard<std::mutex> lg(m);
                while (messages.empty()) {
                    const auto& msg = messages.front();
                }
            }
        }
    }
private:
    void grant_chop_request(int c, int p)
    {
        send(FORK_RESPONSE(c, true), philosophers[p]);
    }
    void add_wait_list(int c, int p) {
        wait_listed[c].push_back(p);
    }
    std::map<int, Philosopher> philosophers;  // mutex is not copiable / movable
    std::vector<bool>    chopsticks_state;   // true mean avaiable
    std::vector<std::deque<int>>  wait_listed;
};


void driver() {
    DiningTable table(5);
}
