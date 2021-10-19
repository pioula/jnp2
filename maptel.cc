#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include "maptel.h"
#include <bits/stdc++.h>
using std::vector;
using std::string;
using std::unordered_map;
using namespace std;

namespace {
    #ifdef NDEBUG
        const bool debug = false;
    #else
        const bool debug = true;
    #endif

    unordered_map<unsigned long, unordered_map<string, string>> maptel;
    unsigned long current_maptel = 0;

    const string prefix = "maptel: ";

    string clone_string(const char *src) {
        string result = "";
        for (size_t k = 0; src[k]; k++) {
            result.push_back(src[k]);
        }

        return result;
    }

    bool is_telephone_number(char const *tel_num) {
        size_t tel_length = strlen(tel_num);

        if (tel_length == 0 || tel_length > TEL_NUM_MAX_LEN)
            return false;

        for (size_t i = 0; tel_num[i]; i++) {
            if (!isdigit(tel_num[i]))
                return false;
        }

        return true;
    }
}

namespace jnp1 {
    unsigned long maptel_create(void) {
        if (debug) {
            cerr << prefix << "maptel_create()\n";
        }

        unordered_map<string,string> new_maptel;
        maptel[current_maptel++] = new_maptel;

        return current_maptel - 1;
    }

    void maptel_delete(unsigned long id) {
        if (debug) {
            assert(maptel.contains(id));

            cerr << prefix << "maptel_delete(" << id << ")\n";
        }

        maptel.erase(id);
    }


    void maptel_insert(unsigned long id,
                       char const *tel_src,
                       char const *tel_dst) {
        maptel[id][clone_string(tel_src)] = clone_string(tel_dst);
    }


    void maptel_erase(unsigned long id, char const *tel_src) {
        maptel[id].erase(clone_string(tel_src));
    }

    void maptel_transform(unsigned long id,
                          char const *tel_src,
                          char *tel_dst, size_t len) {

    }
}
