#ifndef ELIIMAGEHANDLER_H
#define ELIIMAGEHANDLER_H
#include "eliimage.h"
#include "string"

class EliImageHandler
{
public:
	EliImageHandler(){};
	bool readHeader(const std::string &fileName, EliImageHeader &handler);
	bool read16bitEli(const std::string &fileName, EliImage<uint16_t> &output);
	bool write16bitEli(const EliImage<uint16_t> &img, const std::string &fileName);
};

#endif // ELIIMAGEHANDLER_H
