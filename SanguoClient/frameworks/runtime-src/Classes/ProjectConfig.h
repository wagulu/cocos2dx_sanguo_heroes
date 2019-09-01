//
//  DebugConfig.h
//  Game
//
//  Created by fuchenhao on 3/26/15.
//
//

#ifndef Game_DebugConfig_h
#define Game_DebugConfig_h

#ifndef RELEASE_VERSION
#define RELEASE_VERSION     (false)
#endif

#ifndef USE_LUA_ENGINE
#define USE_LUA_ENGINE      (RELEASE_VERSION || false)
#endif

#ifndef TEST_BATTLE
#define TEST_BATTLE         (!RELEASE_VERSION && false)
#endif

#ifndef ACTIVE_AI
#define ACTIVE_AI           (RELEASE_VERSION || false)
#endif

#define SHOW_GRID           (!RELEASE_VERSION && false)
#define SHOW_HERO_TILE      (SHOW_GRID && true)
#define SHOW_STATS          (!RELEASE_VERSION && false)

#ifndef SKIP_DIALOG
#define SKIP_DIALOG         (!RELEASE_VERSION && true)
#endif

#endif
