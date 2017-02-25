#include "cameraShader.h"



extern const float dataVertex[];
extern const float dataTexCoor[];

extern void printData(void* data, const int size, const char * name);
void bindTexture();

void
drawFrame(void* ins)
{
	if(DEBUG)
	  {
	    LOGI_EU("%s", __FUNCTION__);
	  }
	//�����Ȼ���,��ɫ����
	glClear(GL_COLOR_BUFFER_BIT);
	Instance * instance = (Instance *)ins;
	if (instance == 0)
	  {
	    LOGW_EU("%s Program is NULL return!", __FUNCTION__);
	    return;
	  }
	//ѡ����ɫ����
	glUseProgram(instance->pProgram);
	//�趨�ӿ�
	changeViewPort (instance->vHeight, instance->vWidth, instance, instance->angle);
	//����任��������
	//		1.��ʼ����ת����
	float * maMVPMatrix = getRotateM(NULL, 0, instance->angle, 0, 0, 1);
//	//		2.��ʼ���۲����
//	float * lookAtM = setLookAtM(NULL, 0,
//								0, 0, 2,//float eyeX, float eyeY, float eyeZ,
//								0, 0, 0,//float centerX, float centerY, float centerZ,
//								0, 1, 0//float upX, float upY, float upZ
//	);
//	//		3.͸��ͶӰ����
//	float radio = (float)instance->pHeight / (float)instance->pWidth;
//	float * projM = frustumM(NULL, 0,
//							-radio, radio,//float left, float right,
//							-1, 1,//float bottom, float top,
//							1, 4//float near, GLfloat far
//	);
//	//		3.2���Ͼ���
//	matrixMM4(maMVPMatrix, lookAtM);
//	matrixMM4(maMVPMatrix, projM);
//	//		4.���������ݵ�������ɫ��
	glUniformMatrix4fv(instance->maMVPMatrixHandle, 1, GL_FALSE, maMVPMatrix);
//	//		5.�ͷ�
	free(maMVPMatrix);
//	free(lookAtM);
//	free(projM);
	//���붥�����ݵ���ɫ������
	glVertexAttribPointer(instance->maPositionHandle,
						3,//GLint size X Y Z
						GL_FLOAT,//GLenum type
						GL_FALSE,//GLboolean normalized
						3 * 4,//GLsizei stride
						dataVertex//const GLvoid * ptr
	);
	//���붥����������
	glVertexAttribPointer(instance->maTexCoorHandle,
							2,//S T
							GL_FLOAT,//GLenum type
							GL_FALSE,//GLboolean normalized
							2 * 4,//GLsizei stride
							dataTexCoor//const GLvoid * ptr
	);
	//���������
	bindTexture(GL_TEXTURE0, instance->yTexture, instance->pWidth, instance->pHeight, instance->yBuffer);
	bindTexture(GL_TEXTURE1, instance->uTexture, instance->pWidth / 2, instance->pHeight / 2, instance->uBuffer);
	bindTexture(GL_TEXTURE2, instance->vTexture, instance->pWidth / 2, instance->pHeight / 2, instance->vBuffer);
	glUniform1i(instance->myTextureHandle, 0);
	glUniform1i(instance->muTextureHandle, 1);
	glUniform1i(instance->mvTextureHandle, 2);
//	printData(instance->yBuffer, 20, "yBuffer 10 : ");
//	printData(instance->uBuffer, 20, "uBuffer 10 : ");
//	printData(instance->vBuffer, 20, "vBuffer 10 : ");
	//��������������
	glEnableVertexAttribArray(instance->maPositionHandle);
	glEnableVertexAttribArray(instance->maTexCoorHandle);
	//�����������
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
/**
 * ��ʼ��OpenGL ES 2.0
 */
void
openglInit (const char* codeVertexShader, const char* codeFragShader,
	    unsigned int pWidth, unsigned int pHeight, unsigned int angle, Instance* instance)
{
  //	1.��ʼ����ɫ��
  GLuint shaders[2] =
    { 0 };
  shaders[0] = initShader (codeVertexShader, GL_VERTEX_SHADER);
  shaders[1] = initShader (codeFragShader, GL_FRAGMENT_SHADER);
  instance->pProgram = initProgram (shaders, 2);
  instance->maMVPMatrixHandle = glGetUniformLocation (instance->pProgram,
						      "uMVPMatrix");
  instance->maPositionHandle = glGetAttribLocation (instance->pProgram,
						    "aPosition");
  instance->maTexCoorHandle = glGetAttribLocation (instance->pProgram,
						   "aTexCoor");
  instance->myTextureHandle = glGetUniformLocation (instance->pProgram,
						    "yTexture");
  instance->muTextureHandle = glGetUniformLocation (instance->pProgram,
						    "uTexture");
  instance->mvTextureHandle = glGetUniformLocation (instance->pProgram,
						    "vTexture");
  //	2.��ʼ������
  //		2.1��������id
  glGenTextures (1, &instance->yTexture);
  glGenTextures (1, &instance->uTexture);
  glGenTextures (1, &instance->vTexture);
  LOGI_EU("init() yT = %d, uT = %d, vT = %d.", instance->yTexture,
	  instance->uTexture, instance->vTexture);
  LOGI_EU("%s %d error = %d", __FILE__, __LINE__, glGetError ());
  //	3.����Yuv�����ڴ�
  instance->yBufferSize = sizeof(char) * pWidth * pHeight;
  instance->uBufferSize = sizeof(char) * pWidth / 2 * pHeight / 2;
  instance->vBufferSize = sizeof(char) * pWidth / 2 * pHeight / 2;
  instance->yBuffer = (char*) malloc (instance->yBufferSize);
  instance->uBuffer = (char*) malloc (instance->uBufferSize);
  instance->vBuffer = (char*) malloc (instance->vBufferSize);
  memset (instance->yBuffer, 0, instance->yBufferSize);
  memset (instance->uBuffer, 0, instance->uBufferSize);
  memset (instance->vBuffer, 0, instance->vBufferSize);
  instance->pHeight = pHeight;
  instance->pWidth = pWidth;
  LOGI_EU("width = %d, height = %d", instance->pWidth, instance->pHeight);
  //	4.����ͼ��Ƕ�
  instance->angle = angle;
  //������
  glClearColor (0.5f, 0.5f, 0.5f, 1.0f);
  LOGI_EU("%s %d error = %d", __FILE__, __LINE__, glGetError ());
}

/**
 * �������ɺ���
 */
void bindTexture(GLenum texture_n, GLuint texture_id, GLsizei width, GLsizei height, const void * buffer)
{
//	LOGI_EU("texture_n = %x, texture_id = %d, width = %d, height = %d", texture_n, texture_id, width, height);
	//��������
	//		2.������
	glActiveTexture(texture_n);//eg:GL_TEXTURE0
	//		1.1������id
	glBindTexture(GL_TEXTURE_2D, texture_id);
	//		2.3���ò���ģʽ
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//		1.2������������
	glTexImage2D(GL_TEXTURE_2D,
				0,//GLint level
				GL_LUMINANCE,//GLint internalformat
				width,//GLsizei width
				height,// GLsizei height,
				0,//GLint border,
				GL_LUMINANCE,//GLenum format,
				GL_UNSIGNED_BYTE,//GLenum type,
				buffer//const void * pixels
	);
};

/**
 * �ı��ӿں���
 */
void changeViewPort (unsigned int height, unsigned int width, Instance* instance, unsigned int angle)
{
  unsigned int eW, eH;
  float vRadio, radio;
  vRadio = (float) height / width;
  if (angle == 90 || angle == 270)
    {
      radio = ((float) instance->pWidth / instance->pHeight);
    }
  else
    {
      radio = ((float) instance->pHeight / instance->pWidth);
    }
  if (vRadio < radio)
    {
      eH = instance->vHeight;
      eW = (unsigned int) (eH / radio);
    }
  else
    {
      eW = instance->vWidth;
      eH = (unsigned int) (eW * radio);
    }
  LOGI_EU(
      "changeViewPort() eW = %d, eH = %d, radio = %f, vRadio = %f, instance->pHeight = %d, instance->pWidth = %d",
      eW, eH, radio, vRadio, instance->pHeight, instance->pWidth);
  int offsetX, offsetY;
  if (eW == width)
    {
      offsetX = 0;
    }
  else
    {
      offsetX = (width - eW) / 2;
    }
  if (eH == height)
    {
      offsetY = 0;
    }
  else
    {
      offsetY = (height - eH) / 2;
    }
  glViewport (offsetX, offsetY, eW, eH);
  LOGI_EU("changeViewPort() offsetX = %d, offsetY = %d, eW = %d, eH = %d",
	  offsetX, offsetY, eW, eH);
}

/**
 * �ͷ�
 */
void release(Instance* instance)
{
  if(instance != 0)
  		{
  			free(instance->yBuffer);
  			free(instance->uBuffer);
  			free(instance->vBuffer);
  			instance->yBuffer = 0;
  			instance->uBuffer = 0;
  			instance->vBuffer = 0;
  			free(instance);
  		}
}

