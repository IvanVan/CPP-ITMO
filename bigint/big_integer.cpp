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

void big_integer::divide_by_32()
{
	this->data.pop_back();
	if (!this->data.empty()) {
		this->data[(this->data.size()) - 1] = 1;
	}
}

unsigned int big_integer::calculate(unsigned int a, unsigned int b, unsigned int c)
{
	unsigned long long res = a;
	res = ((res << (32ULL)) + b) / c;
	res = std::min(res, base * 1ULL - 1);
	return (unsigned int)res;
}

bool big_integer::compare_equal_vectors(vector<unsigned int> const & a, vector<unsigned int> const & b)
{
	for (size_t i = a.size(); i > 0; i--) {
		if (a[i - 1] != b[i - 1]) {
			return a[i - 1] < b[i - 1];
		}
	}
	return false;
}

void big_integer::sub_equal_vectors(vector<unsigned int>& a, vector<unsigned int> const & b)
{
	unsigned int carry = 0;
	for (size_t i = 0; i < b.size(); ++i) {
		long long sum = a[i] * 1LL - carry - b[i];
		if (sum < 0) {
			carry = 1;
			a[i] = (unsigned int)(base + sum);
		}
		else {
			carry = 0;
			a[i] = (unsigned int)(sum);
		}
	}
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

void big_integer::mul_vector_by_const(std::vector<unsigned int>& res, std::vector<unsigned int> const & a, unsigned int const& b)
{
	unsigned int carry = 0;
	size_t n = a.size();
	res.resize(n + 1);
	for (size_t i = 0; i < n; ++i) {
		unsigned long long cur = carry + a[i] * 1ULL * b;
		res[i] = (unsigned int)(cur % base);
		carry = (unsigned int)(cur / base);
	}
	res[n] = carry;
}

big_integer big_integer::do_division(big_integer const& a, big_integer const& b) {
	big_integer abs_a(a.data, false);
	big_integer abs_b(b.data, false);
	size_t n = a.data.size();
	size_t m = b.data.size();
	if (m > n) {
		return 0;
	}
	unsigned int f = (unsigned int)((UINT32_MAX * 1ULL + 1) / (abs_b.data.back() * 1ULL + 1));
	abs_a.multiply_by_const(f);
	abs_b.multiply_by_const(f);

	const size_t len = n - m + 1;
	const unsigned int divisor = abs_b.data.back();
	vector<unsigned int> temp(len);
	vector<unsigned int> dev(m + 1), div(m + 1, 0);
	for (size_t i = 0; i < m; i++) {
		dev[i] = abs_a.data[n + i - m];
	}
	dev[m] = abs_a.data.size() > n ? abs_a.data[n] : 0;
	for (size_t i = 0; i < len; i++) {
		dev[0] = abs_a.data[n - m - i];
		size_t ri = len - 1 - i;
		unsigned int tq = calculate(dev[m], dev[m - 1], divisor);
		mul_vector_by_const(div, abs_b.data, tq);
		while ((tq >= 0) && compare_equal_vectors(dev, div)) {
			mul_vector_by_const(div, abs_b.data, --tq);
		}
		sub_equal_vectors(dev, div);
		for (size_t j = m; j > 0; j--) {
			dev[j] = dev[j - 1];
		}
		temp[ri] = tq;
	}
	pop_zero(temp);
	return big_integer(temp, a.isNegate ^ b.isNegate);
}

big_integer& big_integer::operator /= (big_integer const& other) {
	std::vector <unsigned int> tmp(1, 0);
	if (compare_module(other, *this)) {
		this->isNegate = false;
		this->data = tmp;
	}
	else {
		*this = do_division(*this, other);
	}
	return *this;
}

big_integer& big_integer::operator %= (big_integer const& other) {
	big_integer tmp(*this);
	*this = tmp - (tmp / other) * other;
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
