#include "SimulationUi.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <algorithm>

SimulationUi::SimulationUi(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

SimulationUi::~SimulationUi()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void SimulationUi::beginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

}

void SimulationUi::drawSimulation(
        bool& playing,
        double& simulationTime,
        double duration,
        const SensorRig& rig)
{
    ImGui::Begin("Simulation");
    if (ImGui::Button(playing ? "Pause" : "Play")) {
        playing = !playing;
    }    
    float time =
    static_cast<float>(simulationTime);

    if (ImGui::SliderFloat(
            "Time",
            &time,
            0.0f,
            static_cast<float>(duration),
            "%.2f s"))
    {
        simulationTime = static_cast<double>(time);
    }
    ImGui::End();
}


void SimulationUi::drawSensorPreview(
    unsigned int textureId,
    int width,
    int height)
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 workPos = viewport->WorkPos;
    const ImVec2 workSize = viewport->WorkSize;

    constexpr float panelWidth = 380.0f;
    constexpr float panelHeight = 320.0f;
    constexpr float margin = 20.0f;

    ImGui::SetNextWindowPos(
        ImVec2(
            workPos.x + workSize.x - panelWidth - margin,
            workPos.y + 220.0f
        ),
        ImGuiCond_Always
    );

    ImGui::SetNextWindowSize(
        ImVec2(panelWidth, panelHeight),
        ImGuiCond_Always
    );

    ImGui::Begin("Sensor");

    const float availableWidth = ImGui::GetContentRegionAvail().x;
    const float aspect = static_cast<float>(width) / static_cast<float>(height);
    const float previewWidth = std::min( availableWidth, static_cast<float>(width) );
    const float previewHeight = previewWidth / aspect;

    ImGui::Image(
        ImTextureRef(
            static_cast<ImTextureID>(textureId)
        ),
        ImVec2(
            previewWidth,
            previewHeight
        ),
        ImVec2(0.0f, 1.0f),
        ImVec2(1.0f, 0.0f)
    );

    ImGui::End();
}



void SimulationUi::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(
        ImGui::GetDrawData()
    );
}