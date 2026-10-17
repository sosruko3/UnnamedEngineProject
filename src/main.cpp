
#include "engine/core/Engine.h"
#include "engine/memory/Arena.h"
#include "engine/scene/SceneManager.h"
//#include "game/game_scenes.h"
//#include "game_config.h"

// Suppress one-time OS shared library allocations from Wayland and graphics drivers
// these allocs has nothing to do with the project. any leak from them might
// be caused by a bug in the OS or a third-party library, so we ignore them.
#if defined(__SANITIZE_ADDRESS__) || (defined(__has_feature) && __has_feature(address_sanitizer))
extern "C" const char* __lsan_default_suppressions() {
    return "leak:Wayland_VideoInit\n"
           "leak:libwayland-client\n"
           "leak:libxkbcommon\n";
}
#endif

int main() {

  Arena masterArena = {};
  masterArena = ArenaAllocate(512*1024*1024);
  if (!masterArena.base_ptr) {
    ArenaFree(&masterArena);
    return -1;
  }
  Engine engine = {};
  engine.init(&masterArena);
  //SceneManager_Init(Game_GetScene);
  //SceneManager_ChangeScene(GAME_STATE_PLAYING);
  engine.run();
  engine.shutdown();

  ArenaFree(&masterArena);
  return 0;
}
