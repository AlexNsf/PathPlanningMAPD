import pygame
import time

from typing import List, Set, Tuple

map_colors = {
    'background': (0, 0, 0),
    'cell_border': (255, 255, 255),
    'obstacle': (127, 127, 127),
    'agent': (0, 255, 0),
    'start': (255, 255, 0),
    'finish': (0, 0, 255),
}

wait_time = 0.5


class Coordinate:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __ne__(self, other):
        return self != other

    def __hash__(self):
        return hash(str(self.x) + ' ' + str(self.y))


class Logs:
    def __init__(self):
        self.height = 0
        self.width = 0
        self.obstacles: List[Coordinate] = []
        self.initial_positions: List[Tuple[Coordinate, int]] = []
        self.start_positions: Set[Coordinate] = set()
        self.finish_positions: Set[Coordinate] = set()
        self.prev_locations: List[Tuple[Coordinate, int, bool, bool]] = []
        self.cur_locations: List[Tuple[Coordinate, int, bool, bool]] = []

    def read_size(self, height, width):
        self.height, self.width = height, width

    def read_obstacle(self, x, y):
        self.obstacles.append(Coordinate(x, y))

    def read_init(self, x, y, num):
        self.initial_positions.append((Coordinate(x, y), num))

    def read_start(self, x, y):
        self.start_positions.add(Coordinate(x, y))

    def read_finish(self, x, y):
        self.finish_positions.add(Coordinate(x, y))

    def read_cur_location(self, x, y, num, finished, started):
        self.cur_locations.append((Coordinate(x, y), num, finished, started))

    def move_cur_to_prev(self):
        self.prev_locations = self.cur_locations.copy()
        self.cur_locations = []


class Drawer:
    @staticmethod
    def draw_background_cell(x, y):
        pygame.draw.rect(display, map_colors['background'], (x * cell_size, y * cell_size, cell_size, cell_size))
        pygame.draw.rect(display, map_colors['cell_border'], (x * cell_size, y * cell_size, cell_size, cell_size),
                         width=cell_border_size)

    @staticmethod
    def draw_obstacle(x, y):
        pygame.draw.rect(display, map_colors['obstacle'], (x * cell_size, y * cell_size, cell_size, cell_size))

    @staticmethod
    def draw_start(x, y):
        pygame.draw.rect(display, map_colors['start'], (x * cell_size, y * cell_size, cell_size, cell_size))

    @staticmethod
    def draw_finish(x, y):
        pygame.draw.rect(display, map_colors['finish'], (x * cell_size, y * cell_size, cell_size, cell_size))

    @staticmethod
    def draw_agent(x, y, num, started, finished):
        color = (0, 100, 100)
        if started:
            color = (125, 38, 205)
        elif finished:
            color = (255, 0, 0)
        pygame.draw.circle(display, color,
                           (x * cell_size + cell_size // 2, y * cell_size + cell_size // 2), cell_size // 2)
        font = pygame.font.SysFont('arial', cell_size // 3)
        text = font.render(str(num), True, (0, 0, 0))
        display.blit(text, (x * cell_size + cell_size // 2, y * cell_size + cell_size // 2))

    @staticmethod
    def draw_move():
        for prev_location in logs_obj.prev_locations:
            if prev_location[0] in logs_obj.start_positions:
                Drawer.draw_start(prev_location[0].x, prev_location[0].y)
            elif prev_location[0] in logs_obj.finish_positions:
                Drawer.draw_finish(prev_location[0].x, prev_location[0].y)
            else:
                Drawer.draw_background_cell(prev_location[0].x, prev_location[0].y)
        for cur_location in logs_obj.cur_locations:
            Drawer.draw_agent(cur_location[0].x, cur_location[0].y, cur_location[1], cur_location[2], cur_location[3])
        pygame.display.update()


log_file = open('logs\\log.txt', 'r')
logs = list(map(lambda x: x.strip(), log_file.readlines()))
log_file.close()

logs_obj = Logs()
log_i = 0
while logs[log_i + 1] != "New move:":
    # print(log_i)
    if logs[log_i] == "Size:":
        log_i += 1
        logs_obj.read_size(*map(int, logs[log_i].split()))
    elif logs[log_i] == "Obstacles:":
        while logs[log_i + 1] != "Initial positions:":
            log_i += 1
            logs_obj.read_obstacle(*map(int, logs[log_i].split()))
    elif logs[log_i] == "Initial positions:":
        while logs[log_i + 1] != "Start positions:":
            log_i += 1
            logs_obj.read_init(*list(map(int, logs[log_i].split()))[:3])
            # print(*map(int, logs[log_i].split()))
            logs_obj.read_cur_location(*map(int, logs[log_i].split()))
    elif logs[log_i] == "Start positions:":
        while logs[log_i + 1] != "Finish positions:":
            log_i += 1
            logs_obj.read_start(*map(int, logs[log_i].split()))
    elif logs[log_i] == "Finish positions:":
        while logs[log_i + 1] != "New move:":
            # print(log_i, "FINFIN")
            log_i += 1
            logs_obj.read_finish(*map(int, logs[log_i].split()))
    else:
        log_i += 1

cell_size = 800 // min(logs_obj.height, logs_obj.width)
cell_border_size = cell_size // 10

pygame.init()
display = pygame.display.set_mode([logs_obj.width * cell_size, logs_obj.height * cell_size])

for i in range(logs_obj.height):
    for j in range(logs_obj.width):
        Drawer.draw_background_cell(j, i)

for c in logs_obj.obstacles:
    Drawer.draw_obstacle(c.x, c.y)

for c in logs_obj.start_positions:
    Drawer.draw_start(c.x, c.y)

for c in logs_obj.finish_positions:
    Drawer.draw_finish(c.x, c.y)

for c in logs_obj.initial_positions:
    Drawer.draw_agent(c[0].x, c[0].y, c[1], 0, 0)

pygame.display.update()

log_i += 1

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            break
    print(log_i)
    if logs[log_i] == "New move:":
        logs_obj.move_cur_to_prev()
        while logs[log_i + 1] != "New move:" and logs[log_i + 1] != "END OF LOGS":
            print(log_i)
            log_i += 1
            logs_obj.read_cur_location(*map(int, logs[log_i].split()))
        # for c in logs_obj.prev_locations:
        #     print(c.x, c.y)
        # for c in logs_obj.cur_locations:
        #     print(c.x, c.y)
        Drawer.draw_move()
        time.sleep(wait_time)
        log_i += 1
