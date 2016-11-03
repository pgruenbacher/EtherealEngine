#pragma once

#include "../Assets/AssetHandle.h"
#include "Graphics/graphics.h"
#include <vector>
#include <unordered_map>
#include <limits>

struct FrameBuffer;
struct Texture;
struct Shader;
struct Uniform;

struct Program
{
	Program() {}
	Program(AssetHandle<Shader> computeShader);
	Program(AssetHandle<Shader> vertexShader, AssetHandle<Shader> fragmentShader);
	//-----------------------------------------------------------------------------
	//  Name : ~Program ()
	/// <summary>
	/// 
	/// 
	/// 
	/// </summary>
	//-----------------------------------------------------------------------------
	~Program();

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
	//  Name : setTexture ()
	/// <summary>
	/// 
	/// 
	/// 
	/// </summary>
	//-----------------------------------------------------------------------------
	void setTexture(std::uint8_t _stage
		, const std::string& _sampler
		, FrameBuffer* _handle
		, uint8_t _attachment = 0
		, std::uint32_t _flags = std::numeric_limits<std::uint32_t>::max());
	void setTexture(std::uint8_t _stage
		, const std::string& _sampler
		, gfx::FrameBufferHandle _handle
		, uint8_t _attachment = 0
		, std::uint32_t _flags = std::numeric_limits<std::uint32_t>::max());

	//-----------------------------------------------------------------------------
	//  Name : setTexture ()
	/// <summary>
	/// 
	/// 
	/// 
	/// </summary>
	//-----------------------------------------------------------------------------
	void setTexture(std::uint8_t _stage
		, const std::string& _sampler
		, Texture* _texture
		, std::uint32_t _flags = std::numeric_limits<std::uint32_t>::max());
	void setTexture(std::uint8_t _stage
		, const std::string& _sampler
		, gfx::TextureHandle _texture
		, std::uint32_t _flags = std::numeric_limits<std::uint32_t>::max());
	//-----------------------------------------------------------------------------
	//  Name : setUniform ()
	/// <summary>
	/// 
	/// 
	/// 
	/// </summary>
	//-----------------------------------------------------------------------------
	void setUniform(const std::string& _name, const void* _value, std::uint16_t _num = 1);

	//-----------------------------------------------------------------------------
	//  Name : getUniform ()
	/// <summary>
	/// 
	/// 
	/// 
	/// </summary>
	//-----------------------------------------------------------------------------
	std::shared_ptr<Uniform> getUniform(const std::string& _name);

	//-----------------------------------------------------------------------------
	//  Name : addShader ()
	/// <summary>
	/// 
	/// 
	/// 
	/// </summary>
	//-----------------------------------------------------------------------------
	void addShader(AssetHandle<Shader> shader);
	void populate();

	gfx::ProgramHandle handle = { gfx::invalidHandle };
	std::vector<AssetHandle<Shader>> shaders;
	std::unordered_map<std::string, std::shared_ptr<Uniform>> uniforms;
};