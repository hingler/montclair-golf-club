# placing sand
- sand coverage should be tighter around the green, but sometimes along fairway
- sand should not obsucre much more than half (ish) of the fairway path, and it should definitely not touch the green
- sand in the rough is fair game, but don't put a ton of it.

## placing sand around a known (metaball) fairway
- pick a point along our curve
  - if the point is rough, then take a couple poisson samples to gauge proximity to fairway
    - within a couple yards, place a sand patch there
  - if the point is fairway, then try to drift out of fairway range
    - same thing - place sand somewhere along the edge

## placing sand around green
- green is generally approached on sides by green
  - speaking of: liking the idea of placing "rough" around sand? or at least "subtracting" weight from fairway samples near green, where we find sand
  - on a tougher course, it's all around it (ex pebble beach - often 180+ degrees of sand coverage)
    - thinking we do something similar to rough - pick a couple target locations, then walk along the gradient and carve them out


### how to gauge fairway location with metaballs?
- metaball sampling is costly(ish) - how can we work around it?
  - drop density, displace with noise
    - (need to experiment with this, doesn't really impact execution)
- poisson sampling
  - find a central point, poisson sample to roughly gauge safe range
  - place sand accordingly, again using metaballs
  - (try to stay away from fairway if we can)
- gradient sampling
  - if possible: find the gradient of a metaball layout, and march along it
    - (gradient of sum is sum of gradients - gradient for metaball is relatively easy (just a vector towards center w mag equal to inv dist) so we could add them all up?)
    - rough has it - pick a point some distance from the central line, then march out a bit, then just place sand balls roughly along the gradient
- path sampling
  - don't bother with metaballs, just place based on course path (some distance away, along cross axis)

## terrain sampling
- thinking: use the "sand deform" as a decrement weight on terrain
  - for terrain: want a "coarse noise" to encode larger scale deformations, and a "fine noise" to handle tighter stuff
  - we can tone down the fine noise when drawing features
