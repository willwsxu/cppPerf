#pragma once

template<typename Cont, typename T>
void erase_remove(Cont& asso, T target)
{
	asso.erase(target);
}

template<typename T, typename A=allocator, template<typename,typename> class Cont>
void erase_remove(Cont<T, A>& vec, T target)
{
	vec.erase(remove(begin(vec), end(vec), target), end(vec));
}
/*
template<typename T, typename Traits=char_traits<T>, typename A = allocator> // , template<typename, typename, typename> class Cont
void erase_remove(string<T, Traits, A>& s, T target)
{
	s.erase(remove(begin(s), end(s), target), end(s));
}
*/
template<typename T>
void erase_remove(basic_string<T>& s, T target)
{
	s.erase(remove(begin(s), end(s), target), end(s));
}

template<typename T, typename A = allocator>
void erase_remove(list<T, A>& li, T target)
{
	li.remove(target);
}
