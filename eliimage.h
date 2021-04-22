#ifndef ELIIMAGE_H
#define ELIIMAGE_H
#include <QtCore>





struct EliImageHeader
{
	char signature[4];
	qint32 header_length = 0;;
	qint32 offset = 0;
	qint32 reserved = 0;
	qint32 image_width = 0;			// Ширина изображения пикселах
	qint32 image_height = 0;		// Высота изображения в пикселах
	qint32 bit_count = 0;			// Количество бит на пиксел
	qint32 line_length = 0;			// Длина одной строки изображения в байтах

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
		m_data(Q_NULLPTR) {}

	EliImage(const EliImageHeader &header)
		 : m_header(header),
		   m_data(Q_NULLPTR) {}

	T **data() const { return m_data;};
	void setData(T** data) { m_data = data; }
	EliImageHeader &header() { return m_header; };
	const EliImageHeader &getHeader() const { return  m_header; };

private:
	T **m_data;				// дата
	EliImageHeader m_header;
};


#endif // ELIIMAGE_H
