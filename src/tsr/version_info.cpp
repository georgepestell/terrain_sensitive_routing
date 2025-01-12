#include "tsr/version_info.hpp"
#include <string>

namespace tsr {

#ifdef @DEBUG_TIME@
#define DEBUG_TIME
#endif

std::string get_git_description() { return "@TSR_GIT_DESCRIPTION@"; }

std::string get_git_hash() { return "@TSR_GIT_COMMIT_HASH@"; }

std::string get_timestamp() { return "@TSR_TIMESTAMP@"; }

} // namespace tsr