#pragma once

#include <Magick++.h>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <exception>
#include <numeric>
#include <algorithm>

using namespace Magick;

int PIXEL_NOISE_THRESHOLD = 10;

class InvalidPixelException : public std::exception {
	virtual const char* msg() const throw(){
		return "Attempted to go out of image bounds for this calculation.";
	}
} InvalidPixelException;

struct ComplexPixelBrightness {
	double avgMiddleBrightness, allPixelBrightness;
	std::vector<ColorHSL> extremePixels;
};

double brightness(ColorHSL pixel) {
	return pixel.lightness();
}

double addPixelBrightness(double a, ColorHSL b) {
	return a + b.lightness();
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
	Calculate the brightness of the pixels surrounding the target pixel (provided by (col, row))
	@return a ComplexPixelBrightness reference with the 9 pixels split into extreme brightness
	pixels, middle brightness pixels, and the average brightness of the middle pixels.
*/
ComplexPixelBrightness surroundingPixelBrightnessAll(Image srcImage, size_t maxCols, size_t maxRows, int col, int row) {
	if (col == 0 || col == maxCols || row == 0 || row == maxRows) {
		throw InvalidPixelException;
	}
	ComplexPixelBrightness cpb;
	std::vector<ColorHSL> allPixels;

	for (int c = col - 1; c <= col + 1; c++) {
		for (int r = row - 1; r <= row + 1; r++) {
			allPixels.push_back((ColorHSL)srcImage.pixelColor(c, r));
		}
	}

	double brightnessSum = std::accumulate(allPixels.begin(), allPixels.end(), 0, addPixelBrightness);

	double bSum = 0;
	for (auto i = allPixels.begin(); i < allPixels.end(); i++) {
		bSum += i->lightness();
	}

	double avgBrightness = brightnessSum / allPixels.size();

	std::sort(allPixels.begin(), allPixels.end(), [](ColorHSL a, ColorHSL b) -> bool {
		return b.lightness() > a.lightness();
	});

	std::vector<ColorHSL> extremePixels;
	
	// get the front two brightnesses (the two minimums)
	ColorHSL tempBrightness = allPixels.front();
	extremePixels.push_back(tempBrightness);
	allPixels.erase(allPixels.begin());

	tempBrightness = allPixels.front();
	extremePixels.push_back(tempBrightness);
	allPixels.erase(allPixels.begin());

	// now get the top two (two maximums)
	tempBrightness = allPixels.back();
	extremePixels.push_back(tempBrightness);
	allPixels.pop_back();

	tempBrightness = allPixels.back();
	extremePixels.push_back(tempBrightness);
	allPixels.pop_back();

	// construct our return object
	cpb.extremePixels = extremePixels;
	cpb.avgMiddleBrightness = std::accumulate(allPixels.begin(), allPixels.end(), 0.0, addPixelBrightness) / allPixels.size();
	cpb.allPixelBrightness = avgBrightness;

	bSum = 0;
	for (auto i = allPixels.begin(); i < allPixels.end(); i++) {
		bSum += i->lightness();
	}

	return cpb;
}

/**
	Return true if the pixel brightness and surrounding brightness exceed the threshold to be
	classified as noisy.
*/
bool isNoisy(double pixelBrightness, double surroundingBrightness) {
	return 255 * abs(pixelBrightness - surroundingBrightness) > PIXEL_NOISE_THRESHOLD;
}

/*
	Change the brightness of the extreme pixels to that of targetBrightness.

	NOTE: This was implemented due to misreading the question 2-2. It's no longer used in the final solution
	but is kept because it's kinda neat.
*/
void overwriteExtremePixels(const Image& srcImage, Image& targetImage, std::vector<ColorHSL> overwriteList, size_t maxCols, size_t maxRows, int col, int row, double targetBrightness) {
	ColorHSL pixel;

	for (int c = col - 1; c <= col + 1; c++) {
		for (int r = row - 1; r <= row + 1; r++) {
			pixel = srcImage.Image::pixelColor(c, r);
			if (std::any_of(overwriteList.begin(), overwriteList.end(), [pixel](ColorHSL a) { return a.lightness() == pixel.lightness(); })) {
				pixel.lightness(targetBrightness);
				targetImage.Image::pixelColor(c, r, pixel);
			}
		}
	}

	return;
}