#ifndef INTERRELATED_H__
#define INTERRELATED_H__
#include <vector>
struct ObjectRenderData
{
	int x{}, y{}, texRectPosX{}, texRectPosY{}, texRectW{}, texRectH{};
	int texId{}, entityId{};
};

struct WorldUpdate
{
	std::vector<ObjectRenderData> objectDataVec{};
};

struct ClientFrameInput
{
	int down, left, up, right;
	int attack, run, start;
};

#endif
