#version 330 core
layout (location = 0) in int meshID;
layout (location = 1) in int vertexID;
layout (location = 2) in vec3 aPos;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in vec3 aNormal;
layout (location = 5) in ivec4 BoneIDs;
layout (location = 6) in vec4 Weights;


out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

struct BoneTransform{
	int meshID;
	mat4 transform;
	int[10] transformedVertexIDs;
	int[10] transformedVertexWeights;
	int numVertexIDs;
};

uniform BoneTransform[64] boneTranformations;
uniform int numBoneTransformations;

void main()
{
	mat4 BoneTransform = 	boneTranformations[BoneIDs[0]].transform * Weights[0];			//mat4(1.0);
	BoneTransform += 	boneTranformations[BoneIDs[1]].transform * Weights[1];
	BoneTransform += 	boneTranformations[BoneIDs[2]].transform * Weights[2];
	BoneTransform += 	boneTranformations[BoneIDs[3]].transform * Weights[3];
	
	//for(int i = 0; i < numBoneTransformations; i++){
		//if(boneTranformations[i].meshID == meshID){
			//for(int v = 0; v <= boneTranformations[i].numVertexIDs; v++){
				//if(boneTranformations[i].transformedVertexIDs[v] == vertexID)
				//{
					//BoneTransform += boneTranformations[i].transform * boneTranformations[i].transformedVertexWeights[v];
			//	}
			//}
		//}
	//}


// note that we read the multiplication from right to left (matrix multiplication rule)
	vec4 boned = BoneTransform*vec4(aPos, 1.0);
    gl_Position = projection * view * model*boned;
    
    TexCoord = aTexCoord;
	Normal = mat3(transpose(inverse(model))) *aNormal ; //inversing matrices is expensive, try to do outside of shader where possible
	FragPos = vec3(model * vec4(aPos, 1.0));//frag in world space, not based on camera
}