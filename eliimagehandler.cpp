#include "eliimagehandler.h"
#include "eliimage.h"

#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <QDataStream>

const int dataOffset = 512;

EliImageHandler::EliImageHandler()
{

}

bool EliImageHandler::readHeader(const QString &fileName, EliImageHeader &handler)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		qWarning() << "Не удалось открыть файл: " << fileName;;
		return false;
	}
	auto data = file.readLine(dataOffset);
	if (data.isEmpty())
	{
		qWarning() << "Файл: " << fileName << " пуст";
		return false;
	}
	QDataStream ds(data);
	ds.setByteOrder(QDataStream::LittleEndian);

	char signature[4];
	ds.readRawData(signature,  4);
	qint32 header_length = 0;;
	ds >> header_length;
	qint32 offset = 0;
	ds >> offset;
	qint32 reserved = 0;
	ds >> reserved;
	qint32 image_width = 0;
	ds >> image_width;
	qint32 image_height = 0;
	ds >> image_height;
	qint32 bit_count = 0;
	ds >> bit_count;
	qint32 line_length = 0;
	ds >> line_length;
	if ((offset % dataOffset != 0) || image_width <= 0 || image_height <= 0)
	{
		qWarning() << "Некорректное изображение: " << fileName;
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

	//	ds.skipRawData(offset - header_length);
	//	quint16 imgData[image_width][image_height];
	//	for (int i = 0 ; i < image_width; i++)
	//	{
	//		for (int j = 0; j < image_height; j++)
	//		{
	//			ds >> imgData[i][j];
	//		}
	//	}
	return true;
}

bool EliImageHandler::read16bitEli(const QString &fileName, EliImage<quint16> &output)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		qWarning() << "Не удалось открыть файл: " << fileName;;
		return false;
	}
	auto data = file.readAll();
	QDataStream ds(data);
	ds.setByteOrder(QDataStream::LittleEndian);
	if (output.header().isEmpty())
	{
		if (!readHeader(fileName, output.header()))
			return false;
	}
	ds.skipRawData(output.header().offset);
	quint16 **imgData;
	imgData = new quint16*[output.getHeader().image_width];
	for (int y = 0 ; y < output.getHeader().image_width; y++)
	{
		imgData[y] = new quint16[output.getHeader().image_height];
		for (int x = 0; x < output.getHeader().image_height; x++)
		{
			ds >> imgData[y][x];
		}
	}
	output.setData(imgData);
	return true;
}

bool EliImageHandler::write16bitEli(const EliImage<quint16> &img, QString fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly))
	{
		qWarning() << "Не удалось открыть файл: " << fileName;;
		return false;
	}
	QDataStream ds(&file);
	ds.setByteOrder(QDataStream::LittleEndian);
	ds.writeRawData(img.getHeader().signature, 4);
	ds << img.getHeader().header_length;
	ds << img.getHeader().offset;
	ds << img.getHeader().reserved;
	ds << img.getHeader().image_width;
	ds << img.getHeader().image_height;
	ds << img.getHeader().bit_count;
	ds << img.getHeader().line_length;
	const char *empty = "0x0";
	ds.writeRawData(empty, img.getHeader().offset - img.getHeader().header_length);
	for (int y = 0 ; y < img.getHeader().image_width; y++)
	{
		for (int x = 0; x < img.getHeader().image_height; x++)
		{
			//qDebug() << y << x;
			ds << img.data()[y][x];
		}
	}
	file.close();
	return true;
}






