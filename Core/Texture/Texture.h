#pragma once

enum class TextureType
{
	PNG = 0,
	JPG = 1,
};

class Texture
{
public:
	Texture(const char* path, TextureType tType = TextureType::JPG);

	unsigned int ID;
};