# Define the environment grid size
GRID_WIDTH = 5
GRID_HEIGHT = 5

# Initial position of the agent
agent_position = {'x': 2, 'y': 2}

def move_up(position):
    if position['y'] > 0:
        position['y'] -= 1

def move_down(position):
    if position['y'] < GRID_HEIGHT - 1:
        position['y'] += 1

def move_left(position):
    if position['x'] > 0:
        position['x'] -= 1

def move_right(position):
    if position['x'] < GRID_WIDTH - 1:
        position['x'] += 1

def decide_action(position):
    if position['x'] == 0:
        return 'move_right'
    elif position['x'] == GRID_WIDTH - 1:
        return 'move_left'
    elif position['y'] == 0:
        return 'move_down'
    elif position['y'] == GRID_HEIGHT - 1:
        return 'move_up'
    else:
        return 'move_up'

def run_agent():
    for _ in range(10):  # Run the agent for 10 steps
        action = decide_action(agent_position)
        print(f"Deciding to {action} from position {agent_position}")

        if action == 'move_up':
            move_up(agent_position)
        elif action == 'move_down':
            move_down(agent_position)
        elif action == 'move_left':
            move_left(agent_position)
        elif action == 'move_right':
            move_right(agent_position)

        print(f"New position: {agent_position}")

run_agent()
