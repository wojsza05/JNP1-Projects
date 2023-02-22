#ifndef KVFIFO_H
#define KVFIFO_H

#include <list>
#include <map>
#include <set>
#include <memory>
#include <iostream>

template<typename K, typename V>
class kvfifo {
private:
    using list_it_t = typename std::list<std::pair<K, V>>::iterator;
    std::shared_ptr<std::list<std::pair<K, V>>> kvlist;
    std::shared_ptr<std::map<K, std::list<list_it_t>>> kvmap;
    std::shared_ptr<std::set<K>> kvkeys;
    bool has_references = false;

    kvfifo copy_kvfifo() {
        try {
            kvfifo copied_kvfifo;
            if (kvmap.use_count() > 1) {
                for (auto kv: *kvlist)
                    copied_kvfifo.push(kv.first, kv.second);
            } else {
                copied_kvfifo = *this;
            }
            copied_kvfifo.has_references = false;
            return copied_kvfifo;
        } catch (...) {
            throw;
        }
    }

    void replace_kvfifo(kvfifo &other) {
        std::swap(kvlist, other.kvlist);
        std::swap(kvmap, other.kvmap);
        std::swap(kvkeys, other.kvkeys);
        has_references = other.has_references;
    }

public:
    using k_iterator = typename std::set<K>::const_iterator;

    k_iterator k_begin() const {
        return kvkeys.get()->begin();
    }

    k_iterator k_end() const {
        return kvkeys.get()->end();
    }

    kvfifo() : kvmap(std::make_shared<std::map<K, std::list<list_it_t>>>()),
               kvlist(std::make_shared<std::list<std::pair<K, V>>>()),
               kvkeys(std::make_shared<std::set<K>>()),
               has_references(false) {};

    kvfifo(kvfifo const &other) {
        kvmap = other.kvmap;
        kvlist = other.kvlist;
        kvkeys = other.kvkeys;
        has_references = false;
        if (other.has_references) {
            kvfifo copied_kvfifo = copy_kvfifo();
            replace_kvfifo(copied_kvfifo);
        }
    }

    kvfifo(kvfifo &&other) noexcept: kvlist(std::move(other.kvlist)),
                                     kvmap(std::move(other.kvmap)),
                                     kvkeys(std::move(other.kvkeys)),
                                     has_references(std::move(other.has_references)) {}

    kvfifo &operator=(kvfifo other) {
        std::swap(kvlist, other.kvlist);
        std::swap(kvmap, other.kvmap);
        std::swap(kvkeys, other.kvkeys);
        if (other.has_references) {
            kvfifo copied_kvfifo = copy_kvfifo();
            replace_kvfifo(copied_kvfifo);
            has_references = true;
        }
        return *this;
    }

    // O(log n)
    void push(K const &k, V const &v) {
        kvfifo copied_kvfifo = copy_kvfifo();

        try {
            copied_kvfifo.kvlist.get()->push_back({k, v});
            auto element_it = --copied_kvfifo.kvlist.get()->end();

            auto it = copied_kvfifo.kvmap.get()->find(k);
            if (it == copied_kvfifo.kvmap.get()->end()) {
                copied_kvfifo.kvmap.get()->insert({k, std::list<list_it_t>()});
                it = copied_kvfifo.kvmap.get()->find(k);
                copied_kvfifo.kvkeys->insert(k);
            }
            (it->second).push_back(element_it);
            replace_kvfifo(copied_kvfifo);
        } catch (...) {
            throw;
        }
    }

    void pop() {
        kvfifo copied_kvfifo = copy_kvfifo();

        if (copied_kvfifo.kvlist.get()->empty()) {
            throw std::invalid_argument("kvfifo is empty");
        }

        try {
            auto it = copied_kvfifo.kvmap.get()->find(copied_kvfifo.kvlist.get()->front().first);
            if (it->second.size() == 1) {
                copied_kvfifo.kvmap.get()->erase(it);
                copied_kvfifo.kvkeys->erase(copied_kvfifo.kvlist.get()->front().first);
            } else {
                it->second.pop_front();
            }
            copied_kvfifo.kvlist.get()->pop_front();
            replace_kvfifo(copied_kvfifo);
        } catch (...) {
            throw;
        }
    }

    void pop(K const &k) {
        kvfifo copied_kvfifo = copy_kvfifo();

        if (copied_kvfifo.kvlist.get()->empty()) {
            throw std::invalid_argument("kvfifo is empty");
        }

        auto it = copied_kvfifo.kvmap.get()->find(k);
        if (it == copied_kvfifo.kvmap.get()->end()) {
            throw std::invalid_argument("key not found");
        }

        try {
            copied_kvfifo.kvlist.get()->erase(it->second.front());
            if (it->second.size() == 1) {
                copied_kvfifo.kvmap.get()->erase(it);
                copied_kvfifo.kvkeys->erase(k);
            } else {
                it->second.pop_front();
            }
            replace_kvfifo(copied_kvfifo);
        } catch (...) {
            throw;
        }
    }

    // O(m + log n)
    void move_to_back(K const &k) {
        kvfifo copied_kvfifo = copy_kvfifo();

        if (copied_kvfifo.kvlist.get()->empty()) {
            throw std::invalid_argument("kvfifo is empty");
        }
        auto it = copied_kvfifo.kvmap.get()->find(k);
        if (it == copied_kvfifo.kvmap.get()->end()) {
            throw std::invalid_argument("key not found");
        }

        try {
            for (auto list_it = it->second.begin(); list_it != it->second.end(); list_it++) {
                auto tmp = *list_it;
                std::pair<K, V> list_element = *tmp;
                copied_kvfifo.kvlist.get()->erase(tmp);
                copied_kvfifo.kvlist.get()->push_back(list_element);
            }
            replace_kvfifo(copied_kvfifo);
        } catch (...) {
            throw;
        }
    }

    // O(1)
    std::pair<K const &, V &> front() {
        kvfifo copied_kvfifo = copy_kvfifo();

        if (copied_kvfifo.kvlist.get()->empty())
            throw std::invalid_argument("kvfifo is empty");

        replace_kvfifo(copied_kvfifo);
        has_references = true;
        return {kvlist.get()->front().first, kvlist.get()->front().second};
    }

    std::pair<K const &, V const &> front() const {
        if (kvlist.get()->empty())
            throw std::invalid_argument("kvfifo is empty");

        return {kvlist.get()->front().first, kvlist.get()->front().second};
    }

    std::pair<K const &, V &> back() {
        kvfifo copied_kvfifo = copy_kvfifo();

        if (copied_kvfifo.kvlist.get()->empty())
            throw std::invalid_argument("kvfifo is empty");

        replace_kvfifo(copied_kvfifo);
        has_references = true;
        return {kvlist.get()->back().first, kvlist.get()->back().second};
    }

    std::pair<K const &, V const &> back() const {
        if (kvlist.get()->empty())
            throw std::invalid_argument("kvfifo is empty");

        return {kvlist.get()->back().first, kvlist.get()->back().second};
    }

    // O(log n)
    std::pair<K const &, V &> first(K const &key) {
        kvfifo copied_kvfifo = copy_kvfifo();

        if (copied_kvfifo.kvlist.get()->empty())
            throw std::invalid_argument("kvfifo is empty");
        auto it = copied_kvfifo.kvmap.get()->find(key);
        if (it == copied_kvfifo.kvmap.get()->end())
            throw std::invalid_argument("key not found");

        replace_kvfifo(copied_kvfifo);
        has_references = true;
        return {it->first, it->second.front()->second};
    }

    std::pair<K const &, V const &> first(K const &key) const {
        if (kvlist.get()->empty())
            throw std::invalid_argument("kvfifo is empty");
        auto it = kvmap.get()->find(key);
        if (it == kvmap.get()->end())
            throw std::invalid_argument("key not found");

        return {it->first, it->second.front()->second};
    }

    std::pair<K const &, V &> last(K const &key) {
        kvfifo copied_kvfifo = copy_kvfifo();

        if (copied_kvfifo.kvlist.get()->empty())
            throw std::invalid_argument("kvfifo is empty");
        auto it = copied_kvfifo.kvmap.get()->find(key);
        if (it == copied_kvfifo.kvmap.get()->end())
            throw std::invalid_argument("key not found");

        replace_kvfifo(copied_kvfifo);
        has_references = true;
        return {it->first, it->second.back()->second};
    }

    std::pair<K const &, V const &> last(K const &key) const {
        if (kvlist.get()->empty())
            throw std::invalid_argument("kvfifo is empty");
        auto it = kvmap.get()->find(key);
        if (it == kvmap.get()->end())
            throw std::invalid_argument("key not found");

        return {it->first, it->second.back()->second};
    }

    // O(1)
    [[nodiscard]] size_t size() const {
        return kvlist.get()->size();
    }

    // O(1)
    [[nodiscard]] bool empty() const {
        return kvlist.get()->empty();
    }

    // O(log n)
    size_t count(K const &k) const {
        auto it = kvmap.get()->find(k);
        if (it == kvmap.get()->end())
            return 0;
        return it->second.size();
    }

    // O(n)
    void clear() {
        try {
            kvfifo copied_kvfifo = copy_kvfifo();
            copied_kvfifo.kvlist.get()->clear();
            copied_kvfifo.kvmap.get()->clear();
            copied_kvfifo.kvkeys->clear();
            replace_kvfifo(copied_kvfifo);
        } catch (...) {
            throw;
        }
    }
};

#endif // KVFIFO_H
