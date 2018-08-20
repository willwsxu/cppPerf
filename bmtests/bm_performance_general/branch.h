#pragma once

int good(int err) {
	if (!err) {
		// do stuff
		return 0;
	}
	else {
		if (err == 1)		return -1;
		else if (err == 2)	return -2;
		else if (err == 3)	return -3;
		else return 1;
	}
};

int bad(int err) {
	if (err == 1) {
		return -1;
	}
	else if (err == 2) {
		return -2;
	}
	else if (err == 3) {
		return -3;
	}
	else if (err == 0) {
		// do stuff
		return 0;
	}
	else
		return 1;
};
// less branches, test 0
static void BM_branch_good(benchmark::State& state) {
	int ret = 0;
	for (auto _ : state)
	{
		ret = good(0);
	}
}

static void BM_branch_bad(benchmark::State& state) {
	int ret = 0;
	for (auto _ : state)
	{
		ret = bad(0);
	}
}

// too fast to show difference here. see test below
BENCHMARK(BM_branch_good); // 1ns, over 10% more iterations
BENCHMARK(BM_branch_bad);  // 1ns
