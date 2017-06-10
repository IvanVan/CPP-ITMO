#pragma once
#include <iostream>
#include <memory.h>

const int SMALL = 7;

template <typename T>
struct info {
	int refs;
	T data[];
};

template<typename T>
struct big_data {
	info<T>* ptr;
	size_t size_;
	size_t capacity_;

	size_t size() const
	{ 
		return size_; 
	}
	size_t capacity() const 
	{ 
		return capacity_; 
	}
	T& operator[](size_t i) 
	{ 
		return ptr->data[i]; 
	}
	T const& operator[](size_t i) const 
	{ 
		return ptr->data[i]; 
	}
	void pop_back()
	{
		--size_; 
	}
	void push_back(T const& value) {
		ptr->data[size()] = value;
		++size_;
	}
};

template<typename T>
struct small_data {
	char magic;
	T reg[SMALL];

	size_t size() const 
	{
		return magic / 2; 
	}
	size_t capacity() const 
	{ 
		return SMALL; 
	}
	T& operator[](size_t i) 
	{ 
		return reg[i]; 
	}
	T const& operator[](size_t i) const
	{ 
		return reg[i]; 
	}
	void pop_back() 
	{ 
		magic -= 2; 
	}
	void push_back(T const& value) {
		reg[size()] = value;
		magic += 2;
	}
};

template <typename T>
struct vector {

	vector(size_t n = 0);
	vector(vector const&);
	vector(int, T);
	vector& operator=(vector const&);
	~vector();

	bool is_small() const;
	size_t size() const;
	bool empty() const;
	void resize(size_t n);
	void pop_back();
	void push_back(T const&);
	void clear();
	T& back();
	T& operator[](size_t);
	T const& operator[](size_t) const;

private:

	union {
		big_data<T> big_object;
		small_data<T> small_object;
	};

	void check_refs();
	void ensure_cap(size_t);
	size_t capacity() const;
	void big_to_small();

};


template <typename T>
vector<T>::vector(size_t n)
{
	small_object.magic = 1;
	resize(n);
}

template <typename T>
vector<T>::vector(vector const& other) : vector(0)
{
	*this = other;
}

template<typename T>
vector<T>::vector(int n, T element)
{
	resize(n);
	for (int i = 0; i < n; ++i) {
		(*this)[i] = element;
	}
}

template <typename T>
vector<T>& vector<T>::operator=(vector const& other)
{
	clear();
	memcpy(this, &other, sizeof(vector));
	if (!is_small()) {
		big_object.ptr->refs++;
	}
	return *this;
}

template <typename T>
vector<T>::~vector()
{
	clear();
}

template <typename T>
bool vector<T>::is_small() const
{
	return small_object.magic & 1;
}

template <typename T>
size_t vector<T>::size() const
{
	return is_small() ? small_object.size() : big_object.size();
}

template <typename T>
bool vector<T>::empty() const
{
	return size() == 0;
}

template <typename T>
void vector<T>::resize(size_t n)
{
	check_refs();
	if (n <= SMALL) {
		if (is_small()) {
			for (size_t i = size(); i < n; ++i) {
				(*this)[i] = 0;
			}
			small_object.magic = 1 | (n << 1);
		}
		else {
			big_object.size_ = n;
			big_to_small();
		}
	}
	else {
		ensure_cap(n);
		for (size_t i = size(); i < n; ++i) {
			(*this)[i] = 0;
		}
		big_object.size_ = n;
	}
}

template <typename T>
void vector<T>::pop_back()
{
	check_refs();
	is_small() ? small_object.pop_back() : big_object.pop_back();
}

template <typename T>
void vector<T>::push_back(T const& value)
{
	check_refs();
	ensure_cap(size() + 1);
	is_small() ? small_object.push_back(value) : big_object.push_back(value);
}

template <typename T>
void vector<T>::clear()
{
	if (!is_small()) {
		big_object.ptr->refs--;
		if (!big_object.ptr->refs)
			delete big_object.ptr;
		small_object.magic = 1;
	}
}

template <typename T>
T& vector<T>::back()
{
	return (*this)[size() - 1];
}

template <typename T>
T& vector<T>::operator[](size_t i)
{
	check_refs();
	return is_small() ? small_object[i] : big_object[i];
}

template <typename T>
T const& vector<T>::operator[](size_t i) const
{
	return is_small() ? small_object[i] : big_object[i];
}

template <typename T>
void vector<T>::check_refs()
{
	if (!is_small() && big_object.ptr->refs > 1) {
		big_object.ptr->refs--;
		info<T>* tmp = big_object.ptr;
		big_object.ptr = (info<T>*) operator new (sizeof(info<T>) + big_object.capacity_ * sizeof(T));
		big_object.ptr->refs = 1;
		for (int i = 0; i < (int)size(); ++i) {
			big_object.ptr->data[i] = tmp->data[i];
		}
	}
}

template <typename T>
void vector<T>::ensure_cap(size_t n)
{

	if (capacity() < n) {
		size_t sz = std::min(size(), n);
		T* buf = new T[sz];
		for (size_t i = 0; i != sz; ++i) {
			buf[i] = (*this)[i];
		}
		clear();
		big_object.ptr = (info<T>*) operator new (sizeof(info<T>) + sizeof(T) * n * 2);
		big_object.ptr->refs = 1;
		big_object.size_ = sz;
		big_object.capacity_ = n * 2;
		for (size_t i = 0; i < sz; ++i) {
			(*this)[i] = buf[i];
		}
		delete[] buf;
	}
}

template <typename T>
size_t vector<T>::capacity() const
{
	return is_small() ? small_object.capacity() : big_object.capacity();
}

template <typename T>
void vector<T>::big_to_small()
{
	info<T>* tmp = big_object.ptr;
	size_t sz = size();
	for (size_t i = 0; i != sz; ++i) {
		small_object.reg[i] = tmp->data[i];
	}
	small_object.magic = (sz << 1) | 1;
	tmp->refs--;
	if (!tmp->refs) {
		delete tmp;
	}
}
