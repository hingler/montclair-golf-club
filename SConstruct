Import('env')

env.Append(CPPPATH=[[env.Dir(d) for d in [
    "include",
    "lib/stb",
    "lib/glm"
]]])

gtest = SConscript("./SConstruct_gtest", exports = 'env')

gog = SConscript("lib/gog43/SConstruct", exports = 'env')

sdf = SConscript("lib/sdf-jak/SConstruct", exports = 'env')

SConscript("lib/extra-corrugated/SConstruct", exports = 'env')

test_dir = "test/"

# busted, need to fix build

env.Append(CXXFLAGS=["-std=c++17"])

# tba: need to handle arch options here i believe
# or: export godot env here

# sucks shit
# dont want to fw rust for a bit

sources = [
]

# glob doubles up sources - need to go thru and actually prune these out

# (tba2: add gog43 to the golf club code)
# lol

sources.extend(Glob("src/*.cpp"))
sources.extend(Glob("src/*/*.cpp"))
sources.extend(Glob("src/*/*/*.cpp"))
sources.extend(Glob("src/*/*/*/*.cpp"))

# dedupe - anything from sources
sources = list(set(sources))

env.Append(LIBS=[ gog, sdf ])

library = env.Library("build/montclairgolfclub", source=sources)

demos = [
    "fairway_test",
    "sampler_write_demo",
    "terrain_convert_demo",
    "gaussian_test",
    "seed_demo",
    "course_demo"
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
    "SeedPathDrawTest",
    "HoleBoxTest",
    "HoleChunkBoxTest",
    "HoleChunkManagerTest",
    "SCPCTest"
]

# keep out of lib build?
env.Append(LIBS=[ gtest ])
baseDir = Dir('#').abspath

for demo in demos:
    print(demo)
    Default(env.Program(baseDir + "/build/" + demo, sources + ["demo/" + demo + ".cpp"]))


test_sources = [test_dir + test + ".cpp" for test in tests]
Default(env.Program(baseDir + "/build/GTEST_mgc", test_sources))

Default(library)
Return("library")
