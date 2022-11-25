import math

###################################################################################################
def radians_to_degrees(radians):
    return math.degrees(radians)

def degrees_to_radians(degrees):
    return math.radians(degrees)

###################################################################################################
def lines_intersection(x11, y11, x12, y12, x21, y21, x22, y22):
    '''
     find the intersection point P(px, py) of L1((x11, y11), (x12, y12)) and L2((x21, y21), (x22, y22))
    '''

    denominator = ((x11 - x12) * (y21 - y22) - (y11 - y12) * (x21 - x22))
    intersected = (denominator != 0.0)
    
    if intersected:
        T1 = +((x11 - x21) * (y21 - y22) - (y11 - y21) * (x21 - x22)) / denominator
        T2 = -((x11 - x12) * (y11 - y21) - (y11 - y12) * (x11 - x21)) / denominator
        px = x21 + T2 * (x22 - x21)
        py = y21 + T2 * (y22 - y21)
    else:
        px = py = T1 = T2 = math.nan

    return px, py, T1, T2

