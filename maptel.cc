#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <cstring>
#include <cassert>
#include "maptel.h"

using std::vector;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::cerr;
using std::endl;
using jnp1::TEL_NUM_MAX_LEN;

using maptel_t = unordered_map<unsigned long, unordered_map<string, string>>;

namespace {
#ifdef NDEBUG
    const bool debug = false;
#else
    const bool debug = true;
#endif

    maptel_t &maptel() {
        static maptel_t maptel;
        return maptel;
    }

    unsigned long &current_maptel() {
        static unsigned long current_maptel = 0;
        return current_maptel;
    }

    string prefix(string fname) {
        return "maptel: " + fname;
    }

    string clone_string(const char *src) {
        string result;
        for (size_t k = 0; src[k]; k++) {
            result.push_back(src[k]);
        }

        return result;
    }

    bool is_telephone_number(char const *tel_num) {
        if (tel_num == nullptr)
            return false;

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
            cerr << prefix(__FUNCTION__) << "()" << endl;
        }

        unordered_map<string, string> new_maptel;
        maptel()[current_maptel()++] = new_maptel;

        if (debug) {
            cerr << prefix(__FUNCTION__) << ": new map id = "
                 << current_maptel() - 1 << endl;
        }

        return current_maptel() - 1;
    }

    void maptel_delete(unsigned long id) {
        if (debug) {
            assert(maptel().find(id) != maptel().end());

            cerr << prefix(__FUNCTION__) << "(" << id << ")" << endl;
        }

        maptel().erase(id);

        if (debug) {
            cerr << prefix(__FUNCTION__) << ": map " << id << " deleted" << endl;
        }
    }


    void maptel_insert(unsigned long id,
                       char const *tel_src,
                       char const *tel_dst) {
        if (debug) {
            assert(maptel().find(id) != maptel().end());
            assert(is_telephone_number(tel_src) &&
                   is_telephone_number(tel_dst));

            cerr << prefix(__FUNCTION__) << "(" << id << ", " << tel_src
                 << ", " << tel_dst << ")" << endl;
        }

        maptel()[id][clone_string(tel_src)] = clone_string(tel_dst);
        if (debug) {
            cerr << prefix(__FUNCTION__) << ": inserted" << endl;
        }
    }


    void maptel_erase(unsigned long id, char const *tel_src) {
        if (debug) {
            assert(maptel().find(id) != maptel().end());
            assert(is_telephone_number(tel_src));

            cerr << prefix(__FUNCTION__) << "(" << id << ", " << tel_src << ")"
                 << endl;
        }

        if (maptel()[id].find(tel_src) == maptel()[id].end()) {
            if (debug)
                cerr << prefix(__FUNCTION__) << ": nothing to erase" << endl;
            return;
        }

        maptel()[id].erase(clone_string(tel_src));

        if (debug) {
            cerr << prefix(__FUNCTION__) << ": erased" << endl;
        }
    }

    void maptel_transform(unsigned long id,
                          char const *tel_src,
                          char *tel_dst, size_t len) {
        if (debug) {
            assert(maptel().find(id) != maptel().end());
            assert(is_telephone_number(tel_src));
            assert(tel_dst != nullptr);

            cerr << prefix(__FUNCTION__) << "(" << id << ", " << tel_src
                 << ", " << static_cast<void *>(tel_dst) << ", "
                 << len << ")" << endl;
        }

        string src = clone_string(tel_src);
        string current = src;
        unordered_set<string> visited;
        unordered_map<string, string>::iterator x;

        while (maptel()[id].end() != (x = maptel()[id].find(current))) {
            if (visited.find(current) != visited.end()) {
                current = src;
                if (debug) {
                    cerr << prefix(__FUNCTION__)
                         << ": cycle detected" << endl;
                }
                break;
            }

            visited.insert(current);
            current = x->second;
        }

        if (debug) {
            assert(len > current.size());

            cerr << prefix(__FUNCTION__) << ": " << src << " -> "
                 << current << endl;
        }

        strncpy(tel_dst, (char *) current.c_str(), current.size() + 1);
    }
}
