RayTra
===

### Todo
- [ ] scene file read (we provide a parser outline for you!)
- [ ] camera & image setup
- [ ] primary ray generation
- [ ] ray/sphere intersection (note: there may be multiple spheres)
- [ ] image write (no shading, lights etc.)

Your raytra should read in the scene file, build the camera model and set up the image, and
generate rays - 1 for each pixel. For any pixel, if there is a ray-sphere intersection, the color
for that pixel should be the diffuse RGB color of any sphere that is hit. If there is no ray ray-sphere
intersection, the color of that pixel should be black. After rendering all the pixels, the
OpenEXR image file should be saved and the program should exit.

raytra takes two arguments - the name of a scene file to read, and the name of the image file
to be written. For this part of the assignment, **only the camera command and the sphere command need
to be working** (from the scene file format).

