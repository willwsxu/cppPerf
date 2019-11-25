#include <thread>
#include <vector>
#include <string>
#include <deque>
#include <mutex>
#include <map>
#include <variant>
#include <chrono>
#include <iostream>
#include <iomanip>

template<typename...ARGS>
void log(ARGS ... args) {

}

template<>
void log() {

}

// FORK=Chopstick
struct FORK_REQUEST
{
    int fork;
    int philo;
    FORK_REQUEST(int f, int p) : fork(f), philo(p) {}
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
    FORK_RELEASE(int f1, int f2) : fork1(f1), fork2(f2) {}
};
using ActorMessages = std::variant<FORK_REQUEST, FORK_RESPONSE, FORK_RELEASE>;

class Actor
{
public:
    Actor()
    {
        t = std::thread([this]() { run(); });
    }
    virtual void run() {};

    //rule of five
    ~Actor()
    {
        shutdown();
        if (t.joinable())
            t.join();
    }

    Actor(const Actor& copy_construct) = delete;
    Actor(Actor&& move_construct) = default;
    Actor& operator=(const Actor&) = delete;
    Actor& operator=(Actor&&) = default;

    void send(ActorMessages&& msg, Actor& receiver) {
        receiver.enqueue(msg);
    }
    virtual void receive(const ActorMessages& msg) = 0;
    void shutdown() {
        terminate = true;
    }

private:
    std::thread t;
protected:
    std::deque<ActorMessages>  messages;
    std::mutex m;
    bool terminate = false;

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
        while (!terminate) {
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
            else {
                bool req_left = true;
                if (left.is_acquired)
                    req_left = false;
                else if (!right.is_acquired && left.chop_id % 2 != 0)  // reuqest left first if id is even
                    req_left = false;
                if (!req_left) {
                    std::cout << "philosopher " << left.chop_id << " request right fork " << right.chop_id << "\n";
                    request_fork(right.chop_id);
                }
                else {
                    std::cout << "philosopher " << left.chop_id << " request left fork " << left.chop_id << "\n";
                    request_fork(left.chop_id);
                }
            }
        }
    }

    void request_fork(int f) {
        send(FORK_REQUEST(f, left.chop_id), *the_table);
    }
    void eat() {
        send(FORK_RELEASE(left.chop_id, right.chop_id), *the_table);
        std::cout << "philosopher " << left.chop_id << " eat\n";
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1ms);
    }
    void receive(const ActorMessages& msg) override {
        std::visit(overloaded{
                [](auto arg) {; },
                [this](FORK_RESPONSE arg)
            {
                if (!arg.granted)
                    return;
                if (left.chop_id == arg.fork) {
                    left.is_acquired = true;
                    std::cout << "philosopher " << left.chop_id << " got left fork " << left.chop_id << "\n";
                }
                else if (right.chop_id == arg.fork) {
                    right.is_acquired = true;
                    std::cout << "philosopher " << left.chop_id << " got right fork " << right.chop_id << "\n";
                }
            },
            }, msg);
    }

    Philosopher(const Philosopher& copy_construct) = delete;
    Philosopher(Philosopher&& move_construct) = default;
    Philosopher& operator=(const Philosopher&) = delete;
    Philosopher& operator=(Philosopher&&) = default;
private:
    struct Chopstick_state
    {
        int chop_id=0;
        bool is_acquired = false;
    };
    Chopstick_state left, right;
    Actor * the_table=nullptr;
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
        std::visit(overloaded{
                [](auto arg) {; },
                [this](FORK_REQUEST arg)
            {
                if (!chopsticks_state[arg.fork])
                    return;
                chopsticks_state[arg.fork] = false;
                send(FORK_RESPONSE(arg.fork, true), philosophers[arg.philo]);
            },
                [this](FORK_RELEASE arg)
            {
                chopsticks_state[arg.fork1] = true;
                chopsticks_state[arg.fork2] = true;
            },
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
                    receive(msg);
                    messages.pop_front();
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

int main()
{
    using namespace std::chrono_literals;
    {
        DiningTable table(5);
        std::this_thread::sleep_for(100ms);
    }
    std::this_thread::sleep_for(100ms);
}
