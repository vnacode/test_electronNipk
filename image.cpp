#include "image.h"

EliImage::ELI readEli(std::ifstream& file)
{
	EliImage::ELI header;

	file.read((char*)(&header.header_length), sizeof header.header_length);
	file.read((char*)(&header.offset), sizeof header.offset);
	file.read((char*)(&header.reserved), sizeof header.reserved);
	file.read((char*)(&header.image_width), sizeof header.image_width);
	file.read((char*)(&header.image_height), sizeof header.image_height);
	file.read((char*)(&header.bit_count), sizeof header.bit_count);
	file.read((char*)(&header.line_length), sizeof header.line_length);

	return header;
}

std::string getExtension(const std::string& name)
{
	auto pos = name.find_last_of(".");
	return name.substr(pos + 1);
}

void EliImage::Image::setData(byte_t* data)
{
	delete m_data;
	m_data = data;
}

size_t EliImage::Image::getSize() const
{
	if (bit_rate > 0)
		return m_width * m_height * (bit_rate / 8);
	return 0;
}

uint16_t EliImage::Image::getPixel(int x, int y) const
{
	if (!isValid(x,y))
		return 0;
	return *(uint16_t*)&m_data[x + y * m_width];
}

void EliImage::Image::setPixel(int x, int y, const uint16_t& val)
{
	if (!isValid(x,y)) 
		return;
	memcpy(&m_data[x + y * m_width], &val, sizeof val);
}

bool EliImage::Image::isValid(int x, int y) const
{
	return !(x < 0 || x > m_width && y < 0 || y > m_height);
}

EliImage::Image EliImage::create(const std::string& path)
{
	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (!file.is_open())
	{
		std::cout << "Cant open file: " << path << std::endl;
		return Image();
	}

	char signature[4];
	file.read(signature, 4);

	if (std::string(signature).compare("ELI") != 0) return Image();

	EliImage::ELI header = readEli(file);
	if ((header.offset % header.dataOffset != 0) || header.image_width <= 0 || header.image_height <= 0)
	{
		std::cout << "Bad header: " << path << std::endl;
		return Image();
	}

	file.seekg(0, std::ios::end);
	size_t endfile = file.tellg();

	Image im(header.image_width, header.image_height, static_cast<EliImage::Format>(header.bit_count));

	byte_t* imgData;
	size_t byteSize = endfile - header.dataOffset;
	imgData = new byte_t[byteSize];

	file.seekg(header.dataOffset);
	file.read((char*)(&imgData[0]), byteSize);

	im.setData(imgData);

	file.close();
	return im;
}

bool EliImage::save(const Image& src, const std::string& filename)
{
	std::ofstream file(filename, std::ios::out | std::ios::binary);
	if (!file)
	{
		std::cout << "Cant open file: " << filename << std::endl;
		return false;
	}
	EliImage::ELI header;
	header.bit_count = src.getRate();
	header.image_height = src.Height();
	header.image_width = src.Width();
	header.line_length = header.image_width * (header.bit_count / 8);

	file.write(header.signature, sizeof header.signature);
	file.write(reinterpret_cast<const char*>(&header.header_length), sizeof header.header_length);
	file.write(reinterpret_cast<const char*>(&header.offset), sizeof header.offset);
	file.write(reinterpret_cast<const char*>(&header.reserved), sizeof header.reserved);
	file.write(reinterpret_cast<const char*>(&header.image_width), sizeof header.image_width);
	file.write(reinterpret_cast<const char*>(&header.image_height), sizeof header.image_height);
	file.write(reinterpret_cast<const char*>(&header.bit_count), sizeof header.bit_count);
	file.write(reinterpret_cast<const char*>(&header.line_length), sizeof header.line_length);

	auto emptysize = header.offset - header.header_length;
	const unsigned char empty = 0x0;
	for (int i = 0; i < emptysize; ++i)
		file << empty;

	auto bytes = src.Height() * src.Width() * (src.getRate() / 8);
	for (int i = 0; i < bytes; ++i)
		file << src.data()[i];

	file.close();
	return true;
}
