#pragma once
#include "arenaMemManager.h"
auto user_arena = ArenaMemManager<1024>{};
class User {
public:
  auto operator new(size_t size) -> void* {
    return user_arena.allocate(size);
  }
  auto operator delete (void* p) -> void {
    user_arena.deallocate(static_cast<char*>(p), sizeof(User));
  }
  auto operator new[](size_t size) -> void* {
    return user_arena.allocate(size);
  }
  auto operator delete[](void* p, size_t size) -> void{
    user_arena.deallocate(static_cast<char*>(p), size);
  }
private:
  int id {};
};

