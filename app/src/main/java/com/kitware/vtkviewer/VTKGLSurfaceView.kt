package com.kitware.vtkviewer

import android.content.Context
import android.opengl.GLSurfaceView
import android.view.KeyEvent
import android.view.MotionEvent

class VTKGLSurfaceView(context: Context) : GLSurfaceView(context) {
    var renderer: VTKRenderer;

    init {
        setEGLContextClientVersion(3)
        renderer = VTKRenderer()
        setRenderer(renderer)
        renderMode = RENDERMODE_WHEN_DIRTY
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        queueEvent {
            VTKViewerJNI.handleTouchEvent(event)
        }
        return true;
    }

    override fun onKeyUp(keyCode: Int, event: KeyEvent): Boolean {
        queueEvent {
            VTKViewerJNI.handleKeyEvent(event)
        }
        return true;
    }

    override fun onKeyDown(keyCode: Int, event: KeyEvent): Boolean {
        queueEvent {
            VTKViewerJNI.handleKeyEvent(event)
        }
        return true;
    }
}