#include <Common.h>
#include <ViewManager.h>
#include <ResourcePath.h>
#include <AntTweakBar/AntTweakBar.h>
#include "CubemapShader.h"
#include "DrawModelShader.h"
#include "MyModel.h"
#include "Light.h"
#include "FrameBufferShader.h"
#include <random>


using namespace glm;
using namespace std;

const std::string ProjectName = "Robot";

GLuint			program;		//shader program
mat4			projMat;		//projection matrix
mat4			viewMat;		//projection matrix
mat4			modelMat;		//projection matrix
mat4	lightSpaceMatrix;		//light proj matrix
float			aspect;
const float		eyeHeight = 5.0f;
float			zoom = 0.f;
vec3			cameraPos(0, eyeHeight, 25.f);
vec3			globalCameraPos;

							//  xz   y    r
vec3			lightPos = vec3(0.f, 40.f, 20.f);
vec3			lightAmb = vec3(0.1f);
vec3			lightDiff = vec3(0.8f);
vec3			lightSpec = vec3(1.0f, 1.0f, 1.0f);
float			bFactor = 0.25;
float			myNoiseFactor = 1.0;
int				reverseNormal = 0;
		
GLuint			textureID;
float yawAngle = 0.0f;
float pitchAngle = 0.0f;
float lastMouseX;
float lastMouseY;
bool isLeftMousePressed = false;
int ActionIndex = 0;
int ShaderIndex = 0;
int FrameIndex = -1;
bool ResetAction = true;
int robotCount = 1;
SpotLight light;

DrawModelShader *targetShader;
FrameBufferShader * targetFrame;

CubemapShader cubemapShader;
DrawModelShader drawModelShader;
DrawModelShader sunShader;
DrawModelShader instanceShader;
FrameBufferShader frameShader;
FrameBufferShader shadowShader;
// Robot
MyModel left_foot, right_foot;
MyModel left_lower_leg, right_lower_leg;
MyModel left_upper_leg, right_upper_leg;
MyModel body_1, body_2, head;
MyModel left_shoulder, right_shoulder;
MyModel left_upper_arm, right_upper_arm;
MyModel left_lower_arm, right_lower_arm;
// Sword
MyModel sword;
// Light
MyModel sun;

TwBar* bar;

void My_Action0(float t = 0, bool reset = true, bool shadow = false);
void My_Action1(float t = 0, bool reset = true, bool shadow = false);
void My_Action2(float t = 0, bool reset = true, bool shadow = false);
void My_Action3(float t = 0, bool reset = true, bool shadow = false);
void My_Action4(float t = 0, bool reset = true, bool shadow = false);
void My_Action5(float t = 0, bool reset = true, bool shadow = false);
void My_Action6(float t = 0, bool reset = true, bool shadow = false);
void My_Action7(float t = 0, bool reset = true, bool shadow = false);
void My_Action8(float t = 0, bool reset = true, bool shadow = false);
void My_Action9(float t = 0, bool reset = true, bool shadow = false);




void Sword_Act0(float t = 0, bool reset = true, bool shadow = false);
void Sword_Act1(float t = 0, bool reset = true, bool shadow = false);
void Sword_Act2(float t = 0, bool reset = true, bool shadow = false);

void InitSkyBox()
{
	//Initialize shaders
	///////////////////////////
	std::vector<CubemapTexture> textures(6);
	/*textures[0].type = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	textures[0].fileName = ResourcePath::imagePath + "posx.jpg";
	textures[1].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	textures[1].fileName = ResourcePath::imagePath + "negx.jpg";
	textures[2].type = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	textures[2].fileName = ResourcePath::imagePath + "negy.jpg";
	textures[3].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	textures[3].fileName = ResourcePath::imagePath + "posy.jpg";
	textures[4].type = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	textures[4].fileName = ResourcePath::imagePath + "posz.jpg";
	textures[5].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	textures[5].fileName = ResourcePath::imagePath + "negz.jpg";*/

	textures[0].type = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	textures[0].fileName = ResourcePath::imagePath + "moondust_ft.tga";
	textures[1].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
	textures[1].fileName = ResourcePath::imagePath + "moondust_bk.tga";
	textures[2].type = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
	textures[2].fileName = ResourcePath::imagePath + "moondust_dn.tga";
	textures[3].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
	textures[3].fileName = ResourcePath::imagePath + "moondust_up.tga";
	textures[4].type = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
	textures[4].fileName = ResourcePath::imagePath + "moondust_rt.tga";
	textures[5].type = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	textures[5].fileName = ResourcePath::imagePath + "moondust_lf.tga";

	cubemapShader.Init(textures);
}

void InitObject()
{
	sword.InitInstanced(ResourcePath::modelPath + "Sword2.obj", 1000, ResourcePath::modelPath + "Sword.mtl");

	left_shoulder.Init(ResourcePath::modelPath + "left_shoulder.obj", ResourcePath::modelPath + "left_shoulder.mtl");		left_shoulder.CenterPivot = glm::vec3(1.2860, 9.4444, -0.2222);
	left_upper_arm.Init(ResourcePath::modelPath + "left_upper_arm.obj", ResourcePath::modelPath + "left_upper_arm.mtl");		left_upper_arm.CenterPivot = glm::vec3(2.1050, 9.0741, -0.2593);
	left_lower_arm.Init(ResourcePath::modelPath + "left_lower_arm.obj", ResourcePath::modelPath + "left_lower_arm.mtl");		left_lower_arm.CenterPivot = glm::vec3(2.2111, 7.7461, -0.2593);
	left_upper_leg.Init(ResourcePath::modelPath + "left_upper_leg.obj", ResourcePath::modelPath + "left_upper_leg.mtl");		left_upper_leg.CenterPivot = glm::vec3(0.8456, 6.4599, -0.3034);
	left_lower_leg.Init(ResourcePath::modelPath + "left_lower_leg.obj", ResourcePath::modelPath + "left_lower_leg.mtl");		left_lower_leg.CenterPivot = glm::vec3(1.1364, 3.7968, -0.2574);
	left_foot.Init(ResourcePath::modelPath + "left_foot.obj", ResourcePath::modelPath + "left_foot.obj");				left_foot.CenterPivot = glm::vec3(1.2780, 1.0789, -0.2827);

	right_shoulder.Init(ResourcePath::modelPath + "right_shoulder2.obj", ResourcePath::modelPath + "right_shoulder.mtl");		right_shoulder.CenterPivot = glm::vec3(-1.1015, 9.4654, -0.2593);
	right_upper_arm.Init(ResourcePath::modelPath + "right_upper_arm.obj", ResourcePath::modelPath + "right_upper_arm.mtl");	right_upper_arm.CenterPivot = glm::vec3(-1.8929, 9.0931, -0.2593);
	right_lower_arm.Init(ResourcePath::modelPath + "right_lower_arm.obj", ResourcePath::modelPath + "right_lower_arm.mtl");	right_lower_arm.CenterPivot = glm::vec3(-2.0595, 7.8915, -0.2222);
	right_upper_leg.Init(ResourcePath::modelPath + "right_upper_leg.obj", ResourcePath::modelPath + "right_upper_leg.mtl");	right_upper_leg.CenterPivot = glm::vec3(-0.6817, 6.3921, -0.3080);
	right_lower_leg.Init(ResourcePath::modelPath + "right_lower_leg.obj", ResourcePath::modelPath + "right_lower_leg.mtl");	right_lower_leg.CenterPivot = glm::vec3(-0.8147, 3.7724, -0.2723);
	right_foot.Init(ResourcePath::modelPath + "right_foot.obj", ResourcePath::modelPath + "right_foot.mtl");				right_foot.CenterPivot = glm::vec3(-1.1013, 1.0630, -0.2781);

	body_1.Init(ResourcePath::modelPath + "body_1.obj", ResourcePath::modelPath + "body_1.mtl");						body_1.CenterPivot = glm::vec3(0.00826, 9.0819, 0.0000);//0.0826
	body_2.Init(ResourcePath::modelPath + "body_2.obj", ResourcePath::modelPath + "body_2.mtl");						body_2.CenterPivot = glm::vec3(0.00895, 6.7826, 0.0000);//0.0895
	head.Init(ResourcePath::modelPath + "head.obj", ResourcePath::modelPath + "head.mtl");						head.CenterPivot = glm::vec3(0.0, 9.8568, 0.0000);//9.8568

	sun.Init(ResourcePath::modelPath + "sphere.obj");
}

void SetupGUI()
{
#ifdef _MSC_VER
	TwInit(TW_OPENGL, NULL);
#else
	TwInit(TW_OPENGL_CORE, NULL);
#endif
	TwGLUTModifiersFunc(glutGetModifiers);
	bar = TwNewBar("Model Parameter Setting");

	TwDefine(" 'Model Parameter Setting' size='220 200' ");
	TwDefine(" 'Model Parameter Setting' fontsize='3' color='96 216 224'");

	TwAddVarRW(bar, "light degree xz", TW_TYPE_FLOAT, &(lightPos.x), "min=0 max=360 step=10");
	TwAddVarRW(bar, "light degree y", TW_TYPE_FLOAT, &(lightPos.y), "min=0 max=360 step=10");
	TwAddVarRW(bar, "light radius", TW_TYPE_FLOAT, &(lightPos.z), "min=0 max=360 step=1");
	// TwAddVarRW(bar, "reverse normal", TW_TYPE_INT32, &(reverseNormal), "min=0 max=1 step=1");
	TwAddVarRW(bar, "bFactor", TW_TYPE_FLOAT, &(bFactor), "min=0 max=10 step=0.25");
	TwAddVarRW(bar, "myNoiseFactor", TW_TYPE_FLOAT, &(myNoiseFactor), "min=0 max=10 step=0.1");
	TwAddVarRW(bar, "robot count", TW_TYPE_INT32, &robotCount, "min=1 max=5 step=1");
}

void My_Init()
{
	/*#pragma omp parallel for
	for (int i = 0; i < 100; i++)
	{
		cout << i << endl;
	}*/
	ResourcePath::shaderPath = "./Shader/" + ProjectName + "/";
	ResourcePath::imagePath = "./Imgs/" + ProjectName + "/";
	ResourcePath::modelPath = "./Model/" + ProjectName + "/";


	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	

	drawModelShader.Init();
	drawModelShader.AddShader(GL_VERTEX_SHADER, ResourcePath::shaderPath + "drawModel.vs.glsl");
	drawModelShader.AddShader(GL_GEOMETRY_SHADER, ResourcePath::shaderPath + "drawModel.gs.glsl");
	drawModelShader.AddShader(GL_FRAGMENT_SHADER, ResourcePath::shaderPath + "drawModel.fs.glsl");
	drawModelShader.Finalize();
	drawModelShader.InitUniformLocation();

	/*coolShader[0].Init();
	coolShader[0].AddShader(GL_VERTEX_SHADER, ResourcePath::shaderPath + "cool1.vs.glsl");
	coolShader[0].AddShader(GL_FRAGMENT_SHADER, ResourcePath::shaderPath + "cool1.fs.glsl");
	coolShader[0].Finalize();
	coolShader[0].InitUniformLocation();

	coolShader[1].Init();
	coolShader[1].AddShader(GL_VERTEX_SHADER, ResourcePath::shaderPath + "cool2.vs.glsl");
	coolShader[1].AddShader(GL_FRAGMENT_SHADER, ResourcePath::shaderPath + "cool2.fs.glsl");
	coolShader[1].Finalize();
	coolShader[1].InitUniformLocation();

	coolShader[2].Init();
	coolShader[2].AddShader(GL_VERTEX_SHADER, ResourcePath::shaderPath + "cool3.vs.glsl");
	coolShader[2].AddShader(GL_FRAGMENT_SHADER, ResourcePath::shaderPath + "cool3.fs.glsl");
	coolShader[2].Finalize();
	coolShader[2].InitUniformLocation();*/

	sunShader.Init();
	sunShader.AddShader(GL_VERTEX_SHADER, ResourcePath::shaderPath + "sun2.vs.glsl");
	sunShader.AddShader(GL_FRAGMENT_SHADER, ResourcePath::shaderPath + "sun2.fs.glsl");
	sunShader.LoadTexture(ResourcePath::imagePath + "explosion.png");
	sunShader.Finalize();
	//sunShader.InitUniformLocation();

	instanceShader.Init();
	instanceShader.AddShader(GL_VERTEX_SHADER, ResourcePath::shaderPath + "InstanceModel.vs.glsl");
	instanceShader.AddShader(GL_FRAGMENT_SHADER, ResourcePath::shaderPath + "InstanceModel.fs.glsl");
	instanceShader.Finalize();

	frameShader.Init();
	frameShader.AddShader(GL_VERTEX_SHADER, ResourcePath::shaderPath + "frameShader1.vs.glsl");
	frameShader.AddShader(GL_FRAGMENT_SHADER, ResourcePath::shaderPath + "frameShader1.fs.glsl");
	frameShader.Finalize();

	shadowShader.Init(SHAOW_MAP);
	shadowShader.AddShader(GL_VERTEX_SHADER, ResourcePath::shaderPath + "depthMap.vs.glsl");
	shadowShader.AddShader(GL_FRAGMENT_SHADER, ResourcePath::shaderPath + "depthMap.fs.glsl");
	shadowShader.Finalize();
	// light.Init();
	viewMat = lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	InitSkyBox();
	InitObject();
	SetupGUI();
}

void Select_Action(float CurrentTime, bool ResetAction, bool shadow = false)
{
	switch (ActionIndex)
	{
	case 0:
		My_Action0(CurrentTime/5.0, ResetAction, shadow);
		Sword_Act0(CurrentTime/20.0, ResetAction, shadow);
		break;
	case 1:
		My_Action1(CurrentTime, ResetAction, shadow);
		break;
	case 2:
		My_Action2(CurrentTime * 5.f, ResetAction, shadow);
		break;
	case 3:
		My_Action3(CurrentTime, ResetAction, shadow);
		break;
	case 4:
		My_Action4(CurrentTime, ResetAction, shadow);
		break;
	case 5:
		My_Action5(CurrentTime, ResetAction, shadow);
		break;
	case 6:
		My_Action6(CurrentTime, ResetAction, shadow);
		break;
	case 7:
		My_Action7(CurrentTime, ResetAction, shadow);
		break;
	case 8:
		My_Action8(CurrentTime * 2, ResetAction, shadow);
		Sword_Act2(CurrentTime * 2, ResetAction, shadow);
		break;
	case 9:
		My_Action9(CurrentTime * 2, ResetAction, shadow);
		break;
	default:
		My_Action0(CurrentTime, ResetAction, shadow);
	}
}

void SetCount()
{
	right_shoulder.SetCurrentCount(robotCount);
	right_upper_arm.SetCurrentCount(robotCount);

	right_lower_arm.SetCurrentCount(robotCount);
	right_upper_leg.SetCurrentCount(robotCount);
	right_lower_leg.SetCurrentCount(robotCount);
	right_foot.SetCurrentCount(robotCount);
	right_shoulder.SetCurrentCount(robotCount);
	right_upper_arm.SetCurrentCount(robotCount);
	right_lower_arm.SetCurrentCount(robotCount);
	right_upper_leg.SetCurrentCount(robotCount);
	right_lower_leg.SetCurrentCount(robotCount);
	right_foot.SetCurrentCount(robotCount);

	left_shoulder.SetCurrentCount(robotCount);
	left_upper_arm.SetCurrentCount(robotCount);
	left_lower_arm.SetCurrentCount(robotCount);
	left_upper_leg.SetCurrentCount(robotCount);
	left_lower_leg.SetCurrentCount(robotCount);
	left_foot.SetCurrentCount(robotCount);
	right_shoulder.SetCurrentCount(robotCount);
	right_upper_arm.SetCurrentCount(robotCount);
	right_lower_arm.SetCurrentCount(robotCount);
	right_upper_leg.SetCurrentCount(robotCount);
	right_lower_leg.SetCurrentCount(robotCount);
	right_foot.SetCurrentCount(robotCount);

	body_1.SetCurrentCount(robotCount);
	body_2.SetCurrentCount(robotCount);
	head.SetCurrentCount(robotCount);
}
// GLUT callback. Called to draw the scene.
void My_Display()
{
	glm::vec3 actualLightPos = vec3(
		sin(glm::radians(lightPos.x)) * lightPos.z * glm::distance(sin(glm::radians(lightPos.y)), 1.0f),
		sin(glm::radians(lightPos.y)) * lightPos.z,
		cos(glm::radians(lightPos.x)) * lightPos.z * glm::distance(sin(glm::radians(lightPos.y)), 1.0f));
	glm::mat4 rotMat;
	rotMat = glm::rotate(rotMat, radians(pitchAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	rotMat = glm::rotate(rotMat, radians(yawAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 newCameraPos = (cameraPos + vec3(0, 0, zoom)) * glm::mat3(rotMat);
	globalCameraPos = newCameraPos;

	viewMat = lookAt(newCameraPos, vec3(0.0f, eyeHeight, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	lightSpaceMatrix = glm::perspective(glm::radians(120.f), 1.f, 0.1f, 100.f) *
		glm::lookAt(actualLightPos, glm::vec3(0), glm::vec3(0.0, 1.0, 0.0));
	SetCount();

	glm::mat4 mvMat = viewMat * modelMat;
	float CurrentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.f;

	// 清除主要buffer的內容
	glViewport(0, 0, 800, 800);
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 設定機器人shader
	targetShader = &drawModelShader;
	if (ShaderIndex <= 10)
	{
		targetShader = &drawModelShader;
		targetShader->Enable();
		targetShader->setUniformFloat("eta", 1.0f / 1.52f);
		targetShader->setUniformInt("mode", ShaderIndex);
		targetShader->setUniformVec2("resolution", glm::vec2(800, 800));
		targetShader->setUniformVec3("light.position", actualLightPos);
		targetShader->setUniformVec3("light.ambient", lightAmb);
		targetShader->setUniformVec3("light.diffuse", lightDiff);
		targetShader->setUniformVec3("light.specular", lightSpec);
		targetShader->setUniformFloat("time", CurrentTime);
		targetShader->setUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
		targetShader->setUniformInt("texs", 0);
		targetShader->setUniformInt("shadowMap", 1);


		targetShader->SetPMat(projMat);
		targetShader->SetViewMat4(viewMat);
		targetShader->SetCameraPos(newCameraPos);
		targetShader->Disable();
	}

	// 設定劍shader
	instanceShader.Enable();
	instanceShader.setUniformVec3("cameraPos", globalCameraPos);
	instanceShader.setUniformVec3("light.position", lightPos);
	instanceShader.setUniformVec3("light.ambient", lightAmb);
	instanceShader.setUniformVec3("light.diffuse", lightDiff);
	instanceShader.setUniformVec3("light.specular", lightSpec);
	instanceShader.setUniformFloat("eta", 1.f / 1.52f);
	instanceShader.setUniformInt("reflectOrRefract", true);
	instanceShader.setUniformMat4("lightSpaceMatrix", lightSpaceMatrix);
	instanceShader.setUniformInt("tex", 0);
	instanceShader.setUniformInt("shadowMap", 1);
	instanceShader.Disable();

	// Generate Shadow Map
	shadowShader.genShadowMapBegin();
	{
		

		Select_Action(CurrentTime, ResetAction, true);

		glCullFace(GL_FRONT);
		glDisable(GL_CULL_FACE);
	}
	shadowShader.genShadowMapEnd();

	Common::PrintGLError();

	// 設定火球shader
	glm::mat4 sunModel;
	sunModel = glm::translate(sunModel, actualLightPos);
	sunModel = glm::scale(sunModel, vec3(1.0));
	sunShader.Enable();
	sunShader.setUniformFloat("time", 0.25f * CurrentTime);
	sunShader.setUniformMat4("um4mv", viewMat * sunModel);
	sunShader.setUniformMat4("um4p", projMat);
	sunShader.setUniformInt("tExplosion", 0);
	sunShader.setUniformFloat("myNoiseFactor", myNoiseFactor);
	sunShader.setUniformFloat("bFactor", bFactor);

	sunShader.Disable();

	if (FrameIndex != -1)
	{
		// 設定frame buffer shader
		switch (FrameIndex)
		{
		case -1:
			targetFrame = NULL;
			break;
		case 0:// 負片
		case 1:// 灰階
		case 2:// 高斯
		case 3:// 銳利化
		case 4:// 邊緣檢測
		case 5:// Cycle
			targetFrame = &frameShader;
			targetFrame->Enable();
			targetFrame->setUniformInt("mode", FrameIndex + 1);
			targetFrame->setUniformFloat("time", CurrentTime * 20.f);
			break;
		default:
			targetFrame = NULL;
		}

		// 綁framebuffer
		targetFrame->bindFrameBuffer();
		glViewport(0, 0, 800, 800);
		glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 繪製skybox
		cubemapShader.Enable();
		cubemapShader.SetMVMat(glm::mat4(glm::mat3(viewMat)));
		cubemapShader.SetPMat(projMat);
		cubemapShader.Render();
		cubemapShader.Disable();

		// 繪製機器人
		targetShader->Enable();
		cubemapShader.UseTexture(true);
		Select_Action(CurrentTime, ResetAction);
		cubemapShader.UseTexture(false);
		targetShader->Disable();

		// 繪製火球
		sunShader.Enable();
		glBindTexture(GL_TEXTURE_2D, sunShader.textures.front());
		sun.Render();
		glBindTexture(GL_TEXTURE_2D, 0);
		sunShader.Disable();

		// 解綁framebuffer
		targetFrame->unbindFrameBuffer();

		glDisable(GL_DEPTH_TEST);
		targetFrame->Enable();
		targetFrame->setUniformInt("screenTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, targetFrame->getTextureId());

		targetFrame->RenderResult();
		targetFrame->Disable();
		glEnable(GL_DEPTH_TEST);


	}
	else
	{
		shadowShader.setupShadowMap();
		{
			// 繪製機器人
			targetShader->Enable();
			targetShader->SetViewMat4(viewMat);
			targetShader->SetPMat(projMat);

			glActiveTexture(GL_TEXTURE0);
			cubemapShader.UseTexture(true);

			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, shadowShader.getTextureId());

			Select_Action(CurrentTime, ResetAction);

			cubemapShader.UseTexture(false);
			targetShader->Disable();
		}

		// 繪製skybox
		cubemapShader.Enable();
		cubemapShader.SetMVMat(glm::mat4(glm::mat3(viewMat)));
		cubemapShader.SetPMat(projMat);
		cubemapShader.Render();
		cubemapShader.Disable();

		// 繪製火球
		sunShader.Enable();
		glBindTexture(GL_TEXTURE_2D, sunShader.textures.front());
		sun.Render();
		glBindTexture(GL_TEXTURE_2D, 0);
		sunShader.Disable();
	}

	if (ResetAction) 
		ResetAction = false;

	TwDraw();
    glutSwapBuffers();
}

// Stand
void My_Action0(float t, bool reset, bool shadow)
{
	targetShader->Enable();
	if (shadow)
	{
		targetShader->SetPMat(lightSpaceMatrix);
		targetShader->SetViewMat4(mat4());
	}
	else
	{
		targetShader->SetPMat(projMat);
		targetShader->SetViewMat4(viewMat);
	}
	glm::mat4 customMmat;
	float f;
	int i;
	t = t * 5.f;
	static float baseTime = t;
	if (reset)
		baseTime = t;
	t = t - baseTime;

	// 身體1
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_1.CenterPivot);
	customMmat = glm::translate(customMmat, -body_1.CenterPivot);
	body_1.setParentModel(mat4());
	body_1.setCurrentModel(customMmat);
	targetShader->SetMMat(body_1.getModelMatrix());
	body_1.Render();

	// 頭
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, head.CenterPivot);
	customMmat = glm::translate(customMmat, -head.CenterPivot);
	head.setParentModel(body_1.getModelMatrix());
	head.setCurrentModel(customMmat);
	targetShader->SetMMat(head.getModelMatrix());
	head.Render();

	// 身體2
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_2.CenterPivot);
	customMmat = glm::translate(customMmat, -body_2.CenterPivot);
	body_2.setParentModel(body_1.getModelMatrix());
	body_2.setCurrentModel(customMmat);
	targetShader->SetMMat(body_2.getModelMatrix());
	body_2.Render();

	// 右肩
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_shoulder.CenterPivot);
	customMmat = glm::translate(customMmat, -right_shoulder.CenterPivot);
	right_shoulder.setParentModel(body_1.getModelMatrix());
	right_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(right_shoulder.getModelMatrix());
	right_shoulder.Render();

	// 右上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_arm.CenterPivot);
	customMmat = glm::translate(customMmat, -right_upper_arm.CenterPivot);
	right_upper_arm.setParentModel(right_shoulder.getModelMatrix());
	right_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_arm.getModelMatrix());
	right_upper_arm.Render();

	// 右下臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_arm.CenterPivot);
	customMmat = glm::translate(customMmat, -right_lower_arm.CenterPivot);
	right_lower_arm.setParentModel(right_upper_arm.getModelMatrix());
	right_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_arm.getModelMatrix());
	right_lower_arm.Render();

	// 左肩
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_shoulder.CenterPivot);
	customMmat = glm::translate(customMmat, -left_shoulder.CenterPivot);
	left_shoulder.setParentModel(body_1.getModelMatrix());
	left_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(left_shoulder.getModelMatrix());
	left_shoulder.Render();

	// 左上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_arm.CenterPivot);
	customMmat = glm::translate(customMmat, -left_upper_arm.CenterPivot);
	left_upper_arm.setParentModel(left_shoulder.getModelMatrix());
	left_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_arm.getModelMatrix());
	left_upper_arm.Render();

	// 左下臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_arm.CenterPivot);
	customMmat = glm::translate(customMmat, -left_lower_arm.CenterPivot);
	left_lower_arm.setParentModel(left_upper_arm.getModelMatrix());
	left_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_arm.getModelMatrix());
	left_lower_arm.Render();

	// 左上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_leg.CenterPivot);
	customMmat = glm::translate(customMmat, -left_upper_leg.CenterPivot);
	left_upper_leg.setParentModel(body_2.getModelMatrix());
	left_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_leg.getModelMatrix());
	left_upper_leg.Render();

	// 左下腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_leg.CenterPivot);
	customMmat = glm::translate(customMmat, -left_lower_leg.CenterPivot);
	left_lower_leg.setParentModel(left_upper_leg.getModelMatrix());
	left_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_leg.getModelMatrix());
	left_lower_leg.Render();

	// 左腳
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_foot.CenterPivot);
	customMmat = glm::translate(customMmat, -left_foot.CenterPivot);
	left_foot.setParentModel(left_lower_leg.getModelMatrix());
	left_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(left_foot.getModelMatrix());
	left_foot.Render();

	// 右上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_leg.CenterPivot);
	customMmat = glm::translate(customMmat, -right_upper_leg.CenterPivot);
	right_upper_leg.setParentModel(body_2.getModelMatrix());
	right_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_leg.getModelMatrix());
	right_upper_leg.Render();

	// 右下腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_leg.CenterPivot);
	customMmat = glm::translate(customMmat, -right_lower_leg.CenterPivot);
	right_lower_leg.setParentModel(right_upper_leg.getModelMatrix());
	right_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_leg.getModelMatrix());
	right_lower_leg.Render();

	// 右腳
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_foot.CenterPivot);
	customMmat = glm::translate(customMmat, -right_foot.CenterPivot);
	right_foot.setParentModel(right_lower_leg.getModelMatrix());
	right_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(right_foot.getModelMatrix());
	right_foot.Render();
	targetShader->Disable();

}

// Walk
void My_Action1(float t, bool reset, bool shadow)
{
	targetShader->Enable();
	if (shadow)
	{
		targetShader->SetPMat(lightSpaceMatrix);
		targetShader->SetViewMat4(mat4());
	}
	else
	{
		targetShader->SetPMat(projMat);
		targetShader->SetViewMat4(viewMat);
	}
	glm::mat4 customMmat; float f; int i;
	t = t * 5.f;
	static float baseTime = t;
	if (reset)
		baseTime = t;
	t = t - baseTime;
	// 身體1
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_1.CenterPivot);
	//customMmat = glm::rotate(customMmat, glm::radians(-30.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -body_1.CenterPivot);
	body_1.setParentModel(glm::mat4());
	body_1.setCurrentModel(customMmat);
	targetShader->SetMMat(body_1.getModelMatrix());
	body_1.Render();

	// 頭
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, head.CenterPivot);
	// customMmat = glm::rotate(customMmat, glm::radians(-30.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -head.CenterPivot);
	head.setParentModel(body_1.getModelMatrix());
	head.setCurrentModel(customMmat);
	targetShader->SetMMat(head.getModelMatrix());
	head.Render();

	// 身體2
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_2.CenterPivot);
	//customMmat = glm::rotate(customMmat, glm::radians(-30.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -body_2.CenterPivot);
	body_2.setParentModel(body_1.getModelMatrix());
	body_2.setCurrentModel(customMmat);
	targetShader->SetMMat(body_2.getModelMatrix());
	body_2.Render();

	// 右肩
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_shoulder.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(30.f * glm::sin(t)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_shoulder.CenterPivot);
	right_shoulder.setParentModel(body_1.getModelMatrix());
	right_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(right_shoulder.getModelMatrix());
	right_shoulder.Render();

		// 右上臂
		customMmat = glm::mat4();
		customMmat = glm::translate(customMmat, right_upper_arm.CenterPivot);
		//customMmat = glm::rotate(customMmat, glm::radians(30.f), glm::vec3(0, 0, 1));
		customMmat = glm::translate(customMmat, -right_upper_arm.CenterPivot);
		right_upper_arm.setParentModel(right_shoulder.getModelMatrix());
		right_upper_arm.setCurrentModel(customMmat);
		targetShader->SetMMat(right_upper_arm.getModelMatrix());
		right_upper_arm.Render();

			// 右下臂
			customMmat = glm::mat4();
			customMmat = glm::translate(customMmat, right_lower_arm.CenterPivot);
			//customMmat = glm::rotate(customMmat, glm::radians(30.f), glm::vec3(0, 0, 1));
			customMmat = glm::translate(customMmat, -right_lower_arm.CenterPivot);
			right_lower_arm.setParentModel(right_upper_arm.getModelMatrix());
			right_lower_arm.setCurrentModel(customMmat);
			targetShader->SetMMat(right_lower_arm.getModelMatrix());
			right_lower_arm.Render();

	// 左肩
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_shoulder.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-30.f * glm::sin(t)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_shoulder.CenterPivot);
	left_shoulder.setParentModel(body_1.getModelMatrix());
	left_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(left_shoulder.getModelMatrix());
	left_shoulder.Render();

		// 左上臂
		customMmat = glm::mat4();
		customMmat = glm::translate(customMmat, left_upper_arm.CenterPivot);
		//customMmat = glm::rotate(customMmat, glm::radians(-30.f), glm::vec3(1, 0, 0));
		customMmat = glm::translate(customMmat, -left_upper_arm.CenterPivot);
		left_upper_arm.setParentModel(left_shoulder.getModelMatrix());
		left_upper_arm.setCurrentModel(customMmat);
		targetShader->SetMMat(left_upper_arm.getModelMatrix());
		left_upper_arm.Render();

			// 左下臂
			customMmat = glm::mat4();
			customMmat = glm::translate(customMmat, left_lower_arm.CenterPivot);
			//customMmat = glm::rotate(customMmat, glm::radians(-30.f), glm::vec3(1, 0, 0));
			customMmat = glm::translate(customMmat, -left_lower_arm.CenterPivot);
			left_lower_arm.setParentModel(left_upper_arm.getModelMatrix());
			left_lower_arm.setCurrentModel(customMmat);
			targetShader->SetMMat(left_lower_arm.getModelMatrix());
			left_lower_arm.Render();

	// 左上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(45.f * glm::sin(t)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_upper_leg.CenterPivot);
	left_upper_leg.setParentModel(body_2.getModelMatrix());
	left_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_leg.getModelMatrix());
	left_upper_leg.Render();

		// 左下腿
		f = -sin(t) < 0 ? sin(t) / 5.f : -sin(t);
		customMmat = glm::mat4();
		customMmat = glm::translate(customMmat, left_lower_leg.CenterPivot);
		customMmat = glm::rotate(customMmat, glm::radians(45.f * f), glm::vec3(1, 0, 0));
		customMmat = glm::translate(customMmat, -left_lower_leg.CenterPivot);
		left_lower_leg.setParentModel(left_upper_leg.getModelMatrix());
		left_lower_leg.setCurrentModel(customMmat);
		targetShader->SetMMat(left_lower_leg.getModelMatrix());
		left_lower_leg.Render();

			// 左腳
			f = -cos(t) < 0 ? 0 : -cos(t);
			customMmat = glm::mat4();
			customMmat = glm::translate(customMmat, left_foot.CenterPivot);
			//customMmat = glm::rotate(customMmat, glm::radians(-45.f * f), glm::vec3(1, 0, 0));
			customMmat = glm::translate(customMmat, -left_foot.CenterPivot);
			left_foot.setParentModel(left_lower_leg.getModelMatrix());
			left_foot.setCurrentModel(customMmat);
			targetShader->SetMMat(left_foot.getModelMatrix());
			left_foot.Render();

	// 右上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-45.f * glm::sin(t)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_upper_leg.CenterPivot);
	right_upper_leg.setParentModel(body_2.getModelMatrix());
	right_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_leg.getModelMatrix());
	right_upper_leg.Render();

		// 右下腿
		f = sin(t) < 0 ? -sin(t) / 5.f : sin(t);
		customMmat = glm::mat4();
		customMmat = glm::translate(customMmat, right_lower_leg.CenterPivot);
		customMmat = glm::rotate(customMmat, glm::radians(45.f * f), glm::vec3(1, 0, 0));
		customMmat = glm::translate(customMmat, -right_lower_leg.CenterPivot);
		right_lower_leg.setParentModel(right_upper_leg.getModelMatrix());
		right_lower_leg.setCurrentModel(customMmat);
		targetShader->SetMMat(right_lower_leg.getModelMatrix());
		right_lower_leg.Render();

			// 右腳
			f = cos(t) < 0 ? 0 : cos(t);
			customMmat = glm::mat4();
			customMmat = glm::translate(customMmat, right_foot.CenterPivot);
			// customMmat = glm::rotate(customMmat, glm::radians(-45.f * f), glm::vec3(1, 0, 0));
			customMmat = glm::translate(customMmat, -right_foot.CenterPivot);
			right_foot.setParentModel(right_lower_leg.getModelMatrix());
			right_foot.setCurrentModel(customMmat);
			targetShader->SetMMat(right_foot.getModelMatrix());
			right_foot.Render();
			targetShader->Disable();
}

// Punch Land
void My_Action2(float t, bool reset, bool shadow)
{
	targetShader->Enable();
	if (shadow)
	{
		targetShader->SetPMat(lightSpaceMatrix);
		targetShader->SetViewMat4(mat4());
	}
	else
	{
		targetShader->SetPMat(projMat);
		targetShader->SetViewMat4(viewMat);
	}
	glm::mat4 customMmat;
	float f;
	int i;
	static float baseTime = t;

	float waitV2 = 1.f * 3.f;
	if (reset || t - baseTime > 3.1415926 + waitV2 + 3.1415926 + 7.f)
		baseTime = t;
	t = t - baseTime;

	float timeV2 = t, timeV3 = t;
	if (t > 3.1415926)
		t = 3.1415926;

	if (timeV2 > t + waitV2 + 3.1415926)
		timeV2 = t + waitV2 + 3.1415926;

	{
	// 身體2
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, glm::vec3(0, -1.f + 1.f * cos(t), 0));
	customMmat = glm::translate(customMmat, body_2.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(30.f - 30.f * cos(t)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -body_2.CenterPivot);
	body_2.setParentModel(glm::mat4());
	body_2.setCurrentModel(customMmat);
	targetShader->SetMMat(body_2.getModelMatrix());
	body_2.Render();

	// 身體1
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_1.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(30.f - 30.f * cos(t)), glm::vec3(0, 1, 0));
	customMmat = glm::translate(customMmat, -body_1.CenterPivot);
	body_1.setParentModel(body_2.getModelMatrix());
	body_1.setCurrentModel(customMmat);
	targetShader->SetMMat(body_1.getModelMatrix());
	body_1.Render();

	// 頭
	float headTime = timeV2 - t > waitV2 ? timeV2 - t - waitV2 : 0;
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, head.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-22.5f + 22.5f * cos(headTime)), glm::vec3(0, 1, 0));
	customMmat = glm::rotate(customMmat, glm::radians(-30.f + 30.f * cos(headTime)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -head.CenterPivot);
	head.setParentModel(body_1.getModelMatrix());
	head.setCurrentModel(customMmat);
	targetShader->SetMMat(head.getModelMatrix());
	head.Render();



	// 右肩
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_shoulder.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-45.f), glm::vec3(1, 0, 0));
	// customMmat = glm::rotate(customMmat, glm::radians(-10.f - 19.f * cos(jojo)), glm::vec3(0, 0, 1));
	customMmat = glm::translate(customMmat, -right_shoulder.CenterPivot);
	right_shoulder.setParentModel(body_1.getModelMatrix());
	right_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(right_shoulder.getModelMatrix());
	right_shoulder.Render();

	// 右上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-30.f - 15.f - 15.f * cos(t)), glm::vec3(0, 0, 1));
	customMmat = glm::rotate(customMmat, glm::radians(90.f), glm::vec3(0, 1, 0));
	customMmat = glm::translate(customMmat, -right_upper_arm.CenterPivot);
	right_upper_arm.setParentModel(right_shoulder.getModelMatrix());
	right_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_arm.getModelMatrix());
	right_upper_arm.Render();

	// 右下臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-30.f - 30.f * cos(t)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_lower_arm.CenterPivot);
	right_lower_arm.setParentModel(right_upper_arm.getModelMatrix());
	right_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_arm.getModelMatrix());
	right_lower_arm.Render();

	// 左肩
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_shoulder.CenterPivot);
	//customMmat = glm::rotate(customMmat, glm::radians(-30.f * glm::sin(t)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_shoulder.CenterPivot);
	left_shoulder.setParentModel(body_1.getModelMatrix());
	left_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(left_shoulder.getModelMatrix());
	left_shoulder.Render();

	// 左上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_arm.CenterPivot);
	//customMmat = glm::rotate(customMmat, glm::radians(-30.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_upper_arm.CenterPivot);
	left_upper_arm.setParentModel(left_shoulder.getModelMatrix());
	left_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_arm.getModelMatrix());
	left_upper_arm.Render();

	// 左下臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_arm.CenterPivot);
	//customMmat = glm::rotate(customMmat, glm::radians(-30.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_lower_arm.CenterPivot);
	left_lower_arm.setParentModel(left_upper_arm.getModelMatrix());
	left_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_arm.getModelMatrix());
	left_lower_arm.Render();

	// 左上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, glm::vec3(0, -1.f + 1.f * cos(t), 0));
	customMmat = glm::translate(customMmat, left_upper_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-30.f + 30.f * cos(t)), glm::vec3(1, 0, 0));
	customMmat = glm::rotate(customMmat, glm::radians(15.f - 15.f * cos(t)), glm::vec3(0, 0, 1));
	customMmat = glm::translate(customMmat, -left_upper_leg.CenterPivot);
	left_upper_leg.setParentModel(glm::mat4());
	left_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_leg.getModelMatrix());
	left_upper_leg.Render();

	// 左下腿
	f = -sin(t) < 0 ? 0 : -sin(t);
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(35.f - 35.f * cos(t)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_lower_leg.CenterPivot);
	left_lower_leg.setParentModel(left_upper_leg.getModelMatrix());
	left_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_leg.getModelMatrix());
	left_lower_leg.Render();

	// 左腳
	f = -cos(t) < 0 ? 0 : -cos(t);
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_foot.CenterPivot);
	// customMmat = glm::rotate(customMmat, glm::radians(22.f * f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_foot.CenterPivot);
	left_foot.setParentModel(left_lower_leg.getModelMatrix());
	left_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(left_foot.getModelMatrix());
	left_foot.Render();

	// 右上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, glm::vec3(0, -1.f + 1.f * cos(t), 0));
	customMmat = glm::translate(customMmat, right_upper_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-30.f + 30.f * cos(t)), glm::vec3(1, 0, 0));
	customMmat = glm::rotate(customMmat, glm::radians(-15.f + 15.f * cos(t)), glm::vec3(0, 0, 1));
	customMmat = glm::translate(customMmat, -right_upper_leg.CenterPivot);
	right_upper_leg.setParentModel(glm::mat4());
	right_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_leg.getModelMatrix());
	right_upper_leg.Render();

	// 右下腿
	f = sin(t) < 0 ? 0 : sin(t);
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(35.f - 35.f * cos(t)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_lower_leg.CenterPivot);
	right_lower_leg.setParentModel(right_upper_leg.getModelMatrix());
	right_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_leg.getModelMatrix());
	right_lower_leg.Render();

	// 右腳
	f = cos(t) < 0 ? 0 : cos(t);
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_foot.CenterPivot);
	// customMmat = glm::rotate(customMmat, glm::radians(22.f * f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_foot.CenterPivot);
	right_foot.setParentModel(right_lower_leg.getModelMatrix());
	right_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(right_foot.getModelMatrix());
	right_foot.Render();
	}

	if (timeV3 > t + waitV2 + 2.8) {
		if (timeV3 > t + waitV2 + 3.0)
			Sword_Act1(timeV3, false);
		else
			Sword_Act1(timeV3, true);
	}
	targetShader->Disable();
}

// Clap Jump
void My_Action3(float t, bool reset, bool shadow)
{
	targetShader->Enable();
	if (shadow)
	{
		targetShader->SetPMat(lightSpaceMatrix);
		targetShader->SetViewMat4(mat4());
	}
	else
	{
		targetShader->SetPMat(projMat);
		targetShader->SetViewMat4(viewMat);
	}
	glm::mat4 customMmat;
	float f;
	int i;
	t = t * 5.f;
	static float baseTime = t;
	if (reset)
		baseTime = t;
	t = t - baseTime;

	// 身體1
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_1.CenterPivot + glm::vec3(0, 1.0f * -cos(t), 0));
	customMmat = glm::translate(customMmat, -body_1.CenterPivot);
	body_1.setParentModel(glm::mat4());
	body_1.setCurrentModel(customMmat);
	targetShader->SetMMat(body_1.getModelMatrix());
	body_1.Render();

	// 頭
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, head.CenterPivot);
	customMmat = glm::translate(customMmat, -head.CenterPivot);
	head.setParentModel(body_1.getModelMatrix());
	head.setCurrentModel(customMmat);
	targetShader->SetMMat(head.getModelMatrix());
	head.Render();

	// 身體2
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_2.CenterPivot);
	customMmat = glm::translate(customMmat, -body_2.CenterPivot);
	body_2.setParentModel(body_1.getModelMatrix());
	body_2.setCurrentModel(customMmat);
	targetShader->SetMMat(body_2.getModelMatrix());
	body_2.Render();

	// 右肩
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_shoulder.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-75.f + 75.f * cos(t)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_shoulder.CenterPivot);
	right_shoulder.setParentModel(body_1.getModelMatrix());
	right_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(right_shoulder.getModelMatrix());
	right_shoulder.Render();

	// 右上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-30.f - 45.f * cos(t)), glm::vec3(0, 0, 1));
	customMmat = glm::translate(customMmat, -right_upper_arm.CenterPivot);
	right_upper_arm.setParentModel(right_shoulder.getModelMatrix());
	right_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_arm.getModelMatrix());
	right_upper_arm.Render();

	// 右下臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-22.5f + 22.5f * cos(t)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_lower_arm.CenterPivot);
	right_lower_arm.setParentModel(right_upper_arm.getModelMatrix());
	right_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_arm.getModelMatrix());
	right_lower_arm.Render();

	// 左肩
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_shoulder.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-75.f + 75.f * cos(t)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_shoulder.CenterPivot);
	left_shoulder.setParentModel(body_1.getModelMatrix());
	left_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(left_shoulder.getModelMatrix());
	left_shoulder.Render();

	// 左上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(30.f + 45.f * cos(t)), glm::vec3(0, 0, 1));
	customMmat = glm::translate(customMmat, -left_upper_arm.CenterPivot);
	left_upper_arm.setParentModel(left_shoulder.getModelMatrix());
	left_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_arm.getModelMatrix());
	left_upper_arm.Render();

	// 左下臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-22.5f + 22.5f * cos(t)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_lower_arm.CenterPivot);
	left_lower_arm.setParentModel(left_upper_arm.getModelMatrix());
	left_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_arm.getModelMatrix());
	left_lower_arm.Render();

	// 左上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(22.5f - 22.5f * cos(t)), glm::vec3(0, 0, 1));
	customMmat = glm::translate(customMmat, -left_upper_leg.CenterPivot);
	left_upper_leg.setParentModel(body_2.getModelMatrix());
	left_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_leg.getModelMatrix());
	left_upper_leg.Render();

	// 左下腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_leg.CenterPivot);
	customMmat = glm::translate(customMmat, -left_lower_leg.CenterPivot);
	left_lower_leg.setParentModel(left_upper_leg.getModelMatrix());
	left_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_leg.getModelMatrix());
	left_lower_leg.Render();

	// 左腳
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_foot.CenterPivot);
	customMmat = glm::translate(customMmat, -left_foot.CenterPivot);
	left_foot.setParentModel(left_lower_leg.getModelMatrix());
	left_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(left_foot.getModelMatrix());
	left_foot.Render();

	// 右上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-22.5f + 22.5f * cos(t)), glm::vec3(0, 0, 1));
	customMmat = glm::translate(customMmat, -right_upper_leg.CenterPivot);
	right_upper_leg.setParentModel(body_2.getModelMatrix());
	right_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_leg.getModelMatrix());
	right_upper_leg.Render();

	// 右下腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_leg.CenterPivot);
	customMmat = glm::translate(customMmat, -right_lower_leg.CenterPivot);
	right_lower_leg.setParentModel(right_upper_leg.getModelMatrix());
	right_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_leg.getModelMatrix());
	right_lower_leg.Render();

	// 右腳
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_foot.CenterPivot);
	customMmat = glm::translate(customMmat, -right_foot.CenterPivot);
	right_foot.setParentModel(right_lower_leg.getModelMatrix());
	right_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(right_foot.getModelMatrix());
	right_foot.Render();
	targetShader->Disable();
}

// Punch Front
void My_Action4(float t, bool reset, bool shadow)
{
	targetShader->Enable();
	if (shadow)
	{
		targetShader->SetPMat(lightSpaceMatrix);
		targetShader->SetViewMat4(mat4());
	}
	else
	{
		targetShader->SetPMat(projMat);
		targetShader->SetViewMat4(viewMat);
	}
	glm::mat4 customMmat;
	float f, f1, f2, f3;
	int i;
	t = t * 10.f;
	static float baseTime = t;
	if (reset)
		baseTime = t;
	t = t - baseTime;

	// 身體2
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_2.CenterPivot);
	customMmat = glm::translate(customMmat, -body_2.CenterPivot);
	body_2.setParentModel(glm::mat4());
	body_2.setCurrentModel(customMmat);
	targetShader->SetMMat(body_2.getModelMatrix());
	body_2.Render();

	// 身體1
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_1.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(7.5f * cos(t)), glm::vec3(0, 1, 0));
	customMmat = glm::translate(customMmat, -body_1.CenterPivot);
	body_1.setParentModel(body_2.getModelMatrix());
	body_1.setCurrentModel(customMmat);
	targetShader->SetMMat(body_1.getModelMatrix());
	body_1.Render();

	// 頭
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, head.CenterPivot);
	customMmat = glm::translate(customMmat, -head.CenterPivot);
	head.setParentModel(body_1.getModelMatrix());
	head.setCurrentModel(customMmat);
	targetShader->SetMMat(head.getModelMatrix());
	head.Render();

	

	// 右肩
	f1 = sin(t) > 0 ? sin(t) : 0;
	f2 = 1 - f1;
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_shoulder.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-80.f * f1), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_shoulder.CenterPivot);
	right_shoulder.setParentModel(body_1.getModelMatrix());
	right_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(right_shoulder.getModelMatrix());
	right_shoulder.Render();

	// 右上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(15.f - 30.f * f2), glm::vec3(0, 0, 1));
	customMmat = glm::translate(customMmat, -right_upper_arm.CenterPivot);
	right_upper_arm.setParentModel(right_shoulder.getModelMatrix());
	right_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_arm.getModelMatrix());
	right_upper_arm.Render();

	// 右下臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-120.f * f2), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_lower_arm.CenterPivot);
	right_lower_arm.setParentModel(right_upper_arm.getModelMatrix());
	right_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_arm.getModelMatrix());
	right_lower_arm.Render();

	// 左肩
	f1 = sin(t + 3.14159) > 0 ? sin(t + 3.14159) : 0;
	f2 = 1 - f1;
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_shoulder.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-80.f * f1), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_shoulder.CenterPivot);
	left_shoulder.setParentModel(body_1.getModelMatrix());
	left_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(left_shoulder.getModelMatrix());
	left_shoulder.Render();

	// 左上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-15.f + 30.f * f2), glm::vec3(0, 0, 1));
	customMmat = glm::translate(customMmat, -left_upper_arm.CenterPivot);
	left_upper_arm.setParentModel(left_shoulder.getModelMatrix());
	left_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_arm.getModelMatrix());
	left_upper_arm.Render();

	// 左下臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-120.f * f2), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_lower_arm.CenterPivot);
	left_lower_arm.setParentModel(left_upper_arm.getModelMatrix());
	left_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_arm.getModelMatrix());
	left_lower_arm.Render();

	// 左上腿
	f = t;
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-20.f + 20.f * sin(f)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_upper_leg.CenterPivot);
	left_upper_leg.setParentModel(body_2.getModelMatrix());
	left_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_leg.getModelMatrix());
	left_upper_leg.Render();

	// 左下腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(20.f - 20.f * sin(f)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_lower_leg.CenterPivot);
	left_lower_leg.setParentModel(left_upper_leg.getModelMatrix());
	left_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_leg.getModelMatrix());
	left_lower_leg.Render();

	// 左腳
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_foot.CenterPivot);
	customMmat = glm::translate(customMmat, -left_foot.CenterPivot);
	left_foot.setParentModel(left_lower_leg.getModelMatrix());
	left_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(left_foot.getModelMatrix());
	left_foot.Render();

	// 右上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-20.f + 20.f * sin(f + 3.14159f)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_upper_leg.CenterPivot);
	right_upper_leg.setParentModel(body_2.getModelMatrix());
	right_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_leg.getModelMatrix());
	right_upper_leg.Render();

	// 右下腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(20.f - 20.f * sin(f + 3.14159f)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_lower_leg.CenterPivot);
	right_lower_leg.setParentModel(right_upper_leg.getModelMatrix());
	right_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_leg.getModelMatrix());
	right_lower_leg.Render();

	// 右腳
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_foot.CenterPivot);
	customMmat = glm::translate(customMmat, -right_foot.CenterPivot);
	right_foot.setParentModel(right_lower_leg.getModelMatrix());
	right_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(right_foot.getModelMatrix());
	right_foot.Render();
	targetShader->Disable();
}

// Sorry Sorry
void My_Action5(float t, bool reset, bool shadow)
{
	targetShader->Enable();
	if (shadow)
	{
		targetShader->SetPMat(lightSpaceMatrix);
		targetShader->SetViewMat4(mat4());
	}
	else
	{
		targetShader->SetPMat(projMat);
		targetShader->SetViewMat4(viewMat);
	}
	glm::mat4 customMmat;
	float f;
	int i;
	t = t * 5.f;
	static float baseTime = t;
	if (reset)
		baseTime = t;
	t = t - baseTime;

	// 身體2
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_2.CenterPivot + glm::vec3(0, -0.5 * cos(t), 0));
	customMmat = glm::translate(customMmat, -body_2.CenterPivot);
	body_2.setParentModel(glm::mat4());
	body_2.setCurrentModel(customMmat);
	targetShader->SetMMat(body_2.getModelMatrix());
	body_2.Render();

	// 身體1
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_1.CenterPivot + glm::vec3(0, 0, 0.4));

	customMmat = glm::rotate(customMmat, glm::radians(30.f * cos(t / 2.f)), glm::vec3(0, 1, 0));
	customMmat = glm::rotate(customMmat, glm::radians(15.f), glm::vec3(1, 0, 0));

	customMmat = glm::translate(customMmat, -body_1.CenterPivot);
	body_1.setParentModel(body_2.getModelMatrix());
	body_1.setCurrentModel(customMmat);
	targetShader->SetMMat(body_1.getModelMatrix());
	body_1.Render();

	// 頭
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, head.CenterPivot);
	customMmat = glm::translate(customMmat, -head.CenterPivot);
	head.setParentModel(body_1.getModelMatrix());
	head.setCurrentModel(customMmat);
	targetShader->SetMMat(head.getModelMatrix());
	head.Render();

	// 右肩
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_shoulder.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-30.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_shoulder.CenterPivot);
	right_shoulder.setParentModel(body_1.getModelMatrix());
	right_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(right_shoulder.getModelMatrix());
	right_shoulder.Render();

	// 右上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(30.f), glm::vec3(0, 0, 1));
	customMmat = glm::rotate(customMmat, glm::radians(30.f), glm::vec3(0, 1, 0));
	customMmat = glm::translate(customMmat, -right_upper_arm.CenterPivot);
	right_upper_arm.setParentModel(right_shoulder.getModelMatrix());
	right_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_arm.getModelMatrix());
	right_upper_arm.Render();

	// 右下臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-45.f + 10.f * sin(t * 3)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_lower_arm.CenterPivot);
	right_lower_arm.setParentModel(right_upper_arm.getModelMatrix());
	right_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_arm.getModelMatrix());
	right_lower_arm.Render();

	// 左肩
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_shoulder.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-30.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_shoulder.CenterPivot);
	left_shoulder.setParentModel(body_1.getModelMatrix());
	left_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(left_shoulder.getModelMatrix());
	left_shoulder.Render();

	// 左上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-30.f), glm::vec3(0, 0, 1));
	customMmat = glm::rotate(customMmat, glm::radians(-30.f), glm::vec3(0, 1, 0));
	customMmat = glm::translate(customMmat, -left_upper_arm.CenterPivot);
	left_upper_arm.setParentModel(left_shoulder.getModelMatrix());
	left_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_arm.getModelMatrix());
	left_upper_arm.Render();

	// 左下臂
	f = -30.f - 10.f * cos(t);
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-45.f + 10.f * sin(t * 3 + 1.57f)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_lower_arm.CenterPivot);
	left_lower_arm.setParentModel(left_upper_arm.getModelMatrix());
	left_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_arm.getModelMatrix());
	left_lower_arm.Render();

	// 左上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(f), glm::vec3(1,0,0));
	customMmat = glm::translate(customMmat, -left_upper_leg.CenterPivot);
	left_upper_leg.setParentModel(body_2.getModelMatrix());
	left_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_leg.getModelMatrix());
	left_upper_leg.Render();

	// 左下腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-2.f * f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_lower_leg.CenterPivot);
	left_lower_leg.setParentModel(left_upper_leg.getModelMatrix());
	left_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_leg.getModelMatrix());
	left_lower_leg.Render();

	// 左腳
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_foot.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_foot.CenterPivot);
	left_foot.setParentModel(left_lower_leg.getModelMatrix());
	left_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(left_foot.getModelMatrix());
	left_foot.Render();

	// 右上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_upper_leg.CenterPivot);
	right_upper_leg.setParentModel(body_2.getModelMatrix());
	right_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_leg.getModelMatrix());
	right_upper_leg.Render();

	// 右下腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-2.f * f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_lower_leg.CenterPivot);
	right_lower_leg.setParentModel(right_upper_leg.getModelMatrix());
	right_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_leg.getModelMatrix());
	right_lower_leg.Render();

	// 右腳
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_foot.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_foot.CenterPivot);
	right_foot.setParentModel(right_lower_leg.getModelMatrix());
	right_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(right_foot.getModelMatrix());
	right_foot.Render();
	targetShader->Disable();
}

// Floor Action
void My_Action6(float t, bool reset, bool shadow)
{
	targetShader->Enable();
	if (shadow)
	{
		targetShader->SetPMat(lightSpaceMatrix);
		targetShader->SetViewMat4(mat4());
	}
	else
	{
		targetShader->SetPMat(projMat);
		targetShader->SetViewMat4(viewMat);
	}
	glm::mat4 customMmat;
	float f, f1, f2, f3;
	int i;
	t = t * 4.f;
	static float baseTime = t;
	if (reset)
		baseTime = t;
	t = t - baseTime;

	// 身體2
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_2.CenterPivot + glm::vec3(0, -0.2 * abs(sin(t * 2)), 0));
	customMmat = glm::translate(customMmat, -body_2.CenterPivot);
	body_2.setParentModel(glm::mat4());
	body_2.setCurrentModel(customMmat);
	targetShader->SetMMat(body_2.getModelMatrix());
	body_2.Render();

	// 身體1
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_1.CenterPivot);
	customMmat = glm::translate(customMmat, -body_1.CenterPivot);
	body_1.setParentModel(body_2.getModelMatrix());
	body_1.setCurrentModel(customMmat);
	targetShader->SetMMat(body_1.getModelMatrix());
	body_1.Render();

	// 頭
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, head.CenterPivot);
	customMmat = glm::translate(customMmat, -head.CenterPivot);
	head.setParentModel(body_1.getModelMatrix());
	head.setCurrentModel(customMmat);
	targetShader->SetMMat(head.getModelMatrix());
	head.Render();

	// 右肩
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_shoulder.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-80.f), glm::vec3(1, 0, 0));
	customMmat = glm::rotate(customMmat, glm::radians(30.f), glm::vec3(0, 0, 1));
	customMmat = glm::translate(customMmat, -right_shoulder.CenterPivot);
	right_shoulder.setParentModel(body_1.getModelMatrix());
	right_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(right_shoulder.getModelMatrix());
	right_shoulder.Render();

	// 右上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(110.f), glm::vec3(0, 1, 0));
	customMmat = glm::translate(customMmat, -right_upper_arm.CenterPivot);
	right_upper_arm.setParentModel(right_shoulder.getModelMatrix());
	right_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_arm.getModelMatrix());
	right_upper_arm.Render();

	// 右下臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-80.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_lower_arm.CenterPivot);
	right_lower_arm.setParentModel(right_upper_arm.getModelMatrix());
	right_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_arm.getModelMatrix());
	right_lower_arm.Render();

	// 左肩
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_shoulder.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-50.f), glm::vec3(1, 0, 0));
	customMmat = glm::rotate(customMmat, glm::radians(-30.f), glm::vec3(0, 0, 1));
	customMmat = glm::translate(customMmat, -left_shoulder.CenterPivot);
	left_shoulder.setParentModel(body_1.getModelMatrix());
	left_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(left_shoulder.getModelMatrix());
	left_shoulder.Render();

	// 左上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-100.f), glm::vec3(0, 1, 0));
	customMmat = glm::translate(customMmat, -left_upper_arm.CenterPivot);
	left_upper_arm.setParentModel(left_shoulder.getModelMatrix());
	left_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_arm.getModelMatrix());
	left_upper_arm.Render();

	// 左下臂
	f = -30.f - 10.f * cos(t);
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-60.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_lower_arm.CenterPivot);
	left_lower_arm.setParentModel(left_upper_arm.getModelMatrix());
	left_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_arm.getModelMatrix());
	left_lower_arm.Render();

	// 左上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-75.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_upper_leg.CenterPivot);
	left_upper_leg.setParentModel(body_2.getModelMatrix());
	left_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_leg.getModelMatrix());
	left_upper_leg.Render();

	

	// 左下腿
	f1 = abs(sin(t));
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(105.f * f1), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_lower_leg.CenterPivot);
	left_lower_leg.setParentModel(left_upper_leg.getModelMatrix());
	left_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_leg.getModelMatrix());
	left_lower_leg.Render();

	// 左腳
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_foot.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-15.f * f1), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_foot.CenterPivot);
	left_foot.setParentModel(left_lower_leg.getModelMatrix());
	left_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(left_foot.getModelMatrix());
	left_foot.Render();

	// 右上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_leg.CenterPivot);	
	customMmat = glm::rotate(customMmat, glm::radians(-75.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_upper_leg.CenterPivot);
	right_upper_leg.setParentModel(body_2.getModelMatrix());
	right_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_leg.getModelMatrix());
	right_upper_leg.Render();
	
	// 右下腿
	f1 = abs(sin(t + 1.57));
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(105.f * f1), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_lower_leg.CenterPivot);
	right_lower_leg.setParentModel(right_upper_leg.getModelMatrix());
	right_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_leg.getModelMatrix());
	right_lower_leg.Render();

	// 右腳
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_foot.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-15.f * f1), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_foot.CenterPivot);
	right_foot.setParentModel(right_lower_leg.getModelMatrix());
	right_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(right_foot.getModelMatrix());
	right_foot.Render();
	targetShader->Disable();
}

// Hand Action
void My_Action7(float t, bool reset, bool shadow)
{
	targetShader->Enable();
	if (shadow)
	{
		targetShader->SetPMat(lightSpaceMatrix);
		targetShader->SetViewMat4(mat4());
	}
	else
	{
		targetShader->SetPMat(projMat);
		targetShader->SetViewMat4(viewMat);
	}
	glm::mat4 customMmat;
	float f;
	int i;
	t = t * 4.f;
	static float baseTime = t;
	if (reset)
		baseTime = t;
	t = t - baseTime;

	// 身體2
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_2.CenterPivot);
	customMmat = glm::translate(customMmat, -body_2.CenterPivot);
	body_2.setParentModel(glm::mat4());
	body_2.setCurrentModel(customMmat);
	targetShader->SetMMat(body_2.getModelMatrix());
	body_2.Render();

	// 身體1
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, (body_2.CenterPivot + glm::vec3(0, 0.4, 0)));
	customMmat = glm::rotate(customMmat, glm::radians(-30.f * sin(t)), glm::vec3(0, 0, 1));
	customMmat = glm::translate(customMmat, -(body_2.CenterPivot + glm::vec3(0, 0.5, 0)));
	body_1.setParentModel(body_2.getModelMatrix());
	body_1.setCurrentModel(customMmat);
	targetShader->SetMMat(body_1.getModelMatrix());
	body_1.Render();

	// 頭
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, head.CenterPivot);
	customMmat = glm::translate(customMmat, -head.CenterPivot);
	head.setParentModel(body_1.getModelMatrix());
	head.setCurrentModel(customMmat);
	targetShader->SetMMat(head.getModelMatrix());
	head.Render();

	// 右肩
	f = sin(t);
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_shoulder.CenterPivot);
	if (f > 0)
		customMmat = glm::rotate(customMmat, glm::radians(-180.f * f), glm::vec3(1, 0, 0));
	else
		customMmat = glm::rotate(customMmat, glm::radians(45.f * f), glm::vec3(0, 0, 1));
	customMmat = glm::translate(customMmat, -right_shoulder.CenterPivot);
	right_shoulder.setParentModel(body_1.getModelMatrix());
	right_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(right_shoulder.getModelMatrix());
	right_shoulder.Render();

	// 右上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_arm.CenterPivot);
	if (f > 0)
		customMmat = glm::rotate(customMmat, glm::radians(30.f * f), glm::vec3(0, 0, 1));
	customMmat = glm::rotate(customMmat, glm::radians(90.f), glm::vec3(0, 1, 0));
	customMmat = glm::translate(customMmat, -right_upper_arm.CenterPivot);
	right_upper_arm.setParentModel(right_shoulder.getModelMatrix());
	right_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_arm.getModelMatrix());
	right_upper_arm.Render();

	// 右下臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_arm.CenterPivot);
	if (f > 0)
		customMmat = glm::rotate(customMmat, glm::radians(-30.f * f), glm::vec3(1, 0, 0));
	else
		customMmat = glm::rotate(customMmat, glm::radians(90.f * f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_lower_arm.CenterPivot);
	right_lower_arm.setParentModel(right_upper_arm.getModelMatrix());
	right_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_arm.getModelMatrix());
	right_lower_arm.Render();

	// 左肩
	f = sin(t + 3.14159);
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_shoulder.CenterPivot);
	if (f > 0)
		customMmat = glm::rotate(customMmat, glm::radians(-180.f * f), glm::vec3(1, 0, 0));
	else
		customMmat = glm::rotate(customMmat, glm::radians(-45.f * f), glm::vec3(0, 0, 1));
	customMmat = glm::translate(customMmat, -left_shoulder.CenterPivot);
	left_shoulder.setParentModel(body_1.getModelMatrix());
	left_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(left_shoulder.getModelMatrix());
	left_shoulder.Render();

	// 左上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_arm.CenterPivot); 
	if (f > 0)
		customMmat = glm::rotate(customMmat, glm::radians(-30.f * f), glm::vec3(0, 0, 1));
	customMmat = glm::rotate(customMmat, glm::radians(-90.f), glm::vec3(0, 1, 0));
	customMmat = glm::translate(customMmat, -left_upper_arm.CenterPivot);
	left_upper_arm.setParentModel(left_shoulder.getModelMatrix());
	left_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_arm.getModelMatrix());
	left_upper_arm.Render();

	// 左下臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_arm.CenterPivot);
	if (f > 0)
		customMmat = glm::rotate(customMmat, glm::radians(-30.f * f), glm::vec3(1, 0, 0));
	else
		customMmat = glm::rotate(customMmat, glm::radians(90.f * f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_lower_arm.CenterPivot);
	left_lower_arm.setParentModel(left_upper_arm.getModelMatrix());
	left_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_arm.getModelMatrix());
	left_lower_arm.Render();

	// 左上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_leg.CenterPivot);
	customMmat = glm::translate(customMmat, -left_upper_leg.CenterPivot);
	left_upper_leg.setParentModel(body_2.getModelMatrix());
	left_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_leg.getModelMatrix());
	left_upper_leg.Render();

	// 左下腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_leg.CenterPivot);
	customMmat = glm::translate(customMmat, -left_lower_leg.CenterPivot);
	left_lower_leg.setParentModel(left_upper_leg.getModelMatrix());
	left_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_leg.getModelMatrix());
	left_lower_leg.Render();

	// 左腳
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_foot.CenterPivot);
	customMmat = glm::translate(customMmat, -left_foot.CenterPivot);
	left_foot.setParentModel(left_lower_leg.getModelMatrix());
	left_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(left_foot.getModelMatrix());
	left_foot.Render();

	// 右上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_leg.CenterPivot);
	customMmat = glm::translate(customMmat, -right_upper_leg.CenterPivot);
	right_upper_leg.setParentModel(body_2.getModelMatrix());
	right_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_leg.getModelMatrix());
	right_upper_leg.Render();

	// 右下腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_leg.CenterPivot);
	customMmat = glm::translate(customMmat, -right_lower_leg.CenterPivot);
	right_lower_leg.setParentModel(right_upper_leg.getModelMatrix());
	right_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_leg.getModelMatrix());
	right_lower_leg.Render();

	// 右腳
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_foot.CenterPivot);
	customMmat = glm::translate(customMmat, -right_foot.CenterPivot);
	right_foot.setParentModel(right_lower_leg.getModelMatrix());
	right_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(right_foot.getModelMatrix());
	right_foot.Render();
	targetShader->Disable();
}

// Shoot Sword
void My_Action8(float t, bool reset, bool shadow)
{
	targetShader->Enable();
	if (shadow)
	{
		targetShader->SetPMat(lightSpaceMatrix);
		targetShader->SetViewMat4(mat4());
	}
	else
	{
		targetShader->SetPMat(projMat);
		targetShader->SetViewMat4(viewMat);
	}
	glm::mat4 customMmat;
	float f;
	int i;
	static float baseTime = t;
	
	if (reset || t - baseTime > 1000.f)
		baseTime = t;
	t = t - baseTime;
	float timeV1 = t, timeV2 = t, timeV3 = t;
	if (timeV1 > 3.14159)
	{
		timeV1 = 3.1416;
	}
	if (timeV2 > 6.28318)
	{
		timeV2 = 6.2832;
	}
	// 身體2
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_2.CenterPivot);
	if (timeV1 < 3.14159)
	{
		customMmat = glm::translate(customMmat, glm::vec3(0, 2.f * sin(timeV1 / 2.f), 0));
		customMmat = glm::rotate(customMmat, glm::radians(-90.f * sin(timeV1 / 2.f)), glm::vec3(1, 0, 0));

	}
	else
	{
		customMmat = glm::translate(customMmat, glm::vec3(0, 2.f + 0.5f * sin(t), 0));
		customMmat = glm::rotate(customMmat, glm::radians(-90.f), glm::vec3(1, 0, 0));
	}
	customMmat = glm::translate(customMmat, -body_2.CenterPivot);
	body_2.setParentModel(glm::mat4());
	body_2.setCurrentModel(customMmat);
	targetShader->SetMMat(body_2.getModelMatrix());
	body_2.Render();

	// 身體1
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_2.CenterPivot);
	if (timeV1 < 3.14159)
	{
		customMmat = glm::rotate(customMmat, glm::radians(30.f * sin(timeV1 / 2.f)), glm::vec3(1, 0, 0));
	}
	else
	{
		customMmat = glm::rotate(customMmat, glm::radians(30.f), glm::vec3(1, 0, 0));
	}

	customMmat = glm::translate(customMmat, -body_2.CenterPivot);
	body_1.setParentModel(body_2.getModelMatrix());
	body_1.setCurrentModel(customMmat);
	targetShader->SetMMat(body_1.getModelMatrix());
	body_1.Render();

	// 頭
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, head.CenterPivot);
	customMmat = glm::translate(customMmat, -head.CenterPivot);
	head.setParentModel(body_1.getModelMatrix());
	head.setCurrentModel(customMmat);
	targetShader->SetMMat(head.getModelMatrix());
	head.Render();

	

	// 右肩
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_shoulder.CenterPivot);
	if (timeV1 < 3.14159)
		customMmat = glm::rotate(customMmat, glm::radians(-180.f * sin(timeV1 / 2)), glm::vec3(1, 0, 0));
	else
		customMmat = glm::rotate(customMmat, glm::radians(-180.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_shoulder.CenterPivot);
	right_shoulder.setParentModel(body_1.getModelMatrix());
	right_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(right_shoulder.getModelMatrix());
	right_shoulder.Render();

	// 右上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_arm.CenterPivot);
	if (timeV1 < 3.14159)
		customMmat = glm::rotate(customMmat, glm::radians(-45.f * sin(timeV1 / 2)), glm::vec3(0, 0, 1));
	else
		customMmat = glm::rotate(customMmat, glm::radians(-45.f), glm::vec3(0, 0, 1));
	customMmat = glm::rotate(customMmat, glm::radians(-90.f), glm::vec3(0, 1, 0));
	customMmat = glm::translate(customMmat, -right_upper_arm.CenterPivot);
	right_upper_arm.setParentModel(right_shoulder.getModelMatrix());
	right_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_arm.getModelMatrix());
	right_upper_arm.Render();

	// 右下臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_arm.CenterPivot);
	if (timeV1 < 3.14159)
		customMmat = glm::rotate(customMmat, glm::radians(30.f * sin(timeV1 / 2)), glm::vec3(1, 0, 0));
	else
		customMmat = glm::rotate(customMmat, glm::radians(30.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_lower_arm.CenterPivot);
	right_lower_arm.setParentModel(right_upper_arm.getModelMatrix());
	right_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_arm.getModelMatrix());
	right_lower_arm.Render();

	// 左肩
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_shoulder.CenterPivot);
	if (timeV1 < 3.14159)
		customMmat = glm::rotate(customMmat, glm::radians(-180.f * sin(timeV1 / 2)), glm::vec3(1, 0, 0));
	else
		customMmat = glm::rotate(customMmat, glm::radians(-180.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_shoulder.CenterPivot);
	left_shoulder.setParentModel(body_1.getModelMatrix());
	left_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(left_shoulder.getModelMatrix());
	left_shoulder.Render();

	// 左上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_arm.CenterPivot);
	if (timeV1 < 3.14159)
		customMmat = glm::rotate(customMmat, glm::radians(45.f * sin(timeV1 / 2)), glm::vec3(0, 0, 1));
	else
		customMmat = glm::rotate(customMmat, glm::radians(45.f), glm::vec3(0, 0, 1));
	customMmat = glm::rotate(customMmat, glm::radians(90.f), glm::vec3(0, 1, 0));
	customMmat = glm::translate(customMmat, -left_upper_arm.CenterPivot);
	left_upper_arm.setParentModel(left_shoulder.getModelMatrix());
	left_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_arm.getModelMatrix());
	left_upper_arm.Render();

	// 左下臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_arm.CenterPivot);
	if (timeV1 < 3.14159)
		customMmat = glm::rotate(customMmat, glm::radians(30.f * sin(timeV1 / 2)), glm::vec3(1, 0, 0));
	else
		customMmat = glm::rotate(customMmat, glm::radians(30.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_lower_arm.CenterPivot);
	left_lower_arm.setParentModel(left_upper_arm.getModelMatrix());
	left_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_arm.getModelMatrix());
	left_lower_arm.Render();

	// 左上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_leg.CenterPivot);
	if (timeV1 < 3.14159)
		customMmat = glm::rotate(customMmat, glm::radians(-30.f * sin(timeV1 / 2)), glm::vec3(1, 0, 0));
	else
		customMmat = glm::rotate(customMmat, glm::radians(-30.f - 30.f * sin(timeV3 * 10.f)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_upper_leg.CenterPivot);
	left_upper_leg.setParentModel(body_2.getModelMatrix());
	left_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_leg.getModelMatrix());
	left_upper_leg.Render();

	// 左下腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_leg.CenterPivot);
	if (timeV1 < 3.14159)
		customMmat = glm::rotate(customMmat, glm::radians(60.f * sin(timeV1 / 2)), glm::vec3(1, 0, 0));
	else
		customMmat = glm::rotate(customMmat, glm::radians(60.f + 60.f * sin(timeV3 * 10.f)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_lower_leg.CenterPivot);
	left_lower_leg.setParentModel(left_upper_leg.getModelMatrix());
	left_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_leg.getModelMatrix());
	left_lower_leg.Render();

	// 左腳
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_foot.CenterPivot);
	if (timeV1 < 3.14159)
		customMmat = glm::rotate(customMmat, glm::radians(30.f * sin(timeV1 / 2)), glm::vec3(1, 0, 0));
	else
		customMmat = glm::rotate(customMmat, glm::radians(30.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_foot.CenterPivot);
	left_foot.setParentModel(left_lower_leg.getModelMatrix());
	left_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(left_foot.getModelMatrix());
	left_foot.Render();

	// 右上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_leg.CenterPivot);
	if (timeV1 > 3.14159)
		customMmat = glm::rotate(customMmat, glm::radians(-30.f - 30.f * sin(timeV3 * 10.f + 3.1416f)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_upper_leg.CenterPivot);
	right_upper_leg.setParentModel(body_2.getModelMatrix());
	right_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_leg.getModelMatrix());
	right_upper_leg.Render();

	// 右下腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_leg.CenterPivot);
	if (timeV1 > 3.14159)
		customMmat = glm::rotate(customMmat, glm::radians(60.f + 60.f * sin(timeV3 * 10.f + 3.1416f)), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_lower_leg.CenterPivot);
	right_lower_leg.setParentModel(right_upper_leg.getModelMatrix());
	right_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_leg.getModelMatrix());
	right_lower_leg.Render();

	// 右腳
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_foot.CenterPivot);
	if (timeV1 < 3.14159)
		customMmat = glm::rotate(customMmat, glm::radians(50.f * sin(timeV1 / 2)), glm::vec3(1, 0, 0));
	else
		customMmat = glm::rotate(customMmat, glm::radians(50.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_foot.CenterPivot);
	right_foot.setParentModel(right_lower_leg.getModelMatrix());
	right_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(right_foot.getModelMatrix());
	right_foot.Render();
	targetShader->Disable();
}

// Ballet Action
void My_Action9(float t, bool reset, bool shadow)
{
	targetShader->Enable();
	if (shadow)
	{
		targetShader->SetPMat(lightSpaceMatrix);
		targetShader->SetViewMat4(mat4());
	}
	else
	{
		targetShader->SetPMat(projMat);
		targetShader->SetViewMat4(viewMat);
	}
	glm::mat4 customMmat;
	float f;
	int i;
	static float baseTime = t;
	if (reset)
		baseTime = t;
	t = t - baseTime;

	f = sin(t) > 0 ? sin(t) : 0;
	float rotateSpeed = t * 5.f;
	const float dist = glm::distance(glm::vec2(left_foot.CenterPivot.x, left_foot.CenterPivot.y), glm::vec2(0.00826, 0));
	// 身體1
	// -dist * sin(rotateSpeed + 1.57)
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_1.CenterPivot + glm::vec3(0, -1.75f * f, 0));
	customMmat = glm::rotate(customMmat, rotateSpeed, glm::vec3(0, 1, 0));
	customMmat = glm::translate(customMmat, -body_1.CenterPivot);
	body_1.setParentModel(glm::mat4());
	body_1.setCurrentModel(customMmat);
	targetShader->SetMMat(body_1.getModelMatrix());
	body_1.Render();

	// 頭
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, head.CenterPivot);
	customMmat = glm::translate(customMmat, -head.CenterPivot);
	head.setParentModel(body_1.getModelMatrix());
	head.setCurrentModel(customMmat);
	targetShader->SetMMat(head.getModelMatrix());
	head.Render();

	// 身體2
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, body_2.CenterPivot);
	customMmat = glm::translate(customMmat, -body_2.CenterPivot);
	body_2.setParentModel(body_1.getModelMatrix());
	body_2.setCurrentModel(customMmat);
	targetShader->SetMMat(body_2.getModelMatrix());
	body_2.Render();

	// 右肩
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_shoulder.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-180.f + 90.f * f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_shoulder.CenterPivot);
	right_shoulder.setParentModel(body_1.getModelMatrix());
	right_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(right_shoulder.getModelMatrix());
	right_shoulder.Render();

	// 右上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-15.f), glm::vec3(0, 0, 1));
	customMmat = glm::rotate(customMmat, glm::radians(90.f), glm::vec3(0, 1, 0));
	customMmat = glm::translate(customMmat, -right_upper_arm.CenterPivot);
	right_upper_arm.setParentModel(right_shoulder.getModelMatrix());
	right_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_arm.getModelMatrix());
	right_upper_arm.Render();

	// 右下臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-75.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_lower_arm.CenterPivot);
	right_lower_arm.setParentModel(right_upper_arm.getModelMatrix());
	right_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_arm.getModelMatrix());
	right_lower_arm.Render();

	// 左肩
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_shoulder.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-180.f + 90.f * f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_shoulder.CenterPivot);
	left_shoulder.setParentModel(body_1.getModelMatrix());
	left_shoulder.setCurrentModel(customMmat);
	targetShader->SetMMat(left_shoulder.getModelMatrix());
	left_shoulder.Render();

	// 左上臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(15.f), glm::vec3(0, 0, 1));
	customMmat = glm::rotate(customMmat, glm::radians(-90.f), glm::vec3(0, 1, 0));
	customMmat = glm::translate(customMmat, -left_upper_arm.CenterPivot);
	left_upper_arm.setParentModel(left_shoulder.getModelMatrix());
	left_upper_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_arm.getModelMatrix());
	left_upper_arm.Render();

	// 左下臂
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_arm.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-75.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_lower_arm.CenterPivot);
	left_lower_arm.setParentModel(left_upper_arm.getModelMatrix());
	left_lower_arm.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_arm.getModelMatrix());
	left_lower_arm.Render();

	
	// 左上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_upper_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-12.5f), glm::vec3(0, 0, 1));
	customMmat = glm::rotate(customMmat, glm::radians(-45.f * f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_upper_leg.CenterPivot);
	left_upper_leg.setParentModel(body_2.getModelMatrix());
	left_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_upper_leg.getModelMatrix());
	left_upper_leg.Render();

	
	// 左下腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_lower_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(90.f * f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_lower_leg.CenterPivot);
	left_lower_leg.setParentModel(left_upper_leg.getModelMatrix());
	left_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(left_lower_leg.getModelMatrix());
	left_lower_leg.Render();

	// 左腳
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, left_foot.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-15.f), glm::vec3(0, 0, 1));
	customMmat = glm::rotate(customMmat, glm::radians(45.f - 45.f * f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -left_foot.CenterPivot);
	left_foot.setParentModel(left_lower_leg.getModelMatrix());
	left_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(left_foot.getModelMatrix());
	left_foot.Render();

	// 右上腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_upper_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(-30.f - 60.f * f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_upper_leg.CenterPivot);
	right_upper_leg.setParentModel(body_2.getModelMatrix());
	right_upper_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_upper_leg.getModelMatrix());
	right_upper_leg.Render();

	// 右下腿
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_lower_leg.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(60.f - 60.f * f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_lower_leg.CenterPivot);
	right_lower_leg.setParentModel(right_upper_leg.getModelMatrix());
	right_lower_leg.setCurrentModel(customMmat);
	targetShader->SetMMat(right_lower_leg.getModelMatrix());
	right_lower_leg.Render();

	// 右腳
	customMmat = glm::mat4();
	customMmat = glm::translate(customMmat, right_foot.CenterPivot);
	customMmat = glm::rotate(customMmat, glm::radians(15.f), glm::vec3(0, 0, 1));
	customMmat = glm::rotate(customMmat, glm::radians(60.f), glm::vec3(1, 0, 0));
	customMmat = glm::translate(customMmat, -right_foot.CenterPivot);
	right_foot.setParentModel(right_lower_leg.getModelMatrix());
	right_foot.setCurrentModel(customMmat);
	targetShader->SetMMat(right_foot.getModelMatrix());
	right_foot.Render();
	targetShader->Disable();
}


void Sword_Act0(float t, bool reset, bool shadow)
{
	instanceShader.Enable();
	cubemapShader.UseTexture(true);
	if (shadow)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		instanceShader.setUniformMat4("um4p", lightSpaceMatrix);
		instanceShader.setUniformMat4("um4v", mat4());
	}
	else
	{
		instanceShader.setUniformMat4("um4p", projMat);
		instanceShader.setUniformMat4("um4v", viewMat);
	}
	
	


	float count = 30.f;
	for (int i = 0; i < count; i++)
	{
		float x = 5.f * sin(6.28318 / count * i + t);
		float y = 8;
		float z = 5.f * cos(6.28318 / count * i + t);
		glm::mat4 customMat;
		customMat = glm::translate(customMat, glm::vec3(x, y, z));
		customMat = glm::scale(customMat, glm::vec3(0.5, 0.5, 0.5));
		customMat = glm::rotate(customMat, float(6.28318 / count * i + t), glm::vec3(0, 1, 0));
		customMat = glm::rotate(customMat, glm::radians(90.f), glm::vec3(1, 0, 0));
		sword.setInstanceModel(customMat, i);
	}
	sword.setParentModel(glm::mat4());
	sword.bufferInstanceModel();
	sword.RenderInstance(count);
	instanceShader.Disable();
}

void Sword_Act1(float t, bool reset, bool shadow)
{
	int count = 20;
	static glm::vec3 swordModel[1000];
	static int swordSpeed[1000];
	static float previous = t;
	instanceShader.Enable();
	cubemapShader.UseTexture(true);
	auto getRand = [](float center = 3.f) { return center * rand() / RAND_MAX - center / 2.f; };
	if (reset) {

		int baseIndex = 0;
		for (int i = 0; i <= 3; i++)
		{
			float radius = 6 + i * 5;
			for (int j = 0; j < count; j++)
			{
				swordModel[j + baseIndex] = glm::vec3(
					(radius + getRand(1.f)) * sin(6.2831 / count * j) + getRand(),
					20.f + 10.f * rand() / RAND_MAX - 1.5f,
					(radius + getRand(1.f)) * cos(6.2831 / count * j) + getRand()
				);
				swordSpeed[j + baseIndex] = 5.f + 4.f * rand() / RAND_MAX;
			}
			baseIndex += count;
			count *= 3;
		}
		previous = t;
		return;
	}
	if (shadow)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		instanceShader.setUniformMat4("um4p", lightSpaceMatrix);
		instanceShader.setUniformMat4("um4v", mat4());
	}
	else
	{
		instanceShader.setUniformMat4("um4p", projMat);
		instanceShader.setUniformMat4("um4v", viewMat);
	}

	for (int i = 0; i < 800; i++)
	{
		if (swordModel[i].y > 4.f) {
			swordModel[i].y -= swordSpeed[i] * (t - previous);
		}

		glm::mat4 customMat;
		customMat = glm::translate(customMat, swordModel[i]);
		customMat = glm::scale(customMat, glm::vec3(0.5, 0.5, 0.5));
		customMat = glm::rotate(customMat, glm::radians(90.f), glm::vec3(1, 0, 0));
		sword.setInstanceModel(customMat, i);
	}
	sword.setParentModel(glm::mat4());
	sword.bufferInstanceModel();
	sword.RenderInstance(800);
	previous = t;
	instanceShader.Disable();
}

void Sword_Act2(float t, bool reset, bool shadow)
{
	const int basicSwordCount = 20;
	const int shootingSwordCount = 200;
	const float CYCLE_RANGE = 10.f;
	const float GEN_RANGE = 20.f;

	static glm::vec3 swordModel[200];
	static int swordSpeed[200];
	bool anyNewSword = false;
	static float previous = t, baseTime = t, num_of_sw = 0;

	instanceShader.Enable();
	cubemapShader.UseTexture(true);
	auto GetRand = [](float center = 3.f, float range = 0.5) { return center +  range * (rand() * 1.f / RAND_MAX - 0.5f); };
	if (reset) {
		previous = t;
		baseTime = t;

		for (int i = 0; i < shootingSwordCount; i++)
		{
			swordModel[i].x = GetRand(0, GEN_RANGE);
			swordModel[i].y = GetRand(10.f, GEN_RANGE);
			swordModel[i].z = 600.0;
			swordSpeed[i] = GetRand(70.f, 10.f);
		}
	}
	if (shadow)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		instanceShader.setUniformMat4("um4p", lightSpaceMatrix);
		instanceShader.setUniformMat4("um4v", mat4());
	}
	else
	{
		instanceShader.setUniformMat4("um4p", projMat);
		instanceShader.setUniformMat4("um4v", viewMat);
	}
	
	// Add new and update sword
	if (t - baseTime > 3.14159)
	{
		float a = GetRand(0.5, 0.5);
		bool shouldGenNewSword = a > 0.6;
		for (int i = 0; i < shootingSwordCount; i++)
		{
			if (swordModel[i].z > 300.f && !anyNewSword && shouldGenNewSword)
			{
				swordModel[i].x = GetRand(0, GEN_RANGE);
				swordModel[i].y = GetRand(10.f, GEN_RANGE);
				swordModel[i].z = 1.0;
				anyNewSword = true;
			}
			swordModel[i].z += swordSpeed[i] * (t - previous);
		}
	}

	// Set Cycle Sword Mode;
	for (int i = 0; i < basicSwordCount; i++)
	{
		float rd = glm::radians(i * 360.f / basicSwordCount) + t * 2.f;
		glm::mat4 customMat;
		customMat = glm::translate(customMat, glm::vec3(CYCLE_RANGE * cos(rd), CYCLE_RANGE * sin(rd) + 10.f, 2.f));
		customMat = glm::rotate(customMat, rd-1.57f, glm::vec3(0, 0, 1));
		customMat = glm::rotate(customMat, glm::radians(-90.f), glm::vec3(1, 0, 0));
		sword.setInstanceModel(customMat, i);
	}

	// Set Shoot Sword Model
	for (int i = 0; i < shootingSwordCount; i++)
	{
		glm::mat4 customMat;
		customMat = glm::translate(customMat, swordModel[i]);
		sword.setInstanceModel(customMat, i + basicSwordCount);
	}

	// sword.setInstanceModel(glm::mat4(), 2);
	sword.setParentModel(glm::mat4());
	sword.bufferInstanceModel();
	sword.RenderInstance(basicSwordCount + 100);
	previous = t;
	instanceShader.Disable();
}



//Call to resize the window
void My_Reshape(int width, int height)
{
	aspect = width * 1.0f / height;
	glViewport(0, 0, width, height);
	projMat = perspective(radians(60.0f), aspect, 0.1f, 1000.0f);

	TwWindowSize(width, height);
}

//Timer event
void My_Timer(int val)
{
	glutPostRedisplay();
	glutTimerFunc(16, My_Timer, val);
}

//Mouse event
void My_Mouse(int button, int state, int x, int y)
{
	if (!TwEventMouseButtonGLUT(button, state, x, y))
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				isLeftMousePressed = true;
				printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
			}
			else if (state == GLUT_UP)
			{
				isLeftMousePressed = false;
				printf("Mouse %d is released at (%d, %d)\n", button, x, y);
			}
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			printf("Mouse %d is pressed\n", button);
		}
		else if (button == 3)
		{
			zoom += 0.5;
		}
		else if (button == 4)
		{
			zoom -= 0.5;
		}
		printf("%d %d %d %d\n", button, state, x, y);
	}
}

//Keyboard event
void My_Keyboard(unsigned char key, int x, int y)
{
	if (!TwEventKeyboardGLUT(key, x, y))
	{
		printf("Key %c is pressed at (%d, %d)\n", key, x, y);
	}
}

void My_Mouse_Moving(int x, int y) {

	if (isLeftMousePressed)
	{
		yawAngle += 0.1f * (x - lastMouseX);
		pitchAngle += 0.1f * (y - lastMouseY);
	}

	lastMouseX = x;
	lastMouseY = y;

}

//Menu event
void My_Menu(int id)
{
	switch (id)
	{
	case 1000000:
		exit(0);
		break;
	case 0:
		ActionIndex = 0;
		break;
	case 1:
		ActionIndex = 1;
		break;
	case 2:
		ActionIndex = 2;
		break;
	case 3:
		ActionIndex = 3;
		break;
	case 4:
		ActionIndex = 4;
		break;
	case 5:
		ActionIndex = 5;
		break;
	case 6:
		ActionIndex = 6;
		break;
	case 7:
		ActionIndex = 7;
		break;
	case 8:
		ActionIndex = 8;
		break;
	case 9:
		ActionIndex = 9;
		break;
	default:
		break;
	}
	if (id < 10) {
		ResetAction = true;
	}

}

void Shader_Menu(int id)
{
	switch (id)
	{
	case 0:
		ShaderIndex = 1;
		break;
	case 1:
		ShaderIndex = 2;
		break;
	case 2:
		ShaderIndex = 3;
		break;
	case 3:
		ShaderIndex = 4;
		break;
	case 4:
		ShaderIndex = 5;
		break;
	case 5:
		ShaderIndex = 0;
		break;
	}
}

void FrameBuffer_Menu(int id)
{
	switch (id)
	{
	case -1:
		FrameIndex = -1;
		break;
	case 0:
		FrameIndex = 0;
		break;
	case 1:
		FrameIndex = 1;
		break;
	case 2:
		FrameIndex = 2;
		break;
	case 3:
		FrameIndex = 3;
		break;
	case 4:
		FrameIndex = 4;
		break;
	case 5:
		FrameIndex = 5;
		break;
	case 6:
		FrameIndex = 6;
		break;
	}
}

int main(int argc, char *argv[])
{
#ifdef __APPLE__
    //Change working directory to source code path
    chdir(__FILEPATH__("/../Assets/"));
#endif
	// Initialize GLUT and GLEW, then create a window.
	////////////////////
	glutInit(&argc, argv);
#ifdef _MSC_VER
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow(ProjectName.c_str()); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
#ifdef _MSC_VER
	glewInit();
#endif

	//Print debug information 
	Common::DumpInfo();
	////////////////////

	//Call custom initialize function
	My_Init();
	////////////////////

	//Define Menu
	////////////////////
	int menu_sub_action = glutCreateMenu(My_Menu);
	glutAddMenuEntry("Stand", 0);
	glutAddMenuEntry("Walk", 1);
	glutAddMenuEntry("Clap Jump", 3);
	glutAddMenuEntry("Punch Front", 4);
	glutAddMenuEntry("Floor Action", 6);
	glutAddMenuEntry("Stretching", 7);
	glutAddMenuEntry("Sorry Sorry", 5);
	glutAddMenuEntry("Ballet", 9);
	glutAddMenuEntry("Punch Land", 2);
	glutAddMenuEntry("Shoot Sword", 8);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	int menu_sub_shader = glutCreateMenu(Shader_Menu);
	glutAddMenuEntry("Normal", 5);
	glutAddMenuEntry("Reflect", 0);
	glutAddMenuEntry("Refract", 1);
	glutAddMenuEntry("Horizontal Lightening", 2);
	glutAddMenuEntry("Vertical Tower", 3);
	glutAddMenuEntry("Lightening", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	int menu_sub_buffer = glutCreateMenu(FrameBuffer_Menu);
	glutAddMenuEntry("None", -1);
	glutAddMenuEntry("Negative", 0);
	glutAddMenuEntry("GrayScale", 1);
	glutAddMenuEntry("Gaussian Blur", 2);
	glutAddMenuEntry("Sharpen", 3);
	glutAddMenuEntry("Edge Detection", 4);
	glutAddMenuEntry("Cycle", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	int menu_main = glutCreateMenu(My_Menu);
	glutAddSubMenu("Action", menu_sub_action);
	glutAddSubMenu("Shader", menu_sub_shader);
	glutAddSubMenu("FBO Shader", menu_sub_buffer);
	glutAddMenuEntry("Exit", 1000000);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	////////////////////

	//Register GLUT callback functions
	////////////////////
	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
	glutMouseFunc(My_Mouse);
	glutKeyboardFunc(My_Keyboard);
	glutTimerFunc(16, My_Timer, 0); 
	glutPassiveMotionFunc(My_Mouse_Moving);           
	glutMotionFunc(My_Mouse_Moving);     
	////////////////////

	// Enter main event loop.
	glutMainLoop();

	return 0;
}

