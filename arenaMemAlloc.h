#pragma once
#include "arenaMemManager.h"
#include <iostream>

template<class T, size_t N>
struct ArenaMemAlloc {
  using value_type = T;
  using arena_type = ArenaMemManager<N>;

  ArenaMemAlloc() = default;
  ~ArenaMemAlloc() { std::cout << "ArenaMemAlloc Destructor Called!" << std::endl; }
  ArenaMemAlloc(ArenaMemAlloc const&) = default;
  ArenaMemAlloc& operator=(ArenaMemAlloc const&) = delete;

  template<class U>
  ArenaMemAlloc(ArenaMemAlloc<U, N>& other) noexcept
  : arena_mem_manager{other.arena_mem_manager} {}

  template <class U>
  struct rebind {
    using other = ArenaMemAlloc<U, N>;
  };

  auto allocate(size_t n) {
    return reinterpret_cast<T*>(arena_mem_manager.allocate(n * sizeof(T)));
  }

  auto deallocate(T* p, size_t n) noexcept {
    if (!arena_mem_manager.is_ptr_in_buffer(reinterpret_cast<char*>(p)))
      std::free(p);    
  }

  template<class U, size_t M>
  auto operator==(ArenaMemAlloc<U, M> const& other) const noexcept {
    return N == M && std::addressof(arena_mem_manager) == std::addressof(other.arena_mem_manager);
  }

  template<class U, size_t M>
  auto operator!=(ArenaMemAlloc<U, M> const& other) const noexcept {
    return !(*this == other);
  }
  arena_type arena_mem_manager{};
};