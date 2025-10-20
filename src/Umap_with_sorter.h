#ifndef UMAP_WITH_SORTER_H
#define UMAP_WITH_SORTER_H
#include <vector>
#include "Ordered_Hash_Map.h"
#include "Radix_Sort.h"
template<typename Key, typename Value>
class Umap_with_sorter : public Ordered_Hash_Map<Key, Value>{
    public:
        static constexpr Key NULL_KEY = std::numeric_limits<Key>::max();
        using Ordered_Hash_Map<Key, Value>::Ordered_Hash_Map;
        Umap_with_sorter() : Ordered_Hash_Map<Key, Value>() {}

        void sort_keys(){
            // Convert umap to vector
            std::vector<std::pair<const Key, Value>> sorted_pairs;
            auto current_key = this->getHead();
            while (current_key != NULL_KEY && sorted_pairs.size() < this->nodeCount()) {
                auto it = this->find(current_key);
                sorted_pairs.push_back({it->first, it->second.value});
                current_key = it->second.next;
            }
            // Sort
            radix_sort(); //Todo: add parameters

            // Convert vector to umap
            rebuild_sorted_links(sorted_pairs);
        }

        void sort_values(){
            // Convert umap to vector
            std::vector<std::pair<const Key, Value>> sorted_pairs;
            auto current_key = this->getHead();
            while (current_key != NULL_KEY && sorted_pairs.size() < this->nodeCount()) {
                auto it = this->find(current_key);
                sorted_pairs.push_back({it->first, it->second.value});
                current_key = it->second.next;
            }
            // Sort
            radix_sort(); //Todo: add parameters

            // Convert vector to umap
            rebuild_sorted_links(sorted_pairs);
        }

    void rebuild_sorted_links(const std::vector<std::pair<const Key, Value>>& sorted_pairs) {
            if (sorted_pairs.empty()) {
                this->head = NULL_KEY;
                this->tail = NULL_KEY;
                this->node_count = 0;
                return;
            }

            size_t N = sorted_pairs.size();
            this->head = sorted_pairs.front().first;
            this->tail = sorted_pairs.back().first;

            for (size_t i = 0; i < N; ++i) {
                Key current_key = sorted_pairs[i].first;

                auto it = this->find(current_key);

                if (i < N - 1) {
                    Key next_key = sorted_pairs[i + 1].first;
                    it->second.next = next_key;
                } else {
                    it->second.next = NULL_KEY;
                }

                if (i > 0) {
                    Key prev_key = sorted_pairs[i - 1].first;
                    it->second.prev = prev_key;
                } else {
                    it->second.prev = NULL_KEY;
                }
            }
        }
};
#endif //UMAP_WITH_SORTER_H
