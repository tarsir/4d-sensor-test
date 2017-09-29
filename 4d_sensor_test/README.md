# Programming Test

### Question 1-1:

Solution in `Main::questionOneOne()` function.

### Question 1-2:

Solution in `Main::questionOneTwo()` function.

### Question 2-1:

Solution in `Main::questionTwoOne()` function.

### Question 2-2:

Solution in `Main::questionTwoTwo()` function.

### Question 2-3:

The first method of image denoising is somewhat imprecise as it only uses a small subset of the 
available information to calculate noise information for a pixel, but as it's checking fewer 
pixels it will run comparatively fast. The resulting denoised image still retains some noise
while still being a marked improvement over the original.

The second method of denoising uses more information (all pixels in a 1-px radius of the target
pixel, including the target pixel itself) and so will be more likely to identify pixels that have
noise from diagonals as well as the directly adjacent pixels (up, left, right, down). It certainly
provides better results on the provided sample image as well, but it also runs a bit over twice as
long due to comparing more pixels as well as computing two independent averages and doing this for
every pixel in the image.