# placing fairway

## current approach
- scatter metaballs around course points (to ensure we have safe fairway there)
- scatter metaballs along some paths (to fill in fairway range, occasionally)

### advantages
- really simple

### disadvantages
- really bad performance
  - not helped by the fact that we're making a shitton of virtual calls :/
  