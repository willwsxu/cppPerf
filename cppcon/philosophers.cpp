#include <thread>
#include <vector>
#include <string>
#include <deque>
#include <mutex>
#include <map>

struct Message {
    std::string sender;
    std::string receiver;
    int chopstick;
    bool acquire_release;
    bool success;
};
class Actor
{
public:
    void start()
    {
        t = std::thread([this]() { run(); });
    }
    virtual void run() {};
    ~Actor()
    {
        if (t.joinable())
            t.join();
    }

private:
    std::thread t;
protected:
    std::deque<Message>  messages;
    std::mutex m;
};

class Philosopher : public Actor
{
public:
    Philosopher(int id_, int N) : id(id_) {
        int chop1 = (id + 1) % N;
        int chop2 = (id + N - 1) % N;
        pair[chop1] = false;
        pair[chop2] = false;
    }
    void run() override {
        while (true) {
            {
                std::lock_guard<std::mutex> lg(m);
                while (messages.empty()) {
                    Message& msg = messages.front();
                }
            }
        }

    }
private:
    std::map<int, bool> pair;
    int id;  // 0 based
};

class DiningTable : public Actor
{
public:
    DiningTable(int N):philosophers(N), chopsticks_state(N, true) {

    }

private:
    std::vector<Philosopher> philosophers;
    std::vector<bool>    chopsticks_state;   // true mean avaiable
};

class MessageDispatch
{
    void send_acquire()
    {

    }
    void send_release(){

    }
};