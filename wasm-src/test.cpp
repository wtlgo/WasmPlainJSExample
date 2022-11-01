#include <string>
#include <random>
#include <algorithm>
#include <set>
#include <cmath>

#include <iostream>

#include <emscripten.h>
#include <emscripten/bind.h>

using fmap = std::map<int, unsigned>;

fmap merge(const fmap& l, const fmap& r) {
    fmap result;

    std::set<int> keys;

    std::transform(l.cbegin(), l.cend(), std::inserter(keys, keys.end()),
                   [](const fmap::value_type& val) { return val.first; });

    std::transform(r.cbegin(), r.cend(), std::inserter(keys, keys.end()),
                   [](const fmap::value_type& val) { return val.first; });

    for (const int key : keys) {
        const fmap::const_iterator lpval = l.find(key);
        const int lval = lpval == l.cend() ? 0 : lpval->second;

        const fmap::const_iterator rpval = r.find(key);
        const int rval = rpval == r.cend() ? 0 : rpval->second;

        result[key] = lval + rval;
    }

    return result;
}

fmap factorize_mem(const int n);

fmap factorize(const int n) {
    if (n < 0) return merge({{-1, 1}}, factorize_mem(-n));
    if (n == 0) throw std::range_error("0 has no factorization!");
    if (n == 1) return {};
    if (n % 2 == 0) return merge({{2, 1}}, factorize_mem(n / 2));

    const int mid = std::ceil(std::sqrt(n));

    for (int i = 3; i <= mid; i += 2) {
        if (n % i != 0) continue;

        return merge(factorize_mem(n / i), factorize_mem(i));
    }

    return {{n, 1}};
}

fmap factorize_mem(const int n) {
    static std::map<int, fmap> memory;

    if (memory.find(n) == memory.cend()) {
        memory[n] = factorize(n);
    }

    return memory.at(n);
}

EMSCRIPTEN_BINDINGS(my_module) {
    namespace em = emscripten;

    em::function("factorize", &factorize_mem);

    em::register_vector<int>("std::vector<int>");
    em::register_map<int, unsigned>("std::map<int, unsigned>");
}
