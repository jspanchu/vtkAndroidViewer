package com.kitware.vtkviewer

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.focusable
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Slider
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.mutableFloatStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.getValue
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.input.key.KeyEventType
import androidx.compose.ui.input.key.onKeyEvent
import androidx.compose.ui.input.key.type
import androidx.compose.ui.viewinterop.AndroidView

lateinit var vtkGLView: VTKGLSurfaceView

@Composable
fun MainView() {
    var sliderPosition by remember { mutableFloatStateOf(INITIAL_BETA) }
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
        Slider(
            value = sliderPosition,
            valueRange = 1.0f..10.0f,
            onValueChange = {
                sliderPosition = it
                vtkGLView.queueEvent { VTKViewerJNI.setBeta(it) }
            }
        )
        Text(
            modifier = Modifier.align(Alignment.CenterHorizontally),
            text = "z = exp(-r) * cos(%.2f * r)".format(sliderPosition)
        )
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