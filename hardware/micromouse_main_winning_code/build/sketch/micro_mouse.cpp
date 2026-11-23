#line 1 "C:\\D_Drive\\Events\\MicroMouse\\hardware\\micromouse_main\\micro_mouse.cpp"
#include "micro_mouse.h"

MicroMouse* MicroMouse::m_micromouse = nullptr;

MicroMouse::MicroMouse(uint8_t x, uint8_t y, MouseSensor* m_mouse_sensor_inst, MazeMemory* m_mazememory_inst, MouseMotor* m_mouse_motor, Logger *loggerInstance, uint8_t max_x, uint8_t max_y, Direction mouse_direction)
    : x(x), y(y), m_mouse_sensor(m_mouse_sensor_inst), m_mazememory(m_mazememory_inst), max_x(max_x), max_y(max_y), mouse_direction(mouse_direction), m_mouse_motor(m_mouse_motor), m_logger(loggerInstance)
{
}

void MicroMouse::turn_left() {
    uint8_t direction_value;
    direction_value = static_cast<uint8_t>(mouse_direction);
    direction_value -= 2;
    if (direction_value < 1) {
        direction_value = 4;
    }
    mouse_direction = static_cast<Direction>(direction_value);
    this->updateAngle();
}


void MicroMouse::turn_right() {
    uint8_t direction_value;
    direction_value = static_cast<uint8_t>(mouse_direction);
    direction_value += 2;
    if (direction_value > 8) {
        direction_value %= 8;
    }
    mouse_direction = static_cast<Direction>(direction_value);
    this->updateAngle();
}

void MicroMouse::turn_reverse() {
    uint8_t direction_value;
    direction_value = static_cast<uint8_t>(mouse_direction);
    direction_value += 4;
    if (direction_value > 8) {
        direction_value %= 8;
    }
    mouse_direction = static_cast<Direction>(direction_value);
    this->updateAngle();
    
}

void MicroMouse::updateAngle() {
    switch (mouse_direction) {
        case Direction::NORTH:
            m_mouse_motor->setDesiredAngle(270);
            break;
        case Direction::NORTH_EAST:
            m_mouse_motor->setDesiredAngle(315);
            break;
        case Direction::EAST:
            m_mouse_motor->setDesiredAngle(0);
            break;
        case Direction::SOUTH_EAST:
            m_mouse_motor->setDesiredAngle(45);
            break;
        case Direction::SOUTH:
            m_mouse_motor->setDesiredAngle(90);
            break;
        case Direction::SOUTH_WEST:
            m_mouse_motor->setDesiredAngle(135);
            break;
        case Direction::WEST:
            m_mouse_motor->setDesiredAngle(180);
            break;
        case Direction::NORTH_WEST:
            m_mouse_motor->setDesiredAngle(225);
            break;
    }
}

void MicroMouse::resetToEast() {
    x = 0;
    y = 0;
    mouse_direction = Direction::EAST;
    updateAngle();
}

std::string MicroMouse::get_direction() {
    switch (mouse_direction) {
        case Direction::NORTH:
            return "NORTH";
        case Direction::NORTH_EAST:
            return "NORTH_EAST";
        case Direction::EAST:
            return "EAST";
        case Direction::SOUTH_EAST:
            return "SOUTH_EAST";
        case Direction::SOUTH:
            return "SOUTH";
        case Direction::SOUTH_WEST:
            return "SOUTH_WEST";
        case Direction::WEST:
            return "WEST";
        case Direction::NORTH_WEST:
            return "NORTH_WEST";
        default:
            return "UNKNOWN";
    }
}

// void MicroMouse::move_forward_blocks(float blocks) {
//     m_mouse_motor->moveStraight(-blocks);
// }

void MicroMouse::updateMemoryData() {
    m_logger->print("in updateMemoryData", true);
    m_logger->println(int(mouse_direction), true);
    uint8_t current_grid = 0;
    switch (mouse_direction) {
    case Direction::SOUTH:
        current_grid |= m_mouse_sensor->isRightObstrucle();
        current_grid = current_grid << 1;
        current_grid |= m_mouse_sensor->isCenterObstrucle();
        current_grid = current_grid << 1;
        current_grid |= m_mouse_sensor->isLeftObstrucle();
        current_grid = current_grid << 1;
        if ((y - 1) >= 0) {
            current_grid |= 0;
        }
        else {
            current_grid |= 1;
        }
        break;

    case Direction::WEST:
        current_grid |= m_mouse_sensor->isCenterObstrucle();
        current_grid = current_grid << 1;
        current_grid |= m_mouse_sensor->isLeftObstrucle();
        current_grid = current_grid << 1;
        if ((x + 1) <= max_x) {
            current_grid |= 0;
        }
        else {
            current_grid |= 1;
        }
        current_grid = current_grid << 1;
        current_grid |= m_mouse_sensor->isRightObstrucle();
        break;

    case Direction::NORTH:
        current_grid |= m_mouse_sensor->isLeftObstrucle();
        current_grid = current_grid << 1;
        if ((y + 1) <= max_y) {
            current_grid |= 0;
        }
        else {
            current_grid |= 1;
        }
        current_grid = current_grid << 1;
        current_grid |= m_mouse_sensor->isRightObstrucle();
        current_grid = current_grid << 1;
        current_grid |= m_mouse_sensor->isCenterObstrucle();
        break;

    case Direction::EAST:
        if ((x -1) >= 0) {
            current_grid |= 0;
        }
        else {
            current_grid |= 1;
        }
        current_grid = current_grid << 1;
        current_grid |= m_mouse_sensor->isRightObstrucle();
        current_grid = current_grid << 1;
        current_grid |= m_mouse_sensor->isCenterObstrucle();
        current_grid = current_grid << 1;
        current_grid |= m_mouse_sensor->isLeftObstrucle();
        break;

    default:
        m_logger->print("Unknown Directions: ");
        //m_logger->println(get_direction());
    }
    m_logger->print("updateObstrucleMemory x: ", true);
    m_logger->print(x, true);
    m_logger->print(" y: ", true);
    m_logger->print(y, true);
    m_logger->print(" Value: ", true);
    m_logger->println(current_grid, true);
    m_mazememory->updateObstrucleMemory(x, y, current_grid);
}

void MicroMouse::printDebugMemory() {
    for (int row = 0; row < max_x; row++) {
        for (int column = 0; column < max_y; column++) {
            if (m_mazememory->isVisited(row, column)) {
                m_logger->print("printDebugMemory x: ");
                m_logger->print(row);
                m_logger->print(" y: ");
                m_logger->print(column);
                m_logger->print(" Value: ");
                m_logger->println(int(m_mazememory->getMemoryData(row, column)));
            }
        }
    }
   
}

void MicroMouse::balanceToSideWall() {
    m_mouse_sensor->triggerSensors();
    double left_distance = m_mouse_sensor->getLeftDistance();
    double right_distance = m_mouse_sensor->getRightDistance();
    if (abs(left_distance - right_distance) >= 20) {
        if (left_distance > right_distance) {
            path_to_travel.push(MovementDirection::LEFT);
            path_to_travel.push(MovementDirection::STRAIGHT);
            path_to_travel.push(MovementDirection::RIGHT);
        } else {
            path_to_travel.push(MovementDirection::RIGHT);
            path_to_travel.push(MovementDirection::STRAIGHT);
            path_to_travel.push(MovementDirection::LEFT);
        }
    }
}

void MicroMouse::updatePathToTravel(uint8_t next_x, uint8_t next_y) {
    m_logger->print("updatePathToTravel:- next_x: ");
    m_logger->print(next_x);
    m_logger->print(" next_y: ");
    m_logger->print(next_y);
    m_logger->println();
    Direction currentDirection;
    currentDirection = mouse_direction;

    if (((x==next_x) && (y == next_y))) {
      //m_logger->println("Returning Empty Stack!!!");
      return;
    }
    if (currentDirection == Direction::NORTH) {
        //m_logger->println("updatePathToTravel current Direction::NORTH");
        if (((x + 1) == next_x) && (y == next_y)) {
            //m_logger->println("updatePathToTravel Push RIGHT");
            path_to_travel.push(MovementDirection::RIGHT);
            currentMovementDirection = MovementDirection::RIGHT;
        }
        else if (((x - 1) == next_x) && (y == next_y)) {
            //m_logger->println("updatePathToTravel Push LEFT");
            path_to_travel.push(MovementDirection::LEFT);
            currentMovementDirection = MovementDirection::LEFT;
        }
        else if ((x == next_x) && ((y - 1) == next_y)) {
            // do nothing            
            currentMovementDirection = MovementDirection::STRAIGHT;
        }
        else if ((x == next_x) && ((y + 1) == next_y)) {
            //m_logger->println("updatePathToTravel Push REVERSE");
            path_to_travel.push(MovementDirection::REVERSE);
            currentMovementDirection = MovementDirection::REVERSE;
        }
    }
    else if (currentDirection == Direction::SOUTH) {
        //m_logger->println("updatePathToTravel current Direction::SOUTH");
        if (((x + 1) == next_x) && (y == next_y)) {
            //m_logger->println("updatePathToTravel Push LEFT");
            path_to_travel.push(MovementDirection::LEFT);
            currentMovementDirection = MovementDirection::LEFT;
        }
        else if (((x - 1) == next_x) && (y == next_y)) {
            //m_logger->println("updatePathToTravel Push RIGHT");
            path_to_travel.push(MovementDirection::RIGHT);
            currentMovementDirection = MovementDirection::RIGHT;
        }
        else if ((x == next_x) && ((y - 1) == next_y)) {
            //m_logger->println("updatePathToTravel Push REVERSE");
            path_to_travel.push(MovementDirection::REVERSE);
            currentMovementDirection = MovementDirection::REVERSE;
        }
        else if ((x == next_x) && ((y + 1) == next_y)) {
            // do nothing            
            currentMovementDirection = MovementDirection::STRAIGHT;
        }
    }
    else if (currentDirection == Direction::EAST) {
        // m_logger->println("updatePathToTravel current Direction::EAST");
        // m_logger->print("(y + 1): ");
        // m_logger->print(y + 1);
        // m_logger->println();
        // m_logger->print("Condition (y+1) == next_y): ");
        // //m_logger->println(((y+1) == next_y));
        // m_logger->print("Condition x == next_x: ");
        // //m_logger->println((x == next_x));
        if (((x + 1) == next_x) && (y == next_y)) {
            // do nothing
            //m_logger->println("Do Nothing");
            currentMovementDirection = MovementDirection::STRAIGHT;
        }
        else if (((x - 1) == next_x) && (y == next_y)) {
            //m_logger->println("updatePathToTravel Push REVERSE");
            path_to_travel.push(MovementDirection::REVERSE);
            currentMovementDirection = MovementDirection::REVERSE;
        }
        else if ((x == next_x) && ((y - 1) == next_y)) {
            //m_logger->println("updatePathToTravel Push LEFT");
            path_to_travel.push(MovementDirection::LEFT);
            currentMovementDirection = MovementDirection::LEFT;
        }
        else if ((x == next_x) && ((y + 1) == next_y)) {
            //m_logger->println("updatePathToTravel Push RIGHT");
            path_to_travel.push(MovementDirection::RIGHT);
            currentMovementDirection = MovementDirection::RIGHT;
        }
    }
    else if (currentDirection == Direction::WEST) {
        //m_logger->println("updatePathToTravel current Direction::WEST");
        if (((x + 1) == next_x) && (y == next_y)) {
            //m_logger->println("updatePathToTravel Push REVERSE");
            path_to_travel.push(MovementDirection::REVERSE);
            currentMovementDirection = MovementDirection::REVERSE;
        }
        else if (((x - 1) == next_x) && (y == next_y)) {
            // do nothing            
            currentMovementDirection = MovementDirection::STRAIGHT;
        }
        else if ((x == next_x) && ((y - 1) == next_y)) {
            //m_logger->println("updatePathToTravel Push RIGHT");
            path_to_travel.push(MovementDirection::RIGHT);
            currentMovementDirection = MovementDirection::RIGHT;
        }
        else if ((x == next_x) && ((y + 1) == next_y)) {
            //m_logger->println("updatePathToTravel Push LEFT");
            path_to_travel.push(MovementDirection::LEFT);
            currentMovementDirection = MovementDirection::LEFT;
        }
    }
    path_to_travel.push(MovementDirection::STRAIGHT);
    //m_logger->println("updatePathToTravel Push STRAIGHT");
    x = next_x;
    y = next_y;
    // m_logger->print("updatePathToTravel Complete:- x: ");
    // m_logger->print(x);
    // m_logger->print(" y: ");
    // m_logger->print(y);
    // m_logger->println();
}


MovementDirection MicroMouse::getCurrentMovementDirection() {
    return currentMovementDirection;
}

MovementDirection MicroMouse::getNextPathToTravel() {
    MovementDirection tempPath = path_to_travel.front();
    path_to_travel.pop();
    return tempPath;
}

bool MicroMouse::isPathEmpty() {
  return path_to_travel.empty();
}

void MicroMouse::destinationFoundMove() {
    delay(1000);
}

void MicroMouse::clearSolutionRunQueue() {
    while (!path_to_travel.empty()) {
        path_to_travel.pop();
    }
}

int MicroMouse::getNextNStraight() {
    uint8_t count = 0;
    while(!path_to_travel.empty()) {
        if (path_to_travel.front() == MovementDirection::STRAIGHT) {
            count++;
            path_to_travel.pop();
        } else {
            break;
        }
    }
    return count;
}