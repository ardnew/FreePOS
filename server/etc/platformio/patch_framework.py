from os.path import join, isfile, dirname
from contextlib import suppress

Import("env")

framework_name = "arduino-mbed"
framework_path = env.PioPlatform().get_package_dir("framework-" + framework_name)

patch_force = join(dirname(env['PROJECT_CONFIG']), env.GetProjectOption("custom_patch_force"))

patch_sentinal = ".patch-applied"
patchflag_path = join(framework_path, patch_sentinal)

def touch(path):
    with open(path, "w") as fp: fp.write("")

def boolstr(str):
    try:    return bool(ast.literal_eval(str))
    except: return False

# patch file only if flag file has not been created (or force flag defined)
if not isfile(patchflag_path) or boolstr(patch_force):
    original_file = join(framework_path, "cores", "arduino", "WMath.cpp")
    patched_file = join("patch", "100-" + framework_name + "-cores-arduino-WMath-replace-srandom.patch")
    assert isfile(original_file) and isfile(patched_file)
    env.Execute("patch %s %s" % (original_file, patched_file))
    # create flag file that indicates patch has been applied
    env.Execute(lambda *args, **kwargs: touch(patchflag_path))
