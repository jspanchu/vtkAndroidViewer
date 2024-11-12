package com.kitware.vtkviewer

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class VTKRenderer() : GLSurfaceView.Renderer {

    override fun onDrawFrame(gl: GL10?) {
        vtkAndroidViewerJNI.draw()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        vtkAndroidViewerJNI.resize(width, height)
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        vtkAndroidViewerJNI.init()
    }
}
