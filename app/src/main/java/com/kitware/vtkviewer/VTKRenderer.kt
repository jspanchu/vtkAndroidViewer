package com.kitware.vtkviewer

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

const val INITIAL_BETA = 5.0f

class VTKRenderer() : GLSurfaceView.Renderer {

    override fun onDrawFrame(gl: GL10?) {
        VTKViewerJNI.draw()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        VTKViewerJNI.resize(width, height)
    }

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        VTKViewerJNI.init()
        VTKViewerJNI.setBeta(INITIAL_BETA)
    }
}
