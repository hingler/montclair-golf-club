Import('env')

env.Append(CPPPATH=[[env.Dir(d) for d in [
    "include",
    "lib/stb",
    "lib/glm"
]]])

gtest = SConscript("./SConstruct_gtest", exports = 'env')

gog = SConscript("lib/gog43/SConstruct", exports = 'env')

test_dir = "test/"

env.Append(CXXFLAGS=["-std=c++17"])

# tba: need to handle arch options here i believe
# or: export godot env here

# sucks shit

sources = [
]

# glob doubles up sources - need to go thru and actually prune these out

# (tba2: add gog43 to the golf club code)

sources.extend(Glob("src/*.cpp"))
sources.extend(Glob("src/*/*.cpp"))
sources.extend(Glob("src/*/*/*.cpp"))
sources.extend(Glob("src/*/*/*/*.cpp"))

# dedupe - anything from sources
sources = list(set(sources))

env.Append(LIBS=[ gog ])

library = env.Library("build/montclairgolfclub", source=sources)

demos = [
    "fairway_test",
    "sampler_write_demo",
    "terrain_convert_demo",
    "gaussian_test"
]

tests = [
    "BezierCurveTest",
    "CompoundCurveTest",
    "GenericImageTest",
    "GetDistanceToCircleTest",
    "InterpolatingImageSamplerTest",
    "MetaballSamplerTest",
    "RGBAColorTest",
    "SimplexNoiseSamplerTest",
    "TerrainDataTest",
    "SeedPathBuilderTest",
    "SeedPathIteratorTest",
    "SeedPathDrawTest"
]

# keep out of lib build?
env.Append(LIBS=[ gtest ])

for demo in demos:
    print(demo)
    Default(env.Program("build/" + demo, sources + ["demo/" + demo + ".cpp"]))

baseDir = Dir('#').abspath

test_sources = [test_dir + test + ".cpp" for test in tests]
Default(env.Program(baseDir + "/build/GTEST_mgc", test_sources))

Default(library)
Return("library")