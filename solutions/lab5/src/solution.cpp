#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <memory_resource>
#include <stdexcept>
#include <string>
#include <utility>
#include <cstddef>

class FixedBlockMemoryResource : public std::pmr::memory_resource {
public:
  explicit FixedBlockMemoryResource(std::size_t totalSize)
      : totalSize_(totalSize) {
    basePtr_ = std::malloc(totalSize_);
    if (!basePtr_) {
      throw std::bad_alloc();
    }
    freeBlocks_[basePtr_] = totalSize_;
  }

  ~FixedBlockMemoryResource() override {
    std::free(basePtr_);
  }

protected:
  void *do_allocate(std::size_t bytes, std::size_t alignment) override {
    for (auto it = freeBlocks_.begin(); it != freeBlocks_.end(); ++it) {
      void* blockPtr = it->first;
      std::size_t blockSize = it->second;

      std::size_t adjustment = alignAdjustment(blockPtr, alignment);
      std::size_t totalNeeded = bytes + adjustment;

      if (blockSize >= totalNeeded) {
        void* alignedPtr = static_cast<std::byte*>(blockPtr) + adjustment;

        freeBlocks_.erase(it);

        std::size_t remaining = blockSize - totalNeeded;
        if (remaining > 0) {
          void* newFreeBlock = static_cast<std::byte*>(alignedPtr) + bytes;
          freeBlocks_[newFreeBlock] = remaining;
        }

        return alignedPtr;
      }
    }
    throw std::bad_alloc();
  }

  void do_deallocate(void *p, std::size_t bytes, std::size_t) override {
    if (!p || bytes == 0) {
      return;
    }
    freeBlocks_[p] = bytes;
    coalesce();
  }

  bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override {
    return this == &other;
  }

private:
  void* basePtr_ = nullptr;
  std::size_t totalSize_;
  std::map<void*, std::size_t> freeBlocks_;

  static std::size_t alignAdjustment(void* p, std::size_t alignment) {
    std::uintptr_t addr = reinterpret_cast<std::uintptr_t>(p);
    std::size_t misalignment = addr % alignment;
    if (misalignment == 0) return 0;
    return alignment - misalignment;
  }

  void coalesce() {
    if (freeBlocks_.empty()) return;
    auto it = freeBlocks_.begin();
    auto prev = it;
    ++it;
    while (it != freeBlocks_.end()) {
      void* prevEnd = static_cast<std::byte*>(prev->first) + prev->second;
      if (prevEnd == it->first) {
        prev->second += it->second;
        it = freeBlocks_.erase(it);
      } else {
        prev = it;
        ++it;
      }
    }
  }
};

template <class T>
struct SListNode {
  T value;
  SListNode* next = nullptr;
};

template <class T, class Alloc = std::pmr::polymorphic_allocator<T>>
requires std::is_default_constructible_v<T> && 
         std::is_same_v<Alloc, std::pmr::polymorphic_allocator<T>>
class SinglyLinkedList {
public:
  explicit SinglyLinkedList(std::pmr::memory_resource *mr = std::pmr::get_default_resource())
      : alloc_(mr), nodeAlloc_(mr), head_(nullptr) {}

  SinglyLinkedList(const SinglyLinkedList &other)
      : alloc_(other.alloc_), nodeAlloc_(other.nodeAlloc_), head_(nullptr) {
    for (auto &val : other) {
      push_back(val);
    }
  }

  SinglyLinkedList(SinglyLinkedList &&other) noexcept
      : alloc_(other.alloc_), nodeAlloc_(other.nodeAlloc_), head_(other.head_) {
    other.head_ = nullptr;
  }

  SinglyLinkedList &operator=(const SinglyLinkedList &other) {
    if (this != &other) {
      clear();
      for (auto &val : other) {
        push_back(val);
      }
    }
    return *this;
  }

  SinglyLinkedList &operator=(SinglyLinkedList &&other) noexcept {
    if (this != &other) {
      clear();
      head_ = other.head_;
      other.head_ = nullptr;
    }
    return *this;
  }

  ~SinglyLinkedList() {
    clear();
  }

  void push_back(const T &value) {
    SListNode<T>* newNode = allocateNode(value);
    if (!head_) {
      head_ = newNode;
    } else {
      auto* cur = head_;
      while (cur->next) {
        cur = cur->next;
      }
      cur->next = newNode;
    }
  }

  void clear() {
    while (head_) {
      auto* next = head_->next;
      deallocateNode(head_);
      head_ = next;
    }
  }

  bool empty() const { return head_ == nullptr; }

  class iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    iterator(SListNode<T>* ptr) : ptr_(ptr) {}

    reference operator*() const { return ptr_->value; }
    pointer operator->() const { return &ptr_->value; }
    iterator &operator++() { ptr_ = ptr_->next; return *this; }
    iterator operator++(int) { iterator tmp = *this; ptr_ = ptr_->next; return tmp; }
    bool operator==(const iterator &other) const { return ptr_ == other.ptr_; }
    bool operator!=(const iterator &other) const { return ptr_ != other.ptr_; }

  private:
    SListNode<T>* ptr_;
  };

  iterator begin() { return iterator(head_); }
  iterator end() { return iterator(nullptr); }

  class const_iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;

    const_iterator(const SListNode<T>* ptr) : ptr_(ptr) {}

    reference operator*() const { return ptr_->value; }
    pointer operator->() const { return &ptr_->value; }
    const_iterator &operator++() { ptr_ = ptr_->next; return *this; }
    const_iterator operator++(int) { const_iterator tmp = *this; ptr_ = ptr_->next; return tmp; }
    bool operator==(const const_iterator &other) const { return ptr_ == other.ptr_; }
    bool operator!=(const const_iterator &other) const { return ptr_ != other.ptr_; }

  private:
    const SListNode<T>* ptr_;
  };

  const_iterator begin() const { return const_iterator(head_); }
  const_iterator end() const { return const_iterator(nullptr); }

private:
  using NodeAlloc = typename std::pmr::polymorphic_allocator<SListNode<T>>;

  SListNode<T>* allocateNode(const T &value) {
    SListNode<T>* node = nodeAlloc_.allocate(1);
    nodeAlloc_.construct(node, SListNode<T>{value, nullptr});
    return node;
  }

  void deallocateNode(SListNode<T>* node) {
    nodeAlloc_.destroy(node);
    nodeAlloc_.deallocate(node, 1);
  }

  Alloc alloc_;
  NodeAlloc nodeAlloc_;
  SListNode<T>* head_;
};
