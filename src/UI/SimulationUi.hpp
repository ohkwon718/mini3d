#pragma once

struct GLFWwindow;
class SensorRig;
class Camera;

class SimulationUi
{
public:
    explicit SimulationUi(GLFWwindow* window);
    ~SimulationUi();

    SimulationUi(const SimulationUi&) = delete;
    SimulationUi& operator=(const SimulationUi&) = delete;

    void beginFrame();

    void drawSimulation(
        bool& playing,
        double& simulationTime,
        double duration
    );

    void drawSensorPreview(
        unsigned int textureId,
        int width,
        int height
    );

    void drawSensorInfo(
        const SensorRig& rig,
        const Camera& camera
    );

    void render();
};