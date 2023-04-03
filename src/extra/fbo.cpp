#include "Voyage/fbo.hpp"
#include "Voyage/core.hpp"
#include <cstdint>

namespace Voyage {

	int FrameBuffer::MAX_TARGETS = 0;
	int FrameBuffer::MAX_SAMPLES = 0;

	FrameBuffer::FrameBuffer(const uint32_t& width, const uint32_t& height, const COLOR_ATTACHMENT_TYPE& color_at, const DEPTH_ATTACHMENT_TYPE& depth_at, const uint32_t& num_samples, const uint32_t& num_attachments): id(0), width(width), height(height), samples(std::max<uint32_t>(num_samples, 1)), attachments(std::max<uint32_t>(num_attachments, 1)), depthTexture(0), depthBuffer(0), colorTexture(nullptr), colorBuffer(nullptr), dirty(nullptr), reserve(nullptr) {
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

	FrameBuffer::FrameBuffer(const FrameBuffer& fbo) noexcept: id(fbo.id), width(fbo.width), height(fbo.height), samples(fbo.samples), attachments(fbo.attachments), depthTexture(fbo.depthTexture), depthBuffer(fbo.depthBuffer), colorTexture(fbo.colorTexture), colorBuffer(fbo.colorBuffer), dirty(fbo.dirty), reserve(fbo.reserve) {}

	FrameBuffer::FrameBuffer(FrameBuffer&& fbo) noexcept: id(fbo.id), width(fbo.width), height(fbo.height), samples(fbo.samples), attachments(fbo.attachments), depthTexture(fbo.depthTexture), depthBuffer(fbo.depthBuffer), colorTexture(fbo.colorTexture), colorBuffer(fbo.colorBuffer), dirty(fbo.dirty), reserve(std::move(fbo.reserve)) {
		fbo.id = 0;
		fbo.depthTexture = 0;
		fbo.depthBuffer = 0;
		fbo.colorTexture = nullptr;
		fbo.colorBuffer = nullptr;
	}

	FrameBuffer& FrameBuffer::operator=(const FrameBuffer& other) noexcept {
		if(this == &other) return *this;
		id = other.id;
		width = other.width;
		height = other.height;
		samples = other.samples;
		attachments = other.attachments;
		depthTexture = other.depthTexture;
		depthBuffer = other.depthBuffer;
		colorTexture = new uint32_t[other.attachments];
		colorBuffer = new uint32_t[other.attachments];
		memcpy(colorTexture, other.colorTexture, attachments * sizeof(uint32_t));
		memcpy(colorBuffer, other.colorBuffer, attachments * sizeof(uint32_t));
		/* for(uint32_t i = 0; i < attachments; ++i) {
		   colorTexture[i] = other.colorTexture[i];
		   colorBuffer[i] = other.colorBuffer[i];
		   other.colorTexture[i] = 0;
		   other.colorBuffer[i] = 0;
		   } */
		dirty = other.dirty;
		reserve = other.reserve;
		return *this;
	}

	FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept {
		if(this == &other) return *this;
		id = other.id;
		width = other.width;
		height = other.height;
		samples = other.samples;
		attachments = other.attachments;
		depthTexture = other.depthTexture;
		depthBuffer = other.depthBuffer;
		colorTexture = other.colorTexture;
		colorBuffer = other.colorBuffer;
		dirty = other.dirty;
		reserve = std::move(other.reserve);

		other.id = 0;
		other.depthTexture = 0;
		other.depthBuffer = 0;
		other.colorTexture = nullptr;
		other.colorBuffer = nullptr;
		return *this;
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

	constexpr const bool FrameBuffer::isMultisampled() const { return samples != 1; }

	void FrameBuffer::resolveToFBO(FrameBuffer* fbo) const {
		if(isMultisampled()) {
			if(!fbo) fbo = reserve;
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo->id);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
			for(uint32_t i = 0; i < attachments; i++) {
				glReadBuffer(GL_COLOR_ATTACHMENT0 + i);
				glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
				glBlitFramebuffer(0, 0, width, height, 0, 0, fbo->width, fbo->height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_LINEAR);
			}
			unbindFrameBuffer();
		}
	}

	void FrameBuffer::resolveToScreen(const uint32_t& attachment) const {
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

	const uint32_t FrameBuffer::getColorTexture(const uint32_t& target) {
		if(target < attachments) {
			if(isMultisampled()) return resolveToTexture(target);
			else if(colorTexture) return colorTexture[target];
		}
		return 0;
	}

	const uint32_t FrameBuffer::getDepthTexture() {
		if(depthTexture) return depthTexture;
		else if(isMultisampled()) { if(isAllDirty()) resolveToTexture(); return reserve->depthTexture; }
		return 0;
	}

	void FrameBuffer::createFrameBuffer() {
		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		for(uint32_t i = 0; i < attachments; i++) glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
	}

	void FrameBuffer::init() {
		if(!MAX_TARGETS) glGetIntegerv(GL_MAX_DRAW_BUFFERS, &MAX_TARGETS);
		if(!MAX_SAMPLES) glGetIntegerv(GL_MAX_INTEGER_SAMPLES, &MAX_SAMPLES);
		attachments = std::min<uint32_t>(attachments, MAX_TARGETS);
		samples = std::min<uint32_t>(samples, MAX_SAMPLES);
	}

	void FrameBuffer::createColorTextureAttachments() {
		colorTexture = new uint32_t[attachments];
		glGenTextures(attachments, colorTexture);
		for(uint32_t i = 0; i < attachments; i++) {
			glBindTexture(GL_TEXTURE_2D, colorTexture[i]);
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorTexture[i], 0);
		}
	}

	void FrameBuffer::createColorBufferAttachments() {
		colorBuffer = new uint32_t[attachments];
		glGenRenderbuffers(attachments, colorBuffer);
		for(uint32_t i = 0; i < attachments; i++) {
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

	const uint32_t& FrameBuffer::resolveToTexture(const uint32_t& attachment) {
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

	const uint32_t* FrameBuffer::resolveToTextures() {
		if(isDirty()) {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, reserve->id);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
			for(uint32_t i = 0; i < this->attachments; i++) {
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
	bool FrameBuffer::isDirty() const { for(uint32_t i = 0; i < attachments; i++) if(dirty[i]) return true; return false; }

	/* True - If all are dirty.
	 * False - If atleast one is found not dirty. */
	bool FrameBuffer::isAllDirty() const { for(uint32_t i = 0; i < attachments; i++) { if(!dirty[i]) { return false; } } return true; }

	inline void FrameBuffer::setDirty(const bool& flag) const { std::fill(dirty, dirty + attachments, flag); }

}
