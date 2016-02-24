#include "ExperienceCheckpoints.h"
        
void ExperienceCheckpoints::initParameters(Parameters &parameters, Metabot::Robot *robot)
{
    ExperienceController::initParameters(parameters, robot);

    parameters.add("maxTurn", 0, 3, 0.8);
}
        
bool ExperienceCheckpoints::optimizeTurn()
{
    return true;
}

void ExperienceCheckpoints::init(Experience::Parameters &parameters, Metabot::Robot *robot)
{
    ExperienceController::init(parameters, robot);

    announcedCheckpoint = -1;
    currentCheckpoint = 0;
    maxStep = parameters.get("dx");
    maxTurn = parameters.get("maxTurn");

    checkpoints.push_back(FPoint2(500, 0.0));
    checkpoints.push_back(FPoint2(500, 500));
    checkpoints.push_back(FPoint2(1000, -250));
    controlT = 0;
}

void ExperienceCheckpoints::control(Simulation *simulation)
{
    if (announcedCheckpoint != currentCheckpoint) {
        if (simulation->server) {
            simulation->server->updateMarker(checkpoints[currentCheckpoint].x, checkpoints[currentCheckpoint].y);
        }
        announcedCheckpoint = currentCheckpoint;
    }

    controlT += simulation->dt;
    if (controlT > 0.02) {
        // Updating control orders
        controlT -= 0.02;

        // Getting robot state
        auto checkpoint = checkpoints[currentCheckpoint];
        auto state = simulation->robot.getState();
        auto angles = state.toRPY();

        // Getting the target vector
        double dX = checkpoint.x-state.x();
        double dY = checkpoint.y-state.y();
        double distance = sqrt(dX*dX + dY*dY);
        if (distance < 100) {
            if (currentCheckpoint < checkpoints.size()-1) {
                currentCheckpoint++;
            } else {
                simulation->over = true;
            }
        } else {
            double theta = atan2(dY, dX);
            double error = theta-angles.z();
            while (error < -M_PI) error += 2*M_PI;
            while (error > M_PI) error -= 2*M_PI;

            // The turn is the error 
            controller->turn = 0.6*controller->turn + 0.4*error*0.7;
            if (controller->turn < -maxTurn) controller->turn = -maxTurn;
            if (controller->turn > maxTurn) controller->turn = maxTurn;

            // The stepping is maxStep multiplied with error cosine
            controller->dx = 0.6*controller->dx + 0.4*maxStep*cos(error);
            if (controller->dx < 0) controller->dx = 0;
        }
    }

    ExperienceController::control(simulation);
}

double ExperienceCheckpoints::score(Simulation *simulation)
{
    if (simulation->over) {
        return scoreOver(simulation);
    } else {
        auto checkpoint = checkpoints[currentCheckpoint];
        int missedCheckpoints = checkpoints.size()-currentCheckpoint;
        auto state = simulation->robot.getState();
        double dX = checkpoint.x-state.x();
        double dY = checkpoint.y-state.y();
        double distance = sqrt(dX*dX + dY*dY);

        return 1e6*missedCheckpoints + distance;
    }
}
                
double ExperienceCheckpointsEfficience::scoreOver(Simulation *simulation)
{
    return cost*pow(collisions, 2);
}

double ExperienceCheckpointsSpeed::scoreOver(Simulation *simulation)
{
    return simulation->t*pow(collisions, 2);
}
