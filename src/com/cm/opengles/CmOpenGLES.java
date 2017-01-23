package com.cm.opengles;

public class CmOpenGLES {
	
	static {
		System.loadLibrary("CameraOpenGLES");
	}
	
	/**
	 * ��ʼ��OpenGL ES 2.0
	 */
	public static native void init(int width, int height);
	
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
}
