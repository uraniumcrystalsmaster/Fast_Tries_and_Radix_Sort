//
// Created by urani on 10/14/2025.
//

#ifndef ORDERED_HASH_MAP_H
#define ORDERED_HASH_MAP_H
#include <iostream>
#include <vector>
#include <limits>
#include <iterator>
#include "submodules/FunnelHashMap/src/Funnel_Hash_Map.h"

template <typename Key, typename Value>
class Ordered_Hash_Map{
  public:
    struct NodeProps {
      Key next = NULL_KEY;
      Key prev = NULL_KEY;
      Value value = Value{};
    };
  private:
	static constexpr Key NULL_KEY = std::numeric_limits<Key>::max();
    size_t node_count;
    Funnel_Hash_Map<Key, NodeProps> umap;
    Key head = NULL_KEY;
    Key tail = NULL_KEY;
  public:
	class iterator {
    public:
        // Iterator traits
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = Value;
        using pointer           = Value*;
        using reference         = Value&;

    private:
        Ordered_Hash_Map* map_ptr;
        Key curr_key;
        iterator(Ordered_Hash_Map* map, Key key) : map_ptr(map), curr_key(key) {}
        friend class Ordered_Hash_Map;

    public:
        iterator() : map_ptr(nullptr), curr_key(NULL_KEY) {}

        reference operator*() const {
            return map_ptr->umap.find(curr_key)->second.value;
        }

        pointer operator->() const {
            return &(map_ptr->umap.find(curr_key)->second.value);
        }

        const Key& key() const {
            return curr_key;
        }

        Value& value() const {
            return map_ptr->umap.find(curr_key)->second.value;
        }

        // Pre-increment (++it)
        iterator& operator++() {
            curr_key = map_ptr->umap.find(curr_key)->second.next;
            return *this;
        }

        // Post-increment (it++)
        iterator operator++(int) {
            iterator old = *this; // Make a copy
            ++(*this);           // Increment self
            return old;          // Return the old copy
        }

        // Pre-decrement (--it)
        iterator& operator--() {
            if (curr_key == NULL_KEY) {
                // If at end(), decrementing should go to the tail
                curr_key = map_ptr->tail;
            } else {
                curr_key = map_ptr->umap.find(curr_key)->second.prev;
            }
            return *this;
        }

        // Post-decrement (it--)
        iterator operator--(int) {
            iterator old = *this; // Make a copy
            --(*this);           // Decrement self
            return old;          // Return the old copy
        }

        bool operator==(const iterator& other) const {
            return map_ptr == other.map_ptr && curr_key == other.curr_key;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };
    explicit Ordered_Hash_Map(size_t N) : umap(N) {
      this->node_count = 0;
      this->head = NULL_KEY;
      this->tail = NULL_KEY;
    }

    Ordered_Hash_Map(const Ordered_Hash_Map& other_ordered_hash_map) : umap(other_ordered_hash_map.umap){
      this->node_count = 0;
      Key nav_node = other_ordered_hash_map.getHead();
      for(size_t i = 0; i<other_ordered_hash_map.nodeCount(); i++){
        this->addTail(nav_node,other_ordered_hash_map.find(nav_node)->second.value);
        nav_node = other_ordered_hash_map.find(nav_node)->second.next;
      }
    }

    Ordered_Hash_Map& operator=(const Ordered_Hash_Map& other_ordered_hash_map){
      if(this != &other_ordered_hash_map){
        clear(); //convert linked list to default
        Key nav_node = other_ordered_hash_map.getHead();
        for(size_t i = 0; i<other_ordered_hash_map.nodeCount(); i++){
          this->addTail(nav_node, other_ordered_hash_map.find(nav_node)->second.value);
          nav_node = other_ordered_hash_map.find(nav_node)->second.next;
        }
      }
      return *this;
    }

    ~Ordered_Hash_Map(){
      clear();
    }

    //Behaviors
    void printForward(){
      Key nav_node = this->head;
      for(size_t i = 0; i<this->node_count; i++){
        std::cout << umap.find(nav_node)->second.value << std::endl;
        nav_node = umap.find(nav_node)->second.next;
      }
    }

    void printReverse(){
      Key nav_node = this->tail;
      for(size_t i = 0; i<this->node_count; i++){
        std::cout << umap.find(nav_node)->second.value << std::endl;
        nav_node = umap.find(nav_node)->second.prev;
      }
    }

    //Accessors
    size_t nodeCount(){
      return this->node_count;
    }

    size_t nodeCount() const{
      return this->node_count;
    }

	iterator begin() {
        return iterator(this, this->head);
    }

    iterator end() {
        // end() is one-past-the-last-element, represented by NULL_KEY
        return iterator(this, NULL_KEY);
    }
  /*

    const_iterator begin() const {
        return const_iterator(this, this->head);
    }

    const_iterator end() const {
        return const_iterator(this, NULL_KEY);
    }

    const_iterator cbegin() const {
        return const_iterator(this, this->head);
    }

    const_iterator cend() const {
        return const_iterator(this, NULL_KEY);
    }
    */

    iterator find(const Key& key) {
        if (umap.contains(key)) {
            return iterator(this, key);
        }
        return end();
    }

  /*

    const_iterator find(const Key& key) const {
        if (umap.contains(key)) {
            return const_iterator(this, key);
        }
        return cend();
    }

    */

    bool empty(){
      if(node_count == 0) {
        return true;
      }
      return false;
    }

    bool contains(const Key& key) {
        return umap.find(key) != umap.end();
    }

    std::vector<Key> findValues(Value& value) {
      std::vector<Key> keys;
      Key nav_node = this->head;
      for(size_t i = 0; i<this->node_count; i++){
        if(umap.find(nav_node)->second.value == value){
          keys.push_back(nav_node);
        }
        nav_node = umap.find(nav_node)->second.next;
      }
      return keys;
    }

    std::vector<Key> findValues(const Value& value) const{
      std::vector<Key> keys;
      Key nav_node = this->head;
      for(size_t i = 0; i<this->node_count; i++){
        if(umap.find(nav_node)->second.value == value){
          keys.push_back(nav_node);
        }
        nav_node = umap.find(nav_node)->second.next;
      }
      return keys;
    }

    Key getNode(int index){
      if(index >= node_count){
        throw std::out_of_range("No node at index!");
      }
      Key nav_node = this->head;
      for(int i = 0; i<index; i++){
        nav_node = umap.find(nav_node)->second.next;
      }
      return nav_node;
    }

    Key getNode(const int index) const{
      if(index >= node_count){
        throw std::out_of_range("No node at index!");
      }
      Key nav_node = this->head;
      for(int i = 0; i<index; i++){
        nav_node = umap.find(nav_node)->second.next;
      }
      return nav_node;
    }

    Key getHead(){
      return this->head;
    }

    Key getHead() const{
      return this->head;
    }

    Key getTail(){
      return this->tail;
    }

    Key getTail() const{
      return this->tail;
    }

    //Insertions
    void addHead(const Key& key, const Value& value){
      if (key == NULL_KEY) {
        throw std::invalid_argument("Key value is reserved and cannot be inserted.");
      }
      if (umap.find(key) != umap.end()) {
        throw std::invalid_argument("Key already exists in the map.");
      }
      umap.emplace(key, NodeProps{NULL_KEY, NULL_KEY, value});
      Key new_head = key;
      if(node_count == 0){
        this->head = new_head;
        this->tail = new_head;
      }
      else if(node_count == 1){
        umap.find(new_head)->second.next = this->tail;
        umap.find(this->tail)->second.prev = new_head;
      }
      else{
        umap.find(this->head)->second.prev = new_head;
        umap.find(new_head)->second.next = this->head;
      }
      this->head = new_head;
      node_count++;
    }

    void addTail(const Key& key, const Value& value){
      if (key == NULL_KEY) {
        throw std::invalid_argument("Key value is reserved and cannot be inserted.");
      }
      if (umap.find(key) != umap.end()) {
        throw std::invalid_argument("Key already exists in the map.");
      }
      umap.emplace(key, NodeProps{NULL_KEY, NULL_KEY, value});

      Key new_tail = key;
      if(node_count == 0){
        this->head = new_tail;
        this->tail = new_tail;
      }
      else if(node_count == 1){
        umap.find(new_tail)->second.prev = this->head;
        umap.find(this->head)->second.next = new_tail;
      }
      else{
        umap.find(this->tail)->second.next = new_tail;
        umap.find(new_tail)->second.prev = this->tail;
      }
      this->tail = new_tail;
      node_count++;
    }

    void insertBefore(const Key& key, const Value& value, const Key& some_node){
      /*
      Let node2insert = A
      Let some_node = B
      Let >,< be next, prev respectively

      A<  = B<
      A>  = B
      B<> = A
      B<  = A
      */

      if (key == NULL_KEY) {
        throw std::invalid_argument("Key value is reserved and cannot be inserted.");
      }
      if (umap.find(key) != umap.end()) {
        throw std::invalid_argument("Key already exists in the map.");
      }
      if (umap.find(some_node) == umap.end()) {
        throw std::out_of_range("Node to insert after does not exist.");
      }

      if(some_node == this->head){
        addHead(key,value);
        return;
      }
      // Create node and update output links
      umap.emplace(key, NodeProps{some_node, umap.find(some_node)->second.prev, value});

      Key node2insert = key;

      // Update input links
      umap.find(umap.find(some_node)->second.prev)->second.next = node2insert;
      umap.find(some_node)->second.prev = node2insert;

      node_count++;
    }

    void insertAfter(const Key& key, const Value& value, const Key& some_node){
      /*
      same as insert after but swap next and prev
      Let node2insert = A
      Let some_node = B
      Let >,< be next, prev respectively

      A>  = B>
      A<  = B
      B>< = A
      B>  = A
      */

      if (key == NULL_KEY) {
        throw std::invalid_argument("Key value is reserved and cannot be inserted.");
      }
      if (umap.find(key) != umap.end()) {
        throw std::invalid_argument("Key already exists in the map.");
      }
      if (umap.find(some_node) == umap.end()) {
        throw std::out_of_range("Node to insert before does not exist.");
      }

      if(some_node == this->tail){
        addTail(key,value);
        return;
      }
      // Create node and update output links
      umap.emplace(key, NodeProps{umap.find(some_node)->second.next, some_node, value});

      Key node2insert = key;

      // Update input links
      umap.find(umap.find(some_node)->second.next)->second.prev = node2insert;
      umap.find(some_node)->second.next = node2insert;

      node_count++;
    }

    void insertAt(const Key& key, const Value& value, const int index){
      if (key == NULL_KEY) {
        throw std::invalid_argument("Key value is reserved and cannot be inserted.");
      }
      if (umap.find(key) != umap.end()) {
        throw std::invalid_argument("Key already exists in the map.");
      }

      if(index > node_count){
        throw std::out_of_range("No node at index!");
      }
      else if(index == 0){
        addHead(key,value);
        return;
      }
      else if(index == node_count){
        addTail(key,value);
        return;
      }

      Key some_node = this->head;
      for(int i = 0; i<index; i++){
        some_node = umap.find(some_node)->second.next;
      }
      insertBefore(key, value, some_node);
    }

    //Removals
    bool removeHead(){
      if(node_count == 0){
        return false;
      }
      else if(node_count == 1){
        umap.erase(this->head);
        this->head = NULL_KEY;
      }
      else{
        Key temp = umap.find(this->head)->second.next;
        umap.erase(this->head);
        this->head = temp;
      }
      this->node_count--;
      return true;
    }

    bool removeTail(){
      if(node_count == 0){
        return false;
      }
      else if(node_count == 1){
        umap.erase(this->tail);
        this->tail = NULL_KEY;
      }
      else{
        Key temp = umap.find(this->tail)->second.prev;
        umap.erase(this->tail);
        this->tail = temp;
      }
      this->node_count--;
      return true;
    }

    bool removeAt(int index){
      if(index == 0){
        removeHead();
        return true;
      }
      else if(index == node_count-1){
        removeTail();
        return true;
      }
      else if(index >= node_count){
        //throw out_of_range("No node at index!");
        return false;
      }
      Key nav_node = this->head;
      for(int i = 0; i<index; i++){
        nav_node = umap.find(nav_node)->second.next;
      }
      Key temp_next = umap.find(nav_node)->second.next;
      Key temp_prev = umap.find(nav_node)->second.prev;
      umap.erase(nav_node);
      umap.find(temp_prev)->second.next = temp_next;
      umap.find(temp_next)->second.prev = temp_prev;

      node_count--;
      return true;
    }

    bool remove(const Key& key){
      if(umap.find(key) == umap.end()){
        return false;
      }
      if(umap.find(key)->second.prev == NULL_KEY){
        removeHead();
      }
      else if(umap.find(key)->second.next == NULL_KEY){
        removeTail();
      }
      else{
        Key temp_next = umap.find(key)->second.next;
        Key temp_prev = umap.find(key)->second.prev;
        umap.erase(key);
        umap.find(temp_prev)->second.next = temp_next;
        umap.find(temp_next)->second.prev = temp_prev;

        node_count--;
      }
      return true;
    }

    int removeNodesWithValue(const Value& value){
      int removal_count = 0;
      Key nav_node = this->head;
      while(nav_node != NULL_KEY){
        Key backup = umap.find(nav_node)->second.next;
        if(umap.find(nav_node)->second.value == value){
          if(umap.find(nav_node)->second.prev == NULL_KEY){
            removeHead();
            nav_node = umap.find(nav_node)->second.next;
          }
          else if(umap.find(nav_node)->second.next == NULL_KEY){
            removeTail();
            removal_count++;
            break;
          }
          else{
            Key temp_next = umap.find(nav_node)->second.next;
            Key temp_prev = umap.find(nav_node)->second.prev;
            umap.erase(nav_node);
            umap.find(temp_prev)->second.next = temp_next;
            umap.find(temp_next)->second.prev = temp_prev;

            node_count--;
          }
          removal_count++;
        }
        nav_node = backup;
      }
      return removal_count;
    }

    void clear(){
      while(this->head != NULL_KEY){
        Key temp = umap.find(this->head)->second.next;
        umap.erase(this->head);
        this->head = temp;
      }
      this->head = NULL_KEY;
      this->tail = NULL_KEY;
      this->node_count = 0;
    }

    //Operators
    Value operator[](const int index){
      if(index >= node_count){
        throw std::out_of_range("No node at index!");
      }
      Key nav_node = this->head;
      for(int i = 0; i<index; i++){
        nav_node = umap.find(nav_node)->second.next;
      }
      return umap.find(nav_node)->second.value;
    }

    bool operator==(Ordered_Hash_Map& other_ordered_hash_map){
      if(this->node_count != other_ordered_hash_map.nodeCount()){
        return false;
      }
      Key nav_node = this->head;
      Key other_nav_node = other_ordered_hash_map.getHead();
      for(size_t i = 0; i<node_count; i++){
        if(umap.find(nav_node)->second.value != umap.find(other_nav_node)->second.value){
          return false;
        }
        nav_node = umap.find(nav_node)->second.next;
        other_nav_node = umap.find(other_nav_node)->second.next;
      }
      return true;
    }

    bool operator!=(Ordered_Hash_Map& other_ordered_hash_map){
      if(!(this==other_ordered_hash_map)){
        return true;
      }
      return false;
    }

};
#endif //ORDERED_HASH_MAP_H
