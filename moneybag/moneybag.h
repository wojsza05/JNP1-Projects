#ifndef MONEYBAG
#define MONEYBAG

#include <cstddef>
#include <iostream>

class Moneybag {
public:
    using coin_number_t = uint64_t;

    constexpr Moneybag(uint64_t num_livres, uint64_t num_soliduses,
                       uint64_t num_deniers);
    constexpr Moneybag(const Moneybag &m);
    constexpr Moneybag(Moneybag &&m);

    constexpr coin_number_t livre_number() const;
    constexpr coin_number_t solidus_number() const;
    constexpr coin_number_t denier_number() const;

    constexpr Moneybag &operator = (const Moneybag &m);
    constexpr Moneybag &operator = (Moneybag &&m);

    constexpr Moneybag &operator += (const Moneybag &m);
    constexpr Moneybag &operator -= (const Moneybag &m);
    constexpr Moneybag &operator *= (const coin_number_t number);

    constexpr Moneybag operator + (const Moneybag &m) const;
    constexpr Moneybag operator - (const Moneybag &m) const;

    constexpr explicit operator bool() const;

    constexpr std::partial_ordering operator <=> (const Moneybag &m) const;
    constexpr bool operator == (const Moneybag &m) const;

private:
    coin_number_t livres, soliduses, deniers;
};

constexpr Moneybag operator * (const Moneybag::coin_number_t number, 
                               const Moneybag &mconstexpr);
constexpr Moneybag operator * (const Moneybag &m, 
                               const Moneybag::coin_number_t number);

std::ostream &operator << (std::ostream &os, const Moneybag &m);

class Value {
public:
    constexpr Value();
    constexpr Value(const Moneybag &m);
    constexpr Value(const uint64_t number);
    constexpr Value(const Value &v);
    constexpr Value(Value &&v);

    constexpr std::strong_ordering operator <=> (const Value &v) const;
    constexpr std::strong_ordering operator <=> (const uint64_t number) const;
    constexpr Value &operator = (const Value &v);
    constexpr Value &operator = (const Value &&v);
    constexpr bool operator == (const Value &v) const;
    constexpr bool operator == (const uint64_t number) const;
    explicit operator std::string() const;

private:
    static const __uint128_t DENIERS_IN_LIVRE = 240;
    static const __uint128_t DENIERS_IN_SOLIDUS = 12;
    __uint128_t value = 0;
};

constexpr Moneybag::Moneybag(uint64_t num_livres, uint64_t num_soliduses,
                             uint64_t num_deniers) : 
    livres(num_livres), soliduses(num_soliduses), deniers(num_deniers){};

constexpr Moneybag::Moneybag(const Moneybag &m) : 
    livres(m.livres), soliduses(m.soliduses), deniers(m.deniers){};

constexpr Moneybag::Moneybag(Moneybag &&m) : 
    livres(std::move(m.livres)), soliduses(std::move(m.soliduses)), 
    deniers(std::move(m.deniers)){};

constexpr Moneybag::coin_number_t Moneybag::livre_number() const {
    return livres;
}

constexpr Moneybag::coin_number_t Moneybag::solidus_number() const {
    return soliduses;
}

constexpr Moneybag::coin_number_t Moneybag::denier_number() const {
    return deniers;
}

constexpr Moneybag &Moneybag::operator = (const Moneybag &m) {
    if (&m != this) {
        livres = m.livres;
        soliduses = m.soliduses;
        deniers = m.deniers;
    }
    return *this;
}

constexpr Moneybag &Moneybag::operator = (Moneybag &&m) {
    if (&m != this) {
        livres = std::move(m.livres);
        soliduses = std::move(m.soliduses);
        deniers = std::move(m.deniers);
    }
    return *this;
}

constexpr Moneybag &Moneybag::operator += (const Moneybag &m) {
    if (UINT64_MAX - livres < m.livres || UINT64_MAX - soliduses < m.soliduses 
        || UINT64_MAX - deniers < m.deniers)
        throw std::out_of_range("The number of coins in the resulting moneybag "
                                "would exceed the allowable range");

    livres += m.livres;
    soliduses += m.soliduses;
    deniers += m.deniers;
    return *this;
}

constexpr Moneybag &Moneybag::operator -= (const Moneybag &m) {
    if (livres < m.livres || soliduses < m.soliduses || deniers < m.deniers)
        throw std::out_of_range("The number of coins of a certain type in the "
                                "resulting moneybag would fall below zero");

    livres -= m.livres;
    soliduses -= m.soliduses;
    deniers -= m.deniers;
    return *this;
}

constexpr Moneybag &Moneybag::operator *= (const coin_number_t number) {
    if (number > 0 && (UINT64_MAX / number < livres 
        || UINT64_MAX / number < soliduses || UINT64_MAX / number < deniers))
        throw std::out_of_range("The number of coins in the resulting moneybag "
                                "would exceed the allowable range");

    livres *= number;
    soliduses *= number;
    deniers *= number;
    return *this;
}

constexpr Moneybag Moneybag::operator + (const Moneybag &m) const {
    return Moneybag(*this) += m;
}

constexpr Moneybag Moneybag::operator - (const Moneybag &m) const {
    return Moneybag(*this) -= m;
}

constexpr std::partial_ordering Moneybag::operator <=> (const Moneybag &m) const {
    coin_number_t livres2 = m.livre_number();
    coin_number_t soliduses2 = m.solidus_number();
    coin_number_t deniers2 = m.denier_number();

    if (livres == livres2 && soliduses == soliduses2 && deniers == deniers2)
        return std::partial_ordering::equivalent;
    if (livres <= livres2 && soliduses <= soliduses2 && deniers <= deniers2)
        return std::partial_ordering::less;
    if (livres >= livres2 && soliduses >= soliduses2 && deniers >= deniers2)
        return std::partial_ordering::greater;
    return std::partial_ordering::unordered;
}

constexpr bool Moneybag::operator == (const Moneybag &m) const {
    coin_number_t livres2 = m.livre_number();
    coin_number_t soliduses2 = m.solidus_number();
    coin_number_t deniers2 = m.denier_number();
    return livres == livres2 && soliduses == soliduses2 && deniers == deniers2;
}

constexpr Moneybag::operator bool() const {
    return livres > 0 || soliduses > 0 || deniers > 0;
}

constexpr Moneybag operator * (const Moneybag::coin_number_t number, 
                               const Moneybag &m) {
    return Moneybag(m) *= number;
}

constexpr Moneybag operator*(const Moneybag &m, 
                             const Moneybag::coin_number_t number) {
    return Moneybag(m) *= number;
}

constexpr Value::Value() {};

constexpr Value::Value(const Moneybag &m) 
    : value((__uint128_t)m.livre_number() * DENIERS_IN_LIVRE 
        + (__uint128_t)m.solidus_number() * DENIERS_IN_SOLIDUS 
        + (__uint128_t)m.denier_number()) {};

constexpr Value::Value(uint64_t number) : value(number) {};

constexpr Value::Value(const Value &v) : value(v.value){};

constexpr Value::Value(Value &&v) : value(std::move(v.value)){};

constexpr std::strong_ordering Value::operator <=> (const Value &v) const {
    return value <=> v.value;
}

constexpr std::strong_ordering Value::operator <=> (const uint64_t number) const {
    return value <=> number;
}

constexpr Value &Value::operator = (const Value &v) {
    if (&v != this)
        value = v.value;
    return *this;
}

constexpr Value &Value::operator = (const Value &&v) {
    if (&v != this)
        value = std::move(v.value);
    return *this;
}

constexpr bool Value::operator == (const Value &v) const {
    return value == v.value;
}

constexpr bool Value::operator == (const uint64_t number) const {
    return value == number;
}

constexpr const Moneybag Livre(1, 0, 0);
constexpr const Moneybag Solidus(0, 1, 0);
constexpr const Moneybag Denier(0, 0, 1);

#endif /* __MONEYBAG__ */
