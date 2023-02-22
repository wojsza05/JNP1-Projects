#ifndef ORGANISM_H
#define ORGANISM_H

#include <optional>
#include <cstddef>
#include <tuple>
#include <cstdint>

namespace {
    // Funkcja zwraca:
    // 3 - dla wszystkożercy,
    // 2 - dla mięsożercy,
    // 1 - dla roślinożercy,
    // 0 - dla rośliny.
    constexpr int get_type(const bool eat_meat, const bool eat_plants) {
        if (eat_meat && eat_plants)
            return 3;
        if (eat_meat)
            return 2;
        if (eat_plants)
            return 1;
        return 0;
    }
}

template <std::equality_comparable species_t, bool can_eat_meat,
        bool can_eat_plants>
class Organism {
public:
    constexpr Organism(species_t const &species, uint64_t vitality) :
        vitality(vitality), species(species) {};

    constexpr uint64_t get_vitality() const {
        return vitality;
    }

    constexpr species_t const &get_species() const {
        return species;
    }

    constexpr bool is_dead() const {
        return vitality == 0;
    }

    constexpr void become_dead() {
        vitality = 0;
    }

    constexpr void add_vitality(uint64_t added_vitality) {
        vitality += added_vitality;
    }

private:
    uint64_t vitality;
    species_t const species;
};

template <typename species_t>
using Carnivore = Organism<species_t, true, false>;

template <typename species_t>
using Omnivore = Organism<species_t, true, true>;

template <typename species_t>
using Herbivore = Organism<species_t, false, true>;

template <typename species_t>
using Plant = Organism<species_t, false, false>;

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p,
        bool sp2_eats_m, bool sp2_eats_p>
constexpr std::tuple<Organism<species_t, sp1_eats_m, sp1_eats_p>,
        Organism<species_t, sp2_eats_m, sp2_eats_p>,
        std::optional<Organism<species_t, sp1_eats_m, sp1_eats_p>>>
encounter(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
          Organism<species_t, sp2_eats_m, sp2_eats_p> organism2) {
    constexpr int type1 = get_type(sp1_eats_m, sp1_eats_p);
    constexpr int type2 = get_type(sp2_eats_m, sp2_eats_p);

    uint64_t vitality1 = organism1.get_vitality();
    uint64_t vitality2 = organism2.get_vitality();

    species_t const &species1 = organism1.get_species();
    species_t const &species2 = organism2.get_species();

    // 2. Spotkanie dwóch roślin.
    static_assert(type1 != 0 || type2 != 0);

    // 3. Spotkanie, w którym jedna ze stron jest martwa.
    if (vitality1 == 0 || vitality2 == 0)
        return {organism1, organism2, {}};

    // 4. Spotkanie dwóch zwierząt tego samego gatunku (gody).
    else if (type1 == type2 && species1 == species2)
        return {organism1, organism2,
                Organism<species_t, sp1_eats_m, sp1_eats_p>
                        (species1, (vitality1 / 2 + vitality2 / 2)
                        + (vitality1 % 2 == 1 && vitality2 % 2 == 1 ? 1 : 0))};

    // 5. Spotkanie organizmów, które nie potrafią się zjadać.
    // Organizmy nie potrafią się zjadać, jeżeli obydwa są roślinożercami
    // lub jeden jest mięsożercą, a drugi rośliną.
    else if ((type1 == 1 && type2 == 1) || (type1 == 2 && type2 == 0)
             || (type1 == 0 && type2 == 2))
        return {organism1, organism2, {}};

    // 6. Spotkanie dwóch zwierząt, które potrafią się nawzajem zjadać (walka).
    // Organizmy potrafią się nawzajem zjadać, jeżeli obydwa są mięsożercami
    // lub wszystkożercami.
    else if (type1 >= 2 && type2 >= 2) {
        if (vitality1 == vitality2) {
            organism1.become_dead();
            organism2.become_dead();
        }
        else if (vitality1 > vitality2) {
            organism1.add_vitality(vitality2 / 2);
            organism2.become_dead();
        }
        else {
            organism2.add_vitality(vitality1/ 2);
            organism1.become_dead();
        }
        return {organism1, organism2, {}};
    }

    // 7. Spotkanie roślinożercy lub wszystkożercy z rośliną.
    else if ((type1 == 1 && type2 == 0) || (type1 == 3 && type2 == 0)) {
        organism1.add_vitality(vitality2);
        organism2.become_dead();
        return {organism1, organism2, {}};
    }
    else if ((type1 == 0 && type2 == 1) || (type1 == 0 && type2 == 3)) {
        organism2.add_vitality(vitality1);
        organism1.become_dead();
        return {organism1, organism2, {}};
    }

    // 8. Zdolność do konsumpcji zachodzi tylko w jedną stronę.
    else if (((type1 == 1 && type2 == 0) || (type1 == 2 && type2 == 1)
             || (type1 == 3 && type2 == 0) || (type1 == 3 && type2 == 1))
             && vitality1 > vitality2) {
        organism1.add_vitality(vitality2 / 2);
        organism2.become_dead();
    }
    else if (((type2 == 1 && type1 == 0) || (type2 == 2 && type1 == 1)
             || (type2 == 3 && type1 == 0) || (type2 == 3 && type1 == 1))
             && vitality2 > vitality1) {
        organism2.add_vitality(vitality1 / 2);
        organism1.become_dead();
    }

    return {organism1, organism2, {}};
}

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p,
        bool sp2_eats_m, bool sp2_eats_p>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
operator + (Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
            Organism<species_t, sp2_eats_m, sp2_eats_p> organism2) {
    return get<0>(encounter(organism1, organism2));
}

template <typename species_t, bool sp1_eats_m, bool sp1_eats_p,
        typename ... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
encounter_series(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
                 Args ... args) {
    return (organism1 + ... + args);
}

#endif //ORGANISM_H
