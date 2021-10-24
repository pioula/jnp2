#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include "maptel.h"
#include <cstring>
#include <cassert>
using std::vector;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::cerr;
using jnp1::TEL_NUM_MAX_LEN;

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
        string result;
        for (size_t k = 0; src[k]; k++) {
            result.push_back(src[k]);
        }

        return result;
    }

    bool is_telephone_number(char const *tel_num) {
        if (tel_num == NULL)
	    return false;

	size_t tel_length = strlen(tel_num);

        if (tel_length > TEL_NUM_MAX_LEN)
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

        if (debug) {
            cerr << prefix << "maptel_create: new map id = "
                << current_maptel - 1 << "\n";
        }

        return current_maptel - 1;
    }

    void maptel_delete(unsigned long id) {
        if (debug) {
            assert(maptel.find(id) != maptel.end());

            cerr << prefix << "maptel_delete(" << id << ")\n";
        }

        maptel.erase(id);

        if (debug) {
            cerr << prefix << "maptel_delete: map " << id << " deleted\n";
        }
    }


    void maptel_insert(unsigned long id,
                       char const *tel_src,
                       char const *tel_dst) {
        if (debug) {
            assert(maptel.find(id) != maptel.end());
            assert(is_telephone_number(tel_src) &&
                    is_telephone_number(tel_dst));

            cerr << prefix << "maptel_insert(" << id << ", " << tel_src
                << ", " << tel_dst << ")\n";
        }

        maptel[id][clone_string(tel_src)] = clone_string(tel_dst);
        if (debug) {
            cerr << prefix << "maptel_insert: inserted\n";
        }
    }


    void maptel_erase(unsigned long id, char const *tel_src) {
        if (debug) {
            assert(maptel.find(id) != maptel.end());
            assert(is_telephone_number(tel_src));

            cerr << prefix << "maptel_erase(" << id << ", " << tel_src << ")\n";
        }
        
        if (maptel[id].find(tel_src) == maptel[id].end()) {
            if (debug)
                cerr << prefix << "maptel_erase: nothing to erase\n";
            return;
        }

        maptel[id].erase(clone_string(tel_src));

        if (debug) {
            cerr << prefix << "maptel_erase: erased\n";
        }
    }

    void maptel_transform(unsigned long id,
                          char const *tel_src,
                          char *tel_dst, size_t len) {
        if (debug) {
            assert(maptel.find(id) != maptel.end());
            assert(is_telephone_number(tel_src));
            assert(tel_dst != NULL);

            cerr << prefix << "maptel_transform(" << id << ", " << tel_src << ", " << static_cast<void*>(tel_dst) << ", " << len << ")" << std::endl;
        }
        string src = clone_string(tel_src);
        string current = src;
        unordered_set<string> visited;
        unordered_map<string,string>::iterator x;
        while (maptel[id].end() != (x = maptel[id].find(current))) {
            if (visited.find(current) != visited.end()) {
                current = src;
                if (debug) {
                    cerr << prefix << "maptel_transform: cycle detected" << std::endl;
                }
                break;
            }
            visited.insert(current);
            current = x->second;
        }
        if (debug) {
            assert(len > current.size());
            cerr << prefix << "maptel_transform: " << src << " -> " << current << std::endl;
        }
         strncpy(tel_dst, (char*)current.c_str(), current.size() + 1);
    }
}
