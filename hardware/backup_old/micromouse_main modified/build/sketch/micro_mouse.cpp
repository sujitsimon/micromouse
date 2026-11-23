#line 1 "C:\\D_Drive\\MicroMouse\\hardware\\micromouse_main\\micro_mouse.cpp"
#include "micro_mouse.h"

MicroMouse *MicroMouse::m_micromouse = nullptr;

MicroMouse::MicroMouse(uint8_t x, uint8_t y, MouseSensor *m_mouse_sensor_inst, MazeMemory *m_mazememory_inst, MouseMotor *m_mouse_motor, uint8_t max_x, uint8_t max_y, Direction mouse_direction)
    : x(x), y(y), m_mouse_sensor(m_mouse_sensor_inst), m_mazememory(m_mazememory_inst), max_x(max_x), max_y(max_y), mouse_direction(mouse_direction), m_mouse_motor(m_mouse_motor)
{
}

void MicroMouse::turn_left(bool isMultiStep)
{
    Serial.println("MicroMouse:: Turning Left");
    uint8_t direction_value;
    if (isMultiStep)
    {
        direction_value = static_cast<uint8_t>(multistep_direction);
    }
    else
    {
        direction_value = static_cast<uint8_t>(mouse_direction);
    }
    direction_value -= 1;
    if (direction_value < 1)
    {
        direction_value = 4;
    }
    if (isMultiStep)
    {
        multistep_direction = static_cast<Direction>(direction_value);
    }
    else
    {
        mouse_direction = static_cast<Direction>(direction_value);
        this->updateAngle();
    }
    // m_mouse_motor -> turnLeft();
}

void MicroMouse::turn_right(bool isMultiStep)
{
    Serial.println("MicroMouse:: Turning Right");
    uint8_t direction_value;
    if (isMultiStep)
    {
        direction_value = static_cast<uint8_t>(multistep_direction);
    }
    else
    {
        direction_value = static_cast<uint8_t>(mouse_direction);
    }
    direction_value += 1;
    if (direction_value > 4)
    {
        direction_value %= 4;
    }
    if (isMultiStep)
    {
        multistep_direction = static_cast<Direction>(direction_value);
    }
    else
    {
        mouse_direction = static_cast<Direction>(direction_value);
        this->updateAngle();
    }
    // m_mouse_motor -> turnRight();
}

void MicroMouse::make_u_turn(bool isMultiStep)
{
    Serial.println("MicroMouse:: Turning U-turn");
    uint8_t direction_value;
    if (isMultiStep)
    {
        direction_value = static_cast<uint8_t>(multistep_direction);
    }
    else
    {
        direction_value = static_cast<uint8_t>(mouse_direction);
    }
    direction_value += 2;
    if (direction_value > 4)
    {
        direction_value %= 4;
    }
    if (isMultiStep)
    {
        multistep_direction = static_cast<Direction>(direction_value);
    }
    else
    {
        mouse_direction = static_cast<Direction>(direction_value);
        this->updateAngle();
    }
    // m_mouse_motor -> turnReverse();
}

void MicroMouse::updateAngle()
{
    Serial.print("Setting Angle: ");
    switch (mouse_direction)
    {
    case Direction::NORTH:
        m_mouse_motor->setDesiredAngle(270);
        Serial.println("270");
        break;
    case Direction::EAST:
        m_mouse_motor->setDesiredAngle(0);
        Serial.println("0");
        break;
    case Direction::SOUTH:
        m_mouse_motor->setDesiredAngle(90);
        Serial.println("90");
        break;
    case Direction::WEST:
        m_mouse_motor->setDesiredAngle(180);
        Serial.println("180");
        break;
    }
}

void MicroMouse::resetToEast()
{
    x = 0;
    y = 0;
    mouse_direction = Direction::EAST;
    updateAngle();
}

std::string MicroMouse::get_direction()
{
    switch (mouse_direction)
    {
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

void MicroMouse::move_forward(float blocks)
{
    Serial.println("MicroMouse:: Moving Straight");
    m_mouse_motor->moveStraight(-blocks);
}

void MicroMouse::updateMemoryData()
{
    uint8_t current_grid = 0;
    switch (mouse_direction)
    {
    case Direction::SOUTH:
        current_grid |= m_mouse_sensor->isRightObstrucle();
        current_grid = current_grid << 1;
        current_grid |= m_mouse_sensor->isCenterObstrucle();
        current_grid = current_grid << 1;
        current_grid |= m_mouse_sensor->isLeftObstrucle();
        current_grid = current_grid << 1;
        if ((y - 1) >= 0)
        {
            current_grid |= 0;
        }
        else
        {
            current_grid |= 1;
        }
        break;

    case Direction::WEST:
        current_grid |= m_mouse_sensor->isCenterObstrucle();
        current_grid = current_grid << 1;
        current_grid |= m_mouse_sensor->isLeftObstrucle();
        current_grid = current_grid << 1;
        if ((x + 1) <= max_x)
        {
            current_grid |= 0;
        }
        else
        {
            current_grid |= 1;
        }
        current_grid = current_grid << 1;
        current_grid |= m_mouse_sensor->isRightObstrucle();
        break;

    case Direction::NORTH:
        current_grid |= m_mouse_sensor->isLeftObstrucle();
        current_grid = current_grid << 1;
        if ((y + 1) <= max_y)
        {
            current_grid |= 0;
        }
        else
        {
            current_grid |= 1;
        }
        current_grid = current_grid << 1;
        current_grid |= m_mouse_sensor->isRightObstrucle();
        current_grid = current_grid << 1;
        current_grid |= m_mouse_sensor->isCenterObstrucle();
        break;

    case Direction::EAST:
        if ((x - 1) >= 0)
        {
            current_grid |= 0;
        }
        else
        {
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
    Serial.print("updateObstrucleMemory x: ");
    Serial.print(x);
    Serial.print(" y: ");
    Serial.print(y);
    Serial.print(" Value: ");
    Serial.println(current_grid);
    m_mazememory->updateObstrucleMemory(x, y, current_grid);
}

void MicroMouse::storeMultiStepDirection()
{
    Serial.println("storeMultiStepDirection called");
    multistep_direction = mouse_direction;
}

void MicroMouse::updatePathToTravel(uint8_t next_x, uint8_t next_y, bool isMultiStep)
{
    Serial.print("updatePathToTravel:- next_x: ");
    Serial.print(next_x);
    Serial.print(" next_y: ");
    Serial.print(next_y);
    Serial.println();
    Direction currentDirection;
    if (isMultiStep)
    {
        currentDirection = multistep_direction;
    }
    else
    {
        currentDirection = mouse_direction;
    }

    if (((x == next_x) && (y == next_y)))
    {
        Serial.println("Returning Empty Stack!!!");
        return;
    }
    if (currentDirection == Direction::NORTH)
    {
        Serial.println("updatePathToTravel current Direction::NORTH");
        if (((x + 1) == next_x) && (y == next_y))
        {
            Serial.println("updatePathToTravel Push RIGHT");
            path_to_travel.push(MovementDirection::RIGHT);
            if (isMultiStep)
                turn_right(isMultiStep);
        }
        else if (((x - 1) == next_x) && (y == next_y))
        {
            Serial.println("updatePathToTravel Push LEFT");
            path_to_travel.push(MovementDirection::LEFT);
            if (isMultiStep)
                turn_left(isMultiStep);
        }
        else if ((x == next_x) && ((y - 1) == next_y))
        {
            // do nothing
        }
        else if ((x == next_x) && ((y + 1) == next_y))
        {
            Serial.println("updatePathToTravel Push REVERSE");
            path_to_travel.push(MovementDirection::REVERSE);
            if (isMultiStep)
                make_u_turn(isMultiStep);
        }
    }
    else if (currentDirection == Direction::SOUTH)
    {
        Serial.println("updatePathToTravel current Direction::SOUTH");
        if (((x + 1) == next_x) && (y == next_y))
        {
            Serial.println("updatePathToTravel Push LEFT");
            path_to_travel.push(MovementDirection::LEFT);
            if (isMultiStep)
                turn_left(isMultiStep);
        }
        else if (((x - 1) == next_x) && (y == next_y))
        {
            Serial.println("updatePathToTravel Push RIGHT");
            path_to_travel.push(MovementDirection::RIGHT);
            if (isMultiStep)
                turn_right(isMultiStep);
        }
        else if ((x == next_x) && ((y - 1) == next_y))
        {
            Serial.println("updatePathToTravel Push REVERSE");
            path_to_travel.push(MovementDirection::REVERSE);
            if (isMultiStep)
                make_u_turn(isMultiStep);
        }
        else if ((x == next_x) && ((y + 1) == next_y))
        {
            // do nothing
        }
    }
    else if (currentDirection == Direction::EAST)
    {
        Serial.println("updatePathToTravel current Direction::EAST");
        // Serial.print("(y + 1): ");
        // Serial.print(y + 1);
        // Serial.println();
        // Serial.print("Condition (y+1) == next_y): ");
        // Serial.println(((y+1) == next_y));
        // Serial.print("Condition x == next_x: ");
        // Serial.println((x == next_x));
        if (((x + 1) == next_x) && (y == next_y))
        {
            // do nothing
            Serial.println("Do Nothing");
        }
        else if (((x - 1) == next_x) && (y == next_y))
        {
            Serial.println("updatePathToTravel Push REVERSE");
            path_to_travel.push(MovementDirection::REVERSE);
            if (isMultiStep)
                make_u_turn(isMultiStep);
        }
        else if ((x == next_x) && ((y - 1) == next_y))
        {
            Serial.println("updatePathToTravel Push LEFT");
            path_to_travel.push(MovementDirection::LEFT);
            if (isMultiStep)
                turn_left(isMultiStep);
        }
        else if ((x == next_x) && ((y + 1) == next_y))
        {
            Serial.println("updatePathToTravel Push RIGHT");
            path_to_travel.push(MovementDirection::RIGHT);
            if (isMultiStep)
                turn_right(isMultiStep);
        }
    }
    else if (currentDirection == Direction::WEST)
    {
        Serial.println("updatePathToTravel current Direction::WEST");
        if (((x + 1) == next_x) && (y == next_y))
        {
            Serial.println("updatePathToTravel Push REVERSE");
            path_to_travel.push(MovementDirection::REVERSE);
            if (isMultiStep)
                make_u_turn(isMultiStep);
        }
        else if (((x - 1) == next_x) && (y == next_y))
        {
            // do nothing
        }
        else if ((x == next_x) && ((y - 1) == next_y))
        {
            Serial.println("updatePathToTravel Push RIGHT");
            path_to_travel.push(MovementDirection::RIGHT);
            if (isMultiStep)
                turn_right(isMultiStep);
        }
        else if ((x == next_x) && ((y + 1) == next_y))
        {
            Serial.println("updatePathToTravel Push LEFT");
            path_to_travel.push(MovementDirection::LEFT);
            if (isMultiStep)
                turn_left(isMultiStep);
        }
    }
    path_to_travel.push(MovementDirection::STRAIGHT);
    Serial.println("updatePathToTravel Push STRAIGHT");
    x = next_x;
    y = next_y;
    Serial.print("updatePathToTravel Complete:- x: ");
    Serial.print(x);
    Serial.print(" y: ");
    Serial.print(y);
    Serial.println();
}

MovementDirection MicroMouse::getNextPathToTravel()
{
    MovementDirection tempPath = path_to_travel.front();
    path_to_travel.pop();
    return tempPath;
}

uint8_t MicroMouse::getNextNStraightPath()
{
    uint8_t count = 0;
    while (!path_to_travel.empty())
    {
        MovementDirection tempPath = path_to_travel.front();
        if (tempPath == MovementDirection::STRAIGHT)
        {
            count += 1;
            path_to_travel.pop();
        }
        else
        {
            break;
        }
    }
    return count;
}

bool MicroMouse::isPathEmpty()
{
    Serial.print("isPathEmpty: ");
    Serial.print(path_to_travel.empty());
    Serial.println();
    Serial.print("path_to_travel_stack_size: ");
    Serial.print(path_to_travel.size());
    Serial.println();
    return path_to_travel.empty();
}

void MicroMouse::addFinishMove()
{
    Serial.println("addFinishMove Push STRAIGHT");
    path_to_travel.push(MovementDirection::STRAIGHT);
    // Serial.println("addFinishMove Push REVERSE");
    // path_to_travel.push(MovementDirection::REVERSE);
}

void MicroMouse::clearSolutionRunQueue()
{
    path_to_travel = {};
}