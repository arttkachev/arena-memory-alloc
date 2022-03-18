#pragma once
#include <iostream>
template <size_t N>
class ArenaMemManager final {
  static constexpr size_t alignment = alignof(max_align_t);
public:
  ArenaMemManager() noexcept : ptr_head(buffer) {}
  //~ArenaMemManager() { std::cout << "Arena Manager Destructor Called!" << std::endl; }
  /*ArenaMemManager(ArenaMemManager const&) = delete;
  ArenaMemManager& operator=(ArenaMemManager const&) = delete;*/
  auto reset() noexcept { ptr_head = buffer; }
  auto get_used_space() const noexcept { return static_cast<size_t>(ptr_head - buffer); }
  static constexpr auto get_arena_size() noexcept -> size_t { return N; }
  auto allocate(size_t n);
  auto deallocate(char* p, size_t n) noexcept;  
private:  
  static auto align_up(size_t n) noexcept -> size_t {
    return n + (alignment - 1) & ~(alignment - 1);
  }
  auto is_ptr_in_buffer(char const* p) const noexcept {
    return buffer <= p && p <= buffer + N;
  }
  alignas(alignment) char buffer[N]{};  
  char* ptr_head {};  
};

template <size_t N>
auto ArenaMemManager<N>::allocate(size_t n) { 
  size_t const aligned_n = align_up(n);
  auto const available_bytes = static_cast<decltype(aligned_n)>(buffer + N - ptr_head);
  if (available_bytes >= aligned_n && available_bytes <= N) {
    std::cout << "arena allocation request " << aligned_n << " available " << available_bytes << " used " << get_used_space() << std::endl;
    auto const oldPtrHead = ptr_head;
    ptr_head += aligned_n;
    return oldPtrHead;
  }
  std::cout << "no available space in arena! " << std::endl;
  return static_cast<char*>(std::malloc(n));
}

template <size_t N>
auto ArenaMemManager<N>::deallocate(char* p, size_t n) noexcept {
  auto const ptr = reinterpret_cast<char*>(p);
  if (is_ptr_in_buffer(ptr)) {
    if (ptr + align_up(n) == ptr_head) {
      std::cout << "arena deallocation" << std::endl;
      ptr_head = ptr;
    }
  } else {
    std::free(p);    
  } 
}
