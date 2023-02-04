#include <VoxelRaytracer/Source/Program/Program.h>

// The console application entry point
int main()
{
	forge::Program program;

	program.run(GetModuleHandleW(NULL), SW_SHOWNORMAL);

	return 0;
}