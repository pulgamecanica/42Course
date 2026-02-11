class AutonomousVehicle:
    def __init__(self):
        self.sensors = {"camera": None, "lidar": None, "gps": None}

    def perceive_environment(self):
        # Simulating sensor data collection
        self.sensors["camera"] = "obstacle detected"
        self.sensors["lidar"] = "distance: 5m"
        self.sensors["gps"] = "location: (40.7128, -74.0060)"

    def reason_and_act(self):
        # Simplified reasoning based on sensor data
        if self.sensors["camera"] == "obstacle detected":
            self.react_to_obstacle()

    def react_to_obstacle(self):
        print("Slowing down and steering to avoid obstacle.")

vehicle = AutonomousVehicle()
vehicle.perceive_environment()
vehicle.reason_and_act()
