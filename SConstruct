Import('env')

env.Append(CPPPATH=[
    "include",
    "lib/stb",
    "lib/glm"
])

env.Append(CXXFLAGS=["-std=c++17"])

# tba: need to handle arch options here i believe
# or: export godot env here

sources = Glob("src/*.cpp")
sources.extend(Glob("src/*/*.cpp"))
sources.extend(Glob("src/*/*/*.cpp"))
sources.extend(Glob("src/*/*/*/*.cpp"))

library = env.Library("build/montclairgolfclub", source=sources)

demos = [
    "fairway_test",
    "sampler_write_demo",
    "terrain_convert_demo",
    "gaussian_test"
]

for demo in demos:
    print(demo)
    Default(env.Program("build/" + demo, sources + ["demo/" + demo + ".cpp"]))

Default(library)
Return("library")