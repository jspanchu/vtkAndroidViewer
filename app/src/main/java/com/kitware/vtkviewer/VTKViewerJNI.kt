package com.kitware.vtkviewer

import android.view.KeyEvent
import android.view.MotionEvent

class vtkAndroidViewerJNI {
    companion object {
        init {
            System.loadLibrary("vtkviewer")
        }

        @JvmStatic
        external fun init()

        @JvmStatic
        external fun resize(width: Int, height: Int)

        @JvmStatic
        external fun resetCamera()

        @JvmStatic
        external fun draw()

        @JvmStatic
        external fun handleTouchEvent(event: MotionEvent)

        @JvmStatic
        external fun handleKeyEvent(event: KeyEvent)
    }
}