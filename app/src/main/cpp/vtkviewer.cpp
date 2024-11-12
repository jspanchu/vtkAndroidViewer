#include <jni.h>
#include <android/input.h>
#include <android/log.h>

#include "vtkAndroidRenderWindowInteractor.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkLightKit.h"
#include "vtkNamedColors.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRendererCollection.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

#include "teapot.h" // has mesh vertices and connectivity.

#include <sstream>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "vtkAndroidViewerJNI", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "vtkAndroidViewerJNI", __VA_ARGS__))

namespace {
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
}

extern "C"
{

JNIEXPORT void JNICALL Java_com_kitware_vtkviewer_vtkAndroidViewerJNI_init(JNIEnv *env, jclass cls) {
    LOGI("init");
    vtkNew<vtkRenderer> renderer;
    renderWindow = vtk::TakeSmartPointer(vtkRenderWindow::New());
    renderWindowInteractor = vtk::TakeSmartPointer(vtkRenderWindowInteractor::New());
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindowInteractor->EnableRenderOff();
    renderWindow->DoubleBufferOff();

    renderWindow->SetWindowInfo("jni");
    renderWindow->AddRenderer(renderer);

    vtkIdType numPoints = sizeof(teapotPositions) / sizeof(teapotPositions[0]) / 3;

    vtkNew<vtkFloatArray> positions;
    positions->SetNumberOfComponents(3);
    positions->SetArray(teapotPositions, numPoints * 3, 1);

    vtkNew<vtkFloatArray> normals;
    normals->SetNumberOfComponents(3);
    normals->SetArray(teapotNormals, numPoints * 3, 1);

    vtkNew<vtkFloatArray> tangents;
    tangents->SetNumberOfComponents(3);
    tangents->SetArray(teapotTangents, numPoints * 3, 1);

    vtkNew<vtkFloatArray> textureCoords;
    textureCoords->SetNumberOfComponents(3);
    textureCoords->SetArray(teapotTexCoords, numPoints * 3, 1);

    vtkNew<vtkPoints> points;
    points->SetData(positions);

    vtkNew<vtkPolyData> mesh;
    mesh->SetPoints(points);
    mesh->GetPointData()->SetNormals(normals);
    mesh->GetPointData()->SetTCoords(textureCoords);
    mesh->GetPointData()->SetTangents(tangents);

    vtkNew<vtkCellArray> triangles;
    vtkNew<vtkIdTypeArray> connectivity;
    vtkIdType numIndices = sizeof(::teapotIndices) / sizeof(teapotIndices[0]);
    connectivity->SetArray(::teapotIndices, numIndices, 1);
    triangles->SetData(3, connectivity);
    mesh->SetPolys(triangles);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputData(mesh);

    actor = vtk::TakeSmartPointer(vtkActor::New());
    actor->SetMapper(mapper);
    vtkNew<vtkNamedColors> colors;
    actor->GetProperty()->SetColor(
            colors->GetColor4d("Tomato").GetData());

    // Assign our actor to the renderer.
    renderer->AddActor(actor);

    renderer->SetBackground(1, 1, 1);
    renderer->AutomaticLightCreationOff();

    vtkNew<vtkLightKit> lightKit;
    lightKit->AddLightsToRenderer(renderer);
}

JNIEXPORT void JNICALL Java_com_kitware_vtkviewer_vtkAndroidViewerJNI_draw(JNIEnv *env, jclass cls) {
    renderWindow->Render();
}

JNIEXPORT void JNICALL
Java_com_kitware_vtkviewer_vtkAndroidViewerJNI_resetCamera(JNIEnv *env, jclass cls) {
    renderWindow->GetRenderers()->GetFirstRenderer()->ResetCamera();
}

JNIEXPORT void
JNICALL
Java_com_kitware_vtkviewer_vtkAndroidViewerJNI_resize(JNIEnv *env, jclass cls, jint width, jint height) {
    LOGI("resize %d %d", width, height);
    renderWindow->SetSize(width, height);
}


JNIEXPORT void JNICALL Java_com_kitware_vtkviewer_vtkAndroidViewerJNI_handleTouchEvent(JNIEnv *env,
                                                                                jclass cls,
                                                                                jobject motionEvent
) {
    auto androidInteractor = vtkAndroidRenderWindowInteractor::SafeDownCast(renderWindowInteractor);
    auto inputEvent = const_cast<AInputEvent *>(AMotionEvent_fromJava(env, motionEvent));
    androidInteractor->HandleInput(inputEvent);
    AInputEvent_release(inputEvent);
}

JNIEXPORT void JNICALL Java_com_kitware_vtkviewer_vtkAndroidViewerJNI_handleKeyEvent(JNIEnv *env,
                                                                              jclass cls,
                                                                              jobject keyEvent
) {
    auto androidInteractor = vtkAndroidRenderWindowInteractor::SafeDownCast(renderWindowInteractor);
    auto inputEvent = const_cast<AInputEvent *>(AKeyEvent_fromJava(env, keyEvent));
    androidInteractor->HandleInput(inputEvent);
    AInputEvent_release(inputEvent);;
}
}
