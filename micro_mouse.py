from enum import Enum

class Direction(Enum):
    NORTH = 1
    EAST = 2
    SOUTH = 3
    WEST = 4


class MicroMouse:
    def __init__(self, x, y, max_x, max_y, direction = Direction.SOUTH):
        self.x = x
        self.y = y
        self.max_x = max_x
        self.max_y = max_y
        self.direction = direction
        
    def turn_left(self):
        direction_value = self.direction.value
        direction_value -= 1
        if direction_value < 1:
            direction_value = 4
        self.direction = Direction(direction_value)
    
    def turn_right(self):
        direction_value = self.direction.value
        direction_value += 1
        if direction_value > 4:
            direction_value %= 4
        self.direction = Direction(direction_value)
        
    def make_u_turn(self):
        for i in range(2):
            self.turn_right()
            
    def get_direction(self):
        if self.direction == Direction.NORTH:
            return "NORTH"
        if self.direction == Direction.EAST:
            return "EAST"
        if self.direction == Direction.SOUTH:
            return "SOUTH"
        if self.direction == Direction.WEST:
            return "WEST"
        
    def move_forward(self):
        if (self.direction == Direction.SOUTH) and ((self.y + 1) < self.max_y):
            self.y += 1
        if (self.direction == Direction.WEST) and ((self.x - 1) > 0):
            self.x -= 1
        if (self.direction == Direction.NORTH) and ((self.y - 1) > 0):
            self.y -= 1
        if (self.direction == Direction.EAST) and ((self.x + 1) < self.max_y):
            self.x += 1
            
if __name__ == "__main__":
    mm_obj = MicroMouse(2, 0)
    mm_obj.direction
    i = 0
    while i<10:
        mm_obj.turn_right()
        print(mm_obj.get_direction())
        i+=1