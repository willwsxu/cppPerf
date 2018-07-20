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

///////////////////////////////////////////
// erase remove if Predicate
template<typename Cont, typename Pred>
void erase_remove_if(Cont& asso, Pred p)  // associative container
{
	for (auto iter = begin(asso); iter != end(asso); ) {
		if (p(*iter))
			asso.erase(iter++);  // iter is still valid because post increment
		else
			++iter;
	}
}

template<typename T, typename A = allocator, template<typename, typename> class Cont, typename Pred>
void erase_remove_if2(Cont<T, A>& vec, Pred p)  // for vector, deque
{
	for (auto iter = begin(vec); iter != end(vec); ) {
		if (p(*iter))
			iter = vec.erase(iter);  // valid iter is returned
		else
			++iter;
	}
}

template<typename T, typename A = allocator, template<typename, typename> class Cont, typename Pred>
void erase_remove_if(Cont<T, A>& vec, Pred p)  // for vector, deque
{
	vec.erase(remove_if(begin(vec), end(vec), p), end(vec));
}

template<typename T, typename Pred>
void erase_remove_if(basic_string<T>& s, Pred p)
{
	s.erase(remove_if(begin(s), end(s), p), end(s));
}

template<typename Pred, typename T, typename A = allocator>
void erase_remove_if(list<T, A>& li, Pred p)
{
	li.remove_if(p);
}