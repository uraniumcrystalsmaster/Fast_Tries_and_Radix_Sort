#ifndef X_FAST_TRIE_H
#define X_FAST_TRIE_H
#include <limits>
#include <vector>
#include <cmath>
#include "Funnel_Hash_Map.h"
#include "Ordered_Hash_Map.h"

template<typename Key, typename Value>
class XFastTrie{
    public:
        static constexpr Key NULL_KEY = std::numeric_limits<Key>::max();
        struct NodeProps{
            Key left = NULL_KEY;
            Key right = NULL_KEY;
            Key descendant = NULL_KEY;
        };
    Ordered_Hash_Map<size_t,NodeProps> lowest_level;
    std::vector<Funnel_Hash_Map<size_t,NodeProps>> upper_levels;
    size_t bit_count;
    public:
        XFastTrie(size_t N){
            this->bit_count = std::numeric_limits<size_t>::digits
            + (std::is_signed<size_t>::value ? 1 : 0); //add one for signed values
            upper_levels.reserve(bit_count-1);
            for (size_t i = 0; i < bit_count - 1; i++) {
                size_t one = 1;
                upper_levels.emplace_back(std::min(N, (one << (i+one))));
            }
        }

        void insert(const Key& key, Value& value){
            NodeProps node_props;
            node_props.value = value;
            if(lowest_level.empty()){
                lowest_level.addHead(key,value);
                size_t curr_bit = key;
                Key follower_key = key;
                for(size_t i = 2; i < bit_count; i++){
                    upper_levels[bit_count-i].emplace(curr_bit, node_props);
                    upper_levels[bit_count-i].left = follower_key;
                    upper_levels[bit_count-i].right = follower_key;
                    follower_key = curr_bit;
                    curr_bit = curr_bit >> 1;
                }
                return;
            }

        }

        size_t successor(const Key& key){
            auto lcpl = findLongestCommonPrefixLevel(key);
            lcpl.descendant;
        }

        auto findLongestCommonPrefixLevel(const Key& key){
            size_t lowest_level_i = bit_count - 1;
            size_t target_prefix_level_i = 0;
            size_t low = 0;
            size_t high = lowest_level_i;
            while(low <= high){
                size_t midpoint = low + (high - low) / 2;
                size_t curr_prefix = key >> (bit_count - (midpoint + 1));
                bool has_found_level;
                if(midpoint == lowest_level_i){
                    has_found_level = lowest_level.contains(curr_prefix);
                }
                else{
                    has_found_level = upper_levels[midpoint].contains(curr_prefix);
                }
                if(has_found_level){
                    low = midpoint + 1;
                    target_prefix_level_i = midpoint;
                }
                else {
                    if(high == 0){ break; } //prevent underflow
                    high = midpoint - 1;
                }
            }
            if(target_prefix_level_i == lowest_level_i){
                return lowest_level[target_prefix_level_i];
            }
            return upper_levels[target_prefix_level_i];
        }

};
#endif //X_FAST_TRIE_H
