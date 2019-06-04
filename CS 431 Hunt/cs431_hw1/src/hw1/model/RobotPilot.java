package hw1.model;

public interface RobotPilot {
    public void init(int deltaRowToGoal, int deltaColToGoal);    
    public Action act(Sensors sensors);
}

