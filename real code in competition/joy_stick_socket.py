# this code to chat between to joy stick and other process in the same machine

import pygame
import socket               # Import socket module

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)         # Create a socket object

port = 12345                     # Reserve a port for the service.
host = "192.168.0.102"           # host name or adress of the \
                                 # target machine


s.connect((host, port))
print s.recv(1024)

def send_data(data):
    if data =="":
        return
    s.send(data)
    print("I recieve : %s" % s.recv(1024))

# Define some colors
BLACK    = (   0,   0,   0)
WHITE    = ( 255, 255, 255)



# This is a simple class that will help us print to the screen
# It has nothing to do with the joysticks, just outputting the
# information.
class TextPrint:
    def __init__(self):
        self.reset()
        self.font = pygame.font.Font(None, 20)

    def printt(self, screen, textString):
        textBitmap = self.font.render(textString, True, BLACK)
        screen.blit(textBitmap, [self.x, self.y])
        self.y += self.line_height

    def reset(self):
        self.x = 10
        self.y = 10
        self.line_height = 15

    def indent(self):
        self.x += 10

    def unindent(self):
        self.x -= 10


pygame.init()
# Set the width and height of the screen [width,height]
size = [700, 700]
screen = pygame.display.set_mode(size)

pygame.display.set_caption("UGVC")

#Loop until the user clicks the close button.
done = False

# Used to manage how fast the screen updates
clock = pygame.time.Clock()

# Initialize the joysticks
pygame.joystick.init()

# Get ready to print
textPrint = TextPrint()

# -------- Main Program Loop -----------
while done==False:
    # EVENT PROCESSING STEP
    for event in pygame.event.get(): # User did something
        if event.type == pygame.QUIT: # If user clicked close
            done=True # Flag that we are done so we exit this loop

        # Possible joystick actions: JOYAXISMOTION JOYBALLMOTION JOYBUTTONDOWN JOYBUTTONUP JOYHATMOTION
        if event.type == pygame.JOYBUTTONDOWN:
            print("Joystick button pressed.")
        if event.type == pygame.JOYBUTTONUP:
            print("Joystick button released.")


    # DRAWING STEP
    # First, clear the screen to white. Don't put other drawing commands
    # above this, or they will be erased with this command.
    screen.fill(WHITE)
    textPrint.reset()

    # Get count of joysticks
    joystick_count = pygame.joystick.get_count()

    textPrint.printt(screen, "Number of joysticks: {}".format(joystick_count) )
    textPrint.indent()

    # For each joystick:
    for i in range(joystick_count):
        joystick = pygame.joystick.Joystick(i)
        joystick.init()

        textPrint.printt(screen, "Joystick {}".format(i) )
        textPrint.indent()

        # Get the name from the OS for the controller/joystick
        name = joystick.get_name()
        textPrint.printt(screen, "Joystick name: {}".format(name) )

        HOR=0
        if name[:3]=="HOR":
            HOR=1
        elif name[:3]=="AVB":
            HOR=0


        if HOR :
            # Usually axis run in pairs, up/down for one, and left/right for
            # the other.
            textPrint.printt(screen, "Number of axes: {}".format(2) )
            textPrint.indent()

            axis1_value = joystick.get_axis(1)
            axis0_value = joystick.get_axis(0)
            if  axis1_value< -0.5:
                send_data("F")
            elif axis1_value > 0.5:
                send_data("B")


            if  axis0_value< -0.5:
                send_data("L")
            elif axis0_value > 0.5:
                send_data("R")

            if(axis0_value > -0.5 and axis0_value < 0.5 and axis1_value > -0.5 and axis1_value <0.5 ):
                send_data("s")

            textPrint.printt(screen, "Axis {} value: {:>6.3f}".format(0, axis0_value) )
            textPrint.printt(screen, "Axis {} value: {:>6.3f}".format(1, axis1_value) )
            textPrint.unindent()
            buttons = joystick.get_numbuttons()
            textPrint.printt(screen, "Number of buttons: {}".format(buttons) )
            textPrint.indent()
            HOR_buttons =["T","V","U","W","","","P","O","","","","Q"]
            for i in range( buttons ):
                button = joystick.get_button( i )
                if button :
                    send_data(HOR_buttons[i])
                textPrint.printt(screen, "Button {:>2} value: {}".format(i,button) )
            textPrint.unindent()
            textPrint.unindent()



        else :
            # Usually axis run in pairs, up/down for one, and left/right for
            # the other.
            textPrint.printt(screen, "Number of axes: {}".format(3) )
            textPrint.indent()

            axis0_value = joystick.get_axis(0)
            axis1_value = joystick.get_axis(1)
            axis3_value = joystick.get_axis(3)

            if  axis0_value< -0.5:
                send_data("C")
            elif axis0_value > 0.5:
                send_data("A")

            if  axis1_value< -0.5:
                send_data("E")
            elif axis1_value > 0.5:
                send_data("D")

            if  axis3_value< -0.5:
                send_data("H")
            elif axis3_value > 0.5:
                send_data("G")

            textPrint.printt(screen, "Axis {} value: {:>6.3f}".format(0, axis0_value) )
            textPrint.printt(screen, "Axis {} value: {:>6.3f}".format(1, axis1_value) )
            textPrint.printt(screen, "Axis {} value: {:>6.3f}".format(3, axis3_value) )

            textPrint.unindent()
            buttons = joystick.get_numbuttons()
            textPrint.printt(screen, "Number of buttons: {}".format(buttons) )
            textPrint.indent()

            AVB_buttons =["S","","J","I","Y","K","N","M"]
            for i in range( buttons ):
                button = joystick.get_button( i )
                if button :
                    send_data(AVB_buttons[i])
                textPrint.printt(screen, "Button {:>2} value: {}".format(i,button) )
            textPrint.unindent()
            textPrint.unindent()


    # ALL CODE TO DRAW SHOULD GO ABOVE THIS COMMENT

    # Go ahead and update the screen with what we've drawn.
    pygame.display.flip()

    # Limit to 20 frames per second
    clock.tick(20)

# Close the window and quit.
# If you forget this line, the program will 'hang'
# on exit if running from IDLE.
pygame.quit ()
