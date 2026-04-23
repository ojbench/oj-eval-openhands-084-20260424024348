
//
// Created by wzj on 2021/12/3.
// Modified by Yifei Ma on 2023/10/25.
//

#ifndef FINAL_SRC_HPP
#define FINAL_SRC_HPP

#include <string>
#include <vector>

// If the two following commands bother you, you may comment them and write
// std::vector and std::string instead.
using std::string;
using std::vector;

struct LinkedHashMap;
struct Node {
  Node* next = nullptr;
  Node* timePre = nullptr;
  Node* timeNext = nullptr;

  int key = 0;
  string value;
};

struct Data {
  int key;
  string value;
};

vector<Data> traverseByTime(const LinkedHashMap& obj);
vector<Data> traverseByIndex(const LinkedHashMap& obj);

struct LinkedHashMap {
  // You can modify beg, cur and related parts in the constructor as you wish.
  // 你可以根据自己写链表的习惯修改或删除头尾节点并修改构造函数.

  int len;
  Node** array;
  Node* beg;
  Node* cur;
  // Function pointer to the traversal method
  vector<Data> (*forEachMethod)(const LinkedHashMap& obj);

  void init(int len_, bool forEachByTime) {
    len = len_;
    beg = nullptr;
    cur = nullptr;
    array = new Node*[len];
    for (int i = 0; i < len; i++) {
      array[i] = nullptr;
    }
    if (forEachByTime)
      forEachMethod = traverseByTime;
    else
      forEachMethod = traverseByIndex;
  }

  void clearMemory() {
    // Free all nodes in the time-based linked list
    Node* current = beg;
    while (current != nullptr) {
      Node* next = current->timeNext;
      delete current;
      current = next;
    }
    // Free the array
    delete[] array;
    // Reset pointers
    beg = nullptr;
    cur = nullptr;
    array = nullptr;
  }

  void insert(int key, string value) {
    // Create new node
    Node* newNode = new Node();
    newNode->key = key;
    newNode->value = value;
    
    // Insert at head of hash table chain
    int index = key % len;
    if (index < 0) index += len;  // Handle negative keys
    newNode->next = array[index];
    array[index] = newNode;
    
    // Insert at end of time-based linked list
    if (beg == nullptr) {
      beg = newNode;
      cur = newNode;
      newNode->timePre = nullptr;
      newNode->timeNext = nullptr;
    } else {
      cur->timeNext = newNode;
      newNode->timePre = cur;
      newNode->timeNext = nullptr;
      cur = newNode;
    }
  }

  void remove(int key, string value) {
    int index = key % len;
    if (index < 0) index += len;  // Handle negative keys
    
    // We need to handle the case where the first node(s) in the chain match
    Node* current = array[index];
    Node* prev = nullptr;
    
    while (current != nullptr) {
      if (current->key == key && current->value == value) {
        // Remove from time-based linked list
        if (current->timePre != nullptr) {
          current->timePre->timeNext = current->timeNext;
        } else {
          // This is the first node in time list
          beg = current->timeNext;
        }
        
        if (current->timeNext != nullptr) {
          current->timeNext->timePre = current->timePre;
        } else {
          // This is the last node in time list
          cur = current->timePre;
        }
        
        // Remove from hash table chain
        if (prev == nullptr) {
          // Removing first node in chain
          array[index] = current->next;
          Node* temp = current;
          current = current->next;
          delete temp;
        } else {
          prev->next = current->next;
          Node* temp = current;
          current = current->next;
          delete temp;
        }
      } else {
        prev = current;
        current = current->next;
      }
    }
  }

  vector<string> ask(int key) const {
    vector<string> result;
    int index = key % len;
    if (index < 0) index += len;  // Handle negative keys
    
    Node* current = array[index];
    while (current != nullptr) {
      if (current->key == key) {
        result.push_back(current->value);
      }
      current = current->next;
    }
    
    return result;
  }

  vector<Data> forEach() const { return forEachMethod(*this); }
};

vector<Data> traverseByTime(const LinkedHashMap& obj) {
  vector<Data> result;
  Node* current = obj.beg;
  while (current != nullptr) {
    Data data;
    data.key = current->key;
    data.value = current->value;
    result.push_back(data);
    current = current->timeNext;
  }
  return result;
}

vector<Data> traverseByIndex(const LinkedHashMap& obj) {
  vector<Data> result;
  // Traverse array in order
  for (int i = 0; i < obj.len; i++) {
    // For each non-empty chain, traverse from head to tail
    Node* current = obj.array[i];
    while (current != nullptr) {
      Data data;
      data.key = current->key;
      data.value = current->value;
      result.push_back(data);
      current = current->next;
    }
  }
  return result;
}

#endif  // FINAL_SRC_HPP
