//
// Created by urani on 10/14/2025.
//

#ifndef ORDERED_HASH_MAP_H
#define ORDERED_HASH_MAP_H
#pragma once
#include <iostream>
#include <vector>
#include "Funnel_Hash_Map.h"

template <typename Key, typename Value>
class Ordered_Hash_Map{
  public:
    struct NodeProps {
      Key next = Key{};
      Key prev = Key{};
      Value value = Value{};
    };
  private:
    size_t node_count;
    Funnel_Hash_Map<Key, NodeProps> umap;
    Key head = Key{};
    Key tail = Key{};
  public:
    Ordered_Hash_Map(){
      this->node_count = 0;
      this->head = Key{};
      this->tail = Key{};
    }

    Ordered_Hash_Map(const Ordered_Hash_Map& other_ordered_hash_map){
      this->node_count = 0;
      Key nav_node = other_ordered_hash_map.GetHead();
      for(size_t i = 0; i<other_ordered_hash_map.NodeCount(); i++){
        this->AddTail(nav_node,other_ordered_hash_map.Find(nav_node)->second.value);
        nav_node = other_ordered_hash_map.Find(nav_node)->second.next;
      }
    }

    Ordered_Hash_Map& operator=(const Ordered_Hash_Map& other_ordered_hash_map){
      if(this != &other_ordered_hash_map){
        Clear(); //convert linked list to default
        Key nav_node = other_ordered_hash_map.GetHead();
        for(size_t i = 0; i<other_ordered_hash_map.NodeCount(); i++){
          this->AddTail(nav_node, other_ordered_hash_map.Find(nav_node)->second.value);
          nav_node = other_ordered_hash_map.Find(nav_node)->second.next;
        }
      }
      return *this;
    }

    ~Ordered_Hash_Map(){
      Clear();
    }

    //Behaviors
    void PrintForward(){
      Key nav_node = this->head;
      for(size_t i = 0; i<this->node_count; i++){
        std::cout << umap.find(nav_node)->second.value << std::endl;
        nav_node = umap.find(nav_node)->second.next;
      }
    }

    void PrintReverse(){
      Key nav_node = this->tail;
      for(size_t i = 0; i<this->node_count; i++){
        std::cout << umap.find(nav_node)->second.value << std::endl;
        nav_node = umap.find(nav_node)->second.prev;
      }
    }

    //Accessors
    size_t NodeCount(){
      return this->node_count;
    }

    size_t NodeCount() const{
      return this->node_count;
    }

    auto Find(Key& key){
      return umap.find(key);
    }

    auto Find(const Key& key) const{
      return umap.find(key);
    }

    std::vector<Key> FindValues(Value& value) {
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

    std::vector<Key> FindValues(const Value& value) const{
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

    Key GetNode(int index){
      if(index >= node_count){
        throw std::out_of_range("No node at index!");
      }
      Key nav_node = this->head;
      for(int i = 0; i<index; i++){
        nav_node = umap.find(nav_node)->second.next;
      }
      return nav_node;
    }

    Key GetNode(const int index) const{
      if(index >= node_count){
        throw std::out_of_range("No node at index!");
      }
      Key nav_node = this->head;
      for(int i = 0; i<index; i++){
        nav_node = umap.find(nav_node)->second.next;
      }
      return nav_node;
    }

    Key GetHead(){
      return this->head;
    }

    Key GetHead() const{
      return this->head;
    }

    Key GetTail(){
      return this->tail;
    }

    Key GetTail() const{
      return this->tail;
    }

    //Insertions
    void AddHead(const Key& key, const Value& value){
      if (umap.find(key) != umap.end()) {
        throw std::invalid_argument("Key already exists in the map.");
      }
      umap.emplace(key, NodeProps{Key{}, Key{}, value});
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

    void AddTail(const Key& key, const Value& value){
      if (umap.find(key) != umap.end()) {
        throw std::invalid_argument("Key already exists in the map.");
      }
      umap.emplace(key, NodeProps{Key{}, Key{}, value});

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

    void InsertBefore(const Key& key, const Value& value, const Key& some_node){
      /*
      Let node2insert = A
      Let some_node = B
      Let >,< be next, prev respectively

      A<  = B<
      A>  = B
      B<> = A
      B<  = A
      */

      if (umap.find(key) != umap.end()) {
        throw std::invalid_argument("Key already exists in the map.");
      }

      if (umap.find(some_node) == umap.end()) {
        throw std::out_of_range("Node to insert before does not exist.");
      }

      if(some_node == this->head){
        AddHead(key,value);
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

    void InsertAfter(const Key& key, const Value& value, const Key& some_node){
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

      if (umap.find(key) != umap.end()) {
        throw std::invalid_argument("Key already exists in the map.");
      }

      if (umap.find(some_node) == umap.end()) {
        throw std::out_of_range("Node to insert before does not exist.");
      }

      if(some_node == this->tail){
        AddTail(key,value);
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

    void InsertAt(const Key& key, const Value& value, const int index){
      if (umap.find(key) != umap.end()) {
        throw std::invalid_argument("Key already exists in the map.");
      }

      if(index > node_count){
        throw std::out_of_range("No node at index!");
      }
      else if(index == 0){
        AddHead(key,value);
        return;
      }
      else if(index == node_count){
        AddTail(key,value);
        return;
      }

      Key some_node = this->head;
      for(int i = 0; i<index; i++){
        some_node = umap.find(some_node)->second.next;
      }
      InsertBefore(key, value, some_node);
    }

    //Removals
    bool RemoveHead(){
      if(node_count == 0){
        return false;
      }
      else if(node_count == 1){
        umap.erase(this->head);
        this->head = Key{};
      }
      else{
        Key temp = umap.find(this->head)->second.next;
        umap.erase(this->head);
        this->head = temp;
      }
      this->node_count--;
      return true;
    }

    bool RemoveTail(){
      if(node_count == 0){
        return false;
      }
      else if(node_count == 1){
        umap.erase(this->tail);
        this->tail = Key{};
      }
      else{
        Key temp = umap.find(this->tail)->second.prev;
        umap.erase(this->tail);
        this->tail = temp;
      }
      this->node_count--;
      return true;
    }

    bool RemoveAt(int index){
      if(index == 0){
        RemoveHead();
        return true;
      }
      else if(index == node_count-1){
        RemoveTail();
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

    bool Remove(const Key& key){
      if(umap.find(key) == umap.end()){
        return false;
      }
      if(umap.find(key)->second.prev == Key{}){
        RemoveHead();
      }
      else if(umap.find(key)->second.next == Key{}){
        RemoveTail();
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

    int RemoveNodesWithValue(const Value& value){
      int removal_count = 0;
      Key nav_node = this->head;
      while(nav_node != Key{}){
        Key backup = umap.find(nav_node)->second.next;
        if(umap.find(nav_node)->second.value == value){
          if(umap.find(nav_node)->second.prev == Key{}){
            RemoveHead();
            nav_node = umap.find(nav_node)->second.next;
          }
          else if(umap.find(nav_node)->second.next == Key{}){
            RemoveTail();
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

    void Clear(){
      while(this->head != Key{}){
        Key temp = umap.find(this->head)->second.next;
        umap.erase(this->head);
        this->head = temp;
      }
      this->head = Key{};
      this->tail = Key{};
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
      if(this->node_count != other_ordered_hash_map.NodeCount()){
        return false;
      }
      Key nav_node = this->head;
      Key other_nav_node = other_ordered_hash_map.GetHead();
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
