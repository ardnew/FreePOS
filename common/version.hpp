#ifndef common_version_hpp
#define common_version_hpp

#ifndef VERSION_NAME
#define VERSION_NAME "FreePOS"
#endif

#ifndef VERSION_SEMVER
#define VERSION_SEMVER "0.0.0"
#endif

static constexpr auto DeviceName = VERSION_NAME;
static constexpr auto ServerName = "Server";
static constexpr auto ClientName = "Client";

#endif // common_version_hpp
