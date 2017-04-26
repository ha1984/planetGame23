#include "pch.h"
#include "ShaderFile.h"


const std::string ShaderFile::vShader()
{
    const std::string vs = R"(
        uniform mat4 uModelMatrix;
        uniform mat4 uViewMatrix;
        uniform mat4 uProjMatrix;
        attribute vec4 aPosition;
        attribute vec4 aColor;
		attribute vec2 aTexCoord;
		varying vec2 vTexCoord;
        varying vec4 vColor;
		varying vec3 positionWorld;
        varying vec3 normalWorld;
	    varying float visibility;
	    float density = 0.0125;
	    //float gradient = 1.01;
        void main()
        {
            gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * aPosition;
			vec4 normal = vec4(aColor.x, aColor.y, aColor.z, 0.0);
			normalWorld = normalize(vec3(uModelMatrix * normal)); //normal = normalModel
			positionWorld = vec3(uModelMatrix * aPosition); //aPositon = positionModel
	        vec4 positionToCam = uViewMatrix * vec4(positionWorld, 1.0); //fog
	        float distance = length(positionToCam.xyz);
	        //float power = pow(distance*density, gradient);
	        visibility = exp(-distance*density);
	        visibility = clamp(visibility, 0.0, 1.0);
	        vColor = aColor;
			vTexCoord = aTexCoord;
        }
		)";

	return vs;
}

const std::string ShaderFile::fShader()
{
	const std::string fs = R"(
        precision mediump float;
		uniform sampler2D OurTexture;
		uniform sampler2D normalMap;
        uniform vec3 lightPosition;
		uniform vec3 eyePosition;
		uniform float ambient;
		uniform float texShift;
	    uniform vec3 fogColor;
        varying vec4 vColor;
		varying vec2 vTexCoord;
		varying vec3 positionWorld;
        varying vec3 normalWorld;
	    varying float visibility;
        void main()
        {
			vec4 normalColor = texture2D(normalMap, vTexCoord);
			//vec3 normal = vec3(normalColor.r*2.0-1.0,0.0,normalColor.g*2.0-1.0);

			vec3 r = lightPosition - positionWorld;
			vec3 lightVector = normalize(r);
	        float len = length(r);

			//specular:
			//vec3 reflectedLightVector = -reflect(lightVector,normalize(normalWorld));
			//vec3 eyeDirection = normalize(eyePosition - positionWorld);
			//float product = dot(reflectedLightVector, eyeDirection);
			//float s = clamp(product,0.0,1.0);
			//float ss = pow(s,20.0)/len;
	        //diffuse:
	        float produkt = dot(lightVector, normalWorld);
	        float ss = clamp(produkt, 0.0, 1.0) / len;
			vec3 light = vec3(ss, ss, ss) + vec3(ambient,ambient,ambient);
			//
            //gl_FragColor = vColor;
			vec2 shift = vec2(0.0, texShift);
            gl_FragColor = texture2D(OurTexture, vTexCoord + shift) * vec4(light.x,light.y,light.z, 1.0);
	        gl_FragColor = mix(vec4(fogColor, 1.0), gl_FragColor, visibility);
        }
    )";

	return fs;
}

const std::string ShaderFile::vShaderAnim()
{
	const std::string vs = R"(
        uniform mat4 uModelMatrix;
        uniform mat4 uViewMatrix;
        uniform mat4 uProjMatrix;
		uniform mat4 mBones[14];
        attribute vec4 aPosition;
        attribute vec4 aColor;
		attribute vec2 aTexCoord;
        attribute vec3 weights;
		attribute vec3 index;
		varying vec2 vTexCoord;
        varying vec4 vColor;
		varying vec3 positionWorld;
        varying vec3 normalWorld;
        void main()
        {    
			vec4 v = vec4(0.0,0.0,0.0,0.0);
			v = uModelMatrix * aPosition;
			for (int i=0; i<3; i++){
            if ((index[i]>=0.0) && (index[i]<14.0)){
            highp int idx = int(index[i]);
            v += uModelMatrix * mBones[idx] * weights[i] * aPosition;
												  }         
			}
            gl_Position = uProjMatrix * uViewMatrix * v;
			vec4 normal = vec4(aColor.x, aColor.y, aColor.z, 0.0);
			normalWorld = normalize(vec3(uModelMatrix * normal)); //normal = normalModel
			positionWorld = vec3(v); //aPositon = positionModel
            vColor = aColor;
			vTexCoord = aTexCoord;
        }
		)";

	return vs;
}

const std::string ShaderFile::fShaderAnim()
{
	const std::string fs = R"(
        precision mediump float;
		uniform sampler2D OurTexture;
		uniform sampler2D normalMap;
        uniform vec3 lightPosition;
		uniform vec3 eyePosition;
		uniform float ambient;
        varying vec4 vColor;
		varying vec2 vTexCoord;
		varying vec3 positionWorld;
        varying vec3 normalWorld;
        void main()
        {
			vec4 normalColor = texture2D(normalMap, vTexCoord);
			//vec3 normal = vec3(normalColor.r*2.0-1.0,0.0,normalColor.g*2.0-1.0);

			vec3 r = lightPosition - positionWorld;
			vec3 lightVector = normalize(r);

			//specular:
			vec3 reflectedLightVector = -reflect(lightVector,normalize(normalWorld));
			vec3 eyeDirection = normalize(eyePosition - positionWorld);
			float product = dot(reflectedLightVector, eyeDirection);
			float s = clamp(product,0.0,1.0);
			float len = length(r);
			float ss = pow(s,20.0)/len;
			vec3 light = vec3(ss, ss, ss) + vec3(ambient,ambient,ambient);
			//
            //gl_FragColor = vColor;
            gl_FragColor = texture2D(OurTexture, vTexCoord) * vec4(light.x,light.y,light.z, 1.0);
        }
    )";

	return fs;
}

const std::string ShaderFile::vShaderText()
{
	const std::string vs = R"(
        uniform float cursorX;
        uniform float cursorY;
        attribute vec4 aPosition;
        attribute vec4 aColor;
		attribute vec2 aTexCoord;
        varying vec4 vColor;
		varying vec2 vTexCoord;
        void main()
        {
            vec4 v = vec4(aPosition.x + cursorX, aPosition.y - cursorY, aPosition.z, 1.0);
			gl_Position = v;
			vTexCoord = aTexCoord;
            vColor = aColor;
        }
		)";
	return vs;
}

const std::string ShaderFile::fShaderText()
{
	const std::string fs = R"(
        precision highp float;
		uniform sampler2D ourTexture1;
        varying vec4 vColor;
        varying vec2 vTexCoord;
        void main()
        {
            vec2 v = vec2(vTexCoord.x, vTexCoord.y);
            gl_FragColor = texture2D(ourTexture1, v);
        }
        )";
	return fs;
}

const std::string ShaderFile::vShaderBlur()
{
	const std::string vs = R"(
        attribute vec4 aPosition;
        attribute vec4 aColor;
        varying vec4 vColor;
        varying vec2 horizBlurCoord[5];
        varying vec2 vertBlurCoord[5];
        void main()
        {
            float pixel = 0.02;
            vec4 v = vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
			gl_Position = v;
            vec2 ndc = (gl_Position.xy/gl_Position.w)/2.0 + 0.5;
            for (int i = -2; i<3; i++){
            float j = float(i);
            horizBlurCoord[i+2] = vec2(ndc.x + pixel * j, ndc.y);
            }
            for (int i = -2; i<3; i++){
            float j = float(i);
            vertBlurCoord[i+2] = vec2(ndc.x, ndc.y + pixel * j);
            }
			//vTexCoord = aTexCoord;
            vColor = aColor;
        }
		)";
	return vs;
}

const std::string ShaderFile::fShaderBlur()
{
	const std::string fs = R"(
        precision highp float;
		uniform sampler2D ourTexture1;
        varying vec4 vColor;
        varying vec2 horizBlurCoord[5];
        varying vec2 vertBlurCoord[5];
        void main()
        {
            // horizontal Blur: 
            vec4 horizColor = vec4(0.0,0.0,0.0,0.0);
            horizColor += texture2D(ourTexture1, horizBlurCoord[0]);
            horizColor += texture2D(ourTexture1, horizBlurCoord[1]);
            horizColor += texture2D(ourTexture1, horizBlurCoord[2]);
            horizColor += texture2D(ourTexture1, horizBlurCoord[3]);
            horizColor += texture2D(ourTexture1, horizBlurCoord[4]);
                       
            // vertical Blur: 
            vec4 vertColor = vec4(0.0,0.0,0.0,0.0);
            vertColor += texture2D(ourTexture1, vertBlurCoord[0]);
            vertColor += texture2D(ourTexture1, vertBlurCoord[1]);
            vertColor += texture2D(ourTexture1, vertBlurCoord[2]);
            vertColor += texture2D(ourTexture1, vertBlurCoord[3]);
            vertColor += texture2D(ourTexture1, vertBlurCoord[4]);

            //vec2 v = vec2(vTexCoord.x, vTexCoord.y);
            //gl_FragColor = texture2D(ourTexture1, v);
            vec4 color = mix(horizColor,vertColor,0.5);
            //float brightness = color.r + color.g + color.b;
            gl_FragColor = color;
        }
        )";
	return fs;
}

ShaderFile::ShaderFile()
{
}


ShaderFile::~ShaderFile()
{
}
