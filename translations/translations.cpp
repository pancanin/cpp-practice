#include <iostream>
#include <memory>

// Used only for testing
#include <assert.h>
#include <sstream>

const size_t growthFactor = 2;
const size_t initialCapacity = 8;

template<typename T, size_t Capacity = initialCapacity, bool isFixed = false>
struct Array {
  Array() : size(0), buffer(nullptr), capacity(Capacity) {}
  void destroy() {
    delete[] buffer;
  }
  bool init() {
    buffer = new (std::nothrow) T[capacity];
    return buffer;
  }
  bool push_back(T item) {
    if (size >= capacity) {
      if (!resize() || isFixed) { return false; }
    }
    buffer[size++] = item;
    return true;
  }

  size_t size;
  T* buffer;
private:
  size_t capacity;

  bool resize() {
    size_t oldCapacity = capacity;
    capacity *= growthFactor;
    T* newArr = new (std::nothrow) T[capacity];
    if (!newArr) { return false; }
    int err = memcpy_s(newArr, capacity * sizeof(T), buffer, oldCapacity * sizeof(T));
    if (err != 0) {
      delete[] newArr;
      return false;
    }
    delete[] buffer;
    buffer = newArr;
    return true;
  }
};

typedef Array<char> String;
typedef Array<String> Strings;

void testDynamicArray() {
  Array<int> arr;
  arr.init();
  
  for (size_t i = 10; i < 1000; i++) {
    arr.push_back(i);
  }

  for (size_t i = 10; i < 1000; i++) {
    assert(arr.buffer[i - 10] == i);
  }
}

Array<Array<Array<char>>> readDictionary(std::istream& in) {
  char c;
  Array<Strings> dictionary;
  dictionary.init();
  Strings dictEntry;
  dictEntry.init();
  String word;
  word.init();
  while (in >> c) { // This ignores spaces... so the below check does not work...
    if (c == ' ') {
      word.push_back('\0');
      dictEntry.push_back(word);
      if (dictEntry.size == 2) {
        dictionary.push_back(dictEntry);
        dictEntry = Strings();
        dictEntry.init();
      }
      word = String();
      word.init();
    }
    else {
      word.push_back(c);
    }
  }
  return dictionary;
}

void testReadDictionary() {
  std::istringstream ss("igraq play ucha study");
  Array<Strings> dict = readDictionary(ss);

  assert(dict.buffer[0].buffer[0].buffer[0], "i");
}

int main()
{
  testDynamicArray();
  testReadDictionary();
  std::cout << "Hello World!\n";
}
