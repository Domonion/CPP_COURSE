#include "big_integer.h"

using namespace std;

typedef unsigned int uint;
typedef unsigned long long ull;

uint get(vector<uint> const &a, int i) {
    return (i < size(a) ? a[i] : 0);
}

void big_integer::check() {
    num = correct(num);
    if (num.back() == 0)
        minus = false;
}

big_integer::big_integer(vector<uint> const &number, bool sign) : minus(sign), num(number) {
    check();
}

big_integer::big_integer() : minus(false) {
    num.push_back(0);
}

big_integer::big_integer(int a) : minus(a < 0) {
    if (minus) {
        a++;
        a *= -1;
        num.push_back(static_cast<uint>(a) + 1);
    } else {
        num.push_back(a);
    }
}

big_integer::big_integer(ull a) : minus(false) {
    while (a) {
        num.push_back(a & UINT_MAX);
        a >>= sizeof(uint) * 8;
    }
}

big_integer::big_integer(uint a) : minus(false) {
    num.push_back(a);
}

big_integer::big_integer(string s) {
    big_integer res(0);
    reverse(s.begin(), s.end());
    minus = false;
    if (s.back() == '-')
        s.pop_back(), minus = true;
    while (!s.empty()) {
        res *= 10;
        res += s.back() - '0';
        s.pop_back();
    }
    num = res.num;
    check();
}

big_integer &big_integer::operator+=(big_integer const &b) {
    if (!minus && !b.minus) {
        minus = false;
        add(num, b.num, num);
    } else if (minus && b.minus) {
        minus = true;
        add(num, b.num, num);
    } else if (!minus && b.minus) {
        minus = isLess(num, b.num);
        sub(num, b.num, num);
    } else if (minus && !b.minus) {
        minus = isLess(b.num, num);
        sub(b.num, num, num);
    }
    return *this;
}

big_integer &big_integer::operator-=(big_integer const &b) {
    if (!minus && !b.minus) {
        minus = isLess(num, b.num);
        sub(num, b.num, num);
    } else if (minus && b.minus) {
        minus = isLess(b.num, num);
        sub(num, b.num, num);
    } else if (!minus && b.minus) {
        minus = false;
        add(num, b.num, num);
    } else if (minus && !b.minus) {
        minus = true;
        add(num, b.num, num);
    }
    return *this;
}

vector<uint> divU(vector<uint> const &a, uint b) {
    big_integer res;
    res.num.resize(size(a));
    ull carry = 0;
    for (int i = size(a) - 1; i >= 0; i--) {
        ull now = a[i] + (carry << 32);
        res.num[i] = now / b;
        carry = now % b;
    }
    correct(res.num);
    return res.num;
}

big_integer &big_integer::operator/=(big_integer const &b) {
    if (size(b.num) == 1) {
        num = divU(num, b.num[0]);
        minus ^= b.minus;
    } else if (b != 0) {
        if (isLess(num, b.num)) {
            num = vector<uint>(1, 0);
            minus = false;
        } else {
            minus = minus ^ b.minus;
            vector<uint> res;
            fastdiv(num, b.num, res);
            num = res;
        }
    }
    check();
    return *this;
}

big_integer &big_integer::operator%=(big_integer const &b) {
    big_integer buffer(*this);
    buffer /= b;
    buffer *= b;
    *this -= buffer;
    return *this;
}

big_integer &big_integer::operator*=(big_integer const &b) {
    minus ^= b.minus;
    mul(num, b.num, num);
    return *this;
}

big_integer &big_integer::operator&=(big_integer const &b) {
    vector<uint> lhs = (minus ? (inverse()).getNum() : getNum());
    vector<uint> rhs = (b.minus ? (b.inverse()).getNum() : b.getNum());
    and_long(lhs, rhs, num);
    if (minus) {
        num = this->inverse().getNum();
        minus = true;
    }
    return *this;
}

big_integer &big_integer::operator|=(big_integer const &b) {
   or_long((minus ? (inverse()).num : num),
                  (b.minus ? (b.inverse()).num : b.num), num);
    minus |= b.minus;
    if (minus) {
        num = this->inverse().getNum();
        minus = true;
    }
    return *this;
}

big_integer &big_integer::operator^=(big_integer const &b) {
    xor_long((minus ? (inverse()).getNum() : getNum()),
                   (b.minus ? (b.inverse()).getNum() : b.getNum()), num);
    minus ^= b.minus;
    if (minus) {
        num = this->inverse().getNum();
        minus = true;
    }
    return *this;
}

big_integer &big_integer::operator<<=(uint b) {
    shift_left(num, b,num );
    return *this;
}

big_integer &big_integer::operator>>=(uint b) {
    shift_right(num, b, num);
    if (minus)
        add(num, vector<uint>(1, 1), num);
    check();
    return *this;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    big_integer res(num, !minus);
    return res;
}

big_integer big_integer::operator~() const {
    big_integer res(*this);
    return -(res + 1);
}

big_integer &big_integer::operator++() {
    *this += 1;
    return *this;
}

big_integer big_integer::operator++(int) {
    big_integer res = *this;
    ++*this;
    return res;
}

big_integer &big_integer::operator--() {
    *this -= 1;
    return *this;
}

big_integer big_integer::operator--(int) {
    big_integer res = *this;
    ++*this;
    return res;
}

bool operator<(big_integer const &a_in, big_integer const &b_in) {
    bool cond = false;
    if (a_in.minus && b_in.minus)
        cond = true;
    if (a_in.minus && !b_in.minus)
        return true;
    if (!a_in.minus && b_in.minus)
        return false;
    big_integer const &a = (cond ? b_in : a_in);
    big_integer const &b = (cond ? a_in : b_in);
    return isLess(a.num, b.num);
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return !(a < b);
}

bool operator==(big_integer const &a, big_integer const &b) {
    if (a.minus != b.minus)
        return false;
    return a.num == b.num;
}

bool operator>(big_integer const &a, big_integer const &b) {
    return b < a;
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return !(a == b);
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return !(a > b);
}

string to_string(big_integer const &a) {
    string res;
    big_integer buf = a;
    if (buf == 0)
        return "0";
    while (buf != 0) {
        res += to_string((buf % 10).getNum().back());
        buf /= 10;
    }
    if (a.minus)
        res.push_back('-');
    reverse(res.begin(), res.end());
    return res;
}

ostream &operator<<(ostream &os, big_integer const &a) {
    string output = to_string(a);
    os << a;
    return os;
}

big_integer operator+(big_integer a, big_integer const &b) {
    return a += b;
}

big_integer operator-(big_integer a, big_integer const &b) {
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const &b) {
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const &b) {
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const &b) {
    return a %= b;
}

big_integer big_integer::inverse() const {
    big_integer res(*this);
    for (auto &i : res.num) i = UINT_MAX - i;
    res.minus ^= true;
    return res + 1;
}

big_integer operator&(big_integer a, big_integer const &b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const &b) {
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const &b) {
    return a ^= b;
}

big_integer operator<<(big_integer a, int b) {
    return a <<= b;
}

big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}

bool isLess(vector<uint> const &a, vector<uint> const &b) {
    if (size(a) < size(b))
        return true;
    if (size(a) > size(b))
        return false;
    return lexicographical_compare(a.rbegin(), a.rend(), b.rbegin(), b.rend());
}

inline vector<uint> &correct(vector<uint> &res) {
    while (size(res) > 1 && res.back() == 0)
        res.pop_back();
    return res;
}

void add(vector<uint> const &a, vector<uint> const &b, vector<uint> &res) {
    res.resize(max(size(a), size(b)) + 1);
    uint carry = 0;
    for (int i = 0; i < max(size(a), size(b)); i++) {
        uint val_a = get(a, i);
        uint val_b = get(b, i);
        res[i] = carry + val_a + val_b;
        if (UINT_MAX - val_a < val_b)
            carry = 1;
        else {
            carry = (UINT_MAX - val_a < val_b + carry);
        }
    }
    if (carry)
        res.back() = 1;
    correct(res);
}

void sub(vector<uint> const &a_in, vector<uint> const &b_in, vector<uint> &res) {
    bool cond = isLess(a_in, b_in);//sure???
    vector<uint> const &a = (cond ? b_in : a_in);
    vector<uint> const &b = (cond ? a_in : b_in);
    res.resize(size(a));
    for (int i = size(res) - 1; i >= 0; i--) {
        uint val_a = get(a, i);
        uint val_b = get(b, i);
        res[i] = val_a - val_b + (val_a < val_b) * UINT_MAX + (val_a < val_b);
        if (val_a < val_b) {
            int j = i + 1;
            while (res[j] == 0) {
                res[j]--;
                j++;
            }
            res[j]--;
        }
    }
    correct(res);
}

void mul(vector<uint> const &a, vector<uint> const &b, vector<uint> &c) {
    int n = a.size();
    c.resize(a.size() + b.size());
    c.back() = 0;
    int j = 0;
    uint carry = 0;
    ull cur = 0;
    for (int i = n - 1; i >= 0; --i) {
        uint ai = a[i];
        for (j = 0, carry = 0; j < (int) b.size() || carry > 0; ++j) {
            cur = (ull) c[i + j] * (j != 0) + (ull) ai * (ull) get(b, j);
            c[i + j] = cur + carry;
            carry = (cur + carry) >> 32;
        }
    }
    correct(c);
}

void fastdiv(vector<uint> const &a_in, vector<uint> const &b_in, vector<uint> &Q) {
    const uint f = (uint) ((1ll << 32) / ull(b_in.back() + 1));
    Q.resize(0);
    vector<uint> a;
    mul(a_in, vector<uint>(1, f), a);
    vector<uint> b;
    mul(b_in, vector<uint>(1, f), b);
    int n = size(a);
    vector<uint> now;
    for (int i = n - 1; i >= 0; i--) {
        now.insert(now.begin(), a[i]);
        while (size(now) > 1 && now.back() == 0) {
            now.pop_back();
        }
        if (isLess(now, b)) {
            if (now.back() == 0)
                Q.push_back(0);
            continue;
        }
        ull digit1 = ull(now.back());
        if (size(now) != size(b)) {
            digit1 <<= 32;
            digit1 += now[size(now) - 2];
        }
        ull digit2 = b.back();
        ull digit = digit1 / digit2;
        digit = min(digit, (1ull << 32) - 1);
        vector<uint> buf;
        mul(b, vector<uint>(1, digit), buf);
        if (isLess(now, buf)) {
            sub(buf, b, buf);
            digit--;
        }
        if (isLess(now, buf)) {
            ull l = 0, r = digit;
            while (r - l > 1) {
                ull mid = (r + l) / 2;
                vector<uint> buffer;
                mul(b, vector<uint>(1, mid), buffer);
                if (isLess(now, buffer)) {
                    r = mid;
                } else
                    l = mid;
            }
            digit = l;
            mul(b, vector<uint>(1, digit), buf);
        }
        sub(now, buf, now);
        Q.push_back(digit);
    }
    reverse(Q.begin(), Q.end());
}

void and_long(vector<uint> const &a, vector<uint> const &b, vector<uint> & res) {
    res.resize(max(size(a), size(b)));
    for (int i = 0; i < size(res); i++)res[i] = get(a, i) & get(b, i);
    correct(res);
}

void or_long(vector<uint> const &a, vector<uint> const &b, vector<uint> & res) {
    res.resize(max(size(a), size(b)));
    for (int i = 0; i < size(res); ++i)res[i] = get(a, i) | get(b, i);
    correct(res);
}

void xor_long(vector<uint> const &a, vector<uint> const &b, vector<uint> & res) {
    res.resize(max(size(a), size(b)));
    for (int i = 0; i < size(res); i++)res[i] = get(a, i) ^ get(b, i);
    correct(res);
}

int getDeg(uint a) {
    int res = 0;
    while (a)
        res++, a /= 2;
    return res;
}

void shift_left(vector<uint> const &a, int b, vector<uint> & res) {
    int maxDeg = getDeg(a.back());
    int rem = b % 32;
    int carry = (maxDeg + rem > 32);
    res.resize(size(a) + b / 32 + carry);
    copy(a.begin(), a.end(), res.begin());
    for (int i = (int) size(res) - 1; i >= (int) size(res) - (int) size(a); i--) {
        res[i] <<= rem;
        if (i != (int) size(res) - (int) size(a))
            res[i] += res[i - 1] >> (32 - maxDeg);
    }
    //return res;
}

void shift_right(vector<uint> const &a, int b,vector<uint> & res) {
    res.resize(max(0, (int) size(a) - b / 32));
    copy(a.begin() + b / 32, a.end(), res.begin());
    if (res.empty())
        res.push_back(0);
    else {
        uint next = 0;
        for (int i = 0; i < b; i++)next *= 2, next += 1;
        for (int i = 0; i < size(res); i++) {
            res[i] >>= b;
            if (i != size(res) - 1)
                res[i] += (res[i + 1] & next) << (sizeof(uint) * 8 - b);
        }
    }
    correct(res);
    //return res;
}