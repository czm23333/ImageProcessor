// ImageProcessor.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <FreeImage.h>
#pragma comment(lib, "FreeImage.lib")
#include <stdlib.h>
#include <time.h>
#include <functional>
#include <conio.h>


template <size_t arg, size_t... rest>
struct cmax;

template <size_t arg>
struct cmax<arg> : std::integral_constant<size_t, arg> {};

template <size_t arg1, size_t arg2>
struct cmax<arg1, arg2> : std::integral_constant<size_t, arg1 >= arg2 ? arg1 : arg2> {};

template <size_t arg1, size_t arg2, size_t... rest>
struct cmax<arg1, arg2, rest...> : std::integral_constant<size_t, cmax<cmax<arg1, arg2>::value, rest...>::value> {};

#define STATIC_CODE(name, code, arg) static code_block<decltype(arg)> name(code, arg)
template <typename T>
struct code_block { code_block(std::function<void(T)> code, T arg) { code(arg); } };


void Process(FIBITMAP* bitmap, const unsigned seed, std::function<decltype(RGBQUAD().rgbRed)(decltype(RGBQUAD().rgbRed), unsigned)> calculator) {
	srand(seed);

	auto w = FreeImage_GetWidth(bitmap) - 1, h = FreeImage_GetHeight(bitmap) - 1;
	for (unsigned i = 0; i <= w; ++i)
		for (unsigned j = 0; j <= h; ++j) {
			RGBQUAD va;
			unsigned random = rand();
			FreeImage_GetPixelColor(bitmap, i, j, &va);
			va.rgbRed = calculator(va.rgbRed, random);
			va.rgbGreen = calculator(va.rgbGreen, random);
			va.rgbBlue = calculator(va.rgbBlue, random);
			FreeImage_SetPixelColor(bitmap, i, j, &va);
		}
}

FIBITMAP* LoadImage(const char* filename) {
	static decltype(WEBP_LOSSLESS) flagsMap[cmax<FREE_IMAGE_FORMAT::FIF_JPEG>::value + 1] = { 0 };
	STATIC_CODE(sc, [](decltype(flagsMap) map) {
		using ::FREE_IMAGE_FORMAT;
		map[FIF_JPEG] = JPEG_ACCURATE;
	}, flagsMap);

	FREE_IMAGE_FORMAT fileFormat = FreeImage_GetFileType(filename);

	if (fileFormat == FIF_UNKNOWN)
		fileFormat = FreeImage_GetFIFFromFilename(filename);

	if (fileFormat != FIF_UNKNOWN && FreeImage_FIFSupportsReading(fileFormat))
		return FreeImage_Load(fileFormat, filename, flagsMap[fileFormat]);
	else
		return nullptr;
}

void SaveImage(const char* filename, FIBITMAP* bitmap) {
	static decltype(WEBP_LOSSLESS) flagsMap[cmax<FREE_IMAGE_FORMAT::FIF_JPEG, FREE_IMAGE_FORMAT::FIF_WEBP>::value + 1] = { 0 };
	STATIC_CODE(sc, [](decltype(flagsMap) map) {
		using ::FREE_IMAGE_FORMAT;
		map[FIF_JPEG] = JPEG_QUALITYSUPERB | JPEG_SUBSAMPLING_444 | JPEG_OPTIMIZE;
		map[FIF_WEBP] = WEBP_LOSSLESS;
	}, flagsMap);

	FREE_IMAGE_FORMAT fileFormat = FreeImage_GetFIFFromFilename(filename);
	FreeImage_Save(fileFormat, bitmap, filename, flagsMap[fileFormat]);
}

std::pair<std::function<void(FIBITMAP*, const unsigned)>, std::function<void(FIBITMAP*)>> BindModules(const char* filename, const unsigned method) {

	if (method == 1)
		return std::make_pair(std::bind(Process, std::placeholders::_1, std::placeholders::_2, [](decltype(RGBQUAD().rgbRed) org, unsigned r) { return org + r; }), std::bind(SaveImage, filename, std::placeholders::_1));
	else if(method == 0)
		return std::make_pair(std::bind(Process, std::placeholders::_1, std::placeholders::_2, [](decltype(RGBQUAD().rgbRed) org, unsigned r) { return org - r; }), std::bind(SaveImage, filename, std::placeholders::_1));
	else
		return std::make_pair(std::function<void(FIBITMAP*, const unsigned)>(), std::function<void(FIBITMAP*)>());
}

int main(int argc, char* argv[])
{
	printf("Image encrypt/decrypt tool by czm23333\n\n");

	if (argc != 2)
		return -1;
	char* filename = argv[1];

	FreeImage_Initialise(0);

	printf("Loading image...\n");
	FIBITMAP* bitmap = LoadImage(filename);
	if (bitmap == nullptr)
		return -1;

	printf("Choose the method to use [add(1) or sub(0)]: ");
	unsigned method;
	scanf_s("%u", &method);

	printf("Enter your seed: ");
	unsigned seed;
	scanf_s("%u", &seed);

	auto modules = BindModules(filename, method);
	clock_t clockStart = clock();
	modules.first(bitmap, seed);
	printf("Operation finished in %d ms.\n", clock() - clockStart);
	printf("Saving image...\n");
	modules.second(bitmap);
	printf("Image has been saved.\n");

	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();

	printf("Press any key to continue...");
	_getch();
    return 0;
}