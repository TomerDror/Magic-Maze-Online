import sys
import pygame
import time
from pygame.locals import *
import sqlite3
def authenticate(username, password):
    cursor.execute("SELECT * FROM users WHERE username = ? AND password = ?", (username, password))
    if cursor.fetchone():
        return True
    else:
        return False
def register(username, password):
    try:
        print("A")
        cursor.execute("INSERT INTO users (username, password) VALUES (?, ?)", (username, password))
        print("b")
        conn.commit()
        print("c")
        return True
    except sqlite3.IntegrityError:
        return False  # Username already exists

pygame.init()

# Set up screen
width, height = 640, 640
screen = pygame.display.set_mode((width, height))
username_input = ""
password_input = ""
is_typing_username = False
is_typing_password = False
# Load background image
bg = pygame.image.load("background.png")
bg = pygame.transform.scale(bg, (width, height))
font = pygame.font.Font(None, 36)
small_font = pygame.font.Font(None, 24)
# Define colors
colors = [(0, 255, 0), (255, 255, 0), (128, 0, 128), (255, 165, 0)]

# Define points
green = 6 - 1
purple = 7 - 1
orange = 11 - 1
yellow = 10 - 1
targetpos = []
lastcolor = "green"
state = "login"
# Size of each cell
cell_size = min(width, height) / 4

# Function to create a surface with per-pixel alpha
def create_transparent_surface(color, size):
    surface = pygame.Surface(size, pygame.SRCALPHA)
    surface.fill(color)
    return surface

# Game loop

conn = sqlite3.connect('users.db')
cursor = conn.cursor()

# Create users table if not exists
cursor.execute('''CREATE TABLE IF NOT EXISTS users (
                    id INTEGER PRIMARY KEY,
                    username TEXT,
                    password TEXT
                )''')
conn.commit()




while True:
    screen.blit(bg, (0, 0))  # Draw background image

    file_open = False

    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            sys.exit()
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if state == "login":
                x, y = event.pos
                if 250 < x < 390 and 400 < y < 440:  # Login button
                    
                    if authenticate(username_input, password_input):
                        state = "starting"
                        
                elif 250 < x < 390 and 470 < y < 510:  # Signup button
                    
                    if register(username_input, password_input):
                        state = "starting"
                elif 200 < x < 440 and 300 < y < 340:  # Username input field
                    is_typing_username = True
                    is_typing_password = False
                elif 200 < x < 440 and 370 < y < 410:  # Password input field
                    is_typing_password = True
                    is_typing_username = False

            if state  == "starting":
                x, y = event.pos
                if 250 < x < 390 and 200 < y < 240:  # Login button
                    state = "target selection"
                    with open("C://Users//tomer//Documents//school//cpProject//src//extras//toCpp.txt", "w") as file:
                            file.write("start")

            else:
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
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_BACKSPACE:
                if is_typing_username:
                    username_input = username_input[:-1]
                elif is_typing_password:
                    password_input = password_input[:-1]
            elif event.key == pygame.K_RETURN:
                if state == "login":
                    if authenticate(username_input, password_input):
                        print("Login successful!")
                        state = "starting"
                    else:
                        print("Login failed. Invalid username or password.")
                elif state == "register":
                    if register(username_input, password_input):
                        print("Registration successful!")
                        state = "starting"
                    else:
                        print("Registration failed. Username already exists.")
            else:
                if is_typing_username:
                    username_input += event.unicode
                elif is_typing_password:
                    password_input += event.unicode
    if(state != "login" and state != "starting"):
        with open("C:/Users/tomer/Documents/school/cpProject/src/extras/toPython.txt", "r") as file:
            file_open = True
            input_data = file.readlines()
            if input_data:
                data = input_data[0].split("$")
                if data[len(data)-1] =='':
                    data = data[0:len(data)-1]

                print("data "+ str(data))
                if data[0] == "move":
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
    elif state == "starting":
        font = pygame.font.Font(None, 36)
        pygame.draw.rect(screen, (0, 0, 255), (250, 200, 140, 40))  
        text = font.render("start", True, (255, 255, 255))
        screen.blit(text, (260, 205))
    else:
        # Draw input fields
        pygame.draw.rect(screen, (255, 255, 255), (200, 300, 240, 40))  # Username input field
        pygame.draw.rect(screen, (255, 255, 255), (200, 370, 240, 40))  # Password input field

        # Draw buttons
        pygame.draw.rect(screen, (0, 0, 255), (250, 400, 140, 40))  # Login button
        pygame.draw.rect(screen, (255, 0, 0), (250, 470, 140, 40))  # Register button

        # Draw button text
        login_text = font.render("Login", True, (255, 255, 255))
        screen.blit(login_text, (290, 405))
        register_text = font.render("Register", True, (255, 255, 255))
        screen.blit(register_text, (275, 475))

        # Draw input field text
        username_text_surface = small_font.render(username_input, True, (0, 0, 0))
        screen.blit(username_text_surface, (210, 310))
        password_text_surface = small_font.render("*" * len(password_input), True, (0, 0, 0))
        screen.blit(password_text_surface, (210, 380))

    pygame.display.flip()
