#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <string.h>
#define WIDTHBYTES(bits) (((bits)+31)/32*4)
#define BYTE    unsigned char






void bmpBWrw()
{
	int histogram[256] = { 0, };
	FILE *infile;
	if ((infile = fopen("hand.bmp", "rb")) == NULL) {
		printf("No Image File g");
		return;
	}
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile);
	if (hf.bfType != 0x4D42) exit(1);


	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile);
	if (hInfo.biBitCount != 8) { printf("Bad File Format!!"); return; }
	RGBQUAD hRGB[256];


	fread(hRGB, sizeof(RGBQUAD), 256, infile);
	BYTE *lpImg = malloc(hInfo.biSizeImage);
	fread(lpImg, sizeof(char), hInfo.biSizeImage, infile);
	fclose(infile);
	BYTE *outImg = malloc(hInfo.biSizeImage);
	int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);
	int total_pixel = hInfo.biHeight * hInfo.biWidth;
	float prod1_before, prod1_now, prod2_now, variance;
	float mean1_before, mean_total = 0, mean1_now, mean2_now, max = 0;
	int before_threshold, threshold;
	memset(outImg, 0, hInfo.biSizeImage);
	for (int i = 0; i<hInfo.biHeight; i++)
		for (int j = 0; j < hInfo.biWidth; j++) {
			histogram[lpImg[i*rwsize + j]]++;
		}

	prod1_before = histogram[0] / (float)total_pixel;
	mean1_before = 0;

	for (int i = 0; i < 256; i++) {
		mean_total += i * histogram[i];
	}

	mean_total = mean_total / (float)total_pixel;

	before_threshold = 1;
	while (before_threshold < 256) {

		prod1_now = prod1_before + histogram[before_threshold] / (float)(total_pixel);
		prod2_now = 1 - prod1_now;
		if (prod1_now == 0)
			mean1_now = 0;
		else
			mean1_now = (prod1_before * mean1_before + before_threshold * (histogram[before_threshold] / (float)(total_pixel))) / prod1_now;
		if (prod1_now == 1)
			mean2_now = 0;
		else
			mean2_now = (mean_total - prod1_now * mean1_now) / (1 - prod1_now);

		variance = prod1_now * prod2_now * (mean1_now - mean2_now) * (mean1_now - mean2_now);

		if (max < variance) {
			max = variance;
			threshold = before_threshold;

		}
		prod1_before = prod1_now;
		mean1_before = mean1_now;
		before_threshold++;
	}
	printf("OTSUThreshold %d\n", threshold);
	for (int i = 0; i < hInfo.biHeight; i++) {
		for (int j = 0; j < hInfo.biWidth; j++) {
			if (lpImg[i*rwsize + j] < threshold)
				outImg[i*rwsize + j] = 0;
			else
				outImg[i*rwsize + j] = 255;
		}
	}

	FILE *outfile = fopen("OTSUThreshold.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile);
	fwrite(outImg, sizeof(char), hInfo.biSizeImage, outfile);
	fclose(outfile);
	free(lpImg);
	return;

}

void main()
{

	bmpBWrw();
}
