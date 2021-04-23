#ifndef ELIIMAGEHANDLER_H
#define ELIIMAGEHANDLER_H
#include <QString>
#include "eliimage.h"

class EliImageHandler
{
public:
	EliImageHandler(){};
	static bool readHeader(const QString &fileName, EliImageHeader &handler);
	static bool read16bitEli(const QString &fileName, EliImage<quint16> &output);
	static bool write16bitEli(const EliImage<quint16> &img, QString fileName);
};

#endif // ELIIMAGEHANDLER_H
