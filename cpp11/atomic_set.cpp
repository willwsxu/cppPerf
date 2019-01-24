#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <cassert>

using namespace std;


struct calc_data_1
{
	uint32_t cat_id;
	uint32_t ref_seq_id;
	uint32_t payload;
};


struct calc_data_2
{
	uint32_t cat_id;
	uint32_t ref_seq_id;
	uint32_t payload;
};


struct ref
{
	uint32_t cat_id;
	uint32_t ref_seq_id;
	uint32_t payload;
};

// same category
/// CD1 1 2 3 ... 50          
/// CD2   2   4 ... 50  
/// REF              1000

// HERE
class Atomic_set
{
	map<uint32_t, vector<calc_data_1>>  data1;
	map<uint32_t, vector<calc_data_2>>  data2;
	map<uint32_t, vector<ref>>          data3
		map<uint32_t, map<uint32_t, uint32_t>>  msg_seq_count;


	void check_atomic(uint32_t cat_id)
	{
		auto& d1 = data1[cat_id];
		auto& d2 = data2[cat_id];
		auto& d3 = data3[cat_id];
		while (!d1.empty() && !d2.empty() && !d3.empty()) {
			int largest = d1.front();

		}
	}
public:
	void add_meesage(const calc_data_1& data) {
		data1[data.cat_id] = data;
		if (++msg_seq_count[data.cat_id][data.ref_seq_id] == 3) {

		}
	}
	void add_meesage(const calc_data_2& data) {
		data2[data.cat_id] = data;

	}
	void add_meesage(const ref& data) {
		data3[data.cat_id] = data;

	}


};




int main() {
	/* Enter your code here. Read input from STDIN. Print output to STDOUT */
	return 0;
}
