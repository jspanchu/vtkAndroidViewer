package com.kitware.vtkviewer

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.focusable
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Button
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.input.key.KeyEventType
import androidx.compose.ui.input.key.onKeyEvent
import androidx.compose.ui.input.key.type
import androidx.compose.ui.viewinterop.AndroidView

lateinit var vtkGLView: VTKGLSurfaceView

@Composable
fun MainView() {
    Column(
        Modifier
            .fillMaxSize()
            .focusable()
            .onKeyEvent {
                if (it.type == KeyEventType.KeyUp) {
                    vtkGLView.onKeyUp(it.nativeKeyEvent.keyCode, it.nativeKeyEvent)
                } else {
                    vtkGLView.onKeyDown(it.nativeKeyEvent.keyCode, it.nativeKeyEvent)
                }
                true
            }) {
        Button(modifier = Modifier.align(Alignment.End), onClick = {
            vtkGLView.queueEvent { vtkAndroidViewerJNI.resetCamera() }
            vtkGLView.requestRender()
        }) {
            Text("Reset camera")
        }
        AndroidView(
            modifier = Modifier.fillMaxSize(), // Occupy the max size in the Compose UI tree
            factory = { context ->
                // Create GLSurfaceView
                VTKGLSurfaceView(context).also { vtkGLView = it }
            }
        )
    }
}


class MainActivity : ComponentActivity() {

    public override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            MainView()
        }
    }
}