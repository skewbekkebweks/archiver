#include <rational.h>
#include <numeric>
#include <string>
#include <compare>

int64_t Mul64(int a, int b) {
    return static_cast<int64_t>(a) * static_cast<int64_t>(b);
}

Rational GetInverse(Rational r) {
    return Rational(r.GetDenominator(), r.GetNumerator());
}

Rational::Rational() : Rational(0) {
}

Rational::Rational(int value) : Rational(value, 1) {  // NOLINT
}

Rational::Rational(int numer, int denom) {
    Set(numer, denom);
}

int Rational::GetNumerator() const {
    return numer_;
}

int Rational::GetDenominator() const {
    return denom_;
}

void Rational::SetNumerator(int value) {
    Set(value, denom_);
}

void Rational::SetDenominator(int value) {
    Set(numer_, value);
}

Rational& operator+=(Rational& lhs, const Rational& rhs) {
    lhs.Set(Mul64(lhs.numer_, rhs.denom_) + Mul64(rhs.numer_, lhs.denom_), Mul64(lhs.denom_, rhs.denom_));
    return lhs;
}

Rational& operator*=(Rational& lhs, const Rational& rhs) {
    lhs.Set(Mul64(lhs.numer_, rhs.numer_), Mul64(lhs.denom_, rhs.denom_));
    return lhs;
}

Rational& operator++(Rational& ratio) {
    ratio.SetNumerator(ratio.GetNumerator() + ratio.GetDenominator());
    return ratio;
}

Rational& operator--(Rational& ratio) {
    ratio.SetNumerator(ratio.GetNumerator() - ratio.GetDenominator());
    return ratio;
}

std::istream& operator>>(std::istream& is, Rational& ratio) {
    std::string rational;
    is >> rational;
    size_t delimeter_index = rational.find('/');
    int64_t numer = static_cast<int64_t>(std::stoll(rational.substr(0, delimeter_index)));
    int64_t denom = 1;
    if (delimeter_index != std::string::npos) {
        denom = static_cast<int64_t>(std::stoll(rational.substr(delimeter_index + 1)));
    }
    ratio.Set(numer, denom);
    return is;
}

void Rational::Set(int64_t numer, int64_t denom) {
    if (denom == 0) {
        throw RationalDivisionByZero{};
    }

    int64_t gcd = std::gcd(numer, denom);
    numer /= gcd;
    denom /= gcd;
    if (denom < 0) {
        numer *= -1;
        denom *= -1;
    }
    numer_ = static_cast<int>(numer);
    denom_ = static_cast<int>(denom);
}

Rational operator+(const Rational& ratio) {
    return ratio;
}

Rational operator-(const Rational& ratio) {
    return ratio * -1;
}

Rational& operator-=(Rational& lhs, const Rational& rhs) {
    return lhs += -rhs;
}

Rational& operator/=(Rational& lhs, const Rational& rhs) {
    return lhs *= GetInverse(rhs);
}

Rational operator+(const Rational& lhs, const Rational& rhs) {
    Rational lhs_copy = lhs;

    return lhs_copy += rhs;
}

Rational operator-(const Rational& lhs, const Rational& rhs) {
    return lhs + -rhs;
}

Rational operator*(const Rational& lhs, const Rational& rhs) {
    Rational lhs_copy = lhs;

    return lhs_copy *= rhs;
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
    Rational lhs_copy = lhs;

    return lhs_copy /= rhs;
}

Rational operator++(Rational& ratio, int) {
    Rational ratio_copy = ratio;
    ++ratio;
    return ratio_copy;
}

Rational operator--(Rational& ratio, int) {
    Rational ratio_copy = ratio;
    --ratio;
    return ratio_copy;
}

std::strong_ordering operator<=>(const Rational& lhs, const Rational& rhs) {
    int64_t lhs_numer = Mul64(lhs.GetNumerator(), rhs.GetDenominator());
    int64_t rhs_numer = Mul64(rhs.GetNumerator(), lhs.GetDenominator());

    return lhs_numer <=> rhs_numer;
}

bool operator==(const Rational& lhs, const Rational& rhs) {
    return lhs.GetNumerator() == rhs.GetNumerator() && lhs.GetDenominator() == lhs.GetDenominator();
}

std::ostream& operator<<(std::ostream& os, const Rational& ratio) {
    if (ratio.GetDenominator() == 1) {
        return os << ratio.GetNumerator();
    }
    return os << ratio.GetNumerator() << '/' << ratio.GetDenominator();
}