import sys
import pygame
import time
from pygame.locals import *

pygame.init()

# Set up screen
width, height = 640, 640
screen = pygame.display.set_mode((width, height))

# Load background image
bg = pygame.image.load("background.png")
bg = pygame.transform.scale(bg, (width, height))

# Define colors
colors = [(0, 255, 0), (255, 255, 0), (128, 0, 128), (255, 165, 0)]

# Define points
green = 6 - 1
purple = 7 - 1
orange = 11 - 1
yellow = 10 - 1
targetpos = []
lastcolor = "green"
state = "character selection"
# Size of each cell
cell_size = min(width, height) / 4

# Function to create a surface with per-pixel alpha
def create_transparent_surface(color, size):
    surface = pygame.Surface(size, pygame.SRCALPHA)
    surface.fill(color)
    return surface

# Game loop

with open("C://Users//tomer//Documents//school//cpProject//src//extras//toCpp.txt", "w") as file:
    file.write("start")
while True:
    screen.blit(bg, (0, 0))  # Draw background image

    file_open = False

    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            sys.exit()
        elif event.type == pygame.MOUSEBUTTONDOWN:
            x, y = event.pos
            # Calculate the row and column index
            pos = int(y // (height / 4)) * 4 + int(x // (width / 4)) + 1

            with open("C://Users//tomer//Documents//school//cpProject//src//extras//toCpp.txt", "w") as file:
                if (state == "character selection"):
                    file.write("getCharacter " + str(pos))
                    print("getChar " + str(pos))
                    time.sleep(0.1)
                if(state == "target selection"):

                    file.write("move "+lastcolor+" "+ str(pos))
                    print("move "+lastcolor+" "+ str(pos))
                    time.sleep(0.1)
    with open("C:/Users/tomer/Documents/school/cpProject/src/extras/toPython.txt", "r") as file:
        file_open = True
        input_data = file.readlines()
        if input_data:
            data = input_data[0].split("$")
            if data[len(data)-1] =='':
                data = data[0:len(data)-1]

            print("data "+ str(data))
            if state == "target selection":
                if data[1] == "green":
                    green = int(data[2])-1
                    targetpos =[]
                if data[1] == "purple":
                    purple = int(data[2])-1
                    targetpos =[]
                if data[1] == "orange":
                    orange = int(data[2])-1
                    targetpos =[]
                if data[1] == "yellow":
                    yellow = int(data[2])-1
                    targetpos =[]
                holdstate = "character selection"
            elif state == "character selection":
                lastcolor = data[0]
                if len( data)>=2: 
                    targetpos = [int(pos)-1 for pos in data[1:]]
                holdstate = "target selection"
            state = holdstate
            print(state)
    if file_open:
        with open("C://Users//tomer//Documents//school//cpProject//src//extras//toPython.txt", "w") as file:
            file.write('')
            file_open = False

    # Draw shapes with transparency
    for i in range(len(targetpos)):
        square = pygame.Rect(targetpos[i] % 4 * cell_size,
                             int(targetpos[i] / 4) * cell_size,
                             cell_size,
                             cell_size)
        transparent_surface = create_transparent_surface((128, 0, 128, 128), (cell_size, cell_size))
        screen.blit(transparent_surface, square.topleft)

    # Draw circles
    pygame.draw.circle(screen, colors[0], (int(green % 4 * cell_size + cell_size // 2), int(green / 4) * cell_size + cell_size // 2), int(cell_size // 3))
    pygame.draw.circle(screen, colors[2], (int(purple % 4 * cell_size + cell_size // 2), int(purple / 4) * cell_size + cell_size // 2), int(cell_size // 3))
    pygame.draw.circle(screen, colors[3], (int(orange % 4 * cell_size + cell_size // 2), int(orange / 4) * cell_size + cell_size // 2), int(cell_size // 3))
    pygame.draw.circle(screen, colors[1], (int(yellow % 4 * cell_size + cell_size // 2), int(yellow / 4) * cell_size + cell_size // 2), int(cell_size // 3))
    pygame.display.flip()
