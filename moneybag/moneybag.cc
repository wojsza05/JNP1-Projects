#include "moneybag.h"

static const __uint128_t LARGE_POWER_OF_TEN = 1e16;

std::ostream &operator << (std::ostream &os, const Moneybag &m) {
    Moneybag::coin_number_t livres = m.livre_number();
    Moneybag::coin_number_t soliduses = m.solidus_number();
    Moneybag::coin_number_t deniers = m.denier_number();

    os << "("
       << livres << " livr" << (livres != 1 ? "es" : "") << ", "
       << soliduses << " solidus" << (soliduses != 1 ? "es" : "") << ", "
       << deniers << " denier" << (deniers != 1 ? "s" : "") << ")";
    return os;
}

Value::operator std::string() const {
    // We print the first, middle and last 10^16 digits of value.
    __uint128_t temp = value;
    uint64_t back_numbers = temp % LARGE_POWER_OF_TEN;
    temp = temp / LARGE_POWER_OF_TEN;
    uint64_t middle_numbers = temp % LARGE_POWER_OF_TEN;
    temp = temp / LARGE_POWER_OF_TEN;
    uint64_t lead_numbers = temp % LARGE_POWER_OF_TEN;

    if ((lead_numbers == 0) && (middle_numbers == 0))
        return std::to_string(back_numbers);
    else if (lead_numbers == 0)
        return std::to_string(middle_numbers) + std::to_string(back_numbers);
    return std::to_string(lead_numbers) + std::to_string(middle_numbers) 
         + std::to_string(back_numbers);
}
