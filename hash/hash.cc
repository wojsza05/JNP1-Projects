#include "hash.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


// Anonimowy namespace do funkcji lokalnych etc.
namespace {
#ifdef NDEBUG
    const bool debug = false;
#else
    const bool debug = true;
#endif

    struct Hash;

    using id_type = unsigned long;
    using sequence_element_type = uint64_t;
    using sequence = std::vector<sequence_element_type>;
    using hash_table = std::unordered_set<sequence, Hash>;
    using hash_tables = std::unordered_map<id_type, hash_table>;

    struct Hash {
        hash_function_t hash_function;

        uint64_t operator()(const sequence &s) const {
            return hash_function(s.data(), s.size());
        }
    };

    id_type get_next_id() {
        static id_type id = 0;
        return id++;
    }

    hash_tables &get_hash_tables() {
        static hash_tables tables;
        return tables;
    }

    // Wypisuje wiadomość na wyjście diagnostyczne.
    void log(const std::string &message) {
        static std::ios_base::Init init;
        if (debug)
            std::cerr << message << '\n';
    }

    // Wypisuje na wyjście diagnostyczne wiadomość postaci
    // "function_name(arguments)".
    void log_function(char const *function_name, const std::string &arguments) {
        log(std::string(function_name) + "(" + arguments + ")");
    }

    // Wypisuje na wyjście diagnostyczne wiadomość postaci
    // "function_name: hash_table #id status".
    void log_table_status(char const *function_name, unsigned long id,
                          const std::string &status) {
        log(std::string(function_name) + ": hash table #" + std::to_string(id) +
            " " + status);
    }

    void log_sequence_status(char const *function_name, unsigned long id,
                             const std::string &sequence,
                             const std::string &status) {
        log(std::string(function_name) + ": hash table #" + std::to_string(id) +
            ", sequence " + sequence + " " + status);
    }

    bool check_arguments(unsigned long id, uint64_t const *seq, size_t size,
                         hash_tables::iterator it, char const *function_name) {
        bool error_occured = false;
        if (seq == NULL) {
            log(std::string(function_name) + ": invalid pointer (NULL)");
            error_occured = true;
        }
        if (size == 0) {
            log(std::string(function_name) + ": invalid size (0)");
            error_occured = true;
        }
        if (it == get_hash_tables().end() && !error_occured) {
            log_table_status(function_name, id, "does not exist");
            error_occured = true;
        }

        return !error_occured;
    }

    // Konwertuje elementy ciągu do std::string
    std::string elements(uint64_t const *seq, size_t size) {
        std::string res = "\"";
        if (seq == NULL) {
            res = "NULL";
        } else {
            for (size_t i = 0; i < size; i++) {
                res += std::to_string(seq[i]);
                if (i < size - 1)
                    res += ' ';
            }
            res += '\"';
        }
        return res;
    }
}


// Interfejs z treści.
namespace jnp1 {
    unsigned long hash_create(hash_function_t hash_function) {
        assert(hash_function != NULL);
        std::ostringstream oss;
        oss << &hash_function;
        log_function(__func__, oss.str());

        hash_tables &tables = get_hash_tables();
        id_type id = get_next_id();

        tables[id] = hash_table(0, {hash_function});
        log_table_status(__func__, id, "created");

        return id;
    }


    void hash_delete(unsigned long id) {
        log_function(__func__, std::to_string(id));

        hash_tables &tables = get_hash_tables();
        auto it = tables.find(id);
        if (it == tables.end()) {
            log_table_status(__func__, id, "does not exist");
            return;
        }

        tables.erase(it);
        log_table_status(__func__, id, "deleted");
    }


    size_t hash_size(unsigned long id) {
        log_function(__func__, std::to_string(id));

        hash_tables &tables = get_hash_tables();
        auto it = tables.find(id);
        if (it == tables.end()) {
            log_table_status(__func__, id, "does not exist");
            return 0;
        }

        size_t result = (it->second).size();
        log_table_status(__func__, id,
                         "contains " + std::to_string(result) + " element(s)");
        return result;
    }


    bool hash_insert(unsigned long id, uint64_t const *seq, size_t size) {
        std::string contents = elements(seq, size);
        log_function(__func__, (std::to_string(id) + ", " + contents + ", " +
                                std::to_string(size)));

        hash_tables &tables = get_hash_tables();
        auto it = tables.find(id);
        if (!check_arguments(id, seq, size, it, __func__))
            return false;

        hash_table &table_id = it->second;
        sequence seq_copy = sequence(seq, seq + size);
        if (table_id.find(seq_copy) != table_id.end()) {
            log_sequence_status(__func__, id, contents, "was present");
            return false;
        }

        table_id.insert(seq_copy);

        log_sequence_status(__func__, id, contents, "inserted");
        return true;
    }


    bool hash_remove(unsigned long id, uint64_t const *seq, size_t size) {
        std::string contents = elements(seq, size);
        log_function(__func__, (std::to_string(id) + ", " + contents + ", " +
                                std::to_string(size)));

        hash_tables &tables = get_hash_tables();
        auto it = tables.find(id);
        if (!check_arguments(id, seq, size, it, __func__))
            return false;

        hash_table &table_id = it->second;
        sequence seq_copy = sequence(seq, seq + size);
        if (table_id.find(seq_copy) == table_id.end()) {
            log_sequence_status(__func__, id, contents, "was not present");
            return false;
        }

        table_id.erase(seq_copy);

        log_sequence_status(__func__, id, contents, "removed");
        return true;
    }


    void hash_clear(unsigned long id) {
        log_function(__func__, std::to_string(id));

        hash_tables &tables = get_hash_tables();
        auto it = tables.find(id);
        if (it == tables.end()) {
            log_table_status(__func__, id, "does not exist");
            return;
        }

        hash_table &table_id = it->second;

        if (!table_id.size()) {
            log_table_status(__func__, id, "was empty");
            return;
        }

        table_id.clear();
        log_table_status(__func__, id, "cleared");
    }


    bool hash_test(unsigned long id, uint64_t const *seq, size_t size) {
        std::string contents = elements(seq, size);
        log_function(__func__, (std::to_string(id) + ", " + contents + ", " +
                                std::to_string(size)));

        hash_tables &tables = get_hash_tables();
        auto it = tables.find(id);
        if (!check_arguments(id, seq, size, it, __func__))
            return false;

        hash_table &table_id = it->second;
        sequence seq_copy = sequence(seq, seq + size);

        auto it2 = table_id.find(seq_copy);

        if (it2 == table_id.end()) {
            log_sequence_status(__func__, id, contents, "is not present");
            return false;
        } else {
            log_sequence_status(__func__, id, contents, "is present");
            return true;
        }
    }
}