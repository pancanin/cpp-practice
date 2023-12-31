#include <iostream>
#include <memory>

// Used only for testing
#include <assert.h>

typedef const char* String;
typedef String* StringArray;

const size_t growthFactor = 2;
const size_t initialCapacity = 8;

template<typename T>
struct Array {
  Array() : size(0), buffer(nullptr), capacity(initialCapacity) {}
  ~Array() {
    delete[] buffer;
  }
  bool init() {
    buffer = new (std::nothrow) T[capacity];
    return buffer;
  }
  bool push_back(T item) {
    if (size >= capacity) {
      if (!resize()) { return false; }
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

void testDynamicArrayWithStringArrays() {
  Array<StringArray> arr;
  arr.init();

  String word1[] = { "smqh", "laughter" };
  String word2[] = { "maitap", "joke" };
  String word3[] = { "dete", "child" };

  StringArray a = word1;
  StringArray b = word2;
  StringArray c = word3;

  arr.push_back(a);
  arr.push_back(b);
  arr.push_back(c);

  assert(arr.buffer[0][0] == "smqh");
  assert(arr.buffer[0][1] == "laughter");
  assert(arr.buffer[1][0] == "maitap");
  assert(arr.buffer[1][1] == "joke");
  assert(arr.buffer[2][0] == "dete");
  assert(arr.buffer[2][1] == "child");
}

static StringArray* buffer = nullptr;

int main()
{
  testDynamicArray();
  testDynamicArrayWithStringArrays();
  std::cout << "Hello World!\n";
}
