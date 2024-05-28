

    # // tiles[0][0] = 100120;
    #     // tiles[0][1] = 200030;
    #     // tiles[0][2] = 300655;
    #     // tiles[0][3] = 400340;
    #     // tiles[1][0] = 500396;
    #     // tiles[1][1] = 600031;
    #     // tiles[1][2] = 700031;
    #     // tiles[1][3] = 800852;
    #     // tiles[2][0] = 900600;
    #     // tiles[2][1] = 1000031;
    #     // tiles[2][2] = 1100023;
    #     // tiles[2][3] = 1206240;
    #     // tiles[3][0] = 1300088;
    #     // tiles[3][1] = 1400911;
    #     // tiles[3][2] = 1509317;
    #     // tiles[3][3] = 0;


tileTypeFeatureStart = 5
tileTypeFeatureEnd = 8
tileTypeColorStart = 9
tileTypeColorEnd = 10
tileTypeDirectionStart = 11
tileTypeDirectionEnd = 14
tileTypeEscalatorStart = 15
tileTypeEscalatorEnd = 18

tileTypeBlockedMoveUp = 1
tileTypeBlockedMoveDown = 2
tileTypeBlockedMoveLeft = 3
tileTypeBlockedMoveRight = 4

def extract_bits(num, start, end):
    mask = ((1 << (end - start + 1)) - 1) << (start - 1)
    return (num & mask) >> (start - 1)

def set_bits(num, value, start, end):
    mask = ((1 << (end - start + 1)) - 1) << (start - 1)
    num &= ~mask
    value = (value << (start - 1)) & mask
    return num | value

def get_tile_feature(tile_value):
    feature = extract_bits(tile_value , tileTypeFeatureStart, tileTypeFeatureEnd)
    feature_map = {
        0: "inaccessible",
        1: "empty",
        2: "exit",
        3: "store",
        4: "opening",
        5: "portal",
        6: "unused",
        7: "time-flip",
        8: "entrance"
    }
    # print(feature_map.get(feature, "function error"))
    return feature_map.get(feature, "function error")

def get_tile_color(tile_value):
    color = extract_bits(tile_value , tileTypeColorStart, tileTypeColorEnd)
    color_map = {
        0: "green",
        1: "purple",
        2: "orange",
        3: "yellow"
    }
    return color_map.get(color, "no-color")

def get_direction_bitwise(tile_value):
    return extract_bits(tile_value , tileTypeDirectionStart, tileTypeDirectionEnd)

def get_direction(tile_value):
    bitwise_direction = get_direction_bitwise(tile_value)
    direction_map = {
        0: "none",
        1: "right",
        2: "left",
        4: "down",
        5: "down-right",
        6: "down-left",
        8: "up",
        9: "up-right",
        10: "up-left"
    }
    return direction_map.get(bitwise_direction, "invalid direction")

def set_tile_feature(tile_value, feature):
    tile_value = set_bits(tile_value , feature, tileTypeFeatureStart, tileTypeFeatureEnd)
    return tile_value

# def rotate_direction_left(tile_value):
#     bitwise = get_direction_bitwise(tile_value)
#     right = bitwise % 2

#     bitwise = set_bits(bitwise, (bitwise // 4) % 2, 1, 1)  # set right from down
#     bitwise = set_bits(bitwise, (bitwise // 2) % 2, 3, 3)  # set down from left
#     bitwise = set_bits(bitwise, (bitwise // 8) % 2, 2, 2)  # set left from up
#     bitwise = set_bits(bitwise, right, 4, 4)  # set up from right

#     tile_value = set_direction(tile_value, bitwise)

#     bitwise = get_escalator_direction_bitwise(tile_value)
#     right = bitwise % 2

#     bitwise = set_bits(bitwise, (bitwise // 4) % 2, 1, 1)  # set right from down
#     bitwise = set_bits(bitwise, (bitwise // 2) % 2, 3, 3)  # set down from left
#     bitwise = set_bits(bitwise, (bitwise // 8) % 2, 2, 2)  # set left from up
#     bitwise = set_bits(bitwise, right, 4, 4)  # set up from right

#     tile_value = set_direction(tile_value, bitwise)


#     can_move_right = tile_blocked_move_right(tile_value)
#     tile_value = set_tile_blocked_move_right(tile_value, tile_blocked_move_down(tile_value))
#     tile_value = set_tile_blocked_move_down(tile_value, tile_blocked_move_left(tile_value))
#     tile_value = set_tile_blocked_move_left(tile_value, tile_blocked_move_up(tile_value))
#     tile_value = set_tile_blocked_move_up(tile_value, can_move_right)

#     return tile_value

def set_tile_blocked_move_up(tile_value, blocked_up):
   
    tile_value =  set_bits(tile_value , blocked_up, tileTypeBlockedMoveUp, tileTypeBlockedMoveUp)
    return tile_value

def tile_blocked_move_up(tile_value):
    return not extract_bits(tile_value , tileTypeBlockedMoveUp, tileTypeBlockedMoveUp)

def set_tile_blocked_move_down(tile_value, blocked_down):
    tile_value =  + set_bits(tile_value , blocked_down, tileTypeBlockedMoveDown, tileTypeBlockedMoveDown)
    return tile_value

def tile_blocked_move_down(tile_value):
    return not extract_bits(tile_value , tileTypeBlockedMoveDown, tileTypeBlockedMoveDown)

def set_tile_blocked_move_left(tile_value, blocked_left):

    tile_value =  set_bits(tile_value , blocked_left, tileTypeBlockedMoveLeft, tileTypeBlockedMoveLeft)
    return tile_value

def tile_blocked_move_left(tile_value):
    return not extract_bits(tile_value , tileTypeBlockedMoveLeft, tileTypeBlockedMoveLeft)

def set_tile_blocked_move_right(tile_value, blocked_right):
    tile_value =  set_bits(tile_value , blocked_right, tileTypeBlockedMoveRight, tileTypeBlockedMoveRight)
    return tile_value

def tile_blocked_move_right(tile_value):
    return not extract_bits(tile_value , tileTypeBlockedMoveRight, tileTypeBlockedMoveRight)

def set_feature(tile_value, feature):
    feature_map = {
        "inaccessible": 0,
        "empty": 1,
        "exit": 2,
        "store": 3,
        "opening": 4,
        "portal": 5,
        "unused": 6,
        "time-flip": 7,
        "entrance": 8
    }
    feature_value = feature_map.get(feature, 0)

    tile_value = set_bits(tile_value , feature_value, tileTypeFeatureStart, tileTypeFeatureEnd)
    return tile_value

def set_color(tile_value, color):
    color_map = {
        "green": 0,
        "purple": 1,
        "orange": 2,
        "yellow": 3
    }
    color_value = color_map.get(color, 0)
    tile_value = set_bits(tile_value , color_value, tileTypeColorStart, tileTypeColorEnd)
    return tile_value

def set_direction(tile_value, direction):

    direction_map = {
        "none": 0,
        "right": 1,
        "left": 2,
        "down": 4,
        "down-right": 5,
        "down-left": 6,
        "up": 8,
        "up-right": 9,
        "up-left": 10
    }
    direction_value = direction_map.get(direction, 0)
    tile_value = set_bits(tile_value , direction_value, tileTypeDirectionStart, tileTypeDirectionEnd)
    return tile_value

def get_escalator_direction_bitwise(tile_value):
    return extract_bits(tile_value , tileTypeEscalatorStart, tileTypeEscalatorEnd)

def get_escalator__direction(tile_value):
    bitwise_direction = get_escalator_direction_bitwise(tile_value)
    
    direction_map = {
        0: "none",
        1: "clock 1",
        2: "clock 2",
        4: "clock 4 ",
        5: "clock 5",
        7: "clock 7",
        8: "clock 8",
        10: "clock 10",
        11: "clock 11",
        12: "up-right",
        13: "down-right",
        14:"down-left",
        15:"up-left"
    }
    return direction_map.get(bitwise_direction, "invalid direction")


# # New escalator functions
# def get_escalator(tile_value):
#     escalator = extract_bits(tile_value , tileTypeEscalatorStart, tileTypeEscalatorEnd)
#     escalator_map = {
#         "none": 0,
#         "right": 1,
#         "left": 2,
#         "down": 4,
#         "down-right": 5,
#         "down-left": 6,
#         "up": 8,
#         "up-right": 9,
#         "up-left": 10
#     }
#     return escalator_map.get(escalator, "invalid escalator")

def set_escalator(tile_value, escalator):
    escalator_map = {
    "none": 0,
    "clock 1": 1,
    "clock 2": 2,
    "clock 4 ": 4,
    "clock 5": 5,
    "clock 7": 7,
    "clock 8": 8,
    "clock 10": 10,
    "clock 11": 11,
    "up-right": 12,
    "down-right": 13,
    "down-left": 14,
    "up-left": 15

    }
    escalator_value = escalator_map.get(escalator, 0)
    tile_value = set_bits(tile_value , escalator_value, tileTypeEscalatorStart, tileTypeEscalatorEnd)
    return tile_value

# Updated main function to include escalator segment
def main():
    # print(extract_bits(147477,5,8))
    while True:
        a = input("1 for data to int, 2 for int to tile 3 to exit ")
        if int(a) == 1:
            feature = input("feature: ")
            color = input("color: ")
            direction = input("direction: ")
            escalator = input("escalator : ")
            canMoveUp = bool(int(input("wall above (1 for yes, 0 for no): ")))
            canMoveDown = bool(int(input("wall below (1 for yes, 0 for no): ")))
            canMoveLeft = bool(int(input("wall to left (1 for yes, 0 for no): ")))
            canMoveRight = bool(int(input("wall to right (1 for yes, 0 for no): ")))
            value = 0
            value = set_direction(value, direction)
            value = set_color(value, color)
            value = set_feature(value, feature)
            value = set_escalator(value, escalator)
            value = set_tile_blocked_move_up(value, not canMoveUp)
            value = set_tile_blocked_move_down(value, not canMoveDown)
            value = set_tile_blocked_move_left(value, not canMoveLeft)
            value = set_tile_blocked_move_right(value, not canMoveRight)
            print(value)

        if int(a) == 2:
            value = int(input("the value: "))
            print("feature: " + str(get_tile_feature(value)))
            print("direction: " + str(get_direction(value)))
            print("color: " + str(get_tile_color(value)))
            print("escalator: " + str(get_escalator__direction(value)))
            print("can move up: " + str(not bool(tile_blocked_move_up(value))))
            print("can move down: " + str(not bool(tile_blocked_move_down(value))))
            print("can move left: " + str(not bool(tile_blocked_move_left(value))))
            print("can move right: " + str(not bool(tile_blocked_move_right(value))))
        if int(a) ==3:
            break

if __name__ == "__main__":
    main()
