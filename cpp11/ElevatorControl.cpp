#include "..\catch.hpp"

#include <vector>
#include <mutex>

using namespace std;

class Elevator
{
	volatile bool exit = false;
	enum Elevator_State { IDLE, MOVE_UP, MOVE_DOWN };
	Elevator_State state = IDLE;	// direction
	//int levels=5; // simpilified, serving levels 1 .. n
	vector<bool> stops;	// which floor to stop
	mutex	mux;
	condition_variable	cv;
	class ElevatorControl *parent = nullptr;
	int id;
	int current_level=0;

	void run();
	void open() {}  // open for people to get in
public:
	Elevator() {
		static int gid = 0;
		id = ++gid;  // elevator ID, start from 1
	}
	bool is_idle() const
	{
		return state == IDLE;
	}
	void start(ElevatorControl *p, int levels) {
		parent = p;
		stops.resize(levels, false);  // one to set elevator floors

		std::thread worker(&Elevator::run, this);  // start elevator thread, called by control
		worker.join();
	}

	// request an idle elevator to the floor
	bool request_service(bool up, int floor)  // called by control to respond a button push
	{
		if (state != IDLE)
			return false;
		{
			lock_guard<mutex> g(mux);
			if (current_level == floor)  // elevator just happen to be at same floor
				state = up ? MOVE_UP : MOVE_DOWN;
			else
				state = current_level < floor ? MOVE_UP : MOVE_DOWN;
		}
		cv.notify_one(); // wake up the threading in waiting
		return true;
	}
};

class ElevatorControl
{
	vector<Elevator> elevators;
	vector<bool>	up_pushed;
	vector<bool>	down_pushed;
	mutex			up_mutx;
	mutex			down_mutx;

	void update_button(mutex& m, vector<bool>& buttons, int level)
	{
		if (level >= (int)buttons.size())
			return;
		lock_guard<mutex> g(m);
		buttons[level] = true;
	}
public:
	ElevatorControl(int levels):up_pushed(levels, false), down_pushed(levels, false), elevators(levels)
	{
	}
	void start() {
		for (auto& ele : elevators) {
			ele.start(this, up_pushed.size());
		}
	}
	bool go_up(int current) {  // called by elevator when it is at this floor
		lock_guard<mutex> g(up_mutx);
		bool up = up_pushed[current];
		up_pushed[current] = false;
		return up;
	}
	int go_down(int current) {
		lock_guard<mutex> g(down_mutx);
		bool down= down_pushed[current];
		down_pushed[current] = false;
		return down;
	}
	void push_up(unsigned int level) {
		update_button(up_mutx, up_pushed, level);
		for (auto& ele : elevators) {
			if (ele.request_service(true, level))  // find one elevator
				break;
		}
	}
	void push_down(int level)	{
		update_button(down_mutx, down_pushed, level);
		for (auto& ele : elevators) {
			if (ele.request_service(false, level))  // find one elevator
				break;
		}
	}
};

void Elevator::run()
{
	while (!exit) {
		unique_lock<mutex>(mux);
		cv.wait(mux, [this] {return !this->is_idle(); });  // wait until it is not idle
		if (state == MOVE_UP) {
			// optimization ideas: stop elevator when there is no stops and no button pushed
			// before go to idle, check if there is still outstanding request
			for (; current_level < (int)stops.size(); current_level++) {
				bool open_door = false;
				if (parent->go_up(current_level)) {
					open_door = true;
				}
				if (stops[current_level]) {
					open_door = true;
					stops[current_level] = false;
				}
				if (open_door)
					open();
			}
		}
		else {

		}
	}
}