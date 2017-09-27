#pragma once

#include <Magick++.h>
#include <iostream>
#include <cstdlib>

using namespace Magick;

int PIXEL_NOISE_THRESHOLD = 10;

double brightness(ColorHSL pixel) {
	return pixel.lightness();
}

/**
	Calculate the average brightness of the cardinal pixels surrounding a coordinate.

	@param srcImage the original image to ensure no existing changes will affect this calculation
	@return a floating-point average of the brightnesses above, below, left, and right of the
		declared coordinate
*/
double surroundingPixelBrightnessCardinal(Image srcImage, size_t maxCols, size_t maxRows, size_t col, size_t row) {
	double brightnessSum = 0, pixelCount = 0;
	if (col > 0) {
		brightnessSum += brightness((ColorHSL)srcImage.pixelColor(col - 1, row));
		pixelCount++;
	}
	if (row > 0) {
		brightnessSum += brightness((ColorHSL)srcImage.pixelColor(col, row - 1));
		pixelCount++;
	}
	if (col < maxCols) {
		brightnessSum += brightness((ColorHSL)srcImage.pixelColor(col, row - 1));
		pixelCount++;
	}
	if (row < maxRows) {
		brightnessSum += brightness((ColorHSL)srcImage.pixelColor(col, row + 1));
		pixelCount++;
	}
	return brightnessSum/pixelCount;
}

/**
	Return true if the pixel brightness and surrounding brightness exceed the threshold to be
	classified as noisy.
*/
bool isNoisy(double pixelBrightness, double surroundingBrightness) {
	return 255 * abs(pixelBrightness - surroundingBrightness) > PIXEL_NOISE_THRESHOLD;
}