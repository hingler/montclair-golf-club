# sampling technique

right now: naively using metaballs to sample terrain. limitation is that we lose a lot of more complex emergent behavior which might be desirable

- q: what behavior do we need???
  - gradients (prob for sand patches + water patches)
    - once we have a "green" metaball, we can march along the gradient to find patches of "sand"
      - alt: we can just do it with our path, and configure offsets st we don't touch the fairway (ie tight negative weights??)
        - should only be necessary around the green - otherwise we're fine
    - water: how to?
      - again: just rely on paths, if we encroach on the fairway we do it "carefully"
      - lends itself to building features "up"

## performance
- not sure why performance is so dogshit:
  - could be that we're sampling a ton of metaballs
  - could be overhead of making literally millions of virtual function calls
  - who can say? :3
  - either way, this will get retooled a bit later

## alternatives to metaball sampling
- poly based??
  - (still need to do the splat maps but it might turn out better)
  - (hard edges aren't the style i want to go for)


# next big demo
- want to combine generic course gen w a swappable terrain gen
  - expose course gen configs
    - (not too many because it's a big struct :D)
  - expose terrain gen seed
  - expose terrain gen offset
  - explore in 3d
    - color terrain w splatmap
    - apply heightmap
    - no collider (slows things down considerably)
    - feed camera pos into "orchestrator"
  - would be nice if we can shuffle "on the fly" or generate terrain in bg?
- stop step - expose terrain info, regen in place
  - place "course origin" and see how it looks!

## terrain ctor
- accept course curve (to ballpark course center)
- accept combined course sampler (to ballpark proximity to course features)

# feature building
- would be nice if i had some way to "preview" terrain in progress :/

