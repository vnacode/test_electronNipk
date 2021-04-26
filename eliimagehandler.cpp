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
	std::ifstream inputDs( fileName, std::ios::out | std::ios::binary);
	//	if (!inputDs.is_open())
	//	{
	//		std::cout << "Не удалось открыть файл: " << fileName << std::endl;
	//		return false;
	//	}
	//	if (inputDs.gcount())
	//auto data = file.readLine(dataOffset);
	//	if (data.isEmpty())
	//	{
	//		std::cout << "Файл: " << fileName << " пуст";
	//		return false;
	//	}
	//	QDataStream ds(data);
	//	ds.setByteOrder(QDataStream::LittleEndian);



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

//		char signature[4];
//		inputDs.read(signature,  4);
//		uint32_t header_length = 0;;
//		inputDs >> header_length;
//		uint32_t offset = 0;
//		inputDs >> offset;
//		uint32_t reserved = 0;
//		inputDs >> reserved;
//		uint32_t image_width = 0;
//		inputDs >> image_width;
//		uint32_t image_height = 0;
//		inputDs >> image_height;
//		uint32_t bit_count = 0;
//		inputDs >> bit_count;
//		uint32_t line_length = 0;
//		inputDs >> line_length;

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


	//	auto data = inputDs.read()
	//	QDataStream ds(data);
	//	ds.setByteOrder(QDataStream::LittleEndian);
	//	if (output.header().isEmpty())
	//	{
	//		if (!readHeader(fileName, output.header()))
	//			return false;
	//	}
	//	ds.skipRawData(output.header().offset);
	//	quint16 **imgData;
	//	imgData = new quint16*[output.getHeader().image_width];
	//	for (int y = 0 ; y < output.getHeader().image_width; y++)
	//	{
	//		imgData[y] = new quint16[output.getHeader().image_height];
	//		for (int x = 0; x < output.getHeader().image_height; x++)
	//		{
	//			ds >> imgData[y][x];
	//		}
	//	}
	//	output.setData(imgData);
	return true;
}

bool EliImageHandler::write16bitEli(const EliImage<uint16_t> &img, const std::string &fileName)
{
	std::ofstream outputDs( fileName, std::ios::binary);
	if (!outputDs)
	{
		std::cout << "Не удалось открыть файл: " << fileName << std::endl;;
		return false;
	}
	outputDs.write(img.getHeader().signature, 4);
	outputDs << img.getHeader().header_length;
	outputDs << img.getHeader().offset;
	outputDs << img.getHeader().reserved;
	outputDs << img.getHeader().image_width;
	outputDs << img.getHeader().image_height;
	outputDs << img.getHeader().bit_count;
	outputDs << img.getHeader().line_length;
	const char *empty = "0x0";
	outputDs.write(empty, img.getHeader().offset - img.getHeader().header_length);
	for (int y = 0 ; y < img.getHeader().image_width; y++)
	{
		for (int x = 0; x < img.getHeader().image_height; x++)
		{
			outputDs << img.data()[y][x];
		}
	}


	//	QDataStream ds(&file);
	//	ds.setByteOrder(QDataStream::LittleEndian);
	//	ds.writeRawData(img.getHeader().signature, 4);
	//	ds << img.getHeader().header_length;
	//	ds << img.getHeader().offset;
	//	ds << img.getHeader().reserved;
	//	ds << img.getHeader().image_width;
	//	ds << img.getHeader().image_height;
	//	ds << img.getHeader().bit_count;
	//	ds << img.getHeader().line_length;
	//	const char *empty = "0x0";
	//	ds.writeRawData(empty, img.getHeader().offset - img.getHeader().header_length);
	//	for (int y = 0 ; y < img.getHeader().image_width; y++)
	//	{
	//		for (int x = 0; x < img.getHeader().image_height; x++)
	//		{
	//			ds << img.data()[y][x];
	//		}
	//	}
	//	file.close();
	return true;
}






