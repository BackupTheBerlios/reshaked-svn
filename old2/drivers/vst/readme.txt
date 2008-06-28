In this folder, place "aeffect.h" and "aeffectx.h" from the VST SDK
distribution. You also have to fix the "#pragma pack" macros so they are
gcc compatible, or the library simply wont work or wont be gcc compatible:

Replace in both files:

#pragma pack(push)
#pragma pack(8)

for

#pragma pack(push,8)


