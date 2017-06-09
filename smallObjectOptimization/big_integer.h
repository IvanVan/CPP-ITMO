#pragma once
#include <string>
#include <algorithm>
#include "vector.h"

struct big_integer {

	big_integer();
	big_integer(big_integer const& other);
	big_integer(int a);
	big_integer(vector<unsigned int> const&, bool);
	explicit big_integer(std::string const& str);
	~big_integer();

	big_integer& operator=(big_integer const& other);

	big_integer& operator+=(big_integer const& rhs);
	big_integer& operator-=(big_integer const& rhs);
	big_integer& operator*=(big_integer const& rhs);
	big_integer& operator/=(big_integer const& rhs);
	big_integer& operator%=(big_integer const& rhs);

	big_integer& operator&=(big_integer const& rhs);
	big_integer& operator|=(big_integer const& rhs);
	big_integer& operator^=(big_integer const& rhs);

	big_integer& operator<<=(int rhs);
	big_integer& operator>>=(int rhs);

	big_integer operator+() const;
	big_integer operator-() const;
	big_integer operator~() const;

	big_integer& operator++();
	big_integer operator++(int);

	big_integer& operator--();
	big_integer operator--(int);

	friend bool operator==(big_integer const& a, big_integer const& b);
	friend bool operator!=(big_integer const& a, big_integer const& b);
	friend bool operator<(big_integer const& a, big_integer const& b);
	friend bool operator>(big_integer const& a, big_integer const& b);
	friend bool operator<=(big_integer const& a, big_integer const& b);
	friend bool operator>=(big_integer const& a, big_integer const& b);

	friend std::string to_string(big_integer const& a);

	void swap(big_integer& a);

private:
	vector<unsigned int> data;

	bool isNegate;

	void mul_vector_by_const(vector<unsigned int>& res, vector<unsigned int> const & a, unsigned int const& b);

	big_integer do_division(big_integer const&, big_integer const&);

	big_integer bin_pow(int);

	void division_by_const(unsigned int);

	void add_zero(int, vector <unsigned int>&);

	void add_module(big_integer const& a, big_integer const& b, vector <unsigned int>& tmp);

	void subtract_module(big_integer const& a, big_integer const& b, vector <unsigned int>& tmp);

	unsigned int make_normalized(big_integer&, big_integer&);

	void make_equal(size_t, vector<unsigned int>&, vector<unsigned int>&);

	void convert(vector <unsigned int>&, bool);

	void do_and(vector<unsigned int> &, vector <unsigned int> const&);

	void do_or(vector <unsigned int> &, vector <unsigned int> const&);

	void do_xor(vector <unsigned int> &, vector <unsigned int> const&);

	void multiply_by_const(unsigned int cnt);

	bool compare_module(big_integer const& a, big_integer const& b) const;

	void make_positive(big_integer&);

	bool is_deg2();

	void correct();

	void divide_by_32();

	unsigned int calculate(unsigned int, unsigned int, unsigned int);

	bool compare_equal_vectors(vector<unsigned int> const &, vector<unsigned int> const &);

	void sub_equal_vectors(vector<unsigned int> &, vector<unsigned int> const &);
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);

std::ostream& operator<<(std::ostream& s, big_integer const& a);