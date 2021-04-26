#ifndef ELIIMAGE_H
#pragma once
#include "inttypes.h"

struct EliImageHeader
{
	char signature[4];
	int32_t header_length = 0;;
	int32_t offset = 0;
	int32_t reserved = 0;
	int32_t image_width = 0;			// Ширина изображения пикселах
	int32_t image_height = 0;		// Высота изображения в пикселах
	int32_t bit_count = 0;			// Количество бит на пиксел
	int32_t line_length = 0;			// Длина одной строки изображения в байтах

	auto size() const { return  image_width * image_height; }
	bool isEmpty() {
		return ( image_width == image_height == bit_count == line_length == offset == 0);
	}
};


template <typename T>
class EliImage
{
public:
	EliImage() :
		m_header(),
		m_data(nullptr) {}

	EliImage(const EliImageHeader &header)
		 : m_header(header),
		   m_data(nullptr) {}

	T **data() const { return m_data;};
	void setData(T** data) { m_data = data; }
	EliImageHeader &header() { return m_header; };
	const EliImageHeader &getHeader() const { return  m_header; };

private:
	T **m_data;				// дата
	EliImageHeader m_header;
};


#endif // ELIIMAGE_H
