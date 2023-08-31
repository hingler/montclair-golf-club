# building terrain
- for now: course is a "safe zone" where we apply only "safe" changes
  - minor slopes, within a couple degrees (we'll stop the ball from rolling too far)
- liking the idea of building the course at a settling point in some broader (larger scale) noise
  - use metaballs as a weight for terrain mapping - if we're below a certain threshold, then start amplifying terrain
    - (adds further cost to metaball sampling - will have to come up w a shortcut to expedite that - whatever for now though)

- if we mark the course as a safe zone then we can come up w any number of generation schemes
  - mountains - generate peaks which transform as we drift away from the fairway - we build in the valley (safe zone crucial here)
  - general - thinking of meadowmeer, we can stick with smooth changes overall and pepper w some trees
  - forest - larger sweeping uphills/downhills (within safe range), in any direction - place our course along any axis (w safe zone, can smooth out course area and start to build up hills as we move away)
    - think of safe area as "padding" preexisting generation rather than coercing it (exc. in mountain case where there's no other option) - we want to return to the generated terrain w/in some delta, but we can fill in surface features in between (need to see how it looks ig)

- how to execute, given safe zone?
  - mountains: easy enough (calculate a factor based on dist, and build up features based on dist - introduce a softer curve for the course itself which is compatible)
  - forest: harder
    - we don't really differentiate btwn "safe" vs. "unsafe" generation, so we need to make the "unsafe" generation "safer"
      - smooth out height changes (could discretize, but that's also not desirable necc.)
      - ensure all locations are "safe" so that we don't have to deal w generation issues
      - scale down noise around "origin" point on course - fade into real noise later
        - liking this - with a sufficiently large "smoothing" distance so as to hide any "terraforming"
        - pick origin stochastically (sample a few points along terrain curve)
        - idr simplex noise gradient - assuming < 45 deg so we can work with that assumption ig (and polish as we go)
        - don't do this yet though!
        
## defining safe range
  - thinking: we keep course slopes w/in 5 deg (ish)
    - also: would be nice to have a "demo program" where we can test out how terrains look
    - might be good to "discretize" sigma field before moving to terrain sampling
  - should prefer building w/in terrain specifications though, if we can (otherwise run into same course synd)

# stacking detail levels
- want course to be hi detail
- would like to use one algo for low/high detail, but have separate maps for it to save mem (vs. generating on the fly)
  - pull from hi if we can, else pull from lo
  - (easy enough to author)