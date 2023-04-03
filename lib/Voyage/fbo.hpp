#pragma once
#include <voyage.hpp>

namespace Voyage {
	/* An implementation of FrameBuffers in OpenGL. */
	class FrameBuffer {
		public:
			typedef enum COLOR_ATTACHMENT_TYPE { COLOR_ATTACHMENT_NONE, COLOR_ATTACHMENT_BUFFER, COLOR_ATTACHMENT_TEXTURE } COLOR_ATTACHMENT_TYPE;
			typedef enum DEPTH_ATTACHMENT_TYPE { DEPTH_ATTACHMENT_BUFFER, DEPTH_ATTACHMENT_TEXTURE } DEPTH_ATTACHMENT_TYPE;

			/* Creates a FrameBuffer.
			 * width - Width of the FrameBuffer.
			 * height - Height of the FrameBuffer.
			 * color_at - The attachment type for the color attachment.
			 * depth_at - The attachment type for the depth attachment.
			 * num_samples - The number of samples per pixel being used for this FrameBuffer. Default (and minimum) value is 1 and is considered non-multisampled.
			 * num_attachments - The number of color attachments to be used in this FrameBuffer. Default (and minimum) value is 1. */
			FrameBuffer(const uint32_t& width, const uint32_t& height, const COLOR_ATTACHMENT_TYPE& color_at = COLOR_ATTACHMENT_TEXTURE, const DEPTH_ATTACHMENT_TYPE& depth_at = DEPTH_ATTACHMENT_BUFFER, const uint32_t& num_samples = 1, const uint32_t& num_attachments = 1);

			FrameBuffer(const FrameBuffer& fbo) noexcept;

			FrameBuffer(FrameBuffer&& fbo) noexcept;

			FrameBuffer& operator=(const FrameBuffer& other) noexcept;

			FrameBuffer& operator=(FrameBuffer&& other) noexcept;

			~FrameBuffer();

			/* Checks if this FrameBuffer is multisampled, i.e., does this  */
			constexpr const bool isMultisampled() const;

			void resolveToFBO(FrameBuffer* fbo = nullptr)  const;

			void resolveToScreen(const uint32_t& attachment = 0) const;

			void bindFrameBuffer() const;

			static void unbindFrameBuffer();

			const uint32_t getColorTexture(const uint32_t& target = 0);

			const uint32_t getDepthTexture();

			void dispose();
		private:
			uint32_t id, width, height, samples, attachments, depthTexture, depthBuffer;
			uint32_t *colorTexture, *colorBuffer;
			bool *dirty;
			FrameBuffer *reserve;
			static int MAX_TARGETS, MAX_SAMPLES;

			void createFrameBuffer();

			void init();

			void createColorTextureAttachments();

			void createColorBufferAttachments();

			void createDepthTextureAttachment();

			void createDepthBufferAttachment();

			const uint32_t& resolveToTexture(const uint32_t& attachment = 0);

			const uint32_t* resolveToTextures();

			bool isDirty() const;

			bool isAllDirty() const;

			inline void setDirty(const bool& flag) const;
	};
}
