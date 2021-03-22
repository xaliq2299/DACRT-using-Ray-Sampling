# DACRT
- [X] construct_AABB method
- [X] Binning found based on different axis per iteration? - Yes
- [X] Remove warnings
- [X] Fix todos
- [X] For each ray: x and y
- [X] Remove unnecessary debugging codes
- [ ] Should I construct the AABB in Main?
- [ ] Values for some consts: thresholds, cost consts, etc.
- [ ] Check the image dimension and macro definitions

- [ ] Try to adapt AABB to your version, change intersect implementation
- [ ] sentinal, we should focus on changing the COST while traversing (cost>0.5 problem)
- [ ] Later fix the argvs

Binning is done based on the longest axis for each iteration. If the barycenter is the same as the axis, we take either box

