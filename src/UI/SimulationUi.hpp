#pragma once

struct GLFWwindow;
class SensorRig;

class SimulationUi
{
public:
    explicit SimulationUi(GLFWwindow* window);
    ~SimulationUi();

    SimulationUi(const SimulationUi&) = delete;
    SimulationUi& operator=(const SimulationUi&) = delete;

    void beginFrame();

    void drawSimulation(
        double simulationTime,
        const SensorRig& rig
    );

    void render();
};