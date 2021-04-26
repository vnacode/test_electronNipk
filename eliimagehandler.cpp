#include "eliimagehandler.h"
#include "eliimage.h"
#include "iostream"
#include <fstream>
#include <iterator>
#include <algorithm>
#include "string.h"

const int dataOffset = 512;

bool EliImageHandler::readHeader(const std::string &fileName, EliImageHeader &handler)
{
	std::ifstream inputDs( fileName, std::ios::in | std::ios::binary);
	char signature[4];
	inputDs.read(signature,  4);
	uint32_t header_length = 0;;
	inputDs .read((char*)(&header_length), sizeof header_length);
	uint32_t offset = 0;
	inputDs .read((char*)(&offset), sizeof offset);
	uint32_t reserved = 0;
	inputDs .read((char*)(&reserved), sizeof reserved);
	uint32_t image_width = 0;
	inputDs .read((char*)(&image_width), sizeof image_width);
	uint32_t image_height = 0;
	inputDs .read((char*)(&image_height), sizeof image_height);
	uint32_t bit_count = 0;
	inputDs .read((char*)(&bit_count), sizeof bit_count);
	uint32_t line_length = 0;
	inputDs .read((char*)(&line_length), sizeof line_length);

	if ((offset % dataOffset != 0) || image_width <= 0 || image_height <= 0)
	{
		std::cout << "Некорректное изображение: " << fileName << std::endl;
		return false;
	}

	strcpy(handler.signature, signature);
	handler.header_length = header_length;
	handler.offset = offset;
	handler.reserved = reserved;
	handler.image_width = image_width;
	handler.image_height = image_height;
	handler.bit_count = bit_count;
	handler.line_length = line_length;

	return true;
}

bool EliImageHandler::read16bitEli(const std::string &fileName, EliImage<uint16_t> &output)
{
	std::ifstream inputDs( fileName, std::ios::binary);
	if (!inputDs.is_open())
	{
		std::cout << "Не удалось открыть файл: " << fileName << std::endl;;
		return false;
	}
	if (output.header().isEmpty())
	{
		if (!readHeader(fileName, output.header()))
			return false;
	}
	inputDs.seekg(output.header().offset);
	uint16_t **imgData;
	imgData = new uint16_t*[output.getHeader().image_width];
	for (int y = 0 ; y < output.getHeader().image_width; y++)
	{
		imgData[y] = new uint16_t[output.getHeader().image_height];
		for (int x = 0; x < output.getHeader().image_height; x++)
		{
			inputDs.read((char*)(&imgData[y][x]), sizeof uint16_t());
		}
	}
	output.setData(imgData);
	return true;
}

bool EliImageHandler::write16bitEli(const EliImage<uint16_t> &img, const std::string &fileName)
{
	std::ofstream outputDs( fileName, std::ios::out | std::ios::binary);
	if (!outputDs)
	{
		std::cout << "Не удалось открыть файл: " << fileName << std::endl;;
		return false;
	}
	outputDs.write(img.getHeader().signature, 4);
	outputDs.write(reinterpret_cast<const char*>(&img.getHeader().header_length), sizeof img.getHeader().header_length);
	outputDs.write(reinterpret_cast<const char*>(&img.getHeader().offset), sizeof img.getHeader().offset);
	outputDs.write(reinterpret_cast<const char*>(&img.getHeader().reserved), sizeof img.getHeader().reserved);
	outputDs.write(reinterpret_cast<const char*>(&img.getHeader().image_width), sizeof img.getHeader().image_width);
	outputDs.write(reinterpret_cast<const char*>(&img.getHeader().image_height), sizeof img.getHeader().image_height);
	outputDs.write(reinterpret_cast<const char*>(&img.getHeader().bit_count), sizeof img.getHeader().bit_count);
	outputDs.write(reinterpret_cast<const char*>(&img.getHeader().line_length), sizeof img.getHeader().line_length);
	const char *empty = "0x0";
	outputDs.write(empty, img.getHeader().offset - img.getHeader().header_length);
	for (int y = 0 ; y < img.getHeader().image_width; y++)
	{
		for (int x = 0; x < img.getHeader().image_height; x++)
		{
			//std::cout << img.data()[y][x] << std::endl;
			outputDs << img.data()[y][x];
		}
	}


	return true;
}






