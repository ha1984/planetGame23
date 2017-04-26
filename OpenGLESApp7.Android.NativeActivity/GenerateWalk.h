#pragma once
#include "BoneRotation.h"


class GenerateWalk
{
	
public:
	std::vector<BoneRotation> step;
	void upTilChest();
	void upTilShoulderR();
	void upTilShoulderL();
	void setupRest();
	mat4 upToChest;
	mat4 upToShoulderR;
	mat4 upToShoulderL;
	mat4 downFromShoulderR;
	mat4 downFromShoulderL;
	GenerateWalk();
	~GenerateWalk();
private:
	vec3 hand_Lhead;
	vec3 hand_Ltail;
	//bottom_arm_L 		 
	vec3 bottomarm_Lhead;
	vec3 bottomarm_Ltail;
	//upper_arm_L (		 ;
	vec3 upperarm_Lhead;
	vec3 upperarm_Ltail;
	//foot_front_L 		 ;
	vec3 footfront_Lhead;
	vec3 footfront_Ltail;
	//foot_back_L (		 ;
	vec3 footback_Lhead;
	vec3 footback_Ltail;
	//bottom_leg_L 		 ;
	vec3 bottomleg_Lhead;
	vec3 bottomleg_Ltail;
	//upper_leg_L (		 ;
	vec3 upperleg_Lhead;
	vec3 upperleg_Ltail;
	//hand_R (head 		 ;
	vec3 hand_Rhead;
	vec3 hand_Rtail;
	//bottom_arm_R 		 ;
	vec3 bottomarm_Rhead;
	vec3 bottomarm_Rtail;
	//upper_arm_R (		 ;
	vec3 upperarm_Rhead;
	vec3 upperarm_Rtail;
	//foot_front_R 		 ;
	vec3 footfront_Rhead;
	vec3 footfront_Rtail;
	//foot_back_R (		 ;
	vec3 footback_Rhead;
	vec3 footback_Rtail;
	//bottom_leg_R 		 ;
	vec3 bottomleg_Rhead;
	vec3 bottomleg_Rtail;
	//upper_leg_R (		 ;
	vec3 upperleg_Rhead;
	vec3 upperleg_Rtail;
};
