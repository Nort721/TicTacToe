import PySimpleGUI as sg
from enum import Enum
import time

BUTTON_MD = {"WasClicked": False}
BUTTON_FONT = ("Arial bold", 10)
TEXT_FONT = ("Helvetica", 18)  

# Defining the Game State enum
class GameState(Enum):
    Running = 'N'
    ComputerWon = 'C'
    PlayerWon = 'P'
    Draw = 'X'

layout = [
    [sg.Text("Welcome to Tic Tac Toe!", key="WelcomeText", font=TEXT_FONT)],
    [sg.Text("", key="UpdateWinner", font=TEXT_FONT)],
    [sg.Button("", font=BUTTON_FONT, size=(10, 5), key="-00-", metadata=BUTTON_MD), sg.Button("", font=BUTTON_FONT, size=(10, 5), key="-01-", metadata=BUTTON_MD), sg.Button("", font=BUTTON_FONT, size=(10, 5), key="-02-", metadata=BUTTON_MD)],
    [sg.Button("", font=BUTTON_FONT, size=(10, 5), key="-10-", metadata=BUTTON_MD), sg.Button("", font=BUTTON_FONT, size=(10, 5), key="-11-", metadata=BUTTON_MD), sg.Button("", font=BUTTON_FONT, size=(10, 5), key="-12-", metadata=BUTTON_MD)],
    [sg.Button("", font=BUTTON_FONT, size=(10, 5), key="-20-", metadata=BUTTON_MD), sg.Button("", font=BUTTON_FONT, size=(10, 5), key="-21-", metadata=BUTTON_MD), sg.Button("", font=BUTTON_FONT, size=(10, 5), key="-22-", metadata=BUTTON_MD)]
    ]

# Create the window
window = sg.Window("Tic Tac Toe", layout, element_justification="center", size=(350, 400), background_color="")

# Create an event loop

status = GameState.Running

while status == GameState.Running:
    event, values = window.read()

    # Updating the button, if it wasn't yet clicked
    if event.startswith("-") and window[event].metadata.get("WasClicked") == False:
        window[event].update("X")
        window[event].metadata={"WasClicked": True}
        # Call the C function to play computer move
        # Update status according to the C program
        
        # REMEMBER TO UPDATE THE METADATA OF THE BUTTON THE COMPUTER PLAYED ACCORDINGLY # 
    
    if event == sg.WINDOW_CLOSED:
        quit()

        
# Updating the message
if status == GameState.ComputerWon:
    winmsg = "The computer has won!"
elif status == GameState.PlayerWon:
    winmsg = "You won!"
elif status == GameState.Draw:
    winmsg = "It's a draw!"

window["UpdateWinner"].update(winmsg)

start_time = time.time()
while (time.time() - start_time) < 3:
    event, values = window.read()
    
    if event == sg.WINDOW_CLOSED:
        quit()