#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#define WIDTHBYTES(bits) (((bits)+31)/32*4)
#define BYTE    unsigned char
void cheakpass();
int H;
int W;
BYTE *outImg;
int *lab;
int *nolab;
int rwsize;
int count = 1;
int recount;
int *labelhistogram;

int ismax(int x, int y) {
	if (x > y)
		return x;
	else {
		return y;
	}
}

float logB(float num)
{
	return (float)(log(num) / log(2));
}
void bottom_up() {

	for (int i = H; i > 0; i--) {
		for (int j = W - 1; j > 0; j--) {
			if ((lab[i*rwsize + j] != 0) && (lab[i*rwsize + (j - 1)] != 0) && (lab[i*rwsize + j] != lab[i*rwsize + (j - 1)])) {
				if (lab[i*rwsize + j] < lab[i*rwsize + (j - 1)])
					lab[i*rwsize + (j - 1)] = lab[i*rwsize + j];
				else
					lab[i*rwsize + j] = lab[i*rwsize + (j - 1)];
			}

			if ((lab[i*rwsize + j] != 0) && (lab[(i - 1)*rwsize + (j - 1)] != 0) && (lab[i*rwsize + j] != lab[(i - 1)*rwsize + (j - 1)])) {
				if (lab[i*rwsize + j] < lab[(i - 1)*rwsize + (j - 1)])
					lab[(i - 1)*rwsize + (j - 1)] = lab[i*rwsize + j];
				else
					lab[i*rwsize + j] = lab[(i - 1)*rwsize + (j - 1)];
			}

			if ((lab[i*rwsize + j] != 0) && (lab[(i - 1)*rwsize + j] != 0) && (lab[i*rwsize + j] != lab[(i - 1)*rwsize + j])) {
				if (lab[i*rwsize + j] < lab[(i - 1)*rwsize + j])
					lab[(i - 1)*rwsize + j] = lab[i*rwsize + j];
				else
					lab[i*rwsize + j] = lab[(i - 1)*rwsize + j];
			}

			if ((lab[i*rwsize + j] != 0) && (lab[(i - 1)*rwsize + (j + 1)] != 0) && (lab[i*rwsize + j] != lab[(i - 1)*rwsize + (j + 1)])) {
				if (lab[i*rwsize + j] < lab[(i - 1)*rwsize + (j + 1)])
					lab[(i - 1)*rwsize + (j + 1)] = lab[i*rwsize + j];
				else
					lab[i*rwsize + j] = lab[(i - 1)*rwsize + (j + 1)];
			}
		}
	}
}

void labeling() {
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			if (outImg[i*rwsize + j] != 0) {
				lab[i*rwsize + j] = count;
				count++;
			}
		}
	}
	printf("There exist %d non-zero pixels\n", count);

}
void top_down() {

	for (int i = 0; i < H - 1; i++) {
		for (int j = 1; j < W - 1; j++) {

			if ((lab[i*rwsize + j] != 0) && (lab[i*rwsize + (j + 1)] != 0) && (lab[i*rwsize + j] != lab[i*rwsize + (j + 1)])) {
				if (lab[i*rwsize + j] < lab[i*rwsize + (j + 1)])
					lab[i*rwsize + (j + 1)] = lab[i*rwsize + j];
				else
					lab[i*rwsize + j] = lab[i*rwsize + (j + 1)];
			}

			if ((lab[i*rwsize + j] != 0) && (lab[(i + 1)*rwsize + (j + 1)] != 0) && (lab[i*rwsize + j] != lab[(i + 1)*rwsize + (j + 1)])) {
				if (lab[i*rwsize + j] < lab[(i + 1)*rwsize + (j + 1)])
					lab[(i + 1)*rwsize + (j + 1)] = lab[i*rwsize + j];
				else
					lab[i*rwsize + j] = lab[(i + 1)*rwsize + (j + 1)];
			}

			if ((lab[i*rwsize + j] != 0) && (lab[(i + 1)*rwsize + j] != 0) && (lab[i*rwsize + j] != lab[(i + 1)*rwsize + j])) {
				if (lab[i*rwsize + j] < lab[(i + 1)*rwsize + j])
					lab[(i + 1)*rwsize + j] = lab[i*rwsize + j];
				else
					lab[i*rwsize + j] = lab[(i + 1)*rwsize + j];
			}

			if ((lab[i*rwsize + j] != 0) && (lab[(i + 1)*rwsize + (j - 1)] != 0) && (lab[i*rwsize + j] != lab[(i + 1)*rwsize + (j - 1)])) {
				if (lab[i*rwsize + j] < lab[(i + 1)*rwsize + (j - 1)])
					lab[(i + 1)*rwsize + (j - 1)] = lab[i*rwsize + j];
				else
					lab[i*rwsize + j] = lab[(i + 1)*rwsize + (j - 1)];
			}
		}
	}
}
void passing() {
	top_down();

	bottom_up();
	cheakpass();
}
void renumbering() {
	int k = 1;

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {

			if (lab[i*rwsize + j] != 0) {
				nolab[lab[i*rwsize + j]] = 1;
			}
		}
	}

	for (int i = 0; i < count; i++) {
		if (nolab[i] != 0) {
			nolab[i] = k;
			k++;
		}
	}
	recount = k + 1;


	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (lab[i*rwsize + j] != 0) {
				lab[i*rwsize + j] = nolab[lab[i*rwsize + j]];
			}
		}
	}

}
void cheakpass() {
	int a = 0, b = 0, c = 0, d = 0;
	for (int i = 0; i < H - 1; i++) {
		for (int j = 1; j < W - 1; j++) {

			if ((lab[i*rwsize + j] != 0) && (lab[i*rwsize + (j + 1)] != 0) && (lab[i*rwsize + j] != lab[i*rwsize + (j + 1)])) {
				a = 1;
			}

			if ((lab[i*rwsize + j] != 0) && (lab[(i + 1)*rwsize + (j + 1)] != 0) && (lab[i*rwsize + j] != lab[(i + 1)*rwsize + (j + 1)])) {
				b = 1;
			}

			if ((lab[i*rwsize + j] != 0) && (lab[(i + 1)*rwsize + j] != 0) && (lab[i*rwsize + j] != lab[(i + 1)*rwsize + j])) {
				c = 1;
			}

			if ((lab[i*rwsize + j] != 0) && (lab[(i + 1)*rwsize + (j - 1)] != 0) && (lab[i*rwsize + j] != lab[(i + 1)*rwsize + (j - 1)])) {
				d = 1;
			}
		}
	}
	if ((a + b + c + d) != 0) {
		passing();
	}
	else {
		renumbering();

	}
}
void Labelhistogram() {
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			labelhistogram[lab[i*rwsize + j]]++;
		}
	}
}
int maxhistogram() {
	int max = 0;
	int max_num = 0;
	for (int i = 1; i < recount; i++) {

		if (max < labelhistogram[i]) {
			max = labelhistogram[i];
			max_num = i;
		}


	}
	labelhistogram[max_num] = 0;
	return max_num;
}
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

	outImg = malloc(hInfo.biSizeImage);
	memset(outImg, 0, hInfo.biSizeImage);

	lab = malloc(hInfo.biSizeImage * sizeof(int));
	memset(lab, 0, hInfo.biSizeImage * sizeof(int));



	rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);
	H = hInfo.biHeight;
	W = hInfo.biWidth;
	int total_pixel = hInfo.biHeight * hInfo.biWidth;
	int prod1 = 0, prod2 = 0;
	float temp;
	float h1_threshold, h2_threshold, h_threshold, max = 0;
	float a_class, b_class;
	int before_threshold, threshold = 0;

	for (int i = 0; i<hInfo.biHeight; i++)
		for (int j = 0; j < hInfo.biWidth; j++) {
			histogram[lpImg[i*rwsize + j]]++;
		}

	before_threshold = 0;

	while (before_threshold < 256) {
		h1_threshold = 0;
		prod1 = 0;
		a_class = 0;
		for (int i = 0; i < before_threshold; i++) {
			a_class += histogram[i];
		}
		for (int i = 0; i < before_threshold; i++) {
			prod1 += histogram[i];

			if (prod1 == 0)
				temp = 0.0;
			else
				temp = logB(prod1 / (float)total_pixel);
			h1_threshold = h1_threshold + -1 * (prod1 / (float)total_pixel)*temp;

		}

		h2_threshold = 0;
		prod2 = prod1;
		b_class = 0;
		for (int i = before_threshold; i < 256; i++) {
			b_class += histogram[i];
		}

		for (int i = before_threshold; i < 256; i++)
		{
			prod2 += histogram[i];
			if (prod2 == 0)
				temp = 0.0;
			else
				temp = logB(prod2 / (float)total_pixel);
			h2_threshold = h2_threshold + -1 * (prod2 / (float)total_pixel)*temp;
		}

		h_threshold = h1_threshold + h2_threshold;
		if (max < h_threshold) {
			threshold = before_threshold;
			max = h_threshold;
		}
		before_threshold++;
	}

	printf("Threshold %d\n", threshold);
	for (int i = 0; i < hInfo.biHeight; i++) {
		for (int j = 0; j < hInfo.biWidth; j++) {
			if (lpImg[i*rwsize + j] < threshold)
				outImg[i*rwsize + j] = 0;
			else {
				outImg[i*rwsize + j] = 255;
			}
		}
	}
	labeling();


	nolab = malloc(count * sizeof(int));
	memset(nolab, 0, count * sizeof(int));
	int max1, max2;
	passing();
	labelhistogram = malloc(recount * sizeof(int));
	memset(labelhistogram, 0, recount * sizeof(int));
	Labelhistogram();
	max1 = maxhistogram();
	max2 = maxhistogram();



	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (lab[i*rwsize + j] != max1 && lab[i*rwsize + j] != max2) {
				outImg[i*rwsize + j] = 0;
			}
		}
	}

	FILE *outfile = fopen("2_pass.bmp", "wb");
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
