#include "../src/solution.cpp"
#include "gtest/gtest.h"
#include <string>

TEST(FixedBlockMemoryResourceTest, AllocateAndDeallocate) {
  FixedBlockMemoryResource mr(1024);
  void *p1 = mr.allocate(100, alignof(std::max_align_t));
  ASSERT_NE(nullptr, p1);
  mr.deallocate(p1, 100, alignof(std::max_align_t));

  void *p2 = mr.allocate(200, alignof(std::max_align_t));
  ASSERT_NE(nullptr, p2);
}

TEST(FixedBlockMemoryResourceTest, DeallocateInvalidPointer) {
  FixedBlockMemoryResource mr(1024);
  void *p = reinterpret_cast<void *>(0xDEADBEEF);
  mr.deallocate(p, 100, alignof(std::max_align_t));
}

TEST(SinglyLinkedListTest, BasicOperations) {
  FixedBlockMemoryResource mr(1024);
  SinglyLinkedList<int> lst{&mr};

  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);

  int count = 0;
  int sum = 0;
  for (int x : lst) {
    sum += x;
    count++;
  }
  ASSERT_EQ(3, count);
  ASSERT_EQ(6, sum);

  lst.clear();
  ASSERT_TRUE(lst.empty());
}

TEST(SinglyLinkedListTest, PushBackAndIterate) {
  FixedBlockMemoryResource mr(1024);
  SinglyLinkedList<int> lst{&mr};

  for (int i = 0; i < 5; ++i) {
    lst.push_back(i);
  }

  int expected = 0;
  for (int x : lst) {
    ASSERT_EQ(expected, x);
    expected++;
  }
  ASSERT_EQ(expected, 5);
}

TEST(SinglyLinkedListTest, CopyConstructor) {
  FixedBlockMemoryResource mr(1024);
  SinglyLinkedList<int> lst1{&mr};
  lst1.push_back(10);
  lst1.push_back(20);

  SinglyLinkedList<int> lst2 = lst1;

  int values1[2];
  int idx = 0;
  for (int x : lst2) {
    values1[idx++] = x;
  }
  ASSERT_EQ(values1[0], 10);
  ASSERT_EQ(values1[1], 20);

  {
    lst1.clear();
    lst1.push_back(100);

    idx = 0;
    for (int x : lst2) {
      if (idx == 0) ASSERT_EQ(x, 10);
      if (idx == 1) ASSERT_EQ(x, 20);
      idx++;
    }
  }
}

TEST(SinglyLinkedListTest, MoveConstructor) {
  FixedBlockMemoryResource mr(1024);
  SinglyLinkedList<int> lst1{&mr};
  lst1.push_back(1);
  lst1.push_back(2);

  SinglyLinkedList<int> lst2 = std::move(lst1);

  int count = 0;
  int arr[2];
  for (int x : lst2) {
    arr[count++] = x;
  }

  ASSERT_EQ(2, count);
  ASSERT_EQ(1, arr[0]);
  ASSERT_EQ(2, arr[1]);
  ASSERT_TRUE(lst1.empty());
}

struct MyStruct {
  int a;
  double b;
  std::string c;

  bool operator==(const MyStruct &other) const {
    return a == other.a && b == other.b && c == other.c;
  }
};

TEST(SinglyLinkedListTest, ComplexType) {
  FixedBlockMemoryResource mr(1024);
  SinglyLinkedList<MyStruct> lst{&mr};

  lst.push_back({1, 2.5, "hello"});
  lst.push_back({2, 3.14, "world"});

  int count = 0;
  MyStruct arr[2];
  for (auto &s : lst) {
    arr[count++] = s;
  }

  ASSERT_EQ(2, count);
  ASSERT_TRUE(arr[0] == (MyStruct{1, 2.5, "hello"}));
  ASSERT_TRUE(arr[1] == (MyStruct{2, 3.14, "world"}));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
