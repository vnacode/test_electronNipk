#ifndef ELIIMAGE_H
#define ELIIMAGE_H
#include <QtCore>

template <typename T>
class EliImage
{
public:
	EliImage()
		: m_image_width(0),
		  m_image_height(0),
		  m_bit_count(0),
		  m_line_length(0),
		  m_data(Q_NULLPTR) {}

	bool load(const QString fileName);
	uint getImage_width() const { return  m_image_width; };
	uint getImage_height() const { return  m_image_height; };
	uint getBit_count() const { return  m_bit_count; };

	void setImage_width(const uint &image_width) { m_image_width = image_width; };
	void setImage_height(const uint &image_height) { m_image_height = image_height; };
	void setBit_count(const uint &bit_count) { m_bit_count = bit_count; };

	T *data() { return m_data;};

	uint getLine_length() const { return  m_line_length; };
	void setLine_length(const uint &line_length) { m_line_length = line_length; };

private:
	uint m_image_width;		// Ширина изображения пикселах
	uint m_image_height;	// Высота изображения в пикселах
	uint m_bit_count;		// Количество бит на пиксел
	uint m_line_length;		// Длина одной строки изображения в байтах
	T *m_data;				// дата
};

#endif // ELIIMAGE_H
