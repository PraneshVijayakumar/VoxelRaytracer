#include "LoadingScreenRenderer.h"
#include <cstdlib>
#include <string>
#include <VoxelRaytracer/Source/Program/ProgramIntersectionTests.h>
#include <thread>
#include <vector>
#include <VoxelRaytracer/Source/Program/Program.h>
#include <Forge/Source/Core/Math/Vector2.h>
#include "LineDrawing.h"

namespace forge
{
	void LoadingScreenRenderer::startup(Program *pProgram)
	{
		this->program = pProgram;
		this->backbuffer = new Framebuffer();
		this->backbuffer->startup(pProgram->getWindow()->getResolutionWidth(), pProgram->getWindow()->getResolutionHeight());
		this->circleCenterAngles[0] = -30.0f;
		this->circleCenterAngles[1] = 90.0f;
		this->circleCenterAngles[2] = 210.0f;
	}

	void LoadingScreenRenderer::shutdown()
	{
		this->backbuffer->shutdown();
		delete this->backbuffer;
	}

	void LoadingScreenRenderer::render(Float pDeltaTime)
	{
		this->backbuffer->clear();

		for(SizeType i = 0; i < 3; ++i)
		{
			this->circleCenterAngles[i] += 90.0f * pDeltaTime;
		}

		Float aspectRatio = static_cast<Float>(this->backbuffer->getWidth()) / static_cast<Float>(this->backbuffer->getHeight());
		IntegerVector2<Int> screenSpaceCircleCenters[3];
		for(SizeType i = 0; i < 3; ++i)
		{
			Float angleInRadians = convertDegreesToRadians(this->circleCenterAngles[i]);
			Vector2 circleCenter;
			circleCenter.x = std::cos(angleInRadians) * 0.5f;
			circleCenter.y = std::sin(angleInRadians) * 0.5f;

			Vector2 lerpFactors;
			lerpFactors.x = (circleCenter.x + aspectRatio) / (aspectRatio * 2.0f);
			lerpFactors.y = (circleCenter.y + 1.0f) / 2.0f;

			screenSpaceCircleCenters[i].x = roundFloatToInteger<Int>(lerp(0.0f, static_cast<Float>(this->backbuffer->getWidth() - 1), lerpFactors.x));
			screenSpaceCircleCenters[i].y = roundFloatToInteger<Int>(lerp(0.0f, static_cast<Float>(this->backbuffer->getHeight() - 1), lerpFactors.y));

			screenSpaceCircleCenters[i].x = clamp<Int>(screenSpaceCircleCenters[i].x, 0, this->backbuffer->getWidth() - 1);
			screenSpaceCircleCenters[i].y = clamp<Int>(screenSpaceCircleCenters[i].y, 0, this->backbuffer->getHeight() - 1);
		}

		drawLine(*this->backbuffer, screenSpaceCircleCenters[0], screenSpaceCircleCenters[1], Color(255, 255, 255));
		drawLine(*this->backbuffer, screenSpaceCircleCenters[1], screenSpaceCircleCenters[2], Color(255, 255, 255));
		drawLine(*this->backbuffer, screenSpaceCircleCenters[2], screenSpaceCircleCenters[0], Color(255, 255, 255));

		this->program->getWindow()->blitFramebufferToScreen(*this->backbuffer);
	}
}