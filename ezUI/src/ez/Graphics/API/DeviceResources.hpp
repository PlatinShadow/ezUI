#pragma once
#include "ez/Core/StrongHandle.hpp"

#include <cstdint>
#include <string>
#include <glm/glm.hpp>

namespace ez::gfx {
	enum class DrawMode {
		TRIANGLES = 0,
		TRIANGLE_STRIP
	};

	enum class Format {
		RGB = 0,
		RGBA,

		DEPTH_COMPONENT16,
		DEPTH_COMPONENT24,
		DEPTH_COMPONENT32
	};

	enum class Filter {
		NEAREST = 0,
		LINEAR
	};

	EZ_MAKE_STRONG_HANDLE(Layer);
	class TextureArray {
	public:
		virtual ~TextureArray() { }
		virtual void Bind() = 0;
		virtual void BindToSlot(uint32_t slot) = 0;
		virtual Layer PushBack(void* data) = 0;
		virtual void Erase(Layer layer) = 0;
		virtual uint32_t GetMaxLayers() = 0;
	};

	class Shader {
	public:
		enum class Type {
			VERTEX = 0,
			FRAGMENT
		};

		virtual ~Shader() { }
		virtual void Bind() = 0;
		virtual void Set(const std::string& location, const glm::mat4& matrix) = 0;
		virtual void Set(const std::string& location, float value) = 0;
		virtual void Set(const std::string& location, int value) = 0;
	};

	class StreamStorage {
	public:
		virtual ~StreamStorage() {}
		virtual void PushBack(void* data) = 0;

		virtual uint32_t Count() = 0;
		virtual uint32_t Offset() = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
	};
}