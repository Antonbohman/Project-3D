#define BLOCK_SIZE 16
[numthreads(BLOCK_SIZE, BLOCK_SIZE, 1)]
void main( /*uint3 DTid : SV_DispatchThreadID*/
	unit3 groupId : SV_GroupID,
	unit3 groupThreadId : SV_GroupThreadID,
	unit groupIndex : SV_GroupIndex,
	unit3 dispatchThreadId : SV_DispatchThreadID)
{
	//. . .
}