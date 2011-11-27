import os

def defaultEnvironment():
    env = Environment(ENV = os.environ)
    env.Append(CCFLAGS = ['-g3'])
    return env

allegro_store = [None]
def allegroLibrary():
    # Return the cached version
    if allegro_store[0] != None:
        return allegro_store[0]
    env = defaultEnvironment()
    build = 'build-allegro'
    env.VariantDir(build, 'allegro4')
    source = Split("""allegro.c math3d.c math.c file.c unicode.c """)
    library = env.StaticLibrary(['%s/%s' % (build, file) for file in source])
    # Cache the allegro library so its not built each time allegroLibrary()
    # is called
    allegro_store[0] = library
    return allegro_store[0]

def makeExample(name, source):
    env = defaultEnvironment()
    env.ParseConfig('pkg-config allegro-5.1 allegro_primitives-5.1 allegro_image-5.1 --cflags --libs')
    env.VariantDir('build-examples', 'examples')
    env.Append(LIBS = [allegroLibrary()])
    # env.Prepend(LIBS = [allegroLibrary()])
    env.Append(CPPPATH = ['allegro4'])
    return env.Program(name, map(lambda x: 'build-examples/%s' % x, source))

def stars():
    return makeExample('exstars', ['exstars.c'])

def shade():
    return makeExample('exshade', ['exshade.c'])

stars()
shade()