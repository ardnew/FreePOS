from os.path import join, isfile

Import("env")

def cflags(env, node):
    return env.Object(
        node,
        CFLAGS=env['CFLAGS'] + [
            '-std=gnu11',
            '-Os'
        ]
    )

def cxxflags(env, node):
    return env.Object(
        node,
        CXXFLAGS=env['CXXFLAGS'] + [
            '-std=gnu++14',
            '-Os'
        ]
    )

env.AddBuildMiddleware(cflags, "*.c")
env.AddBuildMiddleware(cxxflags, "*.cpp")
