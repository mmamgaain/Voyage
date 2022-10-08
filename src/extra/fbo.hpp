#pragma once
#include <voyage.hpp>

namespace Voyage {
	class FrameBuffer {
		public:
			typedef enum COLOR_ATTACHMENT_TYPE { COLOR_ATTACHMENT_NONE, COLOR_ATTACHMENT_BUFFER, COLOR_ATTACHMENT_TEXTURE } COLOR_ATTACHMENT_TYPE;
			typedef enum DEPTH_ATTACHMENT_TYPE { DEPTH_ATTACHMENT_BUFFER, DEPTH_ATTACHMENT_TEXTURE } DEPTH_ATTACHMENT_TYPE;

			FrameBuffer(const unsigned int& width, const unsigned int& height, const COLOR_ATTACHMENT_TYPE& color_at = COLOR_ATTACHMENT_TEXTURE, const DEPTH_ATTACHMENT_TYPE& depth_at = DEPTH_ATTACHMENT_BUFFER, const unsigned int& num_samples = 1, const unsigned int& num_attachments = 1);

			~FrameBuffer();

			constexpr const bool isMultisampled() const;

			void resolveToFBO(FrameBuffer* fbo = nullptr)  const;

			void resolveToScreen(const unsigned int& attachment = 0) const;

			void bindFrameBuffer() const;

			static void unbindFrameBuffer();

			const unsigned int getColorTexture(const unsigned int& target = 0);

			const unsigned int& getDepthTexture();
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

			inline void setDirty(const unsigned int& flag) const;
	};
}
