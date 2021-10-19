#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include "maptel.h"
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

    string clone_string(const char *src) {
        string result;
        for (size_t k = 0; src[k]; k++) {
            result.push_back(src[k]);
        }
        return result;
    }

}

namespace jnp1 {
    unsigned long maptel_create(void) {
        unordered_map<string,string> new_maptel;
        maptel[current_maptel] = new_maptel;
        return current_maptel++;
    }

    void maptel_delete(unsigned long id) {
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
        string src = clone_string(tel_src), dst = clone_string(tel_dst);
        string current = src;
        unordered_map<string,string>::iterator x;
        while (maptel[id].end() != (x = maptel[id].find(current))) {
            current = x->second;
            if (current == src)
                break;
        }
        maptel[id][dst] = current;
    }
}
