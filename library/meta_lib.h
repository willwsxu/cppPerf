#pragma once

// compute bits into int, e.g. 1,0,0 -> 4
int compute_from_bit(int init)
{
	return init;
}
template<typename T, typename ...ARGS>
int compute_from_bit(int init, T first, ARGS ...args)
{
	return compute_from_bit((init << 1) + first, args...);
}

// compute bits into int, in reverse order, e.g. 0,0,1 -> 4
int reverse_compute_from_bit(int N, int init)
{
	return init;
}
template<typename T, typename ...ARGS>
int reverse_compute_from_bit(int N, int init, T first, ARGS ...args)
{
	return reverse_compute_from_bit(N, init | (first << (N - sizeof...(args)-1)), args...);
}
