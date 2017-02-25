package com.cm.opengles;

public class CmOpenGLES {
	
	static {
		System.loadLibrary("CameraOpenGLES");
	}
	
	public static void init(int width, int height){
		init(width, height, 0);
	}
	
	/**
	 * ��ʼ��OpenGL ES 2.0
	 */
	public static native void init(int width, int height, int angle);
	
	/**
	 * �ͷ�
	 */
	public static native void release();
	
	/**
	 * ������
	 * @param width
	 * @param height
	 */
	public static native void changeLayout(int width, int height);
	
	/**
	 * ��ȾYuv����
	 * @param data
	 */
	public static native void drawFrame(byte[] data, int size);
	
	/**
	 * �޸���ʾͼ��ĽǶ�
	 * @param angle
	 */
	public static native void changeAngle(int angle);
	
	/**
	 * ��ʼ��EGL ��SurfaceViewʹ����Ҫ��
	 * @param surface
	 */
	public static native void initEGL(android.view.Surface surface, int width, int height);
	
	/**
	 * ˢ��EGLǰ�󻭲���������ֻ���ͼ��
	 */
	public static native void swapEGL();
	
	/**
	 * ����EGL ��SurfaceViewʹ����Ҫ��
	 * @param surface
	 */
	public static native void destoryEGL();
	
}
