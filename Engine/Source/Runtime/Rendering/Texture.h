#pragma once

#include "Graphics/graphics.h"
#include <memory>

#include "Core/reflection/reflection.h"
struct ITexture
{
	REFLECTABLE(ITexture)

	virtual ~ITexture() {}
};


struct Texture : public ITexture
{
	REFLECTABLE(Texture, ITexture)
	Texture()
	{
	}
	Texture(const gfx::Memory* _mem
		, std::uint32_t _flags = BGFX_TEXTURE_NONE
		, std::uint8_t _skip = 0
		, gfx::TextureInfo* _info = nullptr)
	{
		populate(_mem, _flags, _skip, _info);
	}

	//-----------------------------------------------------------------------------
	//  Name : populate ()
	/// <summary>
	/// 
	/// 
	/// 
	/// </summary>
	//-----------------------------------------------------------------------------
	Texture(std::uint16_t _width
		, std::uint16_t _height
		, bool     _hasMips
		, std::uint16_t _numLayers
		, gfx::TextureFormat::Enum _format
		, std::uint32_t _flags = BGFX_TEXTURE_NONE
		, const gfx::Memory* _mem = nullptr
	)
	{
		populate(_width, _height, _hasMips, _numLayers, _format, _flags, _mem);
	}

	//-----------------------------------------------------------------------------
	//  Name : populate ()
	/// <summary>
	/// 
	/// 
	/// 
	/// </summary>
	//-----------------------------------------------------------------------------
	Texture(gfx::BackbufferRatio::Enum _ratio
		, bool _hasMips
		, std::uint16_t _numLayers
		, gfx::TextureFormat::Enum _format
		, std::uint32_t _flags = BGFX_TEXTURE_NONE
	)
	{
		populate(_ratio, _hasMips, _numLayers, _format, _flags);
	}

	//-----------------------------------------------------------------------------
	//  Name : populate ()
	/// <summary>
	/// 
	/// 
	/// 
	/// </summary>
	//-----------------------------------------------------------------------------
	Texture(std::uint16_t _width
		, std::uint16_t _height
		, std::uint16_t _depth
		, bool _hasMips
		, gfx::TextureFormat::Enum _format
		, std::uint32_t _flags = BGFX_TEXTURE_NONE
		, const gfx::Memory* _mem = nullptr
	)
	{
		populate(_width, _height, _depth, _hasMips, _format, _flags, _mem);
	}


	Texture(std::uint16_t _size
		, bool _hasMips
		, std::uint16_t _numLayers
		, gfx::TextureFormat::Enum _format
		, std::uint32_t _flags = BGFX_TEXTURE_NONE
		, const gfx::Memory* _mem = nullptr
	)
	{
		populate(_size, _hasMips, _numLayers, _format, _flags, _mem);
	}
	//-----------------------------------------------------------------------------
	//  Name : ~Texture ()
	/// <summary>
	/// 
	/// 
	/// 
	/// </summary>
	//-----------------------------------------------------------------------------
	~Texture();

	//-----------------------------------------------------------------------------
	//  Name : isValid ()
	/// <summary>
	/// 
	/// 
	/// 
	/// </summary>
	//-----------------------------------------------------------------------------
	bool isValid() const;

	//-----------------------------------------------------------------------------
	//  Name : dispose ()
	/// <summary>
	/// 
	/// 
	/// 
	/// </summary>
	//-----------------------------------------------------------------------------
	void dispose();

	//-----------------------------------------------------------------------------
	//  Name : populate ()
	/// <summary>
	/// 
	/// 
	/// 
	/// </summary>
	//-----------------------------------------------------------------------------
	void populate(const gfx::Memory* _mem
		, std::uint32_t _flags = BGFX_TEXTURE_NONE
		, std::uint8_t _skip = 0
		, gfx::TextureInfo* _info = nullptr);

	//-----------------------------------------------------------------------------
	//  Name : populate ()
	/// <summary>
	/// 
	/// 
	/// 
	/// </summary>
	//-----------------------------------------------------------------------------
	void populate(
		std::uint16_t _width
		, std::uint16_t _height
		, bool     _hasMips
		, std::uint16_t _numLayers
		, gfx::TextureFormat::Enum _format
		, std::uint32_t _flags = BGFX_TEXTURE_NONE
		, const gfx::Memory* _mem = nullptr
	);

	//-----------------------------------------------------------------------------
	//  Name : populate ()
	/// <summary>
	/// 
	/// 
	/// 
	/// </summary>
	//-----------------------------------------------------------------------------
	void populate(
		gfx::BackbufferRatio::Enum _ratio
		, bool _hasMips
		, std::uint16_t _numLayers
		, gfx::TextureFormat::Enum _format
		, std::uint32_t _flags = BGFX_TEXTURE_NONE
	);

	void populate(
		std::uint16_t _width
		, std::uint16_t _height
		, std::uint16_t _depth
		, bool _hasMips
		, gfx::TextureFormat::Enum _format
		, std::uint32_t _flags = BGFX_TEXTURE_NONE
		, const gfx::Memory* _mem = nullptr
	);

	void populate(
		std::uint16_t _size
		, bool _hasMips
		, std::uint16_t _numLayers
		, gfx::TextureFormat::Enum _format
		, std::uint32_t _flags = BGFX_TEXTURE_NONE
		, const gfx::Memory* _mem = nullptr
	);

	gfx::TextureHandle handle = { gfx::invalidHandle };
	gfx::TextureInfo info;
	std::uint32_t flags = BGFX_TEXTURE_NONE;
	gfx::BackbufferRatio::Enum ratio = gfx::BackbufferRatio::Count;
};