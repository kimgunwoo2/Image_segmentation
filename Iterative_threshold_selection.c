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
	int sum = 0, m_1, m_2, pixel_num;
	int before_threshold, after_threshold;
	memset(outImg, 0, hInfo.biSizeImage);
	for (int i = 0; i<hInfo.biHeight; i++)
		for (int j = 0; j < hInfo.biWidth; j++) {
			histogram[lpImg[i*rwsize + j]]++;
			sum += lpImg[i*rwsize + j];

		}


	before_threshold = sum / (hInfo.biWidth*hInfo.biHeight);


	while (1) {
		pixel_num = 0;
		m_1 = 0;
		sum = 0;
		for (int i = 0; i < before_threshold; i++) {
			sum += i * histogram[i];
			pixel_num += histogram[i];
		}
		m_1 = sum / pixel_num;

		pixel_num = 0;
		sum = 0;
		m_2 = 0;
		for (int i = before_threshold; i < 256; i++) {
			sum += i * histogram[i];
			pixel_num += histogram[i];
		}
		m_2 = sum / pixel_num;

		after_threshold = (m_1 + m_2) / 2;

		if (before_threshold == after_threshold)
			break;
		else
			before_threshold = after_threshold;
	}
	printf("IterativeThreshold %d\n", after_threshold);
	for (int i = 0; i < hInfo.biHeight; i++) {
		for (int j = 0; j < hInfo.biWidth; j++) {
			if (lpImg[i*rwsize + j] > after_threshold) {
				outImg[i*rwsize + j] = 255;
			}
			else {
				outImg[i*rwsize + j] = 0;
			}
		}
	}

	FILE *outfile = fopen("IterativeThreshold.bmp", "wb");
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
