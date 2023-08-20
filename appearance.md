# coordinating appearance
- instead of specifying a fairway, we specify a green
- fairway is just green w looser threshold
- rough is fairway w looser threshold
- deep rough is rough w looser threshold
- (oob is prob defined in polygons)

## crucial spots
- relevant to above: i think it makes sense to mix sdfs with the idea of "shot priority"
  - ie: we don't want hazards trouncing valuable fairway space (ie on nodes) - they can compete, but we want to reserve some space 


## drawing sand
- makes sense to me to prefer having sand just off of fairway
  - in front of green
  - edges of fairway
  - down the middle of fairway, if wide enough

## heightmap considerations
- at some point: we wanted to throw heightmaps into course generation - how?
  - do heightmaps carve into terrain, or does terrain build off heightmaps?
    - latter is more rational, but former might generate better results (since we're landscaping around the course spec!)
    - latter might generate better scenery, but it also leads to the possibility of generating impossible terrain!
    - might come up with approaches to do both
    - for now: gonna stick with the former and generate heightmaps after generating the course
      - keep fairways relatively smooth
      - if we carve, carve into non-essential fairway (ie preferring further from graph nodes, or only on one side)

## eventual features
- cliffs
  - something we'd complement in terrain generation too!
  - into rough/deep rough, or potentially breaking up a fairway portion
- water
  - lakes
    - larger, want to have to one side of the fairway, can impede on fairway but not too much
      - cut in on some cases, but def avoid important stuff
      - like the idea of generating them on one side of the course and letting them "leach" in
  - rivers
    - cutting across stretches of fairway
    - or: in the far distance!
  - oceans
    - always to one side, massive
  - in terrain editor, should almost always manifest as a harsh fall off

## sharp walls
- will prob skip for now - not great w current system
  - as part of some features, i think we'll end up w some sort of "path-based" system
    - ex: for rivers, and for defining course boundaries
  - can bool-cut height map(?) and build walls below