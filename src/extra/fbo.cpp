#include "fbo.hpp"
#include "core/core.hpp"
#include "maths.hpp"

namespace Cyclone {

int FrameBuffer::MAX_TARGETS = 0;
int FrameBuffer::MAX_SAMPLES = 0;

FrameBuffer::FrameBuffer(const unsigned int& width, const unsigned int& height, const COLOR_ATTACHMENT_TYPE& color_at, const DEPTH_ATTACHMENT_TYPE& depth_at, const unsigned int& num_samples, const unsigned int& num_attachments): id(0), width(width), height(height), samples(std::max<unsigned int>(num_samples, 1)), attachments(std::max<unsigned int>(num_attachments, 1)), depthTexture(0), depthBuffer(0), colorTexture({}), colorBuffer({}), reserve(nullptr) {
	init();
	dirty.reserve(attachments);
	createFrameBuffer();
	if(color_at == COLOR_ATTACHMENT_NONE) glDrawBuffer(GL_NONE);
	else if(isMultisampled() || color_at == COLOR_ATTACHMENT_BUFFER) {
		createColorBufferAttachments();
		if(isMultisampled()) {
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

FrameBuffer::~FrameBuffer() { delete reserve; }

constexpr const bool FrameBuffer::isMultisampled() const { return samples != 1; }

void FrameBuffer::createFrameBuffer() {
	glGenFramebuffers(1, &id);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	for(unsigned int i = 0; i < attachments; i++) glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
}

void FrameBuffer::resolveToFBO(FrameBuffer* fbo) const {
	if(isMultisampled()) {
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
		glBlitFramebuffer(0, 0, width, height, 0, 0, Core::width, Core::height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_LINEAR);
		unbindFrameBuffer();
	}
}

void FrameBuffer::bindFrameBuffer() const { glBindFramebuffer(GL_FRAMEBUFFER, id); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void FrameBuffer::unbindFrameBuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

const unsigned int FrameBuffer::getColorTexture(const unsigned int& target) {
	if(target < attachments) return (isMultisampled() ? resolveToTexture(target) : colorTexture[target]);
	return 0;
}

const unsigned int& FrameBuffer::getDepthTexture() {
	if(isMultisampled()) { if(dirty[0]) resolveToTexture(0); return reserve->depthTexture; }
	else return depthTexture;
}

void FrameBuffer::init() {
	if(MAX_TARGETS == 0) {
		glGetIntegerv(GL_MAX_DRAW_BUFFERS, &MAX_TARGETS);
		attachments = clamp<int>(attachments, 1, MAX_TARGETS);
	}
	if(MAX_SAMPLES == 0) {
		glGetIntegerv(GL_MAX_INTEGER_SAMPLES, &MAX_SAMPLES);
		samples = clamp<int>(samples, 1, MAX_SAMPLES);
	}
}

void FrameBuffer::createColorTextureAttachments() {
	colorTexture.reserve(attachments);
	glGenTextures(attachments, colorTexture.data());
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
	colorBuffer.reserve(attachments);
	glGenRenderbuffers(attachments, colorBuffer.data());
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
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, reserve->id);
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
	return reserve->colorTexture.data();
}

bool FrameBuffer::isDirty() const { for(unsigned int i = 0; i < attachments; i++) if(dirty[i]) return true; return false; }

}
