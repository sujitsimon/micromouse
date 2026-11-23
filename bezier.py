import matplotlib.pyplot as plt
import numpy as np
import math

length = 18


def quad_bezier(p0, p1, cp):
    resolution = 0.001
    t = 0
    array = []
    while t <= 1:
        array.append(round(((1 - t)**2 * p0 + 2 * (1 - t) * t * cp + t**2*p1), 2))
        t += resolution
    return array


def draw_curve(x_t, y_t, p0, p1, cp):
    plt.plot(x_t, y_t, label="Bézier Curve")
    plt.plot([p0[0], cp[0], p1[0]], [p0[1], cp[1], p1[1]], 'ro--', label="Control Points")
    plt.text(p0[0], p0[1], 'p0', fontsize=12, ha='right')
    plt.text(cp[0], cp[1], 'cp', fontsize=12, ha='left')
    plt.text(p1[0], p1[1], 'p1', fontsize=12, ha='right')
    plt.title("Quadratic Bézier Curve")
    plt.legend()
    plt.xlabel("x")
    plt.ylabel("y")
    plt.grid(True)
    plt.axis("equal")
    plt.show()
    
    
def compute_wheel_speeds(current_heading, target_heading, base_speed=0.1):
    heading_error = target_heading - current_heading
    Kp = 1.0  # Proportional constant for turning
    turn_speed = Kp * heading_error
    
    left_speed = base_speed - turn_speed
    right_speed = base_speed + turn_speed
    
    return left_speed, right_speed

def distance_2d(point1, point2):
    x1, y1 = point1
    x2, y2 = point2
    return math.sqrt((x2 - x1) ** 2 + (y2 - y1) ** 2)

# def distance_to_travel(x_curve, y_curve):
    

def motor_control(x_curve, y_curve):
    final = 0
    for i in range(len(x_curve) - 1):
        target_x, target_y = x_curve[i], y_curve[i]        
        target_heading = np.arctan2(y_curve[i+1] - y_curve[i], x_curve[i+1] - x_curve[i])
        final += target_heading
        print(np.degrees(target_heading))
    print("final", final, np.degrees(final))



def process_tuple(p):
    return tuple(length * x for x in p) 

def bezier_draw(p0, p1, cp):
    x0, y0 = p0
    x1, y1 = p1
    cx1, cy1 = cp
    array_x = quad_bezier(x0, x1, cx1)
    array_y = quad_bezier(y0, y1, cy1)
    # print(list(zip(array_x, array_y)))
    motor_control(array_x, array_y)
    draw_curve(array_x, array_y, p0, p1, cp)
    
if __name__ == "__main__":
    start_point = (0, 0)
    end_point = (1, 0)
    control_point = (0.5, 1.5)
    bezier_draw(process_tuple(start_point), process_tuple(end_point), process_tuple(control_point))