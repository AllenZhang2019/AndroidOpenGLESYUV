#include <jni.h>
#include "opengles/esUtil.h"
#include "opengles/matrix.h"
#include "cameraShader.h"

//#ifdef __cplusplus
//    extern "C" {
//#endif

#define UNIT 1
#define TEXTURE_COOR_UNIT 1

//������ɫ���ű�����
const char * codeVertexShader = \
"attribute vec3 aPosition;							\n" \
"uniform mat4 uMVPMatrix;	 						\n" \
"attribute vec2 aTexCoor; 							\n" \
"varying vec2 vTexCoor;		 						\n" \
"void main() 										\n" \
"{ 													\n" \
"	gl_Position = uMVPMatrix * vec4(aPosition, 1); 	\n" \
" 	vTexCoor = aTexCoor;							\n" \
"} 													\n" \
;
//ƬԪ��ɫ���ű�����
//const char * codeFragShader = \
//"precision mediump float;											\n" \
//"uniform sampler2D sTexture; 										\n" \
//"varying vec2 vTexCoor;												\n" \
//"void main()														\n" \
//"{																	\n" \
//"	float r,g,b,y;																	\n" \
//"	y = texture2D(sTexture, vTexCoor).r;																	\n" \
//"	r = y;																	\n" \
//"	g = y;																	\n" \
//"	b = y;																	\n" \
//"	gl_FragColor = vec4(r,g,b,1.0);										\n" \
//"}																	\n" \
//;

const char * codeFragShader = \
"precision mediump float;											\n" \
"uniform sampler2D sTexture; 										\n" \
"varying vec2 vTexCoor;												\n" \
"void main()														\n" \
"{																	\n" \
"	vec3 yuv = vec3(texture2D(sTexture, vTexCoor).r, 0.5, 0.5);		\n" \
"	vec3 offset = vec3(16.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0);								\n" \
"	mat3 mtr = mat3(1,1,1, -0.001,-0.3441,1.772, 1.402,-0.7141,0.001);\n" \
"	vec4 curColor = vec4(mtr * (yuv - offset), 1);					\n" \
"	gl_FragColor = curColor;										\n" \
"}																	\n" \
;
//"	mat3 mtr = mat3(1,-0.001,1.402,1,-0.3441,-0.7141,1,1.772,0.001);\n" \
//��Ⱦ������������
const float dataVertex[] = 
{
	-1 * UNIT, 1 * UNIT, 0,
	-1 * UNIT, -1 * UNIT, 0,
	1 * UNIT, -1 * UNIT, 0,
	1 * UNIT, 1 * UNIT, 0
};
//��Ⱦ������������
const float dataTexCoor[] =
{
	0 * TEXTURE_COOR_UNIT, 0 * TEXTURE_COOR_UNIT,
	0 * TEXTURE_COOR_UNIT, 1 * TEXTURE_COOR_UNIT,
	1 * TEXTURE_COOR_UNIT, 1 * TEXTURE_COOR_UNIT,
	1 * TEXTURE_COOR_UNIT, 0 * TEXTURE_COOR_UNIT
};

void printData(void* data, const int size, const char * name);

//ȫ�ֱ���
Instance *instance;

JNIEXPORT
jint
JNI_OnLoad(JavaVM * vm, void * reserved)
{
	LOGI_EU("JNI_OnLoad()");
	return JNI_VERSION_1_6;
}

JNIEXPORT
void
Java_com_cm_opengles_CmOpenGLES_init(JNIEnv *env, jobject obj, jint pWidth, jint pHeight)
{
	LOGI_EU("init()");
	instance = (Instance *)malloc(sizeof(Instance));
	memset(instance, 0, sizeof(Instance));
	//	1.��ʼ����ɫ��
	GLuint shaders[2] = {0};
	shaders[0] = initShader(codeVertexShader, GL_VERTEX_SHADER);
	shaders[1] = initShader(codeFragShader, GL_FRAGMENT_SHADER);
	instance->pProgram = initProgram(shaders, 2);
	instance->maMVPMatrixHandle = glGetUniformLocation( instance->pProgram, "uMVPMatrix");
	instance->maPositionHandle = glGetAttribLocation(instance->pProgram, "aPosition");
	instance->maTexCoorHandle = glGetAttribLocation(instance->pProgram, "aTexCoor");
	instance->msTextureHandle = glGetUniformLocation(instance->pProgram, "sTexture");
	//	2.��ʼ������
	//		2.1��������id
	glGenTextures(1, &instance->texture);
	LOGI_EU("%s %d error = %d", __FILE__,__LINE__, glGetError());
//	glActiveTexture(GL_TEXTURE0);
//	LOGI_EU("%s %d error = %d", __FILE__,__LINE__, glGetError());
//	//		2.2������id
////	glBindTexture(GL_TEXTURE_2D, instance->texture);
//	LOGI_EU("%s %d error = %d", __FILE__,__LINE__, glGetError());
//	//		2.3���ò���ģʽ
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	LOGI_EU("%s %d error = %d", __FILE__,__LINE__, glGetError());
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	LOGI_EU("%s %d error = %d", __FILE__,__LINE__, glGetError());
	//	3.����Yuv�����ڴ�
	instance->yBufferSize = sizeof(char) * pWidth * pHeight;
	instance->yBuffer = (char *)malloc(instance->yBufferSize);
	instance->pHeight = pHeight;
	instance->pWidth = pWidth;
	LOGI_EU("width = %d, height = %d", instance->pWidth, instance->pHeight);
	//������
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	//������ȼ��
//	glEnable(GL_DEPTH_TEST);
	LOGI_EU("%s %d error = %d", __FILE__,__LINE__, glGetError());
}

JNIEXPORT
void
Java_com_cm_opengles_CmOpenGLES_changeLayout(JNIEnv *env, jobject obj, jint width, jint height)
{
	LOGI_EU("changeLayout() width = %d, height = %d", width, height);
	if(instance != 0)
	  {
	    instance->vWidth = width;
	    instance->vHeight = height;
	  }
}
JNIEXPORT
void
Java_com_cm_opengles_CmOpenGLES_drawFrame(JNIEnv *env, jobject obj, jbyteArray yuvDatas, jint size)
{
	jbyte * srcp = (*env)->GetByteArrayElements(env, yuvDatas, 0);
	memcpy(instance->yBuffer, srcp, instance->yBufferSize);
	(*env)->ReleaseByteArrayElements(env, yuvDatas, srcp, JNI_ABORT);
	drawFrame(instance);
}

JNIEXPORT
void
Java_com_cm_opengles_CmOpenGLES_release(JNIEnv *env, jobject obj)
{
	LOGI_EU("release()");
	if(instance != 0)
		{
			free(instance->yBuffer);
			instance->yBuffer = 0;
			free(instance);
			instance = 0;
		}
}

void printData(void* data, const int size, const char * name)
{
	char * dt = (char *)data;
	int i,j;
	char * str = (char *) malloc(size);
	for (i = 0; i < size; i++)
		{
			sprintf(str + 2 * i,"%x%x", dt[i]);
		}
	LOGI_EU("%s , %s", name, str);
	free(str);
}























//#ifdef __cplusplus
//}
//#endif
