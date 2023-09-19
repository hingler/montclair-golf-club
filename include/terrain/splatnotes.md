# course splats

data we need:
- course terrain (rough, fairway, green, sand [for now])
- terrain itself, as a heightmap (can fetch, honestly we can pick whatever and slap the course on top)
- additional splats (generate here, or generate from course/terrain maps? - not worrying about it now)
- particle data ie trees, grass, etc. (down the road - don't need to worry about it now)

interface
- accept a seed and some configuration params
- i think we should let the user pass in the course configuration plus:
  - terrain params (can create a shell for this, but it'll be one-size-fit-all)
  - particle params
  - etc etc... (down the road)

- eventually: would like to move everything into an "internal" path and just expose the relevant stuff (eventual "public" will be mgc)

features
- root generator (accept various generation params to create a course + surrounding terrain)
  - return splats (as samplers - named functions i think, we need to know typings for splat manager)
    - possibly: return hints on which textures we want where
  - return course info
    - curve
    - par
    - yardage
    - tee + hole locations
  - return height map (also as sampler)