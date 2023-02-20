import ctypes

# a little guid of how to access the engine's api
# also serves as a quick test for these api functions

class ai_turn(ctypes.Structure):
    _fields_ = [
        ("row", ctypes.c_int),
        ("col", ctypes.c_int),
        ("flag", ctypes.c_char),
    ]

def get_first_to_play():
    first_turn_func = engine.get_first_turn
    first_turn_func.restype = ctypes.c_char
    return first_turn_func().decode()

def on_click(row, col):
    on_player_turn_func = engine.on_player_turn
    on_player_turn_func.argtypes = (ctypes.c_int, ctypes.c_int)
    on_player_turn_func.restype = ctypes.c_char
    return on_player_turn_func(row, col).decode()

def run_computer_turn():
    play_computer_turn_func = engine.play_computer_turn
    play_computer_turn_func.restype = ai_turn
    return play_computer_turn_func()


# Load the engine
engine = ctypes.CDLL("/Users/nort/Desktop/engine.so")

# init engine library
init_func = engine.init
init_func()

# make engine decide who plays first
first_to_play = get_first_to_play()

print(f'first to play: {first_to_play}')

# registering a player turn to the engine
player_turn_result = on_click(2, 2)

print(f'player turn result: {player_turn_result}')

# make computer ai do a turn
ai_turn_result = run_computer_turn()

print(f'ai turn result: {ai_turn_result.row}, {ai_turn_result.col}, {ai_turn_result.flag.decode()}')

