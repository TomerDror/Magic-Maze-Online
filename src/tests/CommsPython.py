import pygame
from multiprocessing import shared_memory
import threading

# Shared memory size (2 buffers of 256 bytes each + 2 flags)
SHM_SIZE = 514
BUFFER_SIZE = 256


# Create shared memory
shm = shared_memory.SharedMemory(create=True, size=SHM_SIZE, name='my_shared_memory')
buffer = shm.buf
started = 0
global a
a = True
# Pygame setup
pygame.init()
window_size = (800, 600)
screen = pygame.display.set_mode(window_size)
pygame.display.set_caption("Python-C++ IPC")

font = pygame.font.Font(None, 36)
clock = pygame.time.Clock()
movement_ability = (1,1,1,1,1,1,1)
field_piece_to_tile = {1:(5,1), 2:(9,2), 3:(13,3), 4:(17,4), 5:(21,5), 6:( 4,5), 7:(8,6), 8:(12,7), 9:(16,8), 10:(20,9), 11:(3,9), 12:(7,10), 13:(11,11), 14:(15,12),15:(19,13),16:(2,13), 17:(6,14),18:(10,15),19:(14,16),20:(18,17),21:(1,17),22:(5,18),23:(9,19),24:(13,20),25:(17,21)    }
tile_to_field_piece = {value: key for key, value in field_piece_to_tile.items()}

MOVEMENT_ABILITY_IMAGE = [ '../../DB/pictures/movementAbilityUp.png', '../../DB/pictures/movementAbilityDown.png', '../../DB/pictures/movementAbilityLeft.png', '../../DB/pictures/movementAbilityRight.png', '../../DB/pictures/movementAbilityEscalator.png',  '../../DB/pictures/movementAbilityPortal.png', '../../DB/pictures/movementAbilityFieldPiece.png'] 
LOADED_MOVEMENT_ABILITY =[]
for i in range(len(MOVEMENT_ABILITY_IMAGE)):
    # print(i)
    LOADED_MOVEMENT_ABILITY.append(pygame.image.load(MOVEMENT_ABILITY_IMAGE[i]))
# Load images
background_image = pygame.image.load('../../DB/pictures/background.jpg')
# IMAGE = pygame.image.load('../../DB/pictures/image.jpg')
field_settings = pygame.image.load('../../DB/pictures/fieldSettings.png')
image_x, image_y = 100, 100  # Initial position
image_width, image_height = 100, 100  # Initial size
perimeter_width = 75  # Width of the cyan perimeter

# Button class
class Button:
    def __init__(self, x, y, width, height, text, onclick):
        self.rect = pygame.Rect(x, y, width, height)
        self.color = (0, 128, 255)
        self.text = text
        self.onclick = onclick
        self.font = pygame.font.Font(None, 36)

    def draw(self, screen):
        pygame.draw.rect(screen, self.color, self.rect)
        text_surface = self.font.render(self.text, True, (255, 255, 255))
        text_rect = text_surface.get_rect(center=self.rect.center)
        screen.blit(text_surface, text_rect)

    def is_clicked(self, pos):
        return self.rect.collidepoint(pos)

# Create a start button
start_button = Button(350, 275, 100, 50, 'Start', lambda: send_to_cpp('start'))

# Function to send a message to the C++ program
def send_to_cpp(message):
    if len(message) > BUFFER_SIZE:
        raise ValueError("Message too long")
    buffer[0] = 1  # Indicate message is ready
    buffer[2:2 + len(message)] = bytearray(message.encode('utf-8'))
    buffer[2 + len(message):258] = bytearray(BUFFER_SIZE - len(message))  # Clear remaining bytes

# Function to receive a message from the C++ program
def recv_from_cpp():
    if buffer[1] == 1:
        message = bytes(buffer[258:514]).decode('utf-8').rstrip('\x00')
        buffer[1] = 0  # Clear the flag
        return message
    return None

def display_basic_game_UI():
    # screen.blit(background_image, (0, 0))  # Draw background
    a =200
    pygame.draw.rect(screen, (0, 255, 255), (0, 0, window_size[0], window_size[1]), perimeter_width)
    pygame.draw.rect(screen, (0, 255, 255), (0, 0,175,800 ),88)
    pygame.draw.rect(screen, (0, 255, 255), (625, 0,175,800 ), 88)
    display_image(field_settings, 300, 0, 200, 75)
        # a = False
    display_movement_ability()
    pygame.display.flip()

def display_movement_ability():
    count =[]
    
    for i in range(len(movement_ability)):
        if(movement_ability[i]):
            count.append( i)
    # print("count = " + count)
    width = (50)
    for i in range( len(count)):
        if(movement_ability[count[i]]):
            # print([count[i]])
            image_name = LOADED_MOVEMENT_ABILITY[count[i]]
            display_image( image_name,x=  400-(len(count) * width/2)+i*width  , y=525, width= width, height=75)
    # print(" ")

    

def listen_for_cpp_messages():
    do_once = True
    while True:
        message = recv_from_cpp()
        global started
        if message == "start":
            started = 1
        if message:
            print(f"Received from C++: {message}")
            display_message(message)
        else:
            if do_once:
                do_once = False
                # Display the background image if no message is received
                screen.blit(background_image, (0, 0))  # Draw background
                pygame.display.flip()

# Display message on the Pygame window
def display_message(message):
    screen.blit(background_image, (0, 0))  # Draw background
    text = font.render(message, True, (255, 255, 255))
    screen.blit(text, (20, 20))
    pygame.display.flip()

def getTilePos(x,y):
    return (int((x- 175)*24/450 )+1 ,int((y- 75)*24/450)+1)

def getFieldPieceFromTIle(x,y):
    try:
        for i in range(25):
            (x1,y1) = field_piece_to_tile[i+1]
            if  x>=x1 and x<x1+4 and y>=y1 and y<y1+4 :
                return i+1
        
    except:
        return 0
    return 0

# Display image with cyan perimeter on the Pygame window
def display_image(image, x, y, width, height):
    # Do not clear the screen here to preserve the perimeter
    scaled_image = pygame.transform.scale(image, (width, height))
    screen.blit(scaled_image, (x, y))
    pygame.display.flip()

# Start the listener thread
listener_thread = threading.Thread(target=listen_for_cpp_messages)
listener_thread.daemon = True
listener_thread.start()

try:
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                shm.close()
                shm.unlink()
                exit()
            elif event.type == pygame.MOUSEBUTTONDOWN:
                if start_button.is_clicked(event.pos):
                    start_button.onclick()
                # print(started)
                if started == 2:
                    event_x, event_y = pygame.mouse.get_pos()

                    print(str(event_x)+", "+str(event_y))
                    if event_x >= 175 and event_x <= 625 and event_y >=75 and event_y <=525:
                        (x,y) = getTilePos(event_x,event_y)
                        print(str(x)+", "+str(y))
                        fp = getFieldPieceFromTIle(x,y)
                        print(fp)

        # Redraw the screen with the button
        if  started == 0:
            screen.blit(background_image, (0, 0))
            start_button.draw(screen)
        
        if started == 1:
            screen.blit(background_image, (0, 0))
            started =2
            # print("faklsdfj")
            display_basic_game_UI()
        pygame.display.flip()

        clock.tick(30)  # Limit to 30 frames per second

finally:
    shm.close()
    shm.unlink()
