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
			FrameBuffer(const unsigned int& width, const unsigned int& height, const COLOR_ATTACHMENT_TYPE& color_at = COLOR_ATTACHMENT_TEXTURE, const DEPTH_ATTACHMENT_TYPE& depth_at = DEPTH_ATTACHMENT_BUFFER, const unsigned int& num_samples = 1, const unsigned int& num_attachments = 1);

			FrameBuffer(const FrameBuffer& fbo);

			FrameBuffer(FrameBuffer&& fbo);

			~FrameBuffer();

			/* Checks if this FrameBuffer is multisampled, i.e., does this  */
			constexpr const bool isMultisampled() const;

			void resolveToFBO(FrameBuffer* fbo = nullptr)  const;

			void resolveToScreen(const unsigned int& attachment = 0) const;

			void bindFrameBuffer() const;

			static void unbindFrameBuffer();

			const unsigned int getColorTexture(const unsigned int& target = 0);

			const unsigned int getDepthTexture();

			void dispose();
		private:
			unsigned int id, width, height, samples, attachments, depthTexture, depthBuffer;
			unsigned int *colorTexture, *colorBuffer;
			bool *dirty;
			FrameBuffer *reserve;
			static int MAX_TARGETS, MAX_SAMPLES;

			void createFrameBuffer();

			void init();

			void createColorTextureAttachments();

			void createColorBufferAttachments();

			void createDepthTextureAttachment();

			void createDepthBufferAttachment();

			const unsigned int& resolveToTexture(const unsigned int& attachment = 0);

			const unsigned int* resolveToTextures();

			bool isDirty() const;

			bool isAllDirty() const;

			inline void setDirty(const bool& flag) const;
	};
}
