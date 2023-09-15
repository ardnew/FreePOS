#ifndef api_version_hpp
#define api_version_hpp

#ifndef VERSION_NAME
#define VERSION_NAME "FreePOS"
#endif

#ifndef VERSION_SEMVER
#define VERSION_SEMVER "0.0.0"
#endif

static constexpr auto BLEDeviceName = VERSION_NAME;
static constexpr auto BLEServerName = "Server";
static constexpr auto BLEClientName = "Client";

#endif // api_version_hpp
