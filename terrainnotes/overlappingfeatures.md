# overlapping features
we have an issue riight now where features are sort of placed independently of one another, without much consideration for their neighbors.

as a consequence, it's difficult to space out elements properly, plus it's really costly to sample

## single "density function"
- think of metaballs as "peaks/valleys" instead of "fill"
  - greens are a peak, fairways fall in the middle, and rough is at the bottom
  - sand features are "carved out" of the terrain (ie negative biases)
  - same with water
- using a singular "territory" function makes marching a lot easier
  - easy to gauge where fairway stops and rough begins
  - v easy to avoid punching unnecessary holes in things, because everything is powered by the same function
  - can "punch holes" as needed, to get sharp edges or to discourage overlap

### building this density function
- start at 0 - all deep rough
- place metaball "mounds" along the fairway to transform it into fairway
  - instead of building around "peaks" our falloff curve should be more subtle - thinking of using a gaussian instead
    - inf range for now - can truncate if we need the optimization
    - follow same approach as we use for fairway currently - march along path and place mounds as needed
- lastly, place sharp mounds to mark our green
    - fast falloff should mean it's pretty isolated
- start placing "holes" for hazards
  - for sand: march off until we reach a safe range, then start punching negative "gaussian holes" in the terrain
    - occasionally: just punch the hole in
  - for water: similar approach
  - all constructive, all bound to the same function

### adding distortion
- worried this will result in "too smooth" of a fairway - how to get around this?
  - insert mini-perturbations to reshape terrain (duy)
