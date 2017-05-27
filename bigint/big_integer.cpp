#include "big_integer.h"
#include <string>

const long long base = 1LL << 32;

void pop_zero(std::vector <unsigned int>& tmp) {
	while (tmp.size() > 1 && tmp.back() == 0)
		tmp.pop_back();
}

void big_integer::make_positive(big_integer& a) {
	if (a.data.size() == 1 && a.data[0] == 0) {
		a.isNegate = false;
	}
}

bool big_integer::is_deg2()
{
	if (data.back() != 1) return 0;
	for (size_t i = 0; i < data.size() - 1; ++i)
		if (data[i]) return 0;
	return 1;
}

void big_integer::correct()
{

	while (data.size() > 1 && !data.back())
		data.pop_back();
	if (data.empty())
		data.push_back(0);
	if (!data.back())
		isNegate = 0;
}

big_integer big_integer::bin_pow(int n) {
	big_integer a;
	a.data.resize(n + 1);
	a.data[n] = 1;
	a.isNegate = false;
	return a;
}

unsigned int big_integer::make_normalized(big_integer& a, big_integer& b) {
	unsigned int ret = 1;
	unsigned int half_base = (unsigned int)1 << 31;
	while (a.data[(int)a.data.size() - 1] < half_base) {
		a *= 2;
		b *= 2;
		ret <<= 1;
	}
	return ret;
}

big_integer::big_integer() : data(1, 0), isNegate(false) {}

big_integer::big_integer(big_integer const& other) : data(other.data), isNegate(other.isNegate) {}

big_integer::big_integer(int other) : isNegate(other < 0) {
	long long tmp = other;
	if (isNegate) {
		tmp *= -1;
	}
	data.push_back((unsigned int)(tmp));
}

big_integer::big_integer(std::string const& s) : data(), isNegate(false) {
	std::string cur = s[0] == '-' ? s.substr(1, s.size() - 1) : s;
	for (size_t i = 0; i < cur.size(); i++) {
		multiply_by_const(10);
		*this += (int)(cur[i] - '0');
	}
	isNegate = s[0] == '-';
	make_positive(*this);
}

big_integer::~big_integer()
{
}

big_integer& big_integer::operator=(big_integer const& other) {
	this->data = other.data;
	this->isNegate = other.isNegate;
	return *this;
}



void big_integer::multiply_by_const(unsigned int cnt) {
	unsigned int carry = 0;
	std::vector <unsigned int> tmp(data);
	for (size_t i = 0; i < tmp.size() || carry > 0; i++) {
		if (i == tmp.size()) {
			tmp.push_back(0);
		}
		unsigned long long now = tmp[i] * 1ULL * cnt + carry;
		tmp[i] = (unsigned int)(now % base);
		carry = (unsigned int)(now / base);
	}
	pop_zero(tmp);
	data = tmp;
}



bool operator < (big_integer const& lhs, big_integer const& rhs) {
	if (lhs.isNegate) {
		if (!rhs.isNegate) {
			return true;
		}
		else {
			return rhs.compare_module(lhs, rhs);
		}
	}
	else {
		if (rhs.isNegate) {
			return false;
		}
		else {
			return rhs.compare_module(rhs, lhs);
		}
	}
}

bool operator == (big_integer const& lhs, big_integer const& rhs) {
	return !(lhs > rhs) && !(lhs < rhs);
}

bool operator != (big_integer const& lhs, big_integer const& rhs) {
	return !(lhs == rhs);
}

bool operator > (big_integer const& lhs, big_integer const& rhs) {
	return rhs < lhs;
}

bool operator <= (big_integer const& lhs, big_integer const& rhs) {
	return lhs < rhs || lhs == rhs;
}

bool operator >= (big_integer const& lhs, big_integer const& rhs) {
	return !(lhs < rhs);
}


void big_integer::add_module(big_integer const& a, big_integer const& b, std::vector <unsigned int>& ret) {
	ret = a.data;
	unsigned int carry = 0;
	for (size_t i = 0; i < std::max(ret.size(), b.data.size()) || carry; ++i) {
		if (i == ret.size())
			ret.push_back(0);
		long long now = ret[i] * 1LL + carry + (i < b.data.size() ? b.data[i] : 0);
		if (now >= base) {
			carry = 1;
			now -= base;
		}
		else {
			carry = 0;
		}
		ret[i] = (unsigned int)(now);
	}
	pop_zero(ret);
}

void big_integer::subtract_module(big_integer const& a, big_integer const& b, std::vector <unsigned int>& ret) {
	ret = a.data;
	int carry = 0;
	for (size_t i = 0; (i < b.data.size()) || carry; ++i) {
		long long now = ret[i] * 1LL - (carry * 1LL + (i < b.data.size() ? b.data[i] : 0));
		if (now < 0) {
			carry = 1;
			now += base;
		}
		else {
			carry = 0;
		}
		ret[i] = (unsigned int)(now);
	}
	pop_zero(ret);
}

big_integer& big_integer::operator += (big_integer const& other) {
	std::vector <unsigned int> tmp;
	if (isNegate == other.isNegate) {
		add_module(*this, other, tmp);
		this->data = tmp;
	}
	else {
		if (compare_module(*this, other)) {
			subtract_module(*this, other, tmp);
			this->data = tmp;
		}
		else {
			subtract_module(other, *this, tmp);
			this->data = tmp;
			this->isNegate = other.isNegate;
		}
		make_positive(*this);
	}
	return *this;
}

big_integer& big_integer::operator -= (big_integer const& other) {
	std::vector <unsigned int> tmp;
	if (isNegate != other.isNegate) {
		add_module(*this, other, tmp);
		this->data = tmp;
	}
	else {
		if (compare_module(*this, other)) {
			subtract_module(*this, other, tmp);
			this->data = tmp;
		}
		else {
			subtract_module(other, *this, tmp);
			this->data = tmp;
			this->isNegate = !other.isNegate;
		}
		make_positive(*this);
	}
	return *this;
}

big_integer operator + (big_integer lhs, big_integer const& rhs) {
	lhs += rhs;
	return lhs;
}

big_integer operator - (big_integer lhs, big_integer const& rhs) {
	lhs -= rhs;
	return lhs;
}

big_integer operator * (big_integer lhs, big_integer const& rhs) {
	lhs *= rhs;
	return lhs;
}

big_integer operator % (big_integer lhs, big_integer const& rhs) {
	lhs %= rhs;
	return lhs;
}

big_integer operator&(big_integer a, big_integer const & b)
{
	a &= b;
	return a;
}

big_integer operator|(big_integer a, big_integer const & b)
{
	a |= b;
	return a;
}

big_integer operator^(big_integer a, big_integer const & b)
{
	a ^= b;
	return a;
}

void big_integer::division_by_const(unsigned int b) {
	std::vector <unsigned int> a = this->data;
	unsigned int carry = 0;
	for (int i = (int)a.size() - 1; i >= 0; --i) {
		unsigned long long cur = a[i] + carry * 1ULL * base;
		a[i] = (unsigned int)(cur / b);
		carry = (unsigned int)(cur % b);
	}
	pop_zero(a);
	this->data = a;
}

void big_integer::do_division(big_integer const& rhs, big_integer& remaind, std::vector <unsigned int>& tmp) {
	big_integer b(rhs);
	b.isNegate = false;
	unsigned int more = make_normalized(b, remaind);
	int m = (remaind.data.size() - b.data.size());
	int n = b.data.size();
	tmp.resize(m + 1, 0);
	big_integer b_m = bin_pow(m);
	if (!compare_module((b_m * b), remaind)) {
		tmp[m] = 1;
		remaind -= (b_m * b);
	}
	else {
		tmp[m] = 0;
	}
	for (int j = m - 1; j >= 0; j--) {
		unsigned long long now = (unsigned int)(n + j) < remaind.data.size() ? remaind.data[n + j] * 1ULL * base : 0;
		now += (unsigned int)(n + j - 1) < remaind.data.size() ? remaind.data[n + j - 1] * 1ULL : 0;
		now /= (b.data[n - 1] * 1ULL);
		now = std::min(now, base * 1ULL - 1);
		tmp[j] = now;
		big_integer b_j = bin_pow(j);
		big_integer cur = b;
		cur *= b_j;
		cur.multiply_by_const((unsigned int)now);
		remaind -= cur;
		while (remaind < 0) {
			tmp[j]--;
			remaind += (b_j * b);
		}
	}
	pop_zero(tmp);
	remaind.division_by_const(more);
}

big_integer& big_integer::operator /= (big_integer const& other) {
	std::vector <unsigned int> tmp(1, 0);
	if (compare_module(other, *this)) {
		this->isNegate = false;
		this->data = tmp;
	}
	else {
		big_integer remaind(*this);
		remaind.isNegate = false;
		do_division(other, remaind, tmp);
		this->data = tmp;
		this->isNegate = (this->isNegate ^ other.isNegate);
		make_positive(*this);
	}
	return *this;
}

big_integer& big_integer::operator %= (big_integer const& other) {
	std::vector <unsigned int> tmp(1, 0);
	if (compare_module(other, *this)) {
		//nothing
	}
	else {
		big_integer remaind(*this);
		remaind.isNegate = false;
		do_division(other, remaind, tmp);
		std::vector <unsigned int> cur = remaind.data;
		pop_zero(cur);
		this->data = cur;
		make_positive(*this);
	}
	return *this;
}

void big_integer::make_equal(size_t sz, std::vector<unsigned int>& tmp, std::vector<unsigned int>& rhs_to) {
	while (tmp.size() < sz) {
		tmp.push_back(0);
	}
	while (rhs_to.size() < sz) {
		rhs_to.push_back(0);
	}
}

void big_integer::convert(std::vector <unsigned int>& tmp, bool sign) {
	if (sign) {
		for (size_t i = 0; i < tmp.size(); i++) {
			tmp[i] = ~tmp[i];
		}
		unsigned int carry = 1;
		for (size_t i = 0; carry > 0; i++) {
			long long now = carry + tmp[i];
			if (now >= base) {
				carry = 1;
				tmp[i] = (unsigned int)(now - base);
			}
			else {
				tmp[i] = (unsigned int)(now);
				carry = 0;
			}
		}
	}
}

void big_integer::do_and(std::vector<unsigned int> & tmp, std::vector<unsigned int> const& rhs) {
	for (size_t i = 0; i < tmp.size(); i++) {
		tmp[i] = tmp[i] & rhs[i];
	}
}

void big_integer::do_or(std::vector<unsigned int>& tmp, std::vector<unsigned int> const & rhs)
{
	for (size_t i = 0; i < tmp.size(); i++) {
		tmp[i] = tmp[i] | rhs[i];
	}
}

void big_integer::do_xor(std::vector<unsigned int>& tmp, std::vector<unsigned int> const & rhs)
{
	for (size_t i = 0; i < tmp.size(); i++) {
		tmp[i] = tmp[i] ^ rhs[i];
	}
}

big_integer & big_integer::operator&=(big_integer const & rhs)
{
	std::vector <unsigned int> tmp(this->data);
	std::vector <unsigned int> rhs_to(rhs.data);
	size_t sz = std::max(tmp.size(), rhs_to.size()) + 1;
	make_equal(sz, tmp, rhs_to);
	convert(tmp, this->isNegate);
	convert(rhs_to, rhs.isNegate);
	do_and(tmp, rhs_to);
	bool sign = (this->isNegate) & rhs.isNegate;
	convert(tmp, sign);
	pop_zero(tmp);
	this->data = tmp;
	this->isNegate = sign;
	return *this;
}

big_integer & big_integer::operator|=(big_integer const & rhs) {
	std::vector <unsigned int> tmp(this->data);
	std::vector <unsigned int> rhs_to(rhs.data);
	size_t sz = std::max(tmp.size(), rhs_to.size()) + 1;
	make_equal(sz, tmp, rhs_to);
	convert(tmp, this->isNegate);
	convert(rhs_to, rhs.isNegate);
	do_or(tmp, rhs_to);
	bool sign = (this->isNegate) | rhs.isNegate;
	convert(tmp, sign);
	pop_zero(tmp);
	this->data = tmp;
	this->isNegate = sign;
	return *this;
}

big_integer & big_integer::operator^=(big_integer const & rhs)
{
	std::vector <unsigned int> tmp(this->data);
	std::vector <unsigned int> rhs_to(rhs.data);
	size_t sz = std::max(tmp.size(), rhs_to.size()) + 1;
	make_equal(sz, tmp, rhs_to);
	convert(tmp, this->isNegate);
	convert(rhs_to, rhs.isNegate);
	do_xor(tmp, rhs_to);
	bool sign = (this->isNegate) ^ rhs.isNegate;
	convert(tmp, sign);
	pop_zero(tmp);
	this->data = tmp;
	this->isNegate = sign;
	return *this;

}

big_integer& big_integer::operator *= (big_integer const& other) {
	std::vector <unsigned int> tmp(this->data.size() + other.data.size(), 0);
	for (size_t i = 0; i < this->data.size(); ++i) {
		unsigned int carry = 0;
		for (int j = 0; j < (int)other.data.size() || carry; ++j) {
			unsigned long long cur = tmp[i + j] * 1ULL + this->data[i] * 1ULL * (j < (int)other.data.size() ? other.data[j] : 0) + carry;
			tmp[i + j] = (unsigned int)(cur % base);
			carry = (unsigned int)(cur / base);
		}
	}
	pop_zero(tmp);
	this->data = tmp;
	this->isNegate = ((this->isNegate) ^ (other.isNegate));
	make_positive(*this);
	return *this;
}

bool big_integer::compare_module(big_integer const& lhs, big_integer const& rhs) const { // >
	if (lhs.data.size() != rhs.data.size()) {
		return lhs.data.size() > rhs.data.size();
	}
	for (int i = (int)lhs.data.size() - 1; i >= 0; --i) {
		if (lhs.data[i] > rhs.data[i]) {
			return true;
		}
		else if (rhs.data[i] > lhs.data[i]) {
			return false;
		}
		else {
			continue;
		}
	}
	return false;
}

std::string to_string(big_integer const& other) {
	std::vector <unsigned int> a = other.data;
	std::string ans = "";
	while (true) {
		unsigned int carry = 0;
		for (int i = (int)a.size() - 1; i >= 0; --i) {
			long long cur = a[i] + carry * 1ll * base;
			a[i] = (unsigned int)(cur / 10);
			carry = (unsigned int)(cur % 10);
		}
		while (a.size() > 1 && a.back() == 0) {
			a.pop_back();
		}
		ans += (char)(carry + '0');
		if (a.size() == 1 && a[0] == 0) {
			break;
		}
	}
	if (other.isNegate) {
		ans += '-';
	}
	if (ans == "0-") {
		ans = "0";
	}
	reverse(ans.begin(), ans.end());
	return ans;
}

std::ostream & operator<<(std::ostream & s, big_integer const & a)
{
	s << to_string(a);
	return s;
}

big_integer big_integer::operator+() const {
	return *this;
}

big_integer big_integer::operator-() const {
	big_integer tmp(*this);
	tmp.isNegate ^= true;
	tmp.make_positive(tmp);
	return tmp;
}

big_integer &big_integer::operator++() {
	*this += 1;
	return *this;
}

big_integer big_integer::operator++(int) {
	big_integer tmp(*this);
	tmp += 1;
	swap(tmp);
	return tmp;
}

big_integer &big_integer::operator--() {
	*this -= 1;
	return *this;
}

big_integer big_integer::operator--(int) {
	big_integer tmp(*this);
	tmp -= 1;
	swap(tmp);
	return tmp;
}

void big_integer::swap(big_integer &a) {
	std::vector <unsigned int> tmp(a.data);
	bool isNeg = a.isNegate;
	a.data = this->data;
	a.isNegate = this->isNegate;
	this->isNegate = isNeg;
	this->data = tmp;
}

big_integer big_integer::operator~() const {
	big_integer tmp(*this);
	tmp += 1;
	tmp.isNegate = !isNegate;
	return tmp;
}

big_integer &big_integer::operator<<=(int rhs) {
	int add = rhs / 32;
	rhs %= 32;
	std::vector <unsigned int> tmp(this->data);
	add_zero(add, tmp);
	big_integer cnt(tmp, false);
	unsigned int more = ((unsigned int)1) << rhs;
	cnt.multiply_by_const(more);
	swap(cnt);
	return *this;
}

void big_integer::add_zero(int add, std::vector<unsigned int> & now) {
	std::vector <unsigned int> tmp;
	for (int i = 0; i < add; i++)
		tmp.push_back(0);
	for (size_t i = 0; i < now.size(); ++i) {
		tmp.push_back(now[i]);
	}
	now = tmp;
}

big_integer::big_integer(std::vector<unsigned int> const &a, bool sign) :data(a), isNegate(sign) {}

big_integer &big_integer::operator>>=(int rhs) {
	int complete = rhs >> 5;
	rhs &= 31;
	big_integer ans(*this);
	unsigned int tmp = 0;
	for (size_t i = this->data.size() - 1; ; --i) {
		unsigned int digit = ans.data[i];
		ans.data[i] = (tmp << (32 - rhs)) + (ans.data[i] >> rhs);
		tmp = digit & ((1ll << rhs) - 1);
		if (!i) break;
	}
	ans.correct();
	if (!is_deg2() && this->isNegate) ans -= 1;
	this->swap(ans);
	return *this;
}

big_integer operator / (big_integer lhs, big_integer const& rhs) {
	lhs /= rhs;
	return lhs;
}

big_integer operator<<(big_integer lhs, int rhs)
{
	lhs <<= rhs;
	return lhs;
}

big_integer operator>>(big_integer lhs, int rhs)
{
	lhs >>= rhs;
	return lhs;
}
