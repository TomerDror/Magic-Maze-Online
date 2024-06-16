import pygame
from multiprocessing import shared_memory
import threading


def authenticate(username, password):
    cursor.execute("SELECT * FROM users WHERE username = ? AND password = ?", (username, password))
    if cursor.fetchone():
        return True
    else:
        return False
def register(username, password):
    try:
        cursor.execute("INSERT INTO users (username, password) VALUES (?, ?)", (username, password))
        conn.commit()
        return True
    except sqlite3.IntegrityError:
        return False  # Username already exists


# Shared memory size (2 buffers of 256 bytes each + 2 flags)
SHM_SIZE = 514
BUFFER_SIZE = 256
state = "not started"
# def cleanup_shm(shm_name):
#     try:
#         existing_shm = shared_memory.SharedMemory(name=shm_name)
#         existing_shm.unlink()
#     except FileNotFoundError:
#         pass

# # Clean up any existing shared memory segment before creating a new one
# cleanup_shm('my_shared_memory')

# Create shared memory
def open_or_create_shm(shm_name):
    try:
        existing_shm = shared_memory.SharedMemory(name=shm_name)
        return existing_shm
    except FileNotFoundError:
        print("Shared memory segment not found. Creating a new one.")
        return shared_memory.SharedMemory(create=True, size=SHM_SIZE, name=shm_name)

# Open or create shared memory
shm = open_or_create_shm('my_shared_memory')
buffer = shm.buf
global a
a = True
opened_fieldpiece = [1]
last_used_color_name = "green"
field_piece_pos_to_id ={13: 1}
open_direction = "none"
pygame.init()
window_size = (800, 600)
screen = pygame.display.set_mode(window_size)
pygame.display.set_caption("Python-C++ IPC")
target_pos = []
purple = (12,12)
green = (13,13)
yellow = (12,13)
orange = (13,12)
colors = [(0, 255, 0), (255, 255, 0), (128, 0, 128), (255, 165, 0)]
last_used_color = colors[0]
moved_character = True
font = pygame.font.Font(None, 36)
clock = pygame.time.Clock()
movement_ability = (1,1,1,1,1,1,1)
field_piece_to_tile = {1:(5,1), 2:(9,2), 3:(13,3), 4:(17,4), 5:(21,5), 6:( 4,5), 7:(8,6), 8:(12,7), 9:(16,8), 10:(20,9), 11:(3,9), 12:(7,10), 13:(11,11), 14:(15,12),15:(19,13),16:(2,13), 17:(6,14),18:(10,15),19:(14,16),20:(18,17),21:(1,17),22:(5,18),23:(9,19),24:(13,20),25:(17,21)    }
tile_to_field_piece = {value: key for key, value in field_piece_to_tile.items()}

MOVEMENT_ABILITY_IMAGE = [ '../../DB/pictures/movementAbilityUp.png', '../../DB/pictures/movementAbilityDown.png', '../../DB/pictures/movementAbilityLeft.png', '../../DB/pictures/movementAbilityRight.png', '../../DB/pictures/movementAbilityEscalator.png',  '../../DB/pictures/movementAbilityPortal.png', '../../DB/pictures/movementAbilityFieldPiece.png'] 
FIELD_PIECE_IMAGE = [ '../../DB/pictures/FieldPiece1.png', '../../DB/pictures/FieldPiece2.png', '../../DB/pictures/FieldPiece3.png', '../../DB/pictures/FieldPiece4.png', '../../DB/pictures/FieldPiece5.png', '../../DB/pictures/FieldPiece6.png', '../../DB/pictures/FieldPiece7.png', '../../DB/pictures/FieldPiece8.png', '../../DB/pictures/FieldPiece9.png'] 
LOADED_MOVEMENT_ABILITY = []
LOADED_FIELD_PIECE = []
for i in range(len(MOVEMENT_ABILITY_IMAGE)):
    LOADED_MOVEMENT_ABILITY.append(pygame.image.load(MOVEMENT_ABILITY_IMAGE[i]))
for i in range(len(FIELD_PIECE_IMAGE)):
    LOADED_FIELD_PIECE.append(pygame.image.load(FIELD_PIECE_IMAGE[i]))

# Load images
background_image = pygame.image.load('../../DB/pictures/background.jpg')
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
    print(message)
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
    pygame.draw.rect(screen, (0, 255, 255), (0, 0, window_size[0], window_size[1]), perimeter_width)
    pygame.draw.rect(screen, (0, 255, 255), (0, 0,175,800 ),88)
    pygame.draw.rect(screen, (0, 255, 255), (625, 0,175,800 ), 88)
    display_image(field_settings, 300, 0, 200, 75)
    display_movement_ability()
    pygame.display.flip()

def display_movement_ability():
    count = []
    for i in range(len(movement_ability)):
        # print(movement_ability[i])
        if movement_ability[i] :
            # print("pleaase" + str(i))
            count.append(i)
    width = 50
    for i in range(len(count)):
        if movement_ability[count[i]]:
            image_name = LOADED_MOVEMENT_ABILITY[count[i]]
            display_image(image_name, x=400-(len(count) * width/2)+i*width, y=525, width=width, height=75)

def listen_for_cpp_messages():
    do_once = True
    while True:
        global moved_character
        message = recv_from_cpp()
        global state
        if message:
            cmd = message.split('$')
            if cmd[0] == "start":
                state = 'starting'
                global movement_ability 
                movement_ability = (int(cmd[1]),int(cmd[2]),int(cmd[3]),int(cmd[4]),int(cmd[5]),int(cmd[6]),int(cmd[7]))
                # print ("sdljkfhasdljkfh" +cmd[1]+cmd[2]+cmd[3]+cmd[4]+cmd[5]+cmd[6]+cmd[7])

            print(f"Received from C++: {message}")

            if state == 'character selection' :
                color = cmd[0]
                if len(cmd)>1:
                    state = "target selection"
                    global last_used_color
                    global last_used_color_name
                    last_used_color_name = color
                    last_used_color = (
                        colors[0] if color == "green" else
                        colors[1] if color == "yellow" else
                        colors[2] if color == "purple" else
                        colors[3]
                    )
                    global target_pos
                    target_pos =[]
                    moved_character = True
                    try:
                        target_pos = list(map(int, cmd[1:-1]))
                    except:
                        target_pos =[]

                    global open_direction
                    open_direction = cmd[-1]
            if cmd[0 ] == "open":

                moved_character = True
                floating_point_number = float(cmd[4])
                id = int((floating_point_number / 1000000 - 1) / 16) + 1   
                # print()
                # print(cmd[4])
                # print(id)
                opened_fieldpiece.append(id)
                color =cmd[1]
                direction = cmd[3]
                global green
                global yellow
                global purple
                global orange
                if direction !="none":
                    currTile  = (
                        green if color == "green" else
                        yellow if color == "yellow" else
                        purple if color == "purple" else
                        orange
                    )
                    x,y = currTile
                    fp = getFieldPieceFromTile (x,y)
                    new_fp =0
                    if open_direction == "up" and fp >5:
                        new_fp =fp-5
                    if open_direction == "down" and fp<21:
                        new_fp =fp+5
                    if open_direction == "left" and fp%5!=1:
                        new_fp =fp-1
                    if open_direction == "right" and fp%5!=0:
                        new_fp =fp+1
                    if new_fp != 0:
                        field_piece_pos_to_id[new_fp] = id  
                        if open_direction == "down" :
                            LOADED_FIELD_PIECE[id-1] = pygame.transform.rotate(LOADED_FIELD_PIECE[id-1], 180)

                        if open_direction == "left" :
                            LOADED_FIELD_PIECE[id-1] = pygame.transform.rotate(LOADED_FIELD_PIECE[id-1], 90)

                        if open_direction == "right":
                            LOADED_FIELD_PIECE[id-1] = pygame.transform.rotate(LOADED_FIELD_PIECE[id-1], 270)

                state = 'character selection'
                target_pos =[]

            if cmd[0] == "move":
                    try: 
                        pos = tile_value_to_pos(int(cmd[2]))

                        moved_character = True
                        if cmd[1] == "green":
                            green = pos
                            target_pos =[]
                        if cmd[1] == "purple":
                            purple = pos
                            target_pos =[]
                        if cmd[1] == "orange":
                            orange = pos
                            target_pos =[]
                        if cmd[1] == "yellow":
                            yellow = pos
                            target_pos =[]
                    except:
                        pass
            
                    state = 'character selection'
            # display_message(message)
        else:
            if do_once:
                do_once = False
                screen.blit(background_image, (0, 0))  # Draw background
                pygame.display.flip()

# Display message on the Pygame window

def tile_value_to_pos(pos_val):

    temp =int((pos_val -1)/16) +1
    id_to_field_piece = {value: key for key, value in field_piece_pos_to_id.items()}
    # print("asdkflhasd;klfj")
    # print(id_to_field_piece)
    fp = id_to_field_piece[temp]
    (x,y) = field_piece_to_tile[fp]
    return (x+ ((pos_val-1)%4), y+ (int((pos_val-1)/4))%4)

def pos_value_to_pixel(pos_val):
   return (int(pos_val[0]*450/24) +175), (int(pos_val[1]*450/24) +75)

    
def display_message(message):
    screen.blit(background_image, (0, 0))  # Draw background
    text = font.render(message, True, (255, 255, 255))
    screen.blit(text, (20, 20))
    pygame.display.flip()

def getTilePos(x, y):
    return (int((x - 175) * 24 / 450) , int((y - 75) * 24 / 450) )

def getFieldPieceFromTile(x, y):
    try:
        for i in range(25):
            (x1, y1) = field_piece_to_tile[i+1]
            if x >= x1 and x < x1 + 4 and y >= y1 and y < y1 + 4:
                return i + 1
    except:
        return 0
    return 0

def print_field_pieces():
    for i in range(len(opened_fieldpiece)):
        id_to_field_piece = {value: key for key, value in field_piece_pos_to_id.items()}
        try:
            field_piece_pos = id_to_field_piece[opened_fieldpiece[i]]
            (x,y) = field_piece_to_tile[field_piece_pos]
            # (x,y) = (x-1,y-1)
            display_image(image= LOADED_FIELD_PIECE[opened_fieldpiece[i]-1],x= (int(x*450/24) +175),y= (int(y*450/24 )+75), width=int(450/6),height=int(450/6)   )
        except:
            pass

        
# Display image with cyan perimeter on the Pygame window
def display_image(image, x, y, width, height):
    scaled_image = pygame.transform.scale(image, (width, height))
    screen.blit(scaled_image, (x, y))
    pygame.display.flip()

# Start the listener thread
listener_thread = threading.Thread(target=listen_for_cpp_messages)
listener_thread.daemon = True
listener_thread.start()

def handle_text_input_events(events, text_input_box):
    for event in events:
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_RETURN:
                # Get the entered IP address
                ip_address = text_input_box.get_text()
                # Process the IP address as needed
                print("Entered IP Address:", ip_address)
        text_input_box.update(events)


try:
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                shm.close()
                shm.unlink()
                exit()
            elif event.type == pygame.MOUSEBUTTONDOWN:
                # print(len(opened_fieldpiece))
                # print(field_piece_pos_to_id)
                if state == "not started" and start_button.is_clicked(event.pos):
                    start_button.onclick()
                if state == 'character selection' or 'target selection':
                    event_x, event_y = pygame.mouse.get_pos()
                    if 175 <= event_x <= 625 and 75 <= event_y <= 525:
                        print("front end state" +state)
                        try:
                            (x, y) = getTilePos(event_x, event_y)
                            fp = getFieldPieceFromTile(x, y)
                            fpId = field_piece_pos_to_id[fp]
                            tile_pov_value = x-field_piece_to_tile[fp][0]+1 +(y-field_piece_to_tile[fp][1])*4
                            tileId = (fpId-1)*16+ tile_pov_value
                            if state == 'character selection':
                                send_to_cpp('get$'+str(tileId))

                            if state == 'target selection':
                                # id_to_field_piece = {value: key for key, value in field_piece_pos_to_id.items()}
                                if(fpId in opened_fieldpiece):
                                    send_to_cpp('move$'+last_used_color_name+"$"+str(tileId))
                                

                        except:
                            send_to_cpp('open$'+last_used_color_name)
                            

        if state == 'not started':
            screen.blit(background_image, (0, 0))
            start_button.draw(screen)
        if state == 'starting':
            screen.blit(background_image, (0, 0))
            state = 'character selection'
            # print ("solution " +movement_ability[0]+movement_ability[1]+movement_ability[2]+movement_ability[3]+movement_ability[4]+movement_ability[5]+movement_ability[6])

            display_basic_game_UI()
            moved_character = True
        
        if moved_character:
            moved_character = False
            print_field_pieces()
            pygame.draw.circle(screen, colors[0],draw_bottom_left= True,draw_bottom_right=True, draw_top_left= True, draw_top_right= True ,radius=int(450/72),center=((int((green[0]+0.5) *450/24) +175), (int((green[1]+0.5)*450/24) +75)) )
            pygame.draw.circle(screen, colors[1],draw_bottom_left= True,draw_bottom_right=True, draw_top_left= True, draw_top_right= True ,radius=int(450/72),center=((int((yellow[0]+0.5)*450/24) +175), (int((yellow[1]+0.5)*450/24) +75)) )
            pygame.draw.circle(screen, colors[2],draw_bottom_left= True,draw_bottom_right=True, draw_top_left= True, draw_top_right= True ,radius=int(450/72),center=((int((purple[0]+0.5)*450/24) +175), (int((purple[1]+0.5)*450/24) +75)) )
            pygame.draw.circle(screen, colors[3],draw_bottom_left= True,draw_bottom_right=True, draw_top_left= True, draw_top_right= True ,radius=int(450/72),center=((int((orange[0]+0.5)*450/24) +175), (int((orange[1]+0.5)*450/24) +75)) )
        if state == "target selection":
            for pos in target_pos:
                # print(pos)
                x,y = tile_value_to_pos(pos)
                x,y = pos_value_to_pixel((x,y))
                pygame.draw.rect(screen, last_used_color, pygame.Rect(x, y, int(450/24), int(450/24)))
            if open_direction !="none":
                lastUsedTile  = (
                        green if last_used_color_name == "green" else
                        yellow if last_used_color_name == "yellow" else
                        purple if last_used_color_name == "purple" else
                        orange
                    )
                x,y = lastUsedTile
                fp = getFieldPieceFromTile(x,y)
                new_fp =0
                if open_direction == "up" and fp >5:
                    new_fp =fp-5
                if open_direction == "down" and fp<21:
                    new_fp =fp+5
                if open_direction == "left" and fp%5!=1:
                    new_fp =fp-1
                if open_direction == "right" and fp%5!=0:
                    new_fp =fp+1
                if new_fp != 0:

                    x,y = pos_value_to_pixel(field_piece_to_tile[new_fp])
                    pygame.draw.rect(screen, last_used_color, (x,y,450/6,450/6 ), 10)

        pygame.display.flip()
        clock.tick(30)  # Limit to 30 frames per second
finally:
    shm.close()
    shm.unlink()
