#include "pch.h"
#include "GenerateWalk.h"

mat4 calcMatrix(vec3 restHead, vec3 restTail, vec3 poseHead, vec3 poseTail) {
	
	
	//coord flip from blender to opengl:
	vec3 center = vec3(0.0f, 0.0f, 1.735f);
	mat3 flipCoord = mat3(1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f);

	vec3 restH = flipCoord*(0.45f*restHead + center);
	vec3 restT = flipCoord*(0.45f*restTail + center);
	vec3 poseH = flipCoord*(0.45f*poseHead + center);
	vec3 poseT = flipCoord*(0.45f*poseTail + center);

	vec3 rRest = normalize(restT - restH);
	vec3 rPose = normalize(poseT - poseH);

	vec3 axis = cross(rRest, rPose);
	vec3 n = normalize(axis);
	float cosa = dot(rRest, rPose);
	float alpha = acos(cosa);

	mat4 RotMatrix = rotate(mat4(), alpha, n);
	mat4 matrix = translate(mat4(), poseH) * RotMatrix *translate(mat4(), (-1.0f)*restH) - mat4(); // translate(mat4(), poseH) * RotMatrix *
	//vec4 deformation = vec4(restH, 1.0);

	return matrix;
}

void GenerateWalk::upTilChest()
{   
	// mat4 root = mat4(1.000000f, 0.000000f, 0.000000f, 0.000000f,
	//	 0.000000f, -0.000000f, 1.000000f, 0.000000f,
	//	 0.000000f, 1.000000f, 0.000000f, 0.000000f,
	//	 0.000000f, 0.000000f, 0.000000f, 1.000000f);

	// mat4 A_001 = mat4(0.415226f, 0.000000f, 0.000000f, 0.000000f,
	//	0.000000f, 0.415226f, 0.000000f, 0.000000f,
	//	0.000000f, 0.000000f, 0.415226f, 0.000000f,
	//	0.000000f, 0.003481f, 1.735331f, 1.000000f);

	// mat4 hips = mat4(1.000000f, 0.000000f, 0.000000f, 0.000000f,
	//	0.000000f, 0.000000f, 1.000000f, 0.000000f,
	//	0.000000f, -1.000000f, 0.000000f, 0.000000f,
	//	0.000000f, 0.000000f, -0.089768f, 1.000000f);

	//mat4 spine = mat4(0.354301f, -0.069471f, -0.932547f, 0.000000f,
	//	-0.042999f, 0.994971f, -0.090458f, 0.000000f,
	//	0.934142f, 0.072148f, 0.349532f, 0.000000f,
	//	0.000000f, 0.823055f, 0.000000f, 1.000000f);

 //   mat4 chest = mat4(0.961564f, -0.050986f, 0.269806f, 0.000000f,
	//	0.038052f, 0.997872f, 0.052958f, 0.000000f,
	//	-0.271932f, -0.040656f, 0.961457f, 0.000000f,
	//	-0.000000f, 0.844490f, 0.000000f, 1.000000f);

	//upToChest = chest*spine*hips*A_001*root;
}

void GenerateWalk::upTilShoulderR()
{
	//mat4 matrix = mat4(0.745039f, -0.619347f, -0.247641f, 0.000000f,
	//	-0.123878f, 0.236328f, -0.963744f, 0.000000f,
	//	0.655417f, 0.748705f, 0.099350f, 0.000000f,
	//	-0.239986f, 0.946820f, -0.008673f, 1.000000f);

	//upToShoulderR = matrix*upToChest;
}

void GenerateWalk::upTilShoulderL()
{
	//mat4 matrix = mat4(0.233311f, -0.962303f, 0.139779f, 0.000000f,
	//	0.879581f, 0.270139f, 0.391615f, 0.000000f,
	//	-0.414612f, 0.031579f, 0.909450f, 0.000000f,
	//	-0.041199f, 0.953518f, 0.259826f, 1.000000f);

	//upToShoulderL = matrix*upToChest;
}

void GenerateWalk::setupRest()
{
	//hand_L (head and tail):
	hand_Lhead = vec3(3.036f, -0.712f, 2.212f);
	hand_Ltail = vec3(3.244f, -1.164f, 1.988f);
	//bottom_arm_L (head and tail):
	bottomarm_Lhead = vec3(2.021f, 0.469f, 2.381f);
	bottomarm_Ltail = vec3(3.036f, -0.712f, 2.212f);
	//upper_arm_L (head and tail):
	upperarm_Lhead = vec3(0.861f, 0.402f, 2.793f);
	upperarm_Ltail = vec3(2.021f, 0.469f, 2.381f);
	//foot_front_L (head and tail):
	footfront_Lhead = vec3(0.857f, 0.007f, -3.879f);
	footfront_Ltail = vec3(0.873f, -0.678f, -4.027f);
	//foot_back_L (head and tail):
	footback_Lhead = vec3(0.829f, 0.590f, -3.758f);
	footback_Ltail = vec3(0.857f, 0.007f, -3.879f);
	//bottom_leg_L (head and tail):
	bottomleg_Lhead = vec3(0.740f, 0.160f, -1.961f);
	bottomleg_Ltail = vec3(0.829f, 0.590f, -3.758f);
	//upper_leg_L (head and tail):
	upperleg_Lhead = vec3(0.534f, -0.008f, 0.362f);
	upperleg_Ltail = vec3(0.740f, 0.160f, -1.961f);
	//hand_R (head and tail):
	hand_Rhead = vec3(-3.036f, -0.712f, 2.212f);
	hand_Rtail = vec3(-3.244f, -1.164f, 1.988f);
	//bottom_arm_R (head and tail):
	bottomarm_Rhead = vec3(-2.021f, 0.469f, 2.381f);
	bottomarm_Rtail = vec3(-3.036f, -0.712f, 2.212f);
	//upper_arm_R (head and tail):
	upperarm_Rhead = vec3(-0.861f, 0.402f, 2.793f);
	upperarm_Rtail = vec3(-2.021f, 0.469f, 2.381f);
	//foot_front_R (head and tail):
	footfront_Rhead = vec3(-0.857f, 0.007f, -3.879f);
	footfront_Rtail = vec3(-0.873f, -0.678f, -4.027f);
	//foot_back_R (head and tail):
	footback_Rhead = vec3(-0.829f, 0.590f, -3.758f);
	footback_Rtail = vec3(-0.857f, 0.007f, -3.879f);
	//bottom_leg_R (head and tail):
	bottomleg_Rhead = vec3(-0.740f, 0.160f, -1.961f);
	bottomleg_Rtail = vec3(-0.829f, 0.590f, -3.758f);
	//upper_leg_R (head and tail):
	upperleg_Rhead = vec3(-0.534f, -0.008f, 0.362f);
	upperleg_Rtail = vec3(-0.740f, 0.160f, -1.961f);
}


GenerateWalk::GenerateWalk()
{
	setupRest();

	step.resize(8);
	for (int i = 0; i < 8; i++) {
		step[i].boneRot.resize(14);
	}
	vec3 poseHead;
	vec3 poseTail;

    // keyframe 0:

    //hand_L (head and tail):
	poseHead = vec3(1.442f, 1.210f, 0.203f);
	poseTail = vec3(1.302f, 1.380f, -0.297f);
	step[0].boneRot[0] = calcMatrix(hand_Lhead, hand_Ltail, poseHead, poseTail);
	//bottom_arm_L (head and tail):
	poseHead = vec3(1.228f, 0.884f, 1.719f);
	poseTail = vec3(1.442f, 1.210f, 0.203f);
	step[0].boneRot[1] = calcMatrix(bottomarm_Lhead, bottomarm_Ltail, poseHead, poseTail);
	//upper_arm_L (head and tail):
	poseHead = vec3(0.861f, 0.402f, 2.793f);
	poseTail = vec3(1.228f, 0.884f, 1.719f);
	step[0].boneRot[2] = calcMatrix(upperarm_Lhead, upperarm_Ltail, poseHead, poseTail);
	//foot_front_L (head and tail):
	poseHead = vec3(0.604f, -1.550f, -3.561f);
	poseTail = vec3(0.557f, -2.226f, -3.388f);
	step[0].boneRot[6] = calcMatrix(footfront_Lhead, footfront_Ltail, poseHead, poseTail);
	//foot_back_L (head and tail):
	poseHead = vec3(0.628f, -0.972f, -3.712f);
	poseTail = vec3(0.604f, -1.550f, -3.561f);
	step[0].boneRot[7] = calcMatrix(footback_Lhead, footback_Ltail, poseHead, poseTail);
	//bottom_leg_L (head and tail):
	poseHead = vec3(0.587f, -0.552f, -1.911f);
	poseTail = vec3(0.628f, -0.972f, -3.712f);
	step[0].boneRot[8] = calcMatrix(bottomleg_Lhead, bottomleg_Ltail, poseHead, poseTail);
	//upper_leg_L (head and tail):
	poseHead = vec3(0.534f, -0.008f, 0.362f);
	poseTail = vec3(0.587f, -0.552f, -1.911f);
	step[0].boneRot[9] = calcMatrix(upperleg_Lhead, upperleg_Ltail, poseHead, poseTail);
	//hand_R (head and tail):
	poseHead = vec3(-1.403f, -0.877f, 0.363f);
	poseTail = vec3(-1.292f, -1.074f, -0.134f);
	step[0].boneRot[3] = calcMatrix(hand_Rhead, hand_Rtail, poseHead, poseTail);
	//bottom_arm_R (head and tail):
	poseHead = vec3(-1.104f, -0.158f, 1.722f);
	poseTail = vec3(-1.403f, -0.877f, 0.363f);
	step[0].boneRot[4] = calcMatrix(bottomarm_Rhead, bottomarm_Rtail, poseHead, poseTail);
	//upper_arm_R (head and tail):
	poseHead = vec3(-0.861f, 0.402f, 2.793f);
	poseTail = vec3(-1.104f, -0.158f, 1.722f);
	step[0].boneRot[5] = calcMatrix(upperarm_Rhead, upperarm_Rtail, poseHead, poseTail);
	//foot_front_R (head and tail):
	poseHead = vec3(-0.705f, 0.777f, -3.824f);
	poseTail = vec3(-0.769f, 0.095f, -3.968f);
	step[0].boneRot[10] = calcMatrix(footfront_Rhead, footfront_Rtail, poseHead, poseTail);
	//foot_back_R (head and tail):
	poseHead = vec3(-0.694f, 1.320f, -3.575f);
	poseTail = vec3(-0.705f, 0.777f, -3.824f);
	step[0].boneRot[11] = calcMatrix(footback_Rhead, footback_Rtail, poseHead, poseTail);
	//bottom_leg_R (head and tail):
	poseHead = vec3(-0.650f, 0.501f, -1.917f);
	poseTail = vec3(-0.694f, 1.320f, -3.575f);
	step[0].boneRot[12] = calcMatrix(bottomleg_Rhead, bottomleg_Rtail, poseHead, poseTail);
	//upper_leg_R (head and tail):
	poseHead = vec3(-0.534f, -0.008f, 0.362f);
	poseTail = vec3(-0.650f, 0.501f, -1.917f);
	step[0].boneRot[13] = calcMatrix(upperleg_Rhead, upperleg_Rtail, poseHead, poseTail);

	// keyframe 1:

	//hand_L (head and tail):
	poseHead = vec3(1.447f, 0.768f, 0.176f);
	poseTail = vec3(1.239f, 0.805f, -0.327f);
	step[1].boneRot[0] = calcMatrix(hand_Lhead, hand_Ltail, poseHead, poseTail);
	//bottom_arm_L (head and tail):
	poseHead = vec3(1.456f, 0.885f, 1.737f);
	poseTail = vec3(1.447f, 0.768f, 0.176f);
	step[1].boneRot[1] = calcMatrix(bottomarm_Lhead, bottomarm_Ltail, poseHead, poseTail);
	//upper_arm_L (head and tail):
	poseHead = vec3(0.861f, 0.402f, 2.703f);
	poseTail = vec3(1.456f, 0.885f, 1.737f);
	step[1].boneRot[2] = calcMatrix(upperarm_Lhead, upperarm_Ltail, poseHead, poseTail);
	//foot_front_L (head and tail):
	poseHead = vec3(0.637f, -1.118f, -3.879f);
	poseTail = vec3(0.603f, -1.806f, -4.002f);
	step[1].boneRot[6] = calcMatrix(footfront_Lhead, footfront_Ltail, poseHead, poseTail);
	//foot_back_L (head and tail):
	poseHead = vec3(0.643f, -0.535f, -3.749f);
	poseTail = vec3(0.637f, -1.118f, -3.879f);
	step[1].boneRot[7] = calcMatrix(footback_Lhead, footback_Ltail, poseHead, poseTail);
	//bottom_leg_L (head and tail):
	poseHead = vec3(0.668f, -0.810f, -1.920f);
	poseTail = vec3(0.643f, -0.535f, -3.749f);
	step[1].boneRot[8] = calcMatrix(bottomleg_Lhead, bottomleg_Ltail, poseHead, poseTail);
	//upper_leg_L (head and tail):
	poseHead = vec3(0.534f, -0.008f, 0.272f);
	poseTail = vec3(0.668f, -0.810f, -1.920f);
	step[1].boneRot[9] = calcMatrix(upperleg_Lhead, upperleg_Ltail, poseHead, poseTail);
	//hand_R (head and tail):
	poseHead = vec3(-1.451f, -0.570f, 0.146f);
	poseTail = vec3(-1.352f, -0.699f, -0.375f);
	step[1].boneRot[3] = calcMatrix(hand_Rhead, hand_Rtail, poseHead, poseTail);
	//bottom_arm_R (head and tail):
	poseHead = vec3(-1.125f, -0.023f, 1.576f);
	poseTail = vec3(-1.451f, -0.570f, 0.146f);
	step[1].boneRot[4] = calcMatrix(bottomarm_Rhead, bottomarm_Rtail, poseHead, poseTail);
	//upper_arm_R (head and tail):
	poseHead = vec3(-0.861f, 0.402f, 2.703f);
	poseTail = vec3(-1.125f, -0.023f, 1.576f);
	step[1].boneRot[5] = calcMatrix(upperarm_Rhead, upperarm_Rtail, poseHead, poseTail);
	//foot_front_R (head and tail):
	poseHead = vec3(-0.698f, 0.395f, -3.825f);
	poseTail = vec3(-0.763f, -0.263f, -4.053f);
	step[1].boneRot[10] = calcMatrix(footfront_Rhead, footfront_Rtail, poseHead, poseTail);
	//foot_back_R (head and tail):
	poseHead = vec3(-0.682f, 0.864f, -3.455f);
	poseTail = vec3(-0.698f, 0.395f, -3.825f);
	step[1].boneRot[11] = calcMatrix(footback_Rhead, footback_Rtail, poseHead, poseTail);
	//bottom_leg_R (head and tail):
	poseHead = vec3(-0.741f, -0.321f, -2.036f);
	poseTail = vec3(-0.682f, 0.864f, -3.455f);
	step[1].boneRot[12] = calcMatrix(bottomleg_Rhead, bottomleg_Rtail, poseHead, poseTail);
	//upper_leg_R (head and tail):
	poseHead = vec3(-0.534f, -0.008f, 0.272f);
	poseTail = vec3(-0.741f, -0.321f, -2.036f);
	step[1].boneRot[13] = calcMatrix(upperleg_Rhead, upperleg_Rtail, poseHead, poseTail);

	//keyframe 2: 

	//hand_L (head and tail):
	poseHead = vec3(1.458f, -0.157f, 0.120f);
	poseTail = vec3(1.292f, -0.262f, -0.389f);
	step[2].boneRot[0] = calcMatrix(hand_Lhead, hand_Ltail, poseHead, poseTail);
	//bottom_arm_L (head and tail):
	poseHead = vec3(1.327f, 0.344f, 1.598f);
	poseTail = vec3(1.458f, -0.157f, 0.120f);
	step[2].boneRot[1] = calcMatrix(bottomarm_Lhead, bottomarm_Ltail, poseHead, poseTail);
	//upper_arm_L (head and tail):
	poseHead = vec3(0.861f, 0.402f, 2.738f);
	poseTail = vec3(1.327f, 0.344f, 1.598f);
	step[2].boneRot[2] = calcMatrix(upperarm_Lhead, upperarm_Ltail, poseHead, poseTail);
	//foot_front_L (head and tail):
	poseHead = vec3(0.674f, -0.110f, -3.938f);
	poseTail = vec3(0.627f, -0.763f, -4.187f);
	step[2].boneRot[6] = calcMatrix(footfront_Lhead, footfront_Ltail, poseHead, poseTail);
	//foot_back_L (head and tail):
	poseHead = vec3(0.669f, 0.418f, -3.660f);
	poseTail = vec3(0.674f, -0.110f, -3.938f);
	step[2].boneRot[7] = calcMatrix(footback_Lhead, footback_Ltail, poseHead, poseTail);
	//bottom_leg_L (head and tail):
	poseHead = vec3(0.681f, -0.389f, -1.996f);
	poseTail = vec3(0.669f, 0.418f, -3.660f);
	step[2].boneRot[8] = calcMatrix(bottomleg_Lhead, bottomleg_Ltail, poseHead, poseTail);
	//upper_leg_L (head and tail):
	poseHead = vec3(0.534f, -0.008f, 0.307f);
	poseTail = vec3(0.681f, -0.389f, -1.996f);
	step[2].boneRot[9] = calcMatrix(upperleg_Lhead, upperleg_Ltail, poseHead, poseTail);
	//hand_R (head and tail):
	poseHead = vec3(-1.452f, 0.335f, 0.150f);
	poseTail = vec3(-1.251f, 0.293f, -0.356f);
	step[2].boneRot[3] = calcMatrix(hand_Rhead, hand_Rtail, poseHead, poseTail);
	//bottom_arm_R (head and tail):
	poseHead = vec3(-1.431f, 0.674f, 1.679f);
	poseTail = vec3(-1.452f, 0.335f, 0.150f);
	step[2].boneRot[4] = calcMatrix(bottomarm_Rhead, bottomarm_Rtail, poseHead, poseTail);
	//upper_arm_R (head and tail):
	poseHead = vec3(-0.861f, 0.402f, 2.738f);
	poseTail = vec3(-1.431f, 0.674f, 1.679f);
	step[2].boneRot[5] = calcMatrix(upperarm_Rhead, upperarm_Rtail, poseHead, poseTail);
	//foot_front_R (head and tail):
	poseHead = vec3(-0.633f, -0.407f, -3.631f);
	poseTail = vec3(-0.618f, -0.893f, -4.135f);
	step[2].boneRot[10] = calcMatrix(footfront_Rhead, footfront_Rtail, poseHead, poseTail);
	//foot_back_R (head and tail):
	poseHead = vec3(-0.655f, -0.090f, -3.126f);
	poseTail = vec3(-0.633f, -0.407f, -3.631f);
	step[2].boneRot[11] = calcMatrix(footback_Rhead, footback_Rtail, poseHead, poseTail);
	//bottom_leg_R (head and tail):
	poseHead = vec3(-0.707f, -1.236f, -1.676f);
	poseTail = vec3(-0.655f, -0.090f, -3.126f);
	step[2].boneRot[12] = calcMatrix(bottomleg_Rhead, bottomleg_Rtail, poseHead, poseTail);
	//upper_leg_R (head and tail):
	poseHead = vec3(-0.534f, -0.008f, 0.307f);
	poseTail = vec3(-0.707f, -1.236f, -1.676f);
	step[2].boneRot[13] = calcMatrix(upperleg_Rhead, upperleg_Rtail, poseHead, poseTail);

	//keyframe 3:

	//hand_L (head and tail):
	poseHead = vec3(1.424f, -0.711f, 0.269f);
	poseTail = vec3(1.317f, -0.874f, -0.241f);
	step[3].boneRot[0] = calcMatrix(hand_Lhead, hand_Ltail, poseHead, poseTail);
	//bottom_arm_L (head and tail):
	poseHead = vec3(1.112f, -0.086f, 1.670f);
	poseTail = vec3(1.424f, -0.711f, 0.269f);
	step[3].boneRot[1] = calcMatrix(bottomarm_Lhead, bottomarm_Ltail, poseHead, poseTail);
	//upper_arm_L (head and tail):
	poseHead = vec3(0.861f, 0.402f, 2.774f);
	poseTail = vec3(1.112f, -0.086f, 1.670f);
	step[3].boneRot[2] = calcMatrix(upperarm_Lhead, upperarm_Ltail, poseHead, poseTail);
	//foot_front_L (head and tail):
	poseHead = vec3(0.709f, 0.543f, -3.891f);
	poseTail = vec3(0.649f, -0.120f, -4.104f);
	step[3].boneRot[6] = calcMatrix(footfront_Lhead, footfront_Ltail, poseHead, poseTail);
	//foot_back_L (head and tail):
	poseHead = vec3(0.698f, 1.073f, -3.615f);
	poseTail = vec3(0.709f, 0.543f, -3.891f);
	step[3].boneRot[7] = calcMatrix(footback_Lhead, footback_Ltail, poseHead, poseTail);
	//bottom_leg_L (head and tail):
	poseHead = vec3(0.669f, 0.208f, -1.981f);
	poseTail = vec3(0.698f, 1.073f, -3.615f);
	step[3].boneRot[8] = calcMatrix(bottomleg_Lhead, bottomleg_Ltail, poseHead, poseTail);
	//upper_leg_L (head and tail):
	poseHead = vec3(0.534f, -0.008f, 0.344f);
	poseTail = vec3(0.669f, 0.208f, -1.981f);
	step[3].boneRot[9] = calcMatrix(upperleg_Lhead, upperleg_Ltail, poseHead, poseTail);
	//hand_R (head and tail):
	poseHead = vec3(-1.444f, 0.972f, 0.188f);
	poseTail = vec3(-1.271f, 1.066f, -0.320f);
	step[3].boneRot[3] = calcMatrix(hand_Rhead, hand_Rtail, poseHead, poseTail);
	//bottom_arm_R (head and tail):
	poseHead = vec3(-1.340f, 0.890f, 1.749f);
	poseTail = vec3(-1.444f, 0.972f, 0.188f);
	step[3].boneRot[4] = calcMatrix(bottomarm_Rhead, bottomarm_Rtail, poseHead, poseTail);
	//upper_arm_R (head and tail):
	poseHead = vec3(-0.861f, 0.402f, 2.774f);
	poseTail = vec3(-1.340f, 0.890f, 1.749f);
	step[3].boneRot[5] = calcMatrix(upperarm_Rhead, upperarm_Rtail, poseHead, poseTail);
	//foot_front_R (head and tail):
	poseHead = vec3(-0.651f, -1.195f, -3.735f);
	poseTail = vec3(-0.633f, -1.851f, -3.978f);
	step[3].boneRot[10] = calcMatrix(footfront_Rhead, footfront_Rtail, poseHead, poseTail);
	//foot_back_R (head and tail):
	poseHead = vec3(-0.655f, -0.650f, -3.491f);
	poseTail = vec3(-0.651f, -1.195f, -3.735f);
	step[3].boneRot[11] = calcMatrix(footback_Rhead, footback_Rtail, poseHead, poseTail);
	//bottom_leg_R (head and tail):
	poseHead = vec3(-0.624f, -1.131f, -1.705f);
	poseTail = vec3(-0.655f, -0.650f, -3.491f);
	step[3].boneRot[12] = calcMatrix(bottomleg_Rhead, bottomleg_Rtail, poseHead, poseTail);
	//upper_leg_R (head and tail):
	poseHead = vec3(-0.534f, -0.008f, 0.344f);
	poseTail = vec3(-0.624f, -1.131f, -1.705f);
	step[3].boneRot[13] = calcMatrix(upperleg_Rhead, upperleg_Rtail, poseHead, poseTail);

	//keyframe 4:

	//hand_L (head and tail):
	poseHead = vec3(1.403f, -0.877f, 0.363f);
	poseTail = vec3(1.292f, -1.074f, -0.134f);
	step[4].boneRot[0] = calcMatrix(hand_Lhead, hand_Ltail, poseHead, poseTail);
	//bottom_arm_L (head and tail):
	poseHead = vec3(1.103f, -0.159f, 1.722f);
	poseTail = vec3(1.403f, -0.877f, 0.363f);
	step[4].boneRot[1] = calcMatrix(bottomarm_Lhead, bottomarm_Ltail, poseHead, poseTail);
	//upper_arm_L (head and tail):
	poseHead = vec3(0.861f, 0.402f, 2.793f);
	poseTail = vec3(1.103f, -0.159f, 1.722f);
	step[4].boneRot[2] = calcMatrix(upperarm_Lhead, upperarm_Ltail, poseHead, poseTail);
	//foot_front_L (head and tail):
	poseHead = vec3(0.705f, 0.777f, -3.824f);
	poseTail = vec3(0.639f, 0.095f, -3.964f);
	step[4].boneRot[6] = calcMatrix(footfront_Lhead, footfront_Ltail, poseHead, poseTail);
	//foot_back_L (head and tail):
	poseHead = vec3(0.694f, 1.320f, -3.575f);
	poseTail = vec3(0.705f, 0.777f, -3.824f);
	step[4].boneRot[7] = calcMatrix(footback_Lhead, footback_Ltail, poseHead, poseTail);
	//bottom_leg_L (head and tail):
	poseHead = vec3(0.650f, 0.501f, -1.917f);
	poseTail = vec3(0.694f, 1.320f, -3.575f);
	step[4].boneRot[8] = calcMatrix(bottomleg_Lhead, bottomleg_Ltail, poseHead, poseTail);
	//upper_leg_L (head and tail):
	poseHead = vec3(0.534f, -0.008f, 0.362f);
	poseTail = vec3(0.650f, 0.501f, -1.917f);
	step[4].boneRot[9] = calcMatrix(upperleg_Lhead, upperleg_Ltail, poseHead, poseTail);
	//hand_R (head and tail):
	poseHead = vec3(-1.442f, 1.210f, 0.203f);
	poseTail = vec3(-1.304f, 1.383f, -0.297f);
	step[4].boneRot[3] = calcMatrix(hand_Rhead, hand_Rtail, poseHead, poseTail);
	//bottom_arm_R (head and tail):
	poseHead = vec3(-1.226f, 0.885f, 1.719f);
	poseTail = vec3(-1.442f, 1.210f, 0.203f);
	step[4].boneRot[4] = calcMatrix(bottomarm_Rhead, bottomarm_Rtail, poseHead, poseTail);
	//upper_arm_R (head and tail):
	poseHead = vec3(-0.861f, 0.402f, 2.793f);
	poseTail = vec3(-1.226f, 0.885f, 1.719f);
	step[4].boneRot[5] = calcMatrix(upperarm_Rhead, upperarm_Rtail, poseHead, poseTail);
	//foot_front_R (head and tail):
	poseHead = vec3(-0.662f, -1.548f, -3.557f);
	poseTail = vec3(-0.685f, -2.224f, -3.379f);
	step[4].boneRot[10] = calcMatrix(footfront_Rhead, footfront_Rtail, poseHead, poseTail);
	//foot_back_R (head and tail):
	poseHead = vec3(-0.628f, -0.972f, -3.712f);
	poseTail = vec3(-0.662f, -1.548f, -3.557f);
	step[4].boneRot[11] = calcMatrix(footback_Rhead, footback_Rtail, poseHead, poseTail);
	//bottom_leg_R (head and tail):
	poseHead = vec3(-0.590f, -0.542f, -1.914f);
	poseTail = vec3(-0.628f, -0.972f, -3.712f);
	step[4].boneRot[12] = calcMatrix(bottomleg_Rhead, bottomleg_Rtail, poseHead, poseTail);
	//upper_leg_R (head and tail):
	poseHead = vec3(-0.534f, -0.008f, 0.362f);
	poseTail = vec3(-0.590f, -0.542f, -1.914f);
	step[4].boneRot[13] = calcMatrix(upperleg_Rhead, upperleg_Rtail, poseHead, poseTail);

	//keyframe 5:

	//hand_L (head and tail):
	poseHead = vec3(1.451f, -0.570f, 0.146f);
	poseTail = vec3(1.352f, -0.699f, -0.375f);
	step[5].boneRot[0] = calcMatrix(hand_Lhead, hand_Ltail, poseHead, poseTail);
	//bottom_arm_L (head and tail):
	poseHead = vec3(1.124f, -0.024f, 1.576f);
	poseTail = vec3(1.451f, -0.570f, 0.146f);
	step[5].boneRot[1] = calcMatrix(bottomarm_Lhead, bottomarm_Ltail, poseHead, poseTail);
	//upper_arm_L (head and tail):
	poseHead = vec3(0.861f, 0.402f, 2.703f);
	poseTail = vec3(1.124f, -0.024f, 1.576f);
	step[5].boneRot[2] = calcMatrix(upperarm_Lhead, upperarm_Ltail, poseHead, poseTail);
	//foot_front_L (head and tail):
	poseHead = vec3(0.609f, 0.430f, -3.900f);
	poseTail = vec3(0.569f, -0.216f, -4.166f);
	step[5].boneRot[6] = calcMatrix(footfront_Lhead, footfront_Ltail, poseHead, poseTail);
	//foot_back_L (head and tail):
	poseHead = vec3(0.610f, 0.872f, -3.499f);
	poseTail = vec3(0.609f, 0.430f, -3.900f);
	step[5].boneRot[7] = calcMatrix(footback_Lhead, footback_Ltail, poseHead, poseTail);
	//bottom_leg_L (head and tail):
	poseHead = vec3(0.683f, -0.271f, -2.046f);
	poseTail = vec3(0.610f, 0.872f, -3.499f);
	step[5].boneRot[8] = calcMatrix(bottomleg_Lhead, bottomleg_Ltail, poseHead, poseTail);
	//upper_leg_L (head and tail):
	poseHead = vec3(0.534f, -0.008f, 0.272f);
	poseTail = vec3(0.683f, -0.271f, -2.046f);
	step[5].boneRot[9] = calcMatrix(upperleg_Lhead, upperleg_Ltail, poseHead, poseTail);
	//hand_R (head and tail):
	poseHead = vec3(-1.447f, 0.768f, 0.176f);
	poseTail = vec3(-1.241f, 0.805f, -0.328f);
	step[5].boneRot[3] = calcMatrix(hand_Rhead, hand_Rtail, poseHead, poseTail);
	//bottom_arm_R (head and tail):
	poseHead = vec3(-1.452f, 0.889f, 1.737f);
	poseTail = vec3(-1.447f, 0.768f, 0.176f);
	step[5].boneRot[4] = calcMatrix(bottomarm_Rhead, bottomarm_Rtail, poseHead, poseTail);
	//upper_arm_R (head and tail):
	poseHead = vec3(-0.861f, 0.402f, 2.703f);
	poseTail = vec3(-1.452f, 0.889f, 1.737f);
	step[5].boneRot[5] = calcMatrix(upperarm_Rhead, upperarm_Rtail, poseHead, poseTail);
	//foot_front_R (head and tail):
	poseHead = vec3(-0.645f, -1.128f, -3.821f);
	poseTail = vec3(-0.643f, -1.826f, -3.877f);
	step[5].boneRot[10] = calcMatrix(footfront_Rhead, footfront_Rtail, poseHead, poseTail);
	//foot_back_R (head and tail):
	poseHead = vec3(-0.643f, -0.535f, -3.749f);
	poseTail = vec3(-0.645f, -1.128f, -3.821f);
	step[5].boneRot[11] = calcMatrix(footback_Rhead, footback_Rtail, poseHead, poseTail);
	//bottom_leg_R (head and tail):
	poseHead = vec3(-0.686f, -0.807f, -1.920f);
	poseTail = vec3(-0.643f, -0.535f, -3.749f);
	step[5].boneRot[12] = calcMatrix(bottomleg_Rhead, bottomleg_Rtail, poseHead, poseTail);
	//upper_leg_R (head and tail):
	poseHead = vec3(-0.534f, -0.008f, 0.272f);
	poseTail = vec3(-0.686f, -0.807f, -1.920f);
	step[5].boneRot[13] = calcMatrix(upperleg_Rhead, upperleg_Rtail, poseHead, poseTail);

	//keyframe 6:

	//hand_L (head and tail):
	poseHead = vec3(1.455f, 0.116f, 0.137f);
	poseTail = vec3(1.257f, 0.039f, -0.366f);
	step[6].boneRot[0] = calcMatrix(hand_Lhead, hand_Ltail, poseHead, poseTail);
	//bottom_arm_L (head and tail):
	poseHead = vec3(1.424f, 0.557f, 1.639f);
	poseTail = vec3(1.455f, 0.116f, 0.137f);
	step[6].boneRot[1] = calcMatrix(bottomarm_Lhead, bottomarm_Ltail, poseHead, poseTail);
	//upper_arm_L (head and tail):
	poseHead = vec3(0.861f, 0.402f, 2.725f);
	poseTail = vec3(1.424f, 0.557f, 1.639f);
	step[6].boneRot[2] = calcMatrix(upperarm_Lhead, upperarm_Ltail, poseHead, poseTail);
	//foot_front_L (head and tail):
	poseHead = vec3(0.556f, -0.287f, -3.675f);
	poseTail = vec3(0.501f, -0.855f, -4.081f);
	step[6].boneRot[6] = calcMatrix(footfront_Lhead, footfront_Ltail, poseHead, poseTail);
	//foot_back_L (head and tail):
	poseHead = vec3(0.583f, 0.165f, -3.287f);
	poseTail = vec3(0.556f, -0.287f, -3.675f);
	step[6].boneRot[7] = calcMatrix(footback_Lhead, footback_Ltail, poseHead, poseTail);
	//bottom_leg_L (head and tail):
	poseHead = vec3(0.655f, -0.975f, -1.832f);
	poseTail = vec3(0.583f, 0.165f, -3.287f);
	step[6].boneRot[8] = calcMatrix(bottomleg_Lhead, bottomleg_Ltail, poseHead, poseTail);
	//upper_leg_L (head and tail):
	poseHead = vec3(0.534f, -0.008f, 0.294f);
	poseTail = vec3(0.655f, -0.975f, -1.832f);
	step[6].boneRot[9] = calcMatrix(upperleg_Lhead, upperleg_Ltail, poseHead, poseTail);
	//hand_R (head and tail):
	poseHead = vec3(-1.455f, 0.062f, 0.134f);
	poseTail = vec3(-1.262f, -0.023f, -0.370f);
	step[6].boneRot[3] = calcMatrix(hand_Rhead, hand_Rtail, poseHead, poseTail);
	//bottom_arm_R (head and tail):
	poseHead = vec3(-1.412f, 0.524f, 1.629f);
	poseTail = vec3(-1.455f, 0.062f, 0.134f);
	step[6].boneRot[4] = calcMatrix(bottomarm_Rhead, bottomarm_Rtail, poseHead, poseTail);
	//upper_arm_R (head and tail):
	poseHead = vec3(-0.861f, 0.402f, 2.725f);
	poseTail = vec3(-1.412f, 0.524f, 1.629f);
	step[6].boneRot[5] = calcMatrix(upperarm_Rhead, upperarm_Rtail, poseHead, poseTail);
	//foot_front_R (head and tail):
	poseHead = vec3(-0.667f, -0.363f, -3.900f);
	poseTail = vec3(-0.690f, -1.038f, -4.083f);
	step[6].boneRot[10] = calcMatrix(footfront_Rhead, footfront_Rtail, poseHead, poseTail);
	//foot_back_R (head and tail):
	poseHead = vec3(-0.663f, 0.192f, -3.681f);
	poseTail = vec3(-0.667f, -0.363f, -3.900f);
	step[6].boneRot[11] = calcMatrix(footback_Rhead, footback_Rtail, poseHead, poseTail);
	//bottom_leg_R (head and tail):
	poseHead = vec3(-0.686f, -0.533f, -1.979f);
	poseTail = vec3(-0.663f, 0.192f, -3.681f);
	step[6].boneRot[12] = calcMatrix(bottomleg_Rhead, bottomleg_Rtail, poseHead, poseTail);
	//upper_leg_R (head and tail):
	poseHead = vec3(-0.534f, -0.008f, 0.294f);
	poseTail = vec3(-0.686f, -0.533f, -1.979f);
	step[6].boneRot[13] = calcMatrix(upperleg_Rhead, upperleg_Rtail, poseHead, poseTail);

	//keyframe 7:

	//hand_L (head and tail):
	poseHead = vec3(1.446f, 0.847f, 0.181f);
	poseTail = vec3(1.260f, 0.908f, -0.329f);
	step[7].boneRot[0] = calcMatrix(hand_Lhead, hand_Ltail, poseHead, poseTail);
	//bottom_arm_L (head and tail):
	poseHead = vec3(1.377f, 0.859f, 1.745f);
	poseTail = vec3(1.446f, 0.847f, 0.181f);
	step[7].boneRot[1] = calcMatrix(bottomarm_Lhead, bottomarm_Ltail, poseHead, poseTail);
	//upper_arm_L (head and tail):
	poseHead = vec3(0.861f, 0.402f, 2.767f);
	poseTail = vec3(1.377f, 0.859f, 1.745f);
	step[7].boneRot[2] = calcMatrix(upperarm_Lhead, upperarm_Ltail, poseHead, poseTail);
	//foot_front_L (head and tail):
	poseHead = vec3(0.608f, -1.179f, -3.684f);
	poseTail = vec3(0.578f, -1.870f, -3.793f);
	step[7].boneRot[6] = calcMatrix(footfront_Lhead, footfront_Ltail, poseHead, poseTail);
	//foot_back_L (head and tail):
	poseHead = vec3(0.619f, -0.600f, -3.536f);
	poseTail = vec3(0.608f, -1.179f, -3.684f);
	step[7].boneRot[7] = calcMatrix(footback_Lhead, footback_Ltail, poseHead, poseTail);
	//bottom_leg_L (head and tail):
	poseHead = vec3(0.698f, -1.059f, -1.746f);
	poseTail = vec3(0.619f, -0.600f, -3.536f);
	step[7].boneRot[8] = calcMatrix(bottomleg_Lhead, bottomleg_Ltail, poseHead, poseTail);
	//upper_leg_L (head and tail):
	poseHead = vec3(0.534f, -0.008f, 0.336f);
	poseTail = vec3(0.698f, -1.059f, -1.746f);
	step[7].boneRot[9] = calcMatrix(upperleg_Lhead, upperleg_Ltail, poseHead, poseTail);
	//hand_R (head and tail):
	poseHead = vec3(-1.434f, -0.615f, 0.224f);
	poseTail = vec3(-1.330f, -0.759f, -0.293f);
	step[7].boneRot[3] = calcMatrix(hand_Rhead, hand_Rtail, poseHead, poseTail);
	//bottom_arm_R (head and tail):
	poseHead = vec3(-1.117f, -0.043f, 1.646f);
	poseTail = vec3(-1.434f, -0.615f, 0.224f);
	step[7].boneRot[4] = calcMatrix(bottomarm_Rhead, bottomarm_Rtail, poseHead, poseTail);
	//upper_arm_R (head and tail):
	poseHead = vec3(-0.861f, 0.402f, 2.767f);
	poseTail = vec3(-1.117f, -0.043f, 1.646f);
	step[7].boneRot[5] = calcMatrix(upperarm_Rhead, upperarm_Rtail, poseHead, poseTail);
	//foot_front_R (head and tail):
	poseHead = vec3(-0.693f, 0.417f, -3.888f);
	poseTail = vec3(-0.746f, -0.251f, -4.093f);
	step[7].boneRot[10] = calcMatrix(footfront_Rhead, footfront_Rtail, poseHead, poseTail);
	//foot_back_R (head and tail):
	poseHead = vec3(-0.684f, 0.945f, -3.610f);
	poseTail = vec3(-0.693f, 0.417f, -3.888f);
	step[7].boneRot[11] = calcMatrix(footback_Rhead, footback_Rtail, poseHead, poseTail);
	//bottom_leg_R (head and tail):
	poseHead = vec3(-0.673f, 0.039f, -1.997f);
	poseTail = vec3(-0.684f, 0.945f, -3.610f);
	step[7].boneRot[12] = calcMatrix(bottomleg_Rhead, bottomleg_Rtail, poseHead, poseTail);
	//upper_leg_R (head and tail):
	poseHead = vec3(-0.534f, -0.008f, 0.336f);
	poseTail = vec3(-0.673f, 0.039f, -1.997f);
	step[7].boneRot[13] = calcMatrix(upperleg_Rhead, upperleg_Rtail, poseHead, poseTail);
}

GenerateWalk::~GenerateWalk()
{
}
