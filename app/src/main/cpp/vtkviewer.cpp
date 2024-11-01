#include <jni.h>
#include <android/log.h>
#include <android/input.h>
#include <android/native_window_jni.h>

#include "vtkAndroidRenderWindowInteractor.h"
#include "vtkDoubleArray.h"
#include "vtkLightKit.h"
#include "vtkNamedColors.h"
#include "vtkPlaneSource.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkRendererCollection.h"
#include "vtkShaderProperty.h"
#include "vtkTransform.h"
#include "vtkTransformFilter.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkUniforms.h"
#include "vtkWarpScalar.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "VTKViewerJNI", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "VTKViewerJNI", __VA_ARGS__))

namespace {
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;

    const std::string warpByBesselRadius = R"(
float r = sqrt(vertexMC.x * vertexMC.x + vertexMC.y * vertexMC.y);
float b = exp(-r) * cos(beta * r);
vec4 vertexMCNew = vec4(vertexMC.xyz + 2.0 * b * vec3(0, 0, 1), 1.0);
vertexVCVSOutput = MCVCMatrix * vertexMCNew;
gl_Position = MCDCMatrix * vertexMCNew;
)";
}


extern "C"
{

JNIEXPORT void JNICALL Java_com_kitware_vtkviewer_VTKViewerJNI_init(JNIEnv *env, jclass cls) {
    LOGI("init");
    vtkNew<vtkRenderer> renderer;
    renderWindow = vtk::TakeSmartPointer(vtkRenderWindow::New());
    renderWindowInteractor = vtk::TakeSmartPointer(vtkRenderWindowInteractor::New());
    renderWindowInteractor->SetRenderWindow(renderWindow);

    renderWindow->SetWindowInfo("jni");
    renderWindow->AddRenderer(renderer);

    vtkNew<vtkPlaneSource> sheet;
    sheet->SetNormal(0, 0, 1);
    sheet->SetResolution(512, 512);
    sheet->SetPoint1(10, -10, 0);
    sheet->SetPoint2(-10, 10, 0);
    sheet->SetOrigin(-10, -10, 0);

    sheet->Update();

    // cannot rely on the normals since the point positions themselves are warped in the vertex shader.
    auto input = sheet->GetOutput();
    input->GetPointData()->SetNormals(nullptr);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(input);

    actor = vtk::TakeSmartPointer(vtkActor::New());
    actor->SetMapper(mapper);
    vtkNew<vtkNamedColors> colors;
    actor->GetProperty()->SetColor(
            colors->GetColor4d("Tomato").GetData());

    auto *shaderProperty = actor->GetShaderProperty();
    shaderProperty->AddVertexShaderReplacement("//VTK::PositionVC::Impl", true,
                                               warpByBesselRadius, false);
    auto vu = shaderProperty->GetVertexCustomUniforms();
    vu->SetUniformf("beta", 1.0);

    // Assign our actor to the renderer.
    renderer->AddActor(actor);

    renderer->SetBackground(1, 1, 1);
    renderer->AutomaticLightCreationOff();

    vtkNew<vtkLightKit> lightKit;
    lightKit->AddLightsToRenderer(renderer);
}

JNIEXPORT void JNICALL Java_com_kitware_vtkviewer_VTKViewerJNI_draw(JNIEnv *env, jclass cls) {
    auto *shaderProperty = actor->GetShaderProperty();
    renderWindow->Render();
}

JNIEXPORT void JNICALL
Java_com_kitware_vtkviewer_VTKViewerJNI_setBeta(JNIEnv *env, jclass cls, jfloat beta) {
    auto *shaderProperty = actor->GetShaderProperty();
    auto vu = shaderProperty->GetVertexCustomUniforms();
    vu->SetUniformf("beta", beta);
    renderWindow->Render();
}

JNIEXPORT void
JNICALL
Java_com_kitware_vtkviewer_VTKViewerJNI_resize(JNIEnv *env, jclass cls, jint width, jint height) {
    LOGI("resize %d %d", width, height);
    renderWindow->SetSize(width, height);
}


JNIEXPORT void JNICALL Java_com_kitware_vtkviewer_VTKViewerJNI_handleTouchEvent(JNIEnv *env,
                                                                                jclass cls,
                                                                                jobject motionEvent
) {
    auto androidInteractor = vtkAndroidRenderWindowInteractor::SafeDownCast(renderWindowInteractor);
    auto inputEvent = const_cast<AInputEvent *>(AMotionEvent_fromJava(env, motionEvent));
    androidInteractor->HandleInput(inputEvent);
    AInputEvent_release(inputEvent);
}

JNIEXPORT void JNICALL Java_com_kitware_vtkviewer_VTKViewerJNI_handleKeyEvent(JNIEnv *env,
                                                                              jclass cls,
                                                                              jobject keyEvent
) {
    auto androidInteractor = vtkAndroidRenderWindowInteractor::SafeDownCast(renderWindowInteractor);
    auto inputEvent = const_cast<AInputEvent *>(AKeyEvent_fromJava(env, keyEvent));
    androidInteractor->HandleInput(inputEvent);
    AInputEvent_release(inputEvent);
}
}