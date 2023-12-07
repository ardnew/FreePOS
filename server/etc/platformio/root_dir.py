from os.path import join, isfile, dirname

Import("env")

root_dir = join(dirname(env['PROJECT_CONFIG']), env.GetProjectOption("custom_root_dir"))

env['PROJECT_DIR']         = root_dir
env['PROJECT_INCLUDE_DIR'] = join(root_dir, "include")
env['PROJECT_SRC_DIR']     = join(root_dir, "src")
env['PROJECT_DATA_DIR']    = join(root_dir, "data")
env['PROJECT_TEST_DIR']    = join(root_dir, "test")

#print(env.Dump())
