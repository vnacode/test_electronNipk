#pragma once
#include <inttypes.h>
#include <string>
#include <fstream>
#include <iostream>
#include <memory>

using byte_t = unsigned char;

namespace EliImage
{
	enum class Format : byte_t
	{
		ELI8 = 8,
		ELI16 = 16,
		ELI32 = 32
	};
	struct ELI
	{
		static const int dataOffset = 512;

		const char signature[4] = "ELI";			// Signature
		const uint32_t header_length = 32;			// Header length in bytes
		uint32_t offset = 512;						// Offset to image
		uint32_t reserved = 0;						// Reserve
		uint32_t image_width = 0;					// Image width in pixels
		uint32_t image_height = 0;					// Image height in pixels
		uint32_t bit_count = 0;						// Image bit rate for pixel
		uint32_t line_length = 0;					// Length of one image line in bytes
	};

class Image
{
public:


	Image(uint32_t width, uint32_t height, Format type = Format::ELI16)
		: m_width(width), m_height(height), bit_rate(static_cast<byte_t>(type)), m_data(nullptr)
	{	}

	Image(const Image& other)
		: Image(other.m_width, other.m_height)
	{
		bit_rate = other.bit_rate;
		if (other.m_data)
		{
			auto newsize = other.getSize();
			m_data = new byte_t[newsize];
			memcpy(m_data, other.m_data, newsize);
		}
	}
	Image(Image&& other) noexcept
	{
		m_width = std::exchange(other.m_width, 0);
		m_height = std::exchange(other.m_height, 0);
		bit_rate = std::exchange(other.bit_rate, 0);
		m_data = other.m_data;
		other.m_data = nullptr;
	}

	Image()
		: Image(0, 0)
	{}
	~Image()
	{
		delete m_data;
	}

	byte_t* data() const { return m_data;};
	void setData(byte_t* data);

	inline int Width() const { return m_width; }
	inline int Height() const { return m_height; }
	inline bool isEmpty() const { return !m_data; }
	inline byte_t getRate() const { return bit_rate; }

	size_t getSize() const;
	uint16_t getPixel(int x, int y) const;
	void setPixel(int x, int y, const uint16_t& val);
private:

	bool isValid(int x, int y) const;
	uint32_t m_width;
	uint32_t m_height;
	byte_t bit_rate = 8;
	byte_t* m_data;
};

Image create(const std::string& path);
bool save(const Image& src, const std::string& filename);
}
