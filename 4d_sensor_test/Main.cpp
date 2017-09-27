#include <Magick++.h>
#include "PixelAnalysis.h"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using namespace Magick;

void questionOneOne() {

}

void questionOneTwo() {

}

void questionOne() {
	questionOneOne();
	questionOneTwo();
}

void questionTwoOne(Image originalImage, Image& processImage, size_t columns, size_t rows) {
	ColorHSL pixel;
	int change = 0, noChange = 0;
	for (int i = 0; i < columns; i++) {
		for (int j = 0; j < rows; j++) {
			pixel = originalImage.Image::pixelColor(i, j);
			double pBrightness = brightness(pixel), surroundingBrightness = surroundingPixelBrightnessCardinal(originalImage, columns, rows, i, j);
			if (isNoisy(pBrightness, surroundingBrightness)) {
				pixel.lightness(surroundingBrightness);
				processImage.Image::pixelColor(i, j, pixel);
				change++;
			}
			else {
				noChange++;
			}
		}
	}
	cout << "change pixels: " << change << endl;
	cout << "no change pixels: " << noChange;

	processImage.write("denoised_1.png");
}

void questionTwoTwo(Image originalImage, Image& processImage, size_t columns, size_t rows) {
	ColorHSL pixel;

	for (int i = 0; i < columns; i++) {
		for (int j = 0; j < rows; j++) {
			pixel = originalImage.Image::pixelColor(i, j);
		}
	}

	processImage.write("denoised_2.png");
}

void questionTwo() {
	Image original_image("noisy_sample.png");
	Image processed_image(original_image), processed_image_two(original_image);

	size_t columns = original_image.Image::columns();
	size_t rows = original_image.Image::rows();

	questionTwoOne(original_image, processed_image, columns, rows);
	questionTwoTwo(original_image, processed_image_two, columns, rows);
	cout << "Question 2 complete!" << endl;
}

int main(int argc, char** argv) {
	InitializeMagick(*argv);

	questionOne();
	questionTwo();
	cin.get();
	return 0;
}
