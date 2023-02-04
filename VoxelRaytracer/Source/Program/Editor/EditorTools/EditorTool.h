#ifndef FORGE_EDITOR_TOOL_H
#define FORGE_EDITOR_TOOL_H

#include <Forge/Source/Core/Containers/DynamicArray.h>
#include <VoxelRaytracer/Source/Rendering/DebugAABBData.h>

namespace forge
{
	class Editor;

	// Base class for an editor tool
	class EditorTool
	{
	public:

		EditorTool(Editor *pEditor);

		// Called once when the tool is selected
		void activate();

		// Called once every frame while the tool is active
		virtual void update() {}

		// Called once during any situation in which the tool should stop being used, such as when another tool is selected
		void deactivate();

		bool getIsActive() const;

		virtual void getCurrentRenderingData(DynamicArray<DebugAABBData> &pDebugAABBs) {}

	protected:

		// Called once when the tool is selected
		virtual void activateImplementation() {}

		// Called once during any situation in which the tool should stop being used, such as when another tool is selected
		virtual void deactivateImplementation() {}

		Editor* getEditor() const;

	private:

		Editor *editor = nullptr;
		bool isActive = false;
	};

	inline
	EditorTool::EditorTool(Editor *pEditor) :
	editor(pEditor)
	{
	}

	inline
	void EditorTool::activate()
	{
		this->activateImplementation();
		this->isActive = true;
	}

	inline
	void EditorTool::deactivate()
	{
		this->deactivateImplementation();
		this->isActive = false;
	}

	inline
	bool EditorTool::getIsActive() const
	{
		return this->isActive;
	}

	inline
	Editor* EditorTool::getEditor() const
	{
		return this->editor;
	}
}

#endif