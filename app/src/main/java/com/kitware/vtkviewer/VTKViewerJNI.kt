package com.kitware.vtkviewer

import android.view.KeyEvent
import android.view.MotionEvent

class VTKViewerJNI {
    companion object {
        init {
            System.loadLibrary("vtkviewer")
        }

        @JvmStatic
        external fun init()

        @JvmStatic
        external fun resize(width: Int, height: Int)

        @JvmStatic
        external fun setBeta(beta: Float)

        @JvmStatic
        external fun draw()

        @JvmStatic
        external fun handleTouchEvent(event: MotionEvent)

        @JvmStatic
        external fun handleKeyEvent(event: KeyEvent)
    }
}