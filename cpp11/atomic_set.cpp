
#include "..\catch.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <algorithm>
#include <cassert>

using namespace std;


struct calc_data_1
{
	uint32_t cat_id;
	uint32_t ref_seq_id;
	uint32_t payload;
	calc_data_1(uint32_t cat = 0, uint32_t seq = 0, uint32_t pay = 0) :cat_id(cat), ref_seq_id(seq), payload(pay) {}
};


struct calc_data_2
{
	uint32_t cat_id;
	uint32_t ref_seq_id;
	uint32_t payload;
	calc_data_2(uint32_t cat = 0, uint32_t seq = 0, uint32_t pay = 0) :cat_id(cat), ref_seq_id(seq), payload(pay) {}
};


struct ref_data
{
	uint32_t cat_id;
	uint32_t ref_seq_id;
	uint32_t payload;
	ref_data(uint32_t cat = 0, uint32_t seq = 0, uint32_t pay = 0) :cat_id(cat), ref_seq_id(seq), payload(pay) {}
};

// same category
/// CD1 1 2 3 ... 50          
/// CD2   2   4 ... 50  
/// REF              1000

// HERE
class AtomicSet
{
	map<uint32_t, vector<calc_data_1>>  data1;
	map<uint32_t, vector<calc_data_2>>  data2;
	map<uint32_t, vector<ref_data>>          data3;
	map<uint32_t, map<uint32_t, uint32_t>>  msg_seq_count;

	void send_atomic(uint32_t cat_id, uint32_t seq_id)
	{
		auto& d1 = data1[cat_id];
		auto& d2 = data2[cat_id];
		auto& d3 = data3[cat_id];
		if (++msg_seq_count[cat_id][seq_id]<3)
			return;
		auto d1end = remove_if(begin(d1), end(d1), [seq_id](const auto&d) { return d.ref_seq_id <seq_id; });
		auto d2end = remove_if(begin(d2), end(d2), [seq_id](const auto&d) { return d.ref_seq_id <seq_id; });
		auto d3end = remove_if(begin(d3), end(d3), [seq_id](const auto&d) { return d.ref_seq_id <seq_id; });

		cout << cat_id << "," << seq_id << ":" << d1[0].payload << " " << d2[0].payload << " " << d3[0].payload << "\n";
		d1end = remove_if(begin(d1), d1end, [seq_id](const auto&d) { return d.ref_seq_id == seq_id; });
		d2end = remove_if(begin(d2), d2end, [seq_id](const auto&d) { return d.ref_seq_id == seq_id; });
		d3end = remove_if(begin(d3), d3end, [seq_id](const auto&d) { return d.ref_seq_id == seq_id; });
		d1.erase(d1end, end(d1));
		d2.erase(d2end, end(d2));
		d3.erase(d3end, end(d3));
	}
public:
	void add_meesage(const calc_data_1& data) {
		data1[data.cat_id].push_back(data);
		send_atomic(data.cat_id, data.ref_seq_id);
	}
	void add_meesage(const calc_data_2& data) {
		data2[data.cat_id].push_back(data);
		send_atomic(data.cat_id, data.ref_seq_id);
	}
	void add_meesage(const ref_data& data) {
		data3[data.cat_id].push_back(data);
		send_atomic(data.cat_id, data.ref_seq_id);
	}

};


TEST_CASE("Akuna interview", "[NEW]")
{
	AtomicSet aset;
	aset.add_meesage(calc_data_1(1, 1, 10));
	aset.add_meesage(calc_data_2(1, 1, 10));
	aset.add_meesage(ref_data(1, 1, 10));
}
