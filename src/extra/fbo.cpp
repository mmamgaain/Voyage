#include "Voyage/fbo.hpp"
#include "Voyage/core.hpp"

namespace Voyage {

	int FrameBuffer::MAX_TARGETS = 0;
	int FrameBuffer::MAX_SAMPLES = 0;

	FrameBuffer::FrameBuffer(const unsigned int& width, const unsigned int& height, const COLOR_ATTACHMENT_TYPE& color_at, const DEPTH_ATTACHMENT_TYPE& depth_at, const unsigned int& num_samples, const unsigned int& num_attachments): id(0), width(width), height(height), samples(std::max<unsigned int>(num_samples, 1)), attachments(std::max<unsigned int>(num_attachments, 1)), depthTexture(0), depthBuffer(0), colorTexture(nullptr), colorBuffer(nullptr), dirty(nullptr), reserve(nullptr) {
		init();
		createFrameBuffer();
		if(color_at == COLOR_ATTACHMENT_NONE) glDrawBuffer(GL_NONE);
		else if(isMultisampled() || color_at == COLOR_ATTACHMENT_BUFFER) {
			dirty = new bool[attachments];
			if(isMultisampled()) {
				createColorBufferAttachments();
				createDepthBufferAttachment();
				reserve = new FrameBuffer(width, height, COLOR_ATTACHMENT_TEXTURE, depth_at, 1, attachments);
			}
			else {
				createColorTextureAttachments();
				if(depth_at == DEPTH_ATTACHMENT_TEXTURE) createDepthTextureAttachment();
				else createDepthBufferAttachment();
				reserve = nullptr;
			}
		}
		else {
			createColorTextureAttachments();
			if(depth_at == DEPTH_ATTACHMENT_TEXTURE) createDepthTextureAttachment();
			else createDepthBufferAttachment();
			reserve = nullptr;
		}
		unbindFrameBuffer();
	}

	FrameBuffer::~FrameBuffer() { dispose(); }

	void FrameBuffer::dispose() {
		if(colorTexture) {
			glDeleteTextures(attachments, colorTexture);
			delete[] colorTexture;
			colorTexture = nullptr;
		}
		else if(colorBuffer) {
			glDeleteBuffers(attachments, colorBuffer);
			delete[] colorBuffer;
			colorBuffer = nullptr;
		}
		if(depthTexture) glDeleteTextures(1, &depthTexture);
		else if(depthBuffer) glDeleteBuffers(1, &depthBuffer);
		if(dirty) {
			delete[] dirty;
			dirty = nullptr;
		}
		if(reserve) {
			delete reserve;
			reserve = nullptr;
		}
		if(id) glDeleteFramebuffers(1, &id);
	}

	FrameBuffer::FrameBuffer(const FrameBuffer& fbo) {
		if(this == &fbo) return;
		dispose();

		id = fbo.id;
		width = fbo.width;
		height = fbo.height;
		samples = fbo.samples;
		attachments = fbo.attachments;
		depthTexture = fbo.depthTexture;
		depthBuffer = fbo.depthBuffer;
		colorTexture = fbo.colorTexture;
		colorBuffer = fbo.colorBuffer;
		dirty = fbo.dirty;
		reserve = fbo.reserve;
	}

	FrameBuffer::FrameBuffer(FrameBuffer&& fbo) {
		dispose();

		id = fbo.id;
		width = fbo.width;
		height = fbo.height;
		samples = fbo.samples;
		attachments = fbo.attachments;
		depthTexture = fbo.depthTexture;
		depthBuffer = fbo.depthBuffer;
		colorTexture = fbo.colorTexture;
		colorBuffer = fbo.colorBuffer;
		dirty = fbo.dirty;
		reserve = fbo.reserve;

		fbo.id = 0;
		fbo.depthBuffer = 0;
		fbo.depthTexture = 0;
		fbo.colorBuffer = nullptr;
		fbo.colorTexture = nullptr;
		fbo.dirty = nullptr;
		fbo.reserve = nullptr;
	}

	constexpr const bool FrameBuffer::isMultisampled() const { return samples != 1; }

	void FrameBuffer::resolveToFBO(FrameBuffer* fbo) const {
		if(isMultisampled()) {
			if(!fbo) fbo = reserve;
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->id);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
			for(unsigned int i = 0; i < attachments; i++) {
				glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
				glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
				glBlitFramebuffer(0, 0, width, height, 0, 0, fbo->width, fbo->height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_LINEAR);
			}
			unbindFrameBuffer();
		}
	}

	void FrameBuffer::resolveToScreen(const unsigned int& attachment) const {
		if(isMultisampled()) {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
			glReadBuffer(GL_COLOR_ATTACHMENT0 + (attachments > attachment ? attachment : 0));
			glDrawBuffer(GL_BACK);
			glBlitFramebuffer(0, 0, width, height, 0, 0, Core::width, Core::height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
			unbindFrameBuffer();
		}
	}

	void FrameBuffer::bindFrameBuffer() const {
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if(dirty) setDirty(true);
	}

	void FrameBuffer::unbindFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	const unsigned int FrameBuffer::getColorTexture(const unsigned int& target) {
		if(target < attachments) {
			if(isMultisampled()) return resolveToTexture(target);
			else if(colorTexture) return colorTexture[target];
		}
		return 0;
	}

	const unsigned int FrameBuffer::getDepthTexture() {
		if(depthTexture) return depthTexture;
		else if(isMultisampled()) { if(isAllDirty()) resolveToTexture(); return reserve->depthTexture; }
		return 0;
	}

	void FrameBuffer::createFrameBuffer() {
		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		for(unsigned int i = 0; i < attachments; i++) glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
	}

	void FrameBuffer::init() {
		if(!MAX_TARGETS) glGetIntegerv(GL_MAX_DRAW_BUFFERS, &MAX_TARGETS);
		if(!MAX_SAMPLES) glGetIntegerv(GL_MAX_INTEGER_SAMPLES, &MAX_SAMPLES);
		attachments = std::min<unsigned int>(attachments, MAX_TARGETS);
		samples = std::min<unsigned int>(samples, MAX_SAMPLES);
	}

	void FrameBuffer::createColorTextureAttachments() {
		colorTexture = new unsigned int[attachments];
		glGenTextures(attachments, colorTexture);
		for(unsigned int i = 0; i < attachments; i++) {
			glBindTexture(GL_TEXTURE_2D, colorTexture[i]);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorTexture[i], 0);
		}
	}

	void FrameBuffer::createColorBufferAttachments() {
		colorBuffer = new unsigned int[attachments];
		glGenRenderbuffers(attachments, colorBuffer);
		for(unsigned int i = 0; i < attachments; i++) {
			glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer[i]);
			if(isMultisampled()) glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA16F, width, height);
			else glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA16F, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, colorBuffer[i]);
		}
	}

	void FrameBuffer::createDepthTextureAttachment() {
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	}

	void FrameBuffer::createDepthBufferAttachment() {
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		if(isMultisampled()) glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, width, height);
		else glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
	}

	const unsigned int& FrameBuffer::resolveToTexture(const unsigned int& attachment) {
		if(this->attachments >= attachment && dirty[attachment]) {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, reserve->id);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
			glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + attachment);
			glBlitFramebuffer(0, 0, width, height, 0, 0, reserve->width, reserve->height, GL_COLOR_BUFFER_BIT | (reserve->depthTexture ? GL_DEPTH_BUFFER_BIT : 0), GL_NEAREST);
			dirty[attachment] = false;
		}
		return reserve->colorTexture[attachment];
	}

	const unsigned int* FrameBuffer::resolveToTextures() {
		if(isDirty()) {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, reserve->id);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
			for(unsigned int i = 0; i < this->attachments; i++) {
				if(dirty[i]) {
					glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
					glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
					glBlitFramebuffer(0, 0, width, height, 0, 0, reserve->width, reserve->height, GL_COLOR_BUFFER_BIT | (reserve->depthTexture > 0 ? GL_DEPTH_BUFFER_BIT : 0), GL_NEAREST);
					dirty[i] = false;
				}
			}
		}
		unbindFrameBuffer();
		return reserve->colorTexture;
	}

	/* True - If atleast one is dirty.
	 * False - If all are not dirty. */
	bool FrameBuffer::isDirty() const { for(unsigned int i = 0; i < attachments; i++) if(dirty[i]) return true; return false; }

	/* True - If all are dirty.
	 * False - If atleast one is found not dirty. */
	bool FrameBuffer::isAllDirty() const { for(unsigned int i = 0; i < attachments; i++) { if(!dirty[i]) { return false; } } return true; }

	inline void FrameBuffer::setDirty(const bool& flag) const { std::fill(dirty, dirty + attachments, flag); }

}
