#include "micro_mouse.h"

MicroMouse* MicroMouse::m_micromouse = nullptr;

MicroMouse::MicroMouse(uint8_t x, uint8_t y, MouseSensor* m_mouse_sensor_inst, MazeMemory* m_mazememory_inst, MouseMotor* m_mouse_motor, Logger *loggerInstance, uint8_t max_x, uint8_t max_y, Direction mouse_direction)
    : x(x), y(y), m_mouse_sensor(m_mouse_sensor_inst), m_mazememory(m_mazememory_inst), max_x(max_x), max_y(max_y), mouse_direction(mouse_direction), m_mouse_motor(m_mouse_motor), m_logger(loggerInstance)
{
}

void MicroMouse::turn_left(bool isMultiStep) {
    //m_logger->println("MicroMouse:: Turning Left");
    uint8_t direction_value;
    if (isMultiStep) {
        direction_value = static_cast<uint8_t>(multistep_direction);
    } else {
        direction_value = static_cast<uint8_t>(mouse_direction);
    }
    direction_value -= 1;
    if (direction_value < 1) {
        direction_value = 4;
    }
    if (isMultiStep) {
        multistep_direction = static_cast<Direction>(direction_value);
    } else {
        mouse_direction = static_cast<Direction>(direction_value);
        this->updateAngle();
    }
    //m_mouse_motor -> turnLeft();
}


void MicroMouse::turn_right(bool isMultiStep) {
    //m_logger->println("MicroMouse:: Turning Right");
    uint8_t direction_value;
    if (isMultiStep) {
        direction_value = static_cast<uint8_t>(multistep_direction);
    } else {
        direction_value = static_cast<uint8_t>(mouse_direction);
    }
    direction_value += 1;
    if (direction_value > 4) {
        direction_value %= 4;
    }
    if (isMultiStep) {
        multistep_direction = static_cast<Direction>(direction_value);
    } else {
        mouse_direction = static_cast<Direction>(direction_value);
        this->updateAngle();
    }
    //m_mouse_motor -> turnRight();
}

void MicroMouse::make_u_turn(bool isMultiStep) {
    //m_logger->println("MicroMouse:: Turning U-turn");
    uint8_t direction_value;
    if (isMultiStep) {
        direction_value = static_cast<uint8_t>(multistep_direction);
    } else {
        direction_value = static_cast<uint8_t>(mouse_direction);
    }
    direction_value += 2;
    if (direction_value > 4) {
        direction_value %= 4;
    }
    if (isMultiStep) {
        multistep_direction = static_cast<Direction>(direction_value);
    } else {
        mouse_direction = static_cast<Direction>(direction_value);
        this->updateAngle();
    }
    //m_mouse_motor -> turnReverse();
}

void MicroMouse::updateAngle() {
    m_logger->print("Setting Angle: ");
    switch (mouse_direction) {
        case Direction::NORTH:
            m_mouse_motor->setDesiredAngle(270);
            m_logger->println("270");
            break;
        case Direction::EAST:
            m_mouse_motor->setDesiredAngle(0);
            m_logger->println("0");
            break;
        case Direction::SOUTH:
            m_mouse_motor->setDesiredAngle(90);
            m_logger->println("90");
            break;
        case Direction::WEST:
            m_mouse_motor->setDesiredAngle(180);
            m_logger->println("180");
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
    case Direction::EAST:
        return "EAST";
    case Direction::SOUTH:
        return "SOUTH";
    case Direction::WEST:
        return "WEST";
    default:
        return "UNKNOWN";
    }
}

void MicroMouse::move_forward(float blocks) {
    // m_logger->print("MicroMouse:: Moving Straight: ");
    // m_logger->print(-blocks);
    m_mouse_motor->moveStraight(-blocks);
}

void MicroMouse::updateMemoryData() {
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

    }
    m_logger->print("updateObstrucleMemory x: ");
    m_logger->print(x);
    m_logger->print(" y: ");
    m_logger->print(y);
    m_logger->print(" Value: ");
    m_logger->println(current_grid);
    m_mazememory->updateObstrucleMemory(x, y, current_grid);
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

void MicroMouse::storeMultiStepDirection() {
    //m_logger->println("storeMultiStepDirection called");
    multistep_direction = mouse_direction;
}

void MicroMouse::updatePathToTravel(uint8_t next_x, uint8_t next_y, bool isMultiStep) {
    m_logger->print("updatePathToTravel:- next_x: ");
    m_logger->print(next_x);
    m_logger->print(" next_y: ");
    m_logger->print(next_y);
    m_logger->println();
    //m_logger->print("isMultiStep: ");
    //m_logger->println(isMultiStep);
    Direction currentDirection;
    if (isMultiStep) {
        currentDirection = multistep_direction;
    } else {
        currentDirection = mouse_direction;
    }

    if (((x==next_x) && (y == next_y))) {
      //m_logger->println("Returning Empty Stack!!!");
      return;
    }
    if (currentDirection == Direction::NORTH) {
        //m_logger->println("updatePathToTravel current Direction::NORTH");
        if (((x + 1) == next_x) && (y == next_y)) {
            //m_logger->println("updatePathToTravel Push RIGHT");
            path_to_travel.push(MovementDirection::RIGHT);
            if (isMultiStep) turn_right(isMultiStep);
        }
        else if (((x - 1) == next_x) && (y == next_y)) {
            //m_logger->println("updatePathToTravel Push LEFT");
            path_to_travel.push(MovementDirection::LEFT);
            if (isMultiStep) turn_left(isMultiStep);
        }
        else if ((x == next_x) && ((y - 1) == next_y)) {
            // do nothing
        }
        else if ((x == next_x) && ((y + 1) == next_y)) {
            //m_logger->println("updatePathToTravel Push REVERSE");
            path_to_travel.push(MovementDirection::REVERSE);
            if (isMultiStep) make_u_turn(isMultiStep);
        }
    }
    else if (currentDirection == Direction::SOUTH) {
        //m_logger->println("updatePathToTravel current Direction::SOUTH");
        if (((x + 1) == next_x) && (y == next_y)) {
            //m_logger->println("updatePathToTravel Push LEFT");
            path_to_travel.push(MovementDirection::LEFT);
            if (isMultiStep) turn_left(isMultiStep);
        }
        else if (((x - 1) == next_x) && (y == next_y)) {
            //m_logger->println("updatePathToTravel Push RIGHT");
            path_to_travel.push(MovementDirection::RIGHT);
            if (isMultiStep) turn_right(isMultiStep);
        }
        else if ((x == next_x) && ((y - 1) == next_y)) {
            //m_logger->println("updatePathToTravel Push REVERSE");
            path_to_travel.push(MovementDirection::REVERSE);
            if (isMultiStep) make_u_turn(isMultiStep);
        }
        else if ((x == next_x) && ((y + 1) == next_y)) {
            // do nothing
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
        }
        else if (((x - 1) == next_x) && (y == next_y)) {
            //m_logger->println("updatePathToTravel Push REVERSE");
            path_to_travel.push(MovementDirection::REVERSE);
            if (isMultiStep) make_u_turn(isMultiStep);
        }
        else if ((x == next_x) && ((y - 1) == next_y)) {
            //m_logger->println("updatePathToTravel Push LEFT");
            path_to_travel.push(MovementDirection::LEFT);
            if (isMultiStep) turn_left(isMultiStep);
        }
        else if ((x == next_x) && ((y + 1) == next_y)) {
            //m_logger->println("updatePathToTravel Push RIGHT");
            path_to_travel.push(MovementDirection::RIGHT);
            if (isMultiStep) turn_right(isMultiStep);
        }
    }
    else if (currentDirection == Direction::WEST) {
        //m_logger->println("updatePathToTravel current Direction::WEST");
        if (((x + 1) == next_x) && (y == next_y)) {
            //m_logger->println("updatePathToTravel Push REVERSE");
            path_to_travel.push(MovementDirection::REVERSE);
            if (isMultiStep) make_u_turn(isMultiStep);
        }
        else if (((x - 1) == next_x) && (y == next_y)) {
            // do nothing
        }
        else if ((x == next_x) && ((y - 1) == next_y)) {
            //m_logger->println("updatePathToTravel Push RIGHT");
            path_to_travel.push(MovementDirection::RIGHT);
            if (isMultiStep) turn_right(isMultiStep);
        }
        else if ((x == next_x) && ((y + 1) == next_y)) {
            //m_logger->println("updatePathToTravel Push LEFT");
            path_to_travel.push(MovementDirection::LEFT);
            if (isMultiStep) turn_left(isMultiStep);
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

MovementDirection MicroMouse::getNextPathToTravel() {
    MovementDirection tempPath = path_to_travel.front();
    path_to_travel.pop();
    return tempPath;
}

// int MicroMouse::getNextNStraigtMoves() {
//     int count = 0;
//     while (true) {
//         if (!isPathEmpty()) {
//         MovementDirection tempPath = path_to_travel.front();
//             if (tempPath == MotorDirection::STRAIGHT) {
//                 count++;
//                 path_to_travel.pop();
//             } else {
//                 break;
//             }
//         } else {
//             break;
//         }        
//     }
//     return count;
// }

bool MicroMouse::isPathEmpty() {
//   m_logger->print("isPathEmpty: ");
//   m_logger->print(path_to_travel.empty());
//   m_logger->println();
//   m_logger->print("path_to_travel_stack_size: ");
//   m_logger->print(path_to_travel.size());
//   m_logger->println();
  return path_to_travel.empty();
}

void MicroMouse::destinationFoundMove() {
    delay(1000);
    //path_to_travel.push(MovementDirection::ROTATION);
    // path_to_travel.push(MovementDirection::RIGHT);
    // path_to_travel.push(MovementDirection::RIGHT);
    // path_to_travel.push(MovementDirection::LEFT);
}

void MicroMouse::addFinishMove() {
    //m_logger->println("addFinishMove Push STRAIGHT");
    //path_to_travel.push(MovementDirection::STRAIGHT);
    //m_logger->println("addFinishMove Push REVERSE");
    path_to_travel.push(MovementDirection::REVERSE);
    //m_logger->println("addFinishMove Push STRAIGHT");
    //path_to_travel.push(MovementDirection::STRAIGHT);
}

void MicroMouse::clearSolutionRunQueue() {
    while (!path_to_travel.empty()) {
        path_to_travel.pop();
    }
    //m_logger->println("Solution Path Cleared");
    m_logger->print("path_to_travel size: ");
    //m_logger->println(path_to_travel.size());
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