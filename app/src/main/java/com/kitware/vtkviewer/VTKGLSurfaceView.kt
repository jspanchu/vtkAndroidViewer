package com.kitware.vtkviewer

import android.content.Context
import android.opengl.GLSurfaceView
import android.view.KeyEvent
import android.view.MotionEvent

class VTKGLSurfaceView(context: Context) : GLSurfaceView(context) {
    var renderer: VTKRenderer

    init {
        setEGLContextClientVersion(3)
        renderer = VTKRenderer()
        setRenderer(renderer)
        renderMode = RENDERMODE_WHEN_DIRTY
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        var safeEvent = MotionEvent.obtain(event);
        queueEvent {
            vtkAndroidViewerJNI.handleTouchEvent(safeEvent)
            safeEvent.recycle()
        }
        requestRender()
        return true
    }

    override fun onKeyUp(keyCode: Int, event: KeyEvent): Boolean {
        queueEvent {
            vtkAndroidViewerJNI.handleKeyEvent(event)
        }
        return true
    }

    override fun onKeyDown(keyCode: Int, event: KeyEvent): Boolean {
        queueEvent {
            vtkAndroidViewerJNI.handleKeyEvent(event)
        }
        return true
    }
}