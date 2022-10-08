# import os

# PATH_TO_THIS_DIR = os.path.dirname(os.path.abspath(__file__))
flags: list = [ '-xc++', '-isystem', '/usr/include/c++/9/', '-Isrc/', '-Ilib/', '-Ilib/PhysX/physx/', '-Ilib/PhysX/pxshared/', '-O3', '--std=c++17', '-Werror' ]

def Settings(**kwargs):
    return { 'flags': flags, 'do_cache': True }
