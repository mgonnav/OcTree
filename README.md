# Octree
Here we have two implementations of the OcTree.
- [pr-octree](https://github.com/mgonnav/OcTree/tree/master/pr-octree) contains a recursive implementation of a Point Region OcTree.
This one seems to be highly inefficient (unless it is an uncaught bug causing this).
- [region-octree](https://github.com/mgonnav/OcTree/tree/master/region-octree) contains a non-recursive implementation of a Region OcTree.
This one is way more efficient than its recursive counterpart.

## Build
To build this project, run the following commands from the root of the project:
```bash
cmake -B build/ -S .
cd build/
make
```

This will generate `pr_octree` and `region_octree`. If you only want to build one of them, you can specify it using
```bash
make pr_octree
```
or
```bash
make region_octree
```
